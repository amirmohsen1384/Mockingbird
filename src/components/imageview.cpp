#include "include/components/imageview.h"
#include <QPaintEvent>
#include <QPainter>

void ImageView::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    emit viewActivated();
}

void ImageView::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QImage image = this->image;
    const QRect &region = event->rect();

    QPainter painter(this);
    if(image.isNull()) {
        painter.fillRect(region, Qt::black);
        return;
    }

    painter.drawImage(region, image, image.rect());
}

void ImageView::updateImage()
{
    update();
}

ImageView::ImageView(QWidget *parent) : QWidget(parent)
{
    connect(this, &ImageView::imageChanged, this, &ImageView::updateImage);
}
ImageView::ImageView(const QImage &image, QWidget *parent) : ImageView(parent)
{
    this->setImage(image);
}

QImage ImageView::getImage() const
{
    return image;
}


void ImageView::setImage(const QImage &image)
{
    this->image = image;
    emit imageChanged(image);
}
