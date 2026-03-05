#ifndef OPENCVIMAGEPROVIDER_H
#define OPENCVIMAGEPROVIDER_H

#include <QImage>
#include <QObject>
#include <QQuickImageProvider>
#include <QMutex>
class OpencvImageProvider :public QQuickImageProvider
{
    Q_OBJECT
public:
    OpencvImageProvider(QObject *parent = nullptr);

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

public slots:
    void updateImage(const QImage &image);
    void updateImage2(const QImage &image);
    void updateImage3(const QImage &image);
    void updateImage4(const QImage &image);

signals:
    void imageChanged();
    void imageChanged2();
    void imageChanged3();
    void imageChanged4();

    void null_image_changed();

private:
    QImage image;
    QImage image2;
    QImage image3;
    QImage image4;
    QImage resultant_image;
    QMutex mutex;
};

#endif // OPENCVIMAGEPROVIDER_H
