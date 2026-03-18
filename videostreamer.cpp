#include "videostreamer.h"
#include <qdebug.h>
#include <qelapsedtimer.h>
#include <QStandardPaths>
#include <opencv2/videoio.hpp> // Video write
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/opencv.hpp>
#include <qmutex.h>
#include <opencv2/stitching/detail/blenders.hpp>
#include <opencv2/stitching/detail/camera.hpp>

using namespace cv;
QMutex writeMutex;
const QString VIDEO_PATH = "C:/Users/vijay/Videos/test2.mp4";
const QString VIDEO_PATH2 = "C:/Users/vijay/Videos/test2.mp4";
const QString VIDEO_PATH3 = "C:/Users/vijay/Videos/test2.mp4";
const QString VIDEO_PATH4 = "C:/Users/vijay/Videos/Captures/ORCA 2026-02-16 12-22-41.mp4";
static bool recording_status=false,recording_status2=false,recording_status3=false,recording_status4=false;
static VideoWriter video,writing_video,video2,video3,video4;
double fps=25.00,fps2=25.00,fps3=25.00,fps4=25.00;
static int frame_count = 0;
static QString stitching_path = "",stitching_path2 = "";

static int calculated_difference = 0;


static double writing_fps = 0.0;
//AHRS
static qfloat16 yaw1 = 0.0;
static qfloat16 pitch1 = 0.0;
static qfloat16 roll1 = 0.0;
static qfloat16 yaw2 = 0;
static qfloat16 pitch2 = 0;
static qfloat16 roll2 = 0;
int battery_percentage=0;
//Sensors
static qfloat16 press = 0;
static qfloat16 temp = 0;
static qfloat16 depth = 0;
QElapsedTimer timer_1;
//QElapsedTimer timer;
cv::Mat frame,newFrame;
QTimer tUpdate;
QString streaming_path = "";
std::string outputPathStdString="",outputPathStdString2="",outputPathStdString3="",outputPathStdString4="";
VideoStreamer *worker2;
VideoStreamer *worker ;
int video_quality=144;
int video_quality_x=144;
int video_quality_y=144;
int frame_width=0;
int frame_height=0;
int frame_width2=0;
int frame_height2=0;
int frame_width3=0;
int frame_height3=0;
int frame_width4=0;
int frame_height4=0;
QString record_type="";
QString path ="";
static int frame_y_depth=0;
static bool one_time_depth=true;
static int depth_count=0;
static int sub_i=1;
static int sub_i2=2;
QTimer sub_timer;
QString subtitle_path = "";
int delay = 40;
bool run_once=true;
QTimer *timer=nullptr,*timer2=nullptr;
cv::Mat tempFrame,tempFrame2,tempFrame3,tempFrame4;
cv::Mat frame2,frame3,frame4;
static cv::Mat writeFrame,writeFrame2,writeFrame3,writeFrame4;

VideoStreamer::VideoStreamer()
{
    threadStreamer  = new QThread();
    threadStreamer2 = new QThread();
    connect(&tUpdate,&QTimer::timeout,this,&VideoStreamer::streamVideo);
    connect(&tUpdate2,&QTimer::timeout,this,&VideoStreamer::streamVideo2);
    connect(&tUpdate3,&QTimer::timeout,this,&VideoStreamer::streamVideo3);
    connect(&tUpdate4,&QTimer::timeout,this,&VideoStreamer::streamVideo4);
    timer = new QTimer(this);
    timer2 = new QTimer(this);
}

VideoStreamer::~VideoStreamer()
{
    recording_status = false;
    recording_status2 = false;
    recording_status3 = false;
    recording_status4 = false;

    //QThread::msleep(2000);
    if (video.isOpened()) {
        video.release();
        //export_video();

    }

    if (video2.isOpened()) {
        video2.release();
        //export_video();

    }

    if (video3.isOpened()) {
        video3.release();
        //export_video();

    }

    if (video4.isOpened()) {
        video4.release();
        //export_video();

    }

    if (writing_video.isOpened())
        writing_video.release();

    if(subtitleFile.isOpen())
    {
        if(sub_timer.isActive())
        {
            sub_timer.stop();
        }
        out.flush();
        subtitleFile.close();
    }
    if(threadStreamer && threadStreamer->isRunning())
    {
        threadStreamer->requestInterruption();  // Signal worker loop to stop
        threadStreamer->quit();
        threadStreamer->wait();
    }
    if(threadStreamer2 && threadStreamer2->isRunning())
    {
        threadStreamer2->requestInterruption();  // Signal worker loop to stop
        threadStreamer2->quit();
        threadStreamer2->wait();
    }

    if (cap.isOpened())
    cap.release();
    if (cap2.isOpened())
    cap2.release();
    if (cap3.isOpened())
    cap3.release();
    if (cap4.isOpened())
    cap4.release();

    if(tUpdate.isActive())
    tUpdate.stop();
    if(tUpdate2.isActive())
    tUpdate2.stop();
    if(tUpdate3.isActive())
    tUpdate3.stop();
    if(tUpdate4.isActive())
    tUpdate4.stop();
     //threadStreamer->quit();
     //threadStreamer->wait();
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
        ;//qDebug()<<"Frame Empty";
}

void VideoStreamer::streamVideo3()
{

    if(frame3.data)
    {
        QImage img = QImage(frame3.data,frame3.cols,frame3.rows,QImage::Format_RGB888).rgbSwapped();
        emit newImage3(img);
    }
    else
        ;//qDebug()<<"Frame Empty";
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
    frame = emittedFrame;
    QMutexLocker locker(&frameMutex);
    writeFrame = emittedFrame;
}

void VideoStreamer::catchFrame2(cv::Mat emittedFrame)
{
    frame2 = emittedFrame;
    QMutexLocker locker(&frameMutex2);
    writeFrame2 = emittedFrame;

    //qDebug()<<"Helo";

}

void VideoStreamer::catchFrame3(cv::Mat emittedFrame)
{
    frame3 = emittedFrame;
    QMutexLocker locker(&frameMutex3);
    writeFrame3 = emittedFrame;

    //qDebug()<<"Helo 3";
}

void VideoStreamer::catchFrame4(cv::Mat emittedFrame)
{
    frame4 = emittedFrame;
    QMutexLocker locker(&frameMutex4);
    writeFrame4 = emittedFrame;

}

void VideoStreamer::openVideoCamera(QString path)
{
    /*for(int i=1;i<20;i++)
    {
        cap.open(i);

        if(cap.isOpened())
            break;
    }*/
    cap.open(0/*, cv::CAP_FFMPEG*/);

    if(!cap.isOpened())
        qDebug()<<"Error";

    /*if(path.length() == 1)
        cap2.open(1);
    else
        cap2.open(path.toStdString());*/
    //cap2.open(1);
    /*for(int i=1;i<20;i++)
    {
        cap2.open(i);

        if(cap2.isOpened())
            break;
    }*/
    //cap2.open("rtsp://admin:Vikra%40123@192.168.56.50:554/video/live?channel=1&subtype=0", cv::CAP_FFMPEG);
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


    Worker *worker1= new Worker();
    Worker *worker2= new Worker();
    worker1->moveToThread(threadStreamer);
    worker2->moveToThread(threadStreamer2);
    cap.set(cv::CAP_PROP_BUFFERSIZE, 1);
    cap2.set(cv::CAP_PROP_BUFFERSIZE, 1);
    cap3.set(cv::CAP_PROP_BUFFERSIZE, 1);
    cap4.set(cv::CAP_PROP_BUFFERSIZE, 1);

    calculated_difference = 1000 / cap.get(cv::CAP_PROP_FPS);
    if (calculated_difference >= 1000 || calculated_difference <= 0)
        calculated_difference = 1000 / 40;

    if(cap.isOpened())
    {
        if (cap.get(cv::CAP_PROP_FPS) <= 0)
        {
            //timer->setInterval(1000/10);
            tUpdate.start(1000 / 40);
            //timer2->setInterval(1000/40);

            fps = 25;
        }
        else
        {
            //timer->setInterval(1000/cap.get(cv::CAP_PROP_FPS));
            //timer2->setInterval((1000/cap.get(cv::CAP_PROP_FPS)));
            tUpdate.start(1000 /cap.get(cv::CAP_PROP_FPS));
            fps=cap.get(cv::CAP_PROP_FPS);

        }
    }
    if(cap2.isOpened())
    {
        if (cap2.get(cv::CAP_PROP_FPS) <= 0)
        {
            tUpdate2.start(1000 / 40);
            fps2 = 25;
        }
        else
        {
            fps2 = cap2.get(cv::CAP_PROP_FPS);
            tUpdate2.start(1000/fps2);
        }
    }
    if(cap3.isOpened())
    {
        if (cap3.get(cv::CAP_PROP_FPS) <= 0)
        {
            tUpdate3.start(1000 / 40);
            fps3 = 25;
        }
        else
        {
            fps3 = cap3.get(cv::CAP_PROP_FPS);
            tUpdate3.start(1000/fps3);
        }
    }
    if(cap4.isOpened())
    {
        if (cap4.get(cv::CAP_PROP_FPS) <= 0)
        {
            tUpdate4.start(1000 / 40);
            fps4 = 25;
        }
        else
        {
            fps4 = cap4.get(cv::CAP_PROP_FPS);
            tUpdate4.start(1000/fps4);
        }
    }

    timer->setInterval(1000/cap.get(cv::CAP_PROP_FPS));
    timer2->setInterval(1000/40);

    connect(timer, &QTimer::timeout, worker1, &Worker::grabFrame);
    connect(timer2, &QTimer::timeout, worker2, &Worker::write_Frame);

    QObject::connect(threadStreamer, SIGNAL(started()), timer, SLOT(start()));
    QObject::connect(threadStreamer2, SIGNAL(started()), timer2, SLOT(start()));
    QObject::connect(worker1, &Worker::emitThreadImage, this, &VideoStreamer::catchFrame);
    QObject::connect(worker1, &Worker::emitThreadImage2, this, &VideoStreamer::catchFrame2);
    QObject::connect(worker1, &Worker::emitThreadImage3, this, &VideoStreamer::catchFrame3);
    QObject::connect(worker1, &Worker::emitThreadImage4, this, &VideoStreamer::catchFrame4);


    threadStreamer->start();



}

void VideoStreamer::start_recording()
{
    /*if(subtitleFile.isOpen())
    {
        sub_timer.stop();
        out.flush();
        subtitleFile.close();
        sub_i=1;
        sub_i2=2;
    }*/
    /*recording_status = true;
    recording_status2 = true;
    recording_status3 = true;
    recording_status4 = true;*/

    if(frame_width == 0)
    {
        frame_width= cap.get(cv::CAP_PROP_FRAME_WIDTH);
        frame_height= cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    }
    if(frame_width2 == 0)
    {
        frame_width2= cap2.get(cv::CAP_PROP_FRAME_WIDTH);
        frame_height2= cap2.get(cv::CAP_PROP_FRAME_HEIGHT);
    }
    if(frame_width3 == 0)
    {
        frame_width3= cap3.get(cv::CAP_PROP_FRAME_WIDTH);
        frame_height3= cap3.get(cv::CAP_PROP_FRAME_HEIGHT);
    }
    if(frame_width4 == 0)
    {
        frame_width4= cap4.get(cv::CAP_PROP_FRAME_WIDTH);
        frame_height4= cap4.get(cv::CAP_PROP_FRAME_HEIGHT);
    }

    // Get the target directory path for saving files.
    QString targetDirectory = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);
    // Modify the target directory to add a subdirectory for your files.
    QString logFileDir = targetDirectory + "/Pipeline";
    QDir().mkpath(logFileDir);
    QDateTime currentDateTime1 = QDateTime::currentDateTime();
    formattedTime = currentDateTime1.toString("dd.MM.yyyy-hh.mm.ss");
    QString filepath5 = logFileDir + "/Recording - "+ formattedTime;
    //qDebug()<<filepath5;
    QDir().mkpath(filepath5);
    QString filepath = filepath5 + "/Camera 1-";
    QString filepath2 = filepath5 + "/Camera 2-";
    QString filepath3 = filepath5 + "/Camera 3-";
    QString filepath4 = filepath5 + "/Camera 4-";

    // Create the directory if it doesn't exist.
    /*QDir().mkpath(logFileDir);
    QDir().mkpath(logFileDir);
    QDir().mkpath(logFileDir);
    QDir().mkpath(logFileDir);*/

    if(path != "")
        logFileDir = path;
    QDateTime currentDateTime = QDateTime::currentDateTime();
    formattedTime = currentDateTime.toString("dd.MM.yyyy-hh.mm.ss");
    QString outputPath="",outputPath5="",outputPath3="",outputPath4="";
    // Construct the file paths relative to the target directory.
    if(record_type == "")
    {
        outputPath = filepath+ formattedTime + ".mp4";
        outputPath5 = filepath2+ formattedTime + ".mp4";
        outputPath3 = filepath3+ formattedTime + ".mp4";
        outputPath4 = filepath4+ formattedTime + ".mp4";

    }
    else
    {
        outputPath = filepath + "/Recording-" + formattedTime + "." + record_type;
        outputPath5 = filepath2 + "/Recording-" + formattedTime + "." +record_type;
        outputPath3 = filepath3 + "/Recording-" + formattedTime + "." +record_type;
        outputPath4 = filepath4 + "/Recording-" + formattedTime + "." +record_type;

    }

   /* QString outputPath2 = logFileDir + "/Recording-" + formattedTime + ".ass";
    subtitleFile.setFileName(outputPath2);*/

    outputPathStdString= outputPath.toStdString();
    outputPathStdString2= outputPath5.toStdString();
    outputPathStdString3= outputPath3.toStdString();
    outputPathStdString4= outputPath4.toStdString();

    /*qDebug()<<outputPathStdString;
    qDebug()<<outputPathStdString2;
    qDebug()<<outputPathStdString3;
    qDebug()<<outputPathStdString4;*/

    /*if (subtitleFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        out.setDevice(&subtitleFile);
        // Write the ASS file header and styles
        out << "[Script Info]\n";
        out << "Title: Example ASS File\n";
        out << "Original Script: OpenAI\n";
        out << "ScriptType: v4.00+\n";
        out << "Collisions: Normal\n";
        out << "PlayDepth: 0\n\n";

        out << "[V4+ Styles]\n";
        out << "Format: Name, Fontname, Fontsize, PrimaryColour, SecondaryColour, BackColour, Bold, Italic, BorderStyle, Outline, Shadow, Alignment, MarginL, MarginR, MarginV, Encoding\n";
        out << "Style: Default,Arial,20,&H00FFFFFF,&H00000000,&H00000000,-1,0,1,1.0,0.0,2,10,10,10,1\n\n";
        out << "[Events]\n";
        out << "Format: Marked, Start, End, Style, Name, MarginL, MarginR, MarginV, Effect, Text\n";
    }*/
    video.open(outputPathStdString,
               //cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
               cv::VideoWriter::fourcc('M', 'P', '4', 'V'),
               fps,
               Size(frame_width,frame_height),
               true);

    video2.open(outputPathStdString2,
               //cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
               cv::VideoWriter::fourcc('M', 'P', '4', 'V'),
                fps2,
               Size(frame_width2,frame_height2),
               true);

    video3.open(outputPathStdString3,
               //cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
               cv::VideoWriter::fourcc('M', 'P', '4', 'V'),
                fps3,
               Size(frame_width3,frame_height3),
               true);

    video4.open(outputPathStdString4,
               //cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
               cv::VideoWriter::fourcc('M', 'P', '4', 'V'),
                fps4,
               Size(frame_width4,frame_height4),
               true);
    //recording_status=true;

    if (!video.isOpened() ) {
        qDebug() << "Error: Could not open the video writer 1.";
        recording_status = false;
    }
    else
    {
        recording_status = true;

    }
    if (!video2.isOpened() ) {
        qDebug() << "Error: Could not open the video writer 2.";
        recording_status2 = false;
    }
    else
    {
        recording_status2 = true;

    }
    if (!video3.isOpened() ) {
        qDebug() << "Error: Could not open the video writer 3.";
        recording_status3 = false;
    }
    else
    {
        recording_status3 = true;

    }
    if (!video4.isOpened() ) {
        qDebug() << "Error: Could not open the video writer 4.";
        recording_status4 = false;
    }
    else
    {
        recording_status4 = true;

    }
    //sub_timer.start(1000);
    threadStreamer2->start();

}

void VideoStreamer::stop_recording()
{
    recording_status = false;
    recording_status2 = false;
    recording_status3 = false;
    recording_status4 = false;

    QThread::msleep(400);
    if (video.isOpened()) {
        video.release();
        //export_video();

    }

    if (video2.isOpened()) {
        video2.release();
        //export_video();

    }

    if (video3.isOpened()) {
        video3.release();
        //export_video();

    }

    if (video4.isOpened()) {
        video4.release();
        //export_video();

    }

    else
    {
    }

    emit recording_stop();
}

void VideoStreamer::streamerThreadSlot()
{
    /*cv::Mat tempFrame,tempFrame2,tempFrame3,tempFrame4;

    while (!QThread::currentThread()->isInterruptionRequested()) {
        if(cap.isOpened())
        {
            if(!cap.read(tempFrame))
                continue;
            else
            {
                emit emitThreadImage(tempFrame);

            if (recording_status)
            {
                //std::this_thread::sleep_for(std::chrono::milliseconds(delay));
                //qDebug()<<delay;
                video.write(tempFrame);
            }
            }
        }
        if(cap2.isOpened())
        {
            if(!cap2.read(tempFrame2))
                continue;
            else
            {
                emit emitThreadImage2(tempFrame2);

                if (recording_status2)
                {
                    //std::this_thread::sleep_for(std::chrono::milliseconds(delay));
                    //qDebug()<<delay;
                    video2.write(tempFrame2);
                }
            }
        }
        if(cap3.isOpened())
        {
            if(!cap3.read(tempFrame3))
                continue;
            else
            {
                emit emitThreadImage3(tempFrame3);

                if (recording_status3)
                {
                    //std::this_thread::sleep_for(std::chrono::milliseconds(delay));
                    //qDebug()<<delay;
                    video3.write(tempFrame3);
                }
            }
        }
        if(cap4.isOpened())
        {
            if(!cap4.read(tempFrame4))
                continue;
            else
            {
                emit emitThreadImage4(tempFrame4);

                if (recording_status4)
                {
                    //std::this_thread::sleep_for(std::chrono::milliseconds(delay));
                    //qDebug()<<delay;
                    video4.write(tempFrame4);
                }
            }
        }
        /*if(tempFrame.data || tempFrame2.data || tempFrame3.data || tempFrame4.data)
        {
        emit emitThreadImage(tempFrame);
            emit emitThreadImage2(tempFrame2);
        emit emitThreadImage3(tempFrame3);
            emit emitThreadImage4(tempFrame4);

        }*/


        /*if(tempFrame.data)
        emit emitThreadImage2(tempFrame);
        if(tempFrame.data)
        emit emitThreadImage3(tempFrame);
        if(tempFrame.data)
        emit emitThreadImage4(tempFrame);*/

        /*if(QThread::currentThread()->isInterruptionRequested())
        {
            cap.release();
            cap2.release();
            cap3.release();
            cap4.release();
            return;
        }

    }*/
}

void VideoStreamer::image_stitching()
{
    //qDebug()<<stitching_path2;
    if(stitching_path2.isEmpty())
    {
        qDebug() << "Stitching path is empty";
        return;
    }
    else
        ;//qDebug()<<stitching_path;

    QDir dir(stitching_path);
    QDateTime currentDateTime1 = QDateTime::currentDateTime();
    formattedTime = currentDateTime1.toString("dd.MM.yyyy-hh.mm.ss");
    QString filepath5 = stitching_path2 + "/Processed Image - "+ formattedTime;
    QString outputPath=filepath5+ ".jpg";
    QStringList filters;
    filters << "*.jpg" << "*.png" << "*.jpeg" << "*.bmp";

    QFileInfoList fileList = dir.entryInfoList(filters, QDir::Files);

    if(fileList.size() < 4)
    {
        //qDebug() << "Less than 4 images found";
        return;
    }

    std::vector<cv::Mat> imgs;

    for(int i = 0; i < 4; i++)
    {
        QString filePath = fileList[i].absoluteFilePath();

        //qDebug() << "Reading:" << filePath;

        cv::Mat img = cv::imread(filePath.toStdString());

        if(img.empty())
        {
            //qDebug() << "Failed to read image:" << filePath;
            continue;
        }

        imgs.push_back(img);
    }
    /*for(int i = 0; i < imgs.size(); i++)
    {
        qDebug() << "Image" << i << ":" << imgs[i].cols << imgs[i].rows;
    }*/
    //qDebug() << "Total images loaded:" << imgs.size();
    int w = imgs[0].cols;
    int h = imgs[0].rows;
    int overlap = 20;

    // 1. SHAVE: Crop 2-5 pixels off the left and right of EVERY image
    // This removes the "black border" source artifacts.
    int shave = 5;
    cv::Rect crop_zone(shave, 0, w - (2 * shave), h);

    std::vector<cv::Mat> cropped_imgs;
    for(int i = 0; i < 4; i++) {
        cropped_imgs.push_back(imgs[i](crop_zone));
    }

    // 2. CONCAT: Join them side-by-side with 0px overlap
    // This prevents the "ghosting"/translucency you see in your screenshot.
    cv::Mat pano;
    cv::hconcat(cropped_imgs, pano);
    cv::imwrite(outputPath.toStdString(), pano);


    //qDebug() << "ROV view saved:" << outputPath;
    emit image_stitched();
}

void VideoStreamer::set_image_stitching_path(QString path, int mode)
{
    switch (mode) {
    case 0:
        stitching_path = path;
        break;
    case 1:
        stitching_path2 = path;
        break;
    default:
        break;
    }
    //qDebug()<<stitching_path<<stitching_path2;
}

Worker::Worker(QObject *parent)
{

}

Worker::~Worker()
{

}


void Worker::grabFrame()
{
    if (!QThread::currentThread()->isInterruptionRequested()) {

        //auto startTime = std::chrono::high_resolution_clock::now();

        if(cap.isOpened())
        {
            if(!cap.read(tempFrame))
                ;
            else
                emit emitThreadImage(tempFrame);

        }
        if(cap2.isOpened())
        {
            if(!cap2.read(tempFrame2))
                ;
            else
                emit emitThreadImage2(tempFrame2);

        }
        if(cap3.isOpened())
        {
            if(!cap3.read(tempFrame3))
                ;
            else
                emit emitThreadImage3(tempFrame3);

        }
        if(cap4.isOpened())
        {
            if(!cap4.read(tempFrame4))
                ;
            else
                emit emitThreadImage4(tempFrame4);

        }
        /*if(tempFrame.data || tempFrame2.data || tempFrame3.data || tempFrame4.data)
        {
        emit emitThreadImage(tempFrame);
            emit emitThreadImage2(tempFrame2);
        emit emitThreadImage3(tempFrame3);
            emit emitThreadImage4(tempFrame4);

        }*/


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
        // Record end time
        /*auto endTime = std::chrono::high_resolution_clock::now();

        // Calculate the actual time taken to process the frame
        int actual_difference
            = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
        //qDebug() << "calculated" << calculated_difference;
        //qDebug() << "actual" << actual_difference;

        int outcome = std::abs(calculated_difference - actual_difference);
        if (actual_difference < calculated_difference)
            delay = outcome + actual_difference;

        else
            delay = actual_difference - outcome;

        if (delay > 1000 || delay < 0)
            delay = 1000 / 40;
        qDebug() << "delay" << delay;*/

    }
}

void Worker::write_Frame()
{
    if (!QThread::currentThread()->isInterruptionRequested()) {

        QMutexLocker locker(&writeMutex);
        if(cap.isOpened() && recording_status)
        {
            video.write(writeFrame);
        }
        if(cap2.isOpened() && recording_status2)
        {
            video2.write(writeFrame2);
        }
        if(cap3.isOpened() && recording_status3)
        {
            video3.write(writeFrame3);
        }
        if(cap4.isOpened() && recording_status4)
        {
            video4.write(writeFrame4);
        }

    }
}
