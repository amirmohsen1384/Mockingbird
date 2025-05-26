#include "include/dialogs/songedit.h"
#include <QMediaMetaData>
#include <QMediaDevices>
#include "ui_songedit.h"
#include <QAudioOutput>
#include <QAudioDevice>
#include <QFileDialog>
#include <QMessageBox>
#include <exception>

SongEdit::SongEdit(QWidget *parent) : QDialog{parent}
{
    ui = std::make_unique<Ui::SongEdit>();
    ui->setupUi(this);

    output = new QAudioOutput(this);
    output->setDevice(QMediaDevices::defaultAudioOutput());

    player = new QMediaPlayer(this);
    player->setAudioOutput(output);

    ui->genreEdit->setModel(&model);
    ui->playbackButton->setVisible(false);
    ui->initializeButton->setVisible(false);
    ui->releaseYearEdit->setValue(QDate::currentDate().year());

    connect(this, &SongEdit::locationChanged, player, &QMediaPlayer::setSource);
    connect(player, &QMediaPlayer::sourceChanged, this, &SongEdit::updateSource);
    connect(player, &QMediaPlayer::playingChanged, this, &SongEdit::updatePlaybackControl);
    connect(ui->nameEdit, &QLineEdit::textChanged, this, &SongEdit::nameChanged);
    connect(ui->artistEdit, &QLineEdit::textChanged, this, &SongEdit::artistChanged);
    connect(ui->fileNameEdit, &QLineEdit::textChanged, this, [&](const QString &text)
    {
        emit locationChanged(QUrl::fromLocalFile(text));
    }
    );
    connect(ui->genreEdit, &QComboBox::currentIndexChanged, this, [&](int index)
    {
        emit genreChanged(static_cast<Song::Genre>(index));
    }
    );
    connect(ui->coverView, &ImageView::imageChanged, this, [&](const QImage &image)
    {
        emit SongEdit::coverChanged(QPixmap::fromImage(image));
    }
    );
    connect(player, &QMediaPlayer::metaDataChanged, this, [&]()
    {
        if(player->mediaStatus() != QMediaPlayer::InvalidMedia)
        {
            ui->initializeButton->setVisible(true);
        }
    }
    );
    connect(ui->releaseYearEdit, &QSpinBox::valueChanged, this, &SongEdit::releasedYearChanged);
    ui->releaseYearEdit->setMinimum(_min_year);
}

SongEdit::SongEdit(const IDContainer &value, QWidget *parent) : SongEdit(parent)
{
    setValue(value);
}

QFileDialog *SongEdit::browseFile()
{
    QFileDialog *dialog = new QFileDialog(this);
    dialog->setDirectory(QDir::home());
    dialog->setAcceptMode(QFileDialog::AcceptOpen);
    dialog->setFileMode(QFileDialog::ExistingFile);
    dialog->setWindowTitle("Select a file to open");
    dialog->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    return dialog;
}

void SongEdit::updateSource()
{
    player->stop();
    ui->initializeButton->setVisible(false);
    ui->playbackButton->setVisible(player->mediaStatus() != QMediaPlayer::InvalidMedia);
}

void SongEdit::openSongFile()
{
    static QStringList history;
    std::unique_ptr<QFileDialog> dialog(browseFile());
    dialog->setNameFilter("Songs (*.mp3 *.wav *.aiff *.pcm *.aac *.ogg *.wma *.m4a *.ape *.alac)");
    dialog->setHistory(history);
    if(dialog->exec() == QDialog::Accepted)
    {
        const QString fileName = dialog->selectedFiles().constFirst();
        setLocation(QUrl::fromLocalFile(fileName));
        history.append(fileName);
    }
}

void SongEdit::openImageFile()
{
    static QStringList history;
    std::unique_ptr<QFileDialog> dialog(browseFile());
    dialog->setNameFilter("Pictures (*.bmp *.png *.tiff *.gif *.jpeg *.jpg *.tif)");
    dialog->setHistory(history);
    if(dialog->exec() == QDialog::Accepted)
    {
        const QString fileName = dialog->selectedFiles().constFirst();
        setCover(QImage(fileName));
        history.append(fileName);
    }
}

void SongEdit::updateMetaData()
{
    // Fetches the meta data from the media player
    QMediaMetaData data = player->metaData();

    // Initializes the name
    QVariant name = data.value(QMediaMetaData::Title);
    if(name.isNull())
    {
        name = data.value(QMediaMetaData::AlbumTitle);
        if(name.isNull())
        {
            name = data.value(QMediaMetaData::Description);
        }
    }
    setName(name.toString());

    // Initializes the image cover
    QVariant cover = data.value(QMediaMetaData::ThumbnailImage);
    if(cover.isNull())
    {
        cover = data.value(QMediaMetaData::CoverArtImage);
    }
    setCover(qvariant_cast<QImage>(cover));

    // Initializes the artist
    QVariant artist = data.value(QMediaMetaData::AlbumArtist);
    if(artist.isNull())
    {
        artist = data.value(QMediaMetaData::Author);
        if(artist.isNull())
        {
            artist = data.value(QMediaMetaData::ContributingArtist);
        }
    }
    setArtist(artist.toString());
}

void SongEdit::controlPlayback()
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

void SongEdit::updatePlaybackControl(bool value)
{
    ui->playbackButton->setToolTip(value ? "Pause" : "Play");
    ui->playbackButton->setIcon(QIcon(QString(":/images/player/%1.png").arg(value ? "pause" : "play")));
}

SongEdit::~SongEdit() {}

IDContainer SongEdit::getID() const
{
    return id;
}

Song::Genre SongEdit::getGenre() const
{
    return static_cast<Song::Genre>(ui->genreEdit->currentIndex());
}

int SongEdit::getReleasedYear() const
{
    return ui->releaseYearEdit->value();
}

QString SongEdit::getArtist() const
{
    return ui->artistEdit->text();
}

QUrl SongEdit::getLocation() const
{
    return QUrl::fromLocalFile(ui->fileNameEdit->text());
}

QImage SongEdit::getCover() const
{
    return ui->coverView->getImage();
}

QString SongEdit::getName() const
{
    return ui->nameEdit->text();
}

Song SongEdit::getValue() const
{
    Song target;
    target.setName(getName());
    target.setCover(getCover());
    target.setGenre(getGenre());
    target.setArtist(getArtist());
    target.setAddress(getLocation());
    target.setPublicationYear(getReleasedYear());
    return target;
}

void SongEdit::setReleasedYear(int value)
{
    ui->releaseYearEdit->setValue(value);
}

void SongEdit::setValue(const IDContainer &value)
{
    id = value;
    Song target = Song::loadFromRecord(value);
    setValue(target);
}

void SongEdit::setValue(const Song &value)
{
    setName(value.name);
    setGenre(value.genre);
    setCover(value.cover);
    setArtist(value.artist);
    setLocation(value.address);
    setReleasedYear(value.publicationYear);
    if(!value.name.isEmpty())
    {
        setWindowTitle(QString("%1 - Song Editor").arg(value.name));
    }
}

void SongEdit::setGenre(Song::Genre value)
{
    ui->genreEdit->setCurrentIndex(static_cast<int>(value));
}

void SongEdit::setName(const QString &value)
{
    ui->nameEdit->setText(value);
}

void SongEdit::setCover(const QImage &value)
{
    ui->coverView->setImage(value);
}

void SongEdit::setArtist(const QString &value)
{
    ui->artistEdit->setText(value);
}

void SongEdit::setLocation(const QUrl &location)
{
    ui->fileNameEdit->setText(location.toString(QUrl::PreferLocalFile));
}

void SongEdit::removeCover()
{
    ui->coverView->setImage(QImage());
}

void SongEdit::accept()
{
    try
    {
        if(!QFile::exists(ui->fileNameEdit->text()) || !QUrl::fromLocalFile(ui->fileNameEdit->text()).isValid())
        {
            throw std::runtime_error("The file name is not valid or exists.");
        }
        else if(ui->nameEdit->text().isEmpty())
        {
            throw std::runtime_error("You have not entered the name.");
        }
        else if(ui->artistEdit->text().isEmpty())
        {
            throw std::runtime_error("You have not entered the artist.");
        }
        const Song &value = this->getValue();
        if(!ID::isValid(id))
        {
            id = Song::generateKey();
        }
        value.saveToRecord(id);
        QDialog::accept();
    }
    catch(std::exception &e)
    {
        QMessageBox::critical(this, "Error!", e.what());
        return;
    }
}
