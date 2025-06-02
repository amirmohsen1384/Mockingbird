#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include "include/core/general.h"
#include <QWidget>
#include <QImage>
#include <QUrl>

class ImageView : public QWidget
{
    Q_OBJECT
    QImage image;
    QUrl draggingUrl;

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    virtual void dragLeaveEvent(QDragLeaveEvent *event) override;
    virtual void dragEnterEvent(QDragEnterEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void dropEvent(QDropEvent *event) override;
    virtual void updateImage();

public:
    Q_DISABLE_COPY_MOVE(ImageView)
    ImageView(QWidget *parent = nullptr);
    ImageView(const QImage &image, QWidget *parent = nullptr);

    QImage getImage() const;

public slots:
    void setImage(const QImage &image);

signals:
    void viewActivated();
    void imageChanged(const QImage &image);
};

#endif // IMAGEVIEW_H
