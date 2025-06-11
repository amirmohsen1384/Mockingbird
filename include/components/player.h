#ifndef PLAYER_H
#define PLAYER_H

#include <QWidget>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QAbstractItemModel>
#include "include/core/general.h"
#include "include/models/playlistmodel.h"

namespace Ui
{
    class Player;
}

class Player : public QWidget
{
    Q_OBJECT

public:
    Q_DISABLE_COPY_MOVE(Player)
    explicit Player(QWidget *parent = nullptr);

    QAbstractItemModel* model();
    void setModel(QAbstractItemModel *value);

    bool isPlaying() const;
    bool isShuffleMode() const;
    bool isInfiniteMode() const;
    int getCurrentTrack() const;

private slots:
    void updatePlayer();
    void updateProgress();
    void updatePlaybackSpeed();
    void updatePlaybackControl();
    void toggleShuffleButton(bool value);
    void toggleInfiniteButton(bool value);
    void updateModel(QAbstractItemModel *prev, QAbstractItemModel *current);

public slots:
    void stop();
    void replay();
    void forward();
    void advance();
    void goToNextTrack();
    void goToPreviousTrack();
    void changePlaybackMode();
    void changePlaybackSpeed();
    void setCurrentTrack(int value);
    void setShuffleMode(bool value);
    void setInfiniteMode(bool value);

signals:
    void currentTrackChanged(int value);
    void shuffleModeChanged(bool value);
    void infiniteModeChanged(bool value);

private:
    int currentTrack = 0;
    std::unique_ptr<Ui::Player> ui;
    QAbstractItemModel *_model = nullptr;
    std::unique_ptr<QAudioOutput> output;
    std::unique_ptr<QMediaPlayer> player;
};

#endif // PLAYER_H
