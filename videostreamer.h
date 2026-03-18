#ifndef VIDEOSTREAMER_H
#define VIDEOSTREAMER_H

#include <QObject>
#include <QTimer>
#include <opencv2/highgui.hpp>
#include <QImage>
#include <QThread>
#include <qdir.h>
#include <qmutex.h>


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

    QThread* threadStreamer=nullptr,*threadStreamer2=nullptr;
    void catchFrame(cv::Mat emittedFrame);
    void catchFrame2(cv::Mat emittedFrame);
    void catchFrame3(cv::Mat emittedFrame);
    void catchFrame4(cv::Mat emittedFrame);

public slots:
    void openVideoCamera(QString path);
    void start_recording();
    void stop_recording();
    void streamerThreadSlot();
    void image_stitching();
    void set_image_stitching_path(QString,int);
    //void streamerThreadSlot2();

private:
    QTimer tUpdate;
    QTimer tUpdate2;
    QTimer tUpdate3;
    QTimer tUpdate4;
    QString path="",path2="",path3="",path4="";
    //int FPS;
    //PostProcessing *post;
    double yaw;
    int width = 640;  // Replace with your desired width
    int height = 480; // Replace with your desired height
    //QString FPS;
    QString from = "", to = "", selected_time = "";
    QDateTime date;
    QString formattedTime;
    QByteArray formattedTimeMsg;
    int framer_rate = 0;
    int FPS_count = 0;
    qint64 lastTimeStamp;
    qint64 currentTime = 0, elapsedTime;
    int thickness = 4;
    QString FPS, YAW, PITCH, ROLL, TEMP, PRESSURE, DEPTH, BATTERY;
    int pitch_spacing = 0;
    double angle_rad =0.0;
    cv::Point textPos;
    QFile subtitleFile;
    QTextStream out;
    QMutex frameMutex,frameMutex2,frameMutex3,frameMutex4;

signals:
    void newImage(QImage &);
    void newImage2(QImage &);
    void newImage3(QImage &);
    void newImage4(QImage &);



    void recording_stop();
    void image_stitched();

};

class Worker: public QObject
{
    Q_OBJECT

public:
    explicit Worker(QObject *parent = nullptr);
    ~Worker();

public slots:
    void grabFrame();
    void write_Frame();
signals:
    void emitThreadImage(cv::Mat frameThread);
    void emitThreadImage2(cv::Mat frameThread);
    void emitThreadImage3(cv::Mat frameThread);
    void emitThreadImage4(cv::Mat frameThread);
};


#endif // VIDEOSTREAMER_H
