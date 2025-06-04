#include "include/components/imageview.h"
#include <QImageReader>
#include <QFileDialog>
#include <QPaintEvent>
#include <QMimeData>
#include <QPainter>

void ImageView::openImageFile()
{
    auto dialog = ImageFile::initializeImageDialog(this->parentWidget());
    if(dialog->exec() == QDialog::Accepted)
    {
        const QString fileName = dialog->selectedFiles().constFirst();
        QImageReader reader(fileName);
        setImage(reader.read());
    }
}


void ImageView::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    if(!acceptDrops())
    {
        return;
    }
    openImageFile();
}

void ImageView::dragLeaveEvent(QDragLeaveEvent *event)
{
    dragging = false;
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
            dragging = true;
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
    if(image.isNull())
    {
        image = QImage(":/images/playlist/single-mode.png");
    }
    const QRect &region = event->rect();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.drawImage(region, image, image.rect());

    if(dragging)
    {
        painter.fillRect(event->rect(), QColor(0, 0, 0, 128)); // A little transparent black;
    }
}

void ImageView::dropEvent(QDropEvent *event)
{
    const QMimeData *data = event->mimeData();
    const QUrl target = data->urls().constFirst();
    QImageReader reader(target.toLocalFile());
    this->setImage(reader.read());
    dragging = false;
    event->accept();
    update();
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
