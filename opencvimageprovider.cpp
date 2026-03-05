#include "opencvimageprovider.h"
#include <QMutexLocker>
OpencvImageProvider::OpencvImageProvider(QObject *parent)
    : QQuickImageProvider(QQuickImageProvider::Image)
{
    image = QImage(200, 200, QImage::Format_RGB32);
    image.fill(QColor("black"));
    image2 = QImage(200, 200, QImage::Format_RGB32);
    image2.fill(QColor("black"));
    image3 = QImage(200, 200, QImage::Format_RGB32);
    image3.fill(QColor("black"));
    image4 = QImage(200, 200, QImage::Format_RGB32);
    image4.fill(QColor("black"));
}

QImage OpencvImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    //Q_UNUSED(id); // ID is not used here as we serve a single stream
    QMutexLocker locker(&mutex);
    //qDebug()<<id;
    /*switch (id.toInt()) {
    case 0:
        resultant_image = this->image;
        break;
    case 1:
        resultant_image = this->image2;
        break;
    case 2:
        resultant_image = this->image3;
        break;
    case 3:
        resultant_image = this->image4;
        break;
    default:
        resultant_image = this->image;
        break;
    }*/
    if (size) {
        *size = resultant_image.size();
    }

    // If QML requests a specific size, scale the image while maintaining aspect ratio
    if (requestedSize.width() > 0 && requestedSize.height() > 0) {
        return resultant_image.scaled(requestedSize.width(), requestedSize.height(), Qt::KeepAspectRatio);
    }

    return resultant_image;
}

void OpencvImageProvider::updateImage(const QImage &image)
{
    if (!image.isNull() && this->image != image) {
        this->image = image;
        resultant_image = this->image;
        emit imageChanged();
    }

    else if (image.isNull()) {
        emit null_image_changed();
    }
}

void OpencvImageProvider::updateImage2(const QImage &image)
{
    if (!image.isNull() && this->image2 != image) {
        this->image2 = image;
        resultant_image = this->image2;
        emit imageChanged2();
        //qDebug()<<"Frame 2";

    }

    else if (image2.isNull()) {
        emit null_image_changed();
    }
}

void OpencvImageProvider::updateImage3(const QImage &image)
{
    if (!image.isNull() && this->image3 != image) {
        this->image3 = image;
        resultant_image = this->image3;
        emit imageChanged3();
        //qDebug()<<"Frame 3";

    }

    else if (image3.isNull()) {
        emit null_image_changed();
    }
}

void OpencvImageProvider::updateImage4(const QImage &image)
{
    if (!image.isNull() && this->image4 != image) {
        this->image4 = image;
        resultant_image = this->image4;
        emit imageChanged4();
        //qDebug()<<"Frame 4";

    }

    else if (image4.isNull()) {
        emit null_image_changed();
    }
    //qDebug()<<"Img";
}
