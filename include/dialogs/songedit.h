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
private:
    QFileDialog* browseFile();

private slots:
    void updateSource();
    void openSongFile();
    void openImageFile();
    void updateMetaData();
    void controlPlayback();
    void updatePlaybackControl(bool value);

public:
    Q_DISABLE_COPY_MOVE(SongEdit)
    explicit SongEdit(QWidget *parent = nullptr);
    explicit SongEdit(const Song &value, QWidget *parent = nullptr);

    ~SongEdit();

    Song::Genre getGenre() const;
    int getReleasedYear() const;
    QString getArtist() const;
    QUrl getLocation() const;
    QPixmap getCover() const;
    QString getName() const;
    Song getValue() const;

public slots:
    void setReleasedYear(int value);
    void setValue(const Song &value);
    void setGenre(Song::Genre value);
    void setName(const QString &value);
    void setCover(const QPixmap &value);
    void setArtist(const QString &value);
    void setLocation(const QUrl &location);

    void removeCover();
    virtual void accept() override;

signals:
    void nameChanged(QString value);
    void locationChanged(QUrl value);
    void coverChanged(QPixmap value);
    void artistChanged(QString value);
    void releasedYearChanged(int value);
    void genreChanged(Song::Genre value);

private:
    GenreModel model;
    std::unique_ptr<Ui::SongEdit> ui;
    std::unique_ptr<QAudioOutput> output;
    std::unique_ptr<QMediaPlayer> player;
};

#endif // SONGEDIT_H
