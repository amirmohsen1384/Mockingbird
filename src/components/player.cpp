#include "include/components/player.h"
#include "include/core/song.h"
#include <QRandomGenerator64>
#include <QMediaDevices>
#include <QAudioDevice>
#include "ui_player.h"

Player::Player(QWidget *parent) : QWidget(parent), ui(std::make_unique<Ui::Player>())
{
    ui->setupUi(this);

    player = std::make_unique<QMediaPlayer>();
    output = std::make_unique<QAudioOutput>();

    player->setAudioOutput(output.get());
    output->setDevice(QMediaDevices::defaultAudioOutput());

    QMediaPlayer *media = player.get();

    // Updates the playback button as the playing status changes
    connect(media, &QMediaPlayer::playingChanged, this, [&](bool value)
    {
        ui->playbackButton->setToolTip(value ? "Pause" : "Play");
        ui->playbackButton->setIcon(QIcon(QString(":/images/player/%1.png").arg(value ? "pause" : "play")));
    });

    // Updates Sequence-management buttons as they change
    connect(this, &Player::shuffleModeChanged, this, &Player::toggleShuffleButton);
    connect(this, &Player::infiniteModeChanged, this, &Player::toggleInfiniteButton);

    // Controls over the playback control buttons
    connect(media, &QMediaPlayer::sourceChanged, this, &Player::updatePlaybackControl);

    // Controls the flow of the media
    connect(media, &QMediaPlayer::positionChanged, this, &Player::updateProgress);

    // Updates the playback rate of the player
    connect(media, &QMediaPlayer::playbackRateChanged, this, &Player::updatePlaybackSpeed);
    connect(ui->progressSlider, &QSlider::sliderMoved, media, &QMediaPlayer::setPosition);
    connect(ui->progressSlider, &QSlider::sliderPressed, [&]()
    {
        player->setPosition(ui->progressSlider->value());
    });

    // Advances to the next track if the current media reached the end.
    connect(media, &QMediaPlayer::mediaStatusChanged, this, [&](QMediaPlayer::MediaStatus s)
    {
        if(s == QMediaPlayer::EndOfMedia)
        {
            advance();
        }
    });

    // The player gets updated as the current track changes
    connect(this, &Player::currentTrackChanged, this, &Player::updatePlayer);
}

bool Player::isPlaying() const
{
    return player->isPlaying();
}

bool Player::isShuffleMode() const
{
    return ui->shuffleButton->isChecked();
}

bool Player::isInfiniteMode() const
{
    return player->loops() == QMediaPlayer::Infinite ? true : false;
}

int Player::getCurrentTrack() const
{
    return currentTrack;
}

void Player::updateModel(QAbstractItemModel *prev, QAbstractItemModel *current)
{
    if(prev != nullptr)
    {
        disconnect(prev, &QAbstractItemModel::rowsRemoved, this, &Player::updatePlayer);
        disconnect(prev, &QAbstractItemModel::rowsInserted, this, &Player::updatePlayer);
    }
    if(current != nullptr)
    {
        connect(current, &QAbstractItemModel::rowsRemoved, this, &Player::updatePlayer);
        connect(current, &QAbstractItemModel::rowsInserted, this, &Player::updatePlayer);
    }
}

void Player::updatePlayer()
{
    if(_model == nullptr)
    {
        return;
    }
    Song song = qvariant_cast<Song>(_model->index(currentTrack, 0, QModelIndex()).data(Qt::UserRole));
    player->setSource(song.getAddress());
    player->play();
    _model->setData(QModelIndex(), currentTrack, Playlist::PlayingRole);
}

void Player::advance()
{
    int index = 0;
    if(_model == nullptr)
    {
        return;
    }
    const int count = _model->rowCount();
    if(count <= 0)
    {
        return;
    }
    else if(!isShuffleMode())
    {
        index = (currentTrack + 1) % count;
    }
    else
    {
        int target = 0;
        do
        {
            target = QRandomGenerator64::global()->bounded(0, count);
        }
        while(index == target && count > 1);
        index = target;
    }
    setCurrentTrack(index);
}

void Player::updateProgress()
{
    const qint64 duration = player->duration();
    const qint64 position = player->position();

    ui->progressSlider->setMaximum(duration);
    ui->progressSlider->setValue(position);

    QTime elapsedTime = QTime::fromMSecsSinceStartOfDay(position);
    ui->elapsedTimeLabel->setText(elapsedTime.toString(elapsedTime.hour() > 0 ? "hh:mm:ss" : "mm:ss"));

    QTime totalTime = QTime::fromMSecsSinceStartOfDay(duration);
    ui->totalTimeLabel->setText(totalTime.toString(totalTime.hour() > 0 ? "hh:mm:ss" : "mm:ss"));
}

void Player::changePlaybackMode()
{
    if(player->isPlaying())
    {
        player->pause();
    }
    else
    {
        player->play();
    }
}

void Player::updatePlaybackSpeed()
{
    ui->speedButton->setText(QString::number(player->playbackRate(), '0', 2));
}

void Player::updatePlaybackControl()
{
    bool state = player->error() == QMediaPlayer::NoError || player->mediaStatus() != QMediaPlayer::InvalidMedia;
    ui->stopButton->setEnabled(state);
    ui->speedButton->setEnabled(state);
    ui->replayButton->setEnabled(state);
    ui->forwardButton->setEnabled(state);
    ui->progressSlider->setEnabled(state);
    ui->playbackButton->setEnabled(state);
    ui->totalTimeLabel->setEnabled(state);
    ui->elapsedTimeLabel->setEnabled(state);
}

void Player::toggleShuffleButton(bool value)
{
    ui->shuffleButton->setToolTip(value ? "Sequential" : "Shuffle");
    ui->shuffleButton->setIcon(QIcon(QString(":/images/player/%1.png").arg(value ? "no-shuffle" : "shuffle")));
}

void Player::toggleInfiniteButton(bool value)
{
    ui->repeatButton->setToolTip(value ? "Once" : "Infinite");
    ui->repeatButton->setIcon(QIcon(QString(":/images/player/%1.png").arg(value ? "no-repeat" : "repeat")));
}

void Player::stop()
{
    player->stop();
}

void Player::replay()
{
    if(player->isSeekable())
    {
        constexpr qint64 SEEK_INTERVAL_MS = 10'000;
        player->setPosition(player->position() - SEEK_INTERVAL_MS);

    }

}

void Player::forward()
{
    if(player->isSeekable())
    {
        constexpr qint64 SEEK_INTERVAL_MS = 10'000;
        player->setPosition(player->position() + SEEK_INTERVAL_MS);

    }
}

void Player::goToNextTrack()
{
    setCurrentTrack((currentTrack + 1) % _model->rowCount());
}

void Player::goToPreviousTrack()
{
    if((currentTrack - 1) < 0)
    {
        setCurrentTrack(_model->rowCount() - 1);
    }
    else
    {
        setCurrentTrack((currentTrack - 1) % _model->rowCount());
    }
}

void Player::changePlaybackSpeed()
{
    player->setPlaybackRate(std::max(std::fmod(player->playbackRate() + 0.25, 2.25), 0.25));
}

void Player::setCurrentTrack(int value)
{
    if(_model != nullptr)
    {
        if(value >= 0 && value < _model->rowCount())
        {
            currentTrack = value;
            emit currentTrackChanged(value);
        }
    }
}

void Player::setShuffleMode(bool value)
{
    ui->shuffleButton->setChecked(value);
    emit shuffleModeChanged(value);
}

void Player::setInfiniteMode(bool value)
{
    player->setLoops(value ? QMediaPlayer::Infinite : QMediaPlayer::Once);
    emit infiniteModeChanged(value);
}

Player::~Player() {}

QAbstractItemModel *Player::model()
{
    return _model;
}

void Player::setModel(QAbstractItemModel *value)
{
    auto temp = this->_model;
    this->_model = value;
    updateModel(temp, value);
    currentTrack = 0;
    player->stop();
    updatePlayer();
}
