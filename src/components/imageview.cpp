#include "include/components/imageview.h"
#include <QImageReader>
#include <QPaintEvent>
#include <QMimeData>
#include <QPainter>

void ImageView::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    emit viewActivated();
}

void ImageView::dragLeaveEvent(QDragLeaveEvent *event)
{
    draggingUrl = QUrl();
    update();
    event->accept();
}

void ImageView::dragEnterEvent(QDragEnterEvent *event)
{
    const QMimeData *data = event->mimeData();
    if(data->urls().size() != 1)
    {
        event->ignore();
    }
    else
    {
        const QUrl target = data->urls().constFirst();
        if(ImageFile::isValid(target) && target.isLocalFile())
        {
            event->acceptProposedAction();
            draggingUrl = target;
            update();
        }
        else
        {
            event->ignore();
        }
    }
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

    if(!draggingUrl.isEmpty())
    {
        painter.fillRect(event->rect(), QColor(0, 0, 0, 128)); // A little transparent black;
    }
}

void ImageView::dropEvent(QDropEvent *event)
{
    const QMimeData *data = event->mimeData();
    if(data->urls().size() != 1)
    {
        event->ignore();
    }
    else
    {
        const QUrl target = data->urls().constFirst();
        QImageReader reader(target.toLocalFile());
        this->setImage(reader.read());
        draggingUrl = QUrl();
        update();
        event->accept();
    }
}

void ImageView::updateImage()
{
    update();
}

ImageView::ImageView(QWidget *parent) : QWidget(parent)
{
    connect(this, &ImageView::imageChanged, this, &ImageView::updateImage);
    setAcceptDrops(true);
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
