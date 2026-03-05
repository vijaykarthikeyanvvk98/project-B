#include "videostreamer.h"
#include <qdebug.h>
const QString VIDEO_PATH = "C:/Users/vijay/Videos/test2.mp4";
const QString VIDEO_PATH2 = "C:/Users/vijay/Videos/test2.mp4";
const QString VIDEO_PATH3 = "C:/Users/vijay/Videos/test2.mp4";
const QString VIDEO_PATH4 = "C:/Users/vijay/Videos/Captures/ORCA 2026-02-16 12-22-41.mp4";

VideoStreamer::VideoStreamer()
{
    connect(&tUpdate,&QTimer::timeout,this,&VideoStreamer::streamVideo);
    connect(&tUpdate2,&QTimer::timeout,this,&VideoStreamer::streamVideo2);
    connect(&tUpdate3,&QTimer::timeout,this,&VideoStreamer::streamVideo3);
    connect(&tUpdate4,&QTimer::timeout,this,&VideoStreamer::streamVideo4);

}

VideoStreamer::~VideoStreamer()
{
    cap.release();
    cap2.release();
    cap3.release();
    cap4.release();

    tUpdate.stop();
    tUpdate2.stop();
    tUpdate3.stop();
    tUpdate4.stop();

    threadStreamer->requestInterruption();
}

void VideoStreamer::streamVideo()
{

    if(frame.data)
    {
        QImage img = QImage(frame.data,frame.cols,frame.rows,QImage::Format_RGB888).rgbSwapped();
        emit newImage(img);
    }
}

void VideoStreamer::streamVideo2()
{

    if(frame2.data)
    {
        QImage img = QImage(frame2.data,frame2.cols,frame2.rows,QImage::Format_RGB888).rgbSwapped();
        emit newImage2(img);
    }
    else
        qDebug()<<"Frame Empty";
}

void VideoStreamer::streamVideo3()
{

    if(frame3.data)
    {
        QImage img = QImage(frame3.data,frame3.cols,frame3.rows,QImage::Format_RGB888).rgbSwapped();
        emit newImage3(img);
    }
    else
        qDebug()<<"Frame Empty";
}

void VideoStreamer::streamVideo4()
{

    if(frame4.data)
    {
        QImage img = QImage(frame4.data,frame4.cols,frame4.rows,QImage::Format_RGB888).rgbSwapped();
        emit newImage4(img);
    }
    //qDebug()<<"image";

}

void VideoStreamer::catchFrame(cv::Mat emittedFrame)
{
    frame  = emittedFrame;
}

void VideoStreamer::catchFrame2(cv::Mat emittedFrame)
{
    frame2 = emittedFrame;
    //qDebug()<<"Helo";
}

void VideoStreamer::catchFrame3(cv::Mat emittedFrame)
{
    frame3 = emittedFrame;
    //qDebug()<<"Helo 3";
}

void VideoStreamer::catchFrame4(cv::Mat emittedFrame)
{
    frame4 = emittedFrame;

}

void VideoStreamer::openVideoCamera(QString path)
{
    /*for(int i=1;i<20;i++)
    {
        cap.open(i);

        if(cap.isOpened())
            break;
    }*/
    cap.open(0);

    if(!cap.isOpened())
        qDebug()<<"Error";

    /*if(path.length() == 1)
        cap2.open(1);
    else
        cap2.open(path.toStdString());*/
    //cap2.open(1);
    for(int i=1;i<20;i++)
    {
        cap2.open(i);

        if(cap2.isOpened())
            break;
    }
    if(!cap2.isOpened())
        qDebug()<<"Error";


    /*if(path.length() == 1)
        cap3.open(path.toInt());
    else
        cap3.open(path.toStdString());*/
    cap3.open(VIDEO_PATH3.toStdString());
    if(!cap3.isOpened())
        qDebug()<<"Error";

    /*if(path.length() == 1)
        cap4.open(path.toInt());
    else
        cap4.open(path.toStdString());*/
    cap4.open(VIDEO_PATH4.toStdString());

    if(!cap4.isOpened())
        qDebug()<<"Error";


    VideoStreamer* worker = new VideoStreamer();
    worker->moveToThread(threadStreamer);
    QObject::connect(threadStreamer,SIGNAL(started()),worker,SLOT(streamerThreadSlot()));
    QObject::connect(worker,&VideoStreamer::emitThreadImage,this,&VideoStreamer::catchFrame);
    QObject::connect(worker,&VideoStreamer::emitThreadImage2,this,&VideoStreamer::catchFrame2);
    QObject::connect(worker,&VideoStreamer::emitThreadImage3,this,&VideoStreamer::catchFrame3);
    QObject::connect(worker,&VideoStreamer::emitThreadImage4,this,&VideoStreamer::catchFrame4);

    threadStreamer->start();

    if(cap.isOpened())
    {
        double fps = cap.get(cv::CAP_PROP_FPS);
        tUpdate.start(1000/fps);
    }
    if(cap2.isOpened())
    {
    double fps2 = cap2.get(cv::CAP_PROP_FPS);
        tUpdate2.start(1000/fps2);
    }
    if(cap3.isOpened())
    {
    double fps3 = cap3.get(cv::CAP_PROP_FPS);
        //qDebug()<<fps3;
        tUpdate3.start(1000/fps3);
    }
    if(cap4.isOpened())
    {
    double fps4 = cap4.get(cv::CAP_PROP_FPS);
        tUpdate4.start(1000/fps4);
    }


}

void VideoStreamer::streamerThreadSlot()
{
    cv::Mat tempFrame,tempFrame2,tempFrame3,tempFrame4;

    while (!QThread::currentThread()->isInterruptionRequested()) {
        if(cap.isOpened())
        cap>>tempFrame;
        if(cap2.isOpened())
        cap2>>tempFrame2;
        if(cap3.isOpened())
        cap3>>tempFrame3;
        if(cap4.isOpened())
        cap4>>tempFrame4;

        if(tempFrame.data || tempFrame2.data || tempFrame3.data || tempFrame4.data)
        {
        emit emitThreadImage(tempFrame);
            emit emitThreadImage2(tempFrame2);
        emit emitThreadImage3(tempFrame3);
            emit emitThreadImage4(tempFrame4);

        }
        /*if(tempFrame.data)
        emit emitThreadImage2(tempFrame);
        if(tempFrame.data)
        emit emitThreadImage3(tempFrame);
        if(tempFrame.data)
        emit emitThreadImage4(tempFrame);*/

        if(QThread::currentThread()->isInterruptionRequested())
        {
            cap.release();
            cap2.release();
            cap3.release();
            cap4.release();
            return;
        }

    }
}
