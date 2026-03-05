#ifndef VIDEOSTREAMER_H
#define VIDEOSTREAMER_H

#include <QObject>
#include <QTimer>
#include <opencv2/highgui.hpp>
#include <QImage>
#include <iostream>
#include <QThread>


static cv::VideoCapture cap;
static cv::VideoCapture cap2;
static cv::VideoCapture cap3;
static cv::VideoCapture cap4;


class VideoStreamer: public QObject
{
    Q_OBJECT
public:
    VideoStreamer();
    ~VideoStreamer();

public:
    void streamVideo();
    void streamVideo2();
    void streamVideo3();
    void streamVideo4();

    QThread* threadStreamer = new QThread();
    void catchFrame(cv::Mat emittedFrame);
    void catchFrame2(cv::Mat emittedFrame);
    void catchFrame3(cv::Mat emittedFrame);
    void catchFrame4(cv::Mat emittedFrame);

public slots:
    void openVideoCamera(QString path);

    void streamerThreadSlot();
    //void streamerThreadSlot2();

private:
    cv::Mat frame,frame2,frame3,frame4;
    QTimer tUpdate;
    QTimer tUpdate2;
    QTimer tUpdate3;
    QTimer tUpdate4;
    QString path="",path2="",path3="",path4="";

signals:
    void newImage(QImage &);
    void newImage2(QImage &);
    void newImage3(QImage &);
    void newImage4(QImage &);

    void emitThreadImage(cv::Mat frameThread);
    void emitThreadImage2(cv::Mat frameThread);
    void emitThreadImage3(cv::Mat frameThread);
    void emitThreadImage4(cv::Mat frameThread);

};

#endif // VIDEOSTREAMER_H
