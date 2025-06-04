#ifndef SONGEDIT_H
#define SONGEDIT_H

#include <QDialog>
#include <QMediaPlayer>
#include "include/models/genremodel.h"

namespace Ui
{
    class SongEdit;
}

class QFileDialog;

class SongEdit : public QDialog
{
    Q_OBJECT

protected:
    virtual void dragEnterEvent(QDragEnterEvent *event) override;
    virtual void dragLeaveEvent(QDragLeaveEvent *event) override;
    virtual void dropEvent(QDropEvent *event) override;

private slots:
    void updateSource();
    void openSongFile();
    void updateMetaData();
    void controlPlayback();
    void updatePlaybackControl(bool value);

public:
    Q_DISABLE_COPY_MOVE(SongEdit)
    explicit SongEdit(QWidget *parent = nullptr);
    explicit SongEdit(const Song &song, QWidget *parent = nullptr);

    ~SongEdit();

    Song::Genre getGenre() const;
    int getReleasedYear() const;
    QString getArtist() const;
    QUrl getLocation() const;
    QImage getCover() const;
    QString getName() const;
    Song getSong() const;

public slots:
    void setSong(const Song &value);
    void setReleasedYear(int value);
    void setGenre(Song::Genre value);
    void setName(const QString &value);
    void setCover(const QImage &value);
    void setArtist(const QString &value);
    void setLocation(const QUrl &location);

    virtual void accept() override;
    void removeCover();

signals:
    void nameChanged(QString value);
    void locationChanged(QUrl value);
    void coverChanged(QPixmap value);
    void artistChanged(QString value);
    void releasedYearChanged(int value);
    void genreChanged(Song::Genre value);

private:
    GenreModel model;
    IDContainer id = 0;
    QAudioOutput *output = nullptr;
    QMediaPlayer *player = nullptr;
    std::unique_ptr<Ui::SongEdit> ui;
};

#endif // SONGEDIT_H
