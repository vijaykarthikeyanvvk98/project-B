#include "link.h"
#include <videostreamer.h>
#include <QDebug>
Link *link;
static bool data_arrived1=true,data_arrived2=true,data_arrived3=true,data_arrived4=true,data_arrived5=true,data_arrived6=true,data_arrived7=true,data_arrived8=true,data_arrived9=true,data_arrived10=true,data_arrived11=true,data_arrived12=true,data_arrived13=true,data_arrived14=true,data_arrived15=true,data_arrived16=true;
//VideoStreamer videostream;
Q_DECLARE_METATYPE(long double)

Link::Link()
{
    create_directory();
    connect(&mainWindow, &MainWindow::data_received, this, &Link::send_data_to_process);
    connect(this, &Link::data_processed, this, &Link::data_to_be_updated);

    QTimer *timer2 = new QTimer(this);
    QTimer *timer3 = new QTimer(this);
}

Link::~Link()
{

}

bool Link::areDoublesEqual(double a, double b, double epsilon)
{
    return qAbs(a - b) < epsilon;

}

QVariantList Link::get_heartbeat()
{
    return heart_array;

}

QVariantList Link::get_ahrs()
{
    return ahrs_Array;

}

QVariantList Link::get_gps()
{
    return gps_array;

}

QVariantList Link::get_imu()
{
    return imu_array;

}

QVariantList Link::get_atm()
{
    return env_array;
}

QVariantList Link::get_sys()
{
    return sys_array;
}

QVariantList Link::get_son()
{
    return sonar_array;
}

QVariantList Link::get_enc()
{
    return encoder_array;

}

QVariantList Link::get_dead()
{
    return dead_array;

}

QVariantList Link::get_leak()
{
    return leak_array;

}

QVariantList Link::get_flow()
{
    return flow_array;

}

QVariantList Link::get_sub1()
{
    return cpt_array;

}

QVariantList Link::get_err()
{
    return error_array;

}

QVariantList Link::get_joy()
{
    return joy_array;

}
/*bool areDoublesEqual(double a, double b, double epsilon = 1e-9) {
    return qAbs(a - b) < epsilon;
}*/
void Link::display_log(QString msg)
{
    //qDebug()<<msg;
}

void Link::doWork()
{
    //emit udp_connected();
    //qDebug()<<"Hai";
    emit state_connected();
}

void Link::trigger()
{
    // timer4->start(2000);

}


void Link::config(QByteArray) {}

void Link::graph_calculation(qfloat16 hyp_D_dist, qfloat16 theta)
{

    qreal theta_tare = qDegreesToRadians(pitch1 /*theta - pitch_tare*/);
    //pitch_tare2 = pitch1; //theta - pitch_tare;

    D_height = hyp_D_dist * sin(theta_tare);
    base_D_distance = (hyp_D_dist * cos(theta_tare));

    graph_x = base_D_distance ;
    graph_y = D_height;
    //qDebug()<<graph_x<<pitch1;
    if (graph_maxY < graph_y)
        graph_maxY = graph_y;
    if (graph_minY > graph_y)
        graph_minY = graph_y;
    if (graph_maxX < graph_x)
        graph_maxX = graph_x;
    if (graph_minX > graph_x)
        graph_minX = graph_x;

    emit new_data();


}

QVariantList Link::cpt_graph()
{
    array_3.clear();
    array_3.append(cpt_pressure);
    array_3.append(cpt_distance_mm);
    array_3.append(QString::number(cpt_angle));
    array_3.append(cpt_minX);
    array_3.append(cpt_maxX);
    array_3.append(cpt_minY);
    array_3.append(cpt_maxY);
    /*cpt_array.clear();
    cpt_array.append(cpt_pressure);
    cpt_array.append(cpt_distance_mm);
    cpt_array.append(QString::number(cpt_angle));
    cpt_array.append(cpt_minX);
    cpt_array.append(cpt_maxX);
    cpt_array.append(cpt_minY);
    cpt_array.append(cpt_maxY);*/
    return array_3;
}

void Link::get_pitch_tare()
{
    pitch_tare += pitch1;
}

void Link::get_cpt_tare()
{
    //cpt_tare += cpt_tare_value;?'
    CPT_DEPTH_ERROR += cpt_piston_count;
    cpt_pressure = 0;
}

QVariantList Link::get_graph_components()
{
    //array2.clear();
    graph_array.clear();
    graph_array.append(graph_x);
    //array2.append(graph_y - height_tare_value);
    graph_array.append(graph_y);
    /*array2.append(graph_maxY);
                            array2.append(graph_minY);
                            array2.append(graph_maxX);
                            array2.append(graph_minX);*/
    //array2.append((double)pitch1);
    //qDebug()<<array2;
    return graph_array;
}

void Link::thread_start()
{
    this->moveToThread(thread);
    connect(thread, &QThread::started, this, &Link::parse_string);
    thread->start();
}


void Link::imu_parsing()
{

}

void Link::AHRS_parsing()
{
}

void Link::gps_parsing()
{
}

void Link::Encoder_parsing()
{

}

void Link::Hull_parsing()
{

}

void Link::heart_beat_parsing()
{

}

void Link::env_parsing()
{
}

void Link::ping_parsing()
{
}


double Link::get_yaw()
{
    return yaw1;
}

void Link::writeToLogFile()
{

}

void Link::leak_parsing()
{
}

void Link::flow_parsing()
{
}

void Link::joystick_calibration()
{
}

void Link::export_file(const QVariantList &summary)
{
}

void Link::export_log()
{
}


void Link::create_directory()
{

}


void Link::send_data_to_process(QByteArray buffer)
{

}

void Link::data_to_be_updated(int data_id)
{
}

void Link::ack_check()
{
}

void Link::err_check()
{
}

void Link::login(QString user, QString pass)
{

}

void Link::send_data(QByteArray buffer)
{

}

void Link::mavlink_send_buffer()
{
    //QMutexLocker locker(&bufferMutex);  // Locks until function exit

    // Determine the total size of the buffer based on the size of vedtp.vedtp_send
    //size_t bufferSize = sizeof(108);

    // Allocate memory for the buffer
    //uint8_t buffer[bufferSize];

    // Copy data from vedtp.vedtp_send to buffer
    //memcpy(buffer, &vedtp.vedtp_send, bufferSize);

    // Write the buffer to the serial port
    //writeBytesThreadSafe(reinterpret_cast<const char *>(buffer), bufferSize);


}

void Link::writeBytesThreadSafe(const char *bytes, int length)
{

}


void Link::splitDate(uint32_t gps_date, int *day, int *month, int *year)
{
    // Extracting day, month, and year components
    *day = gps_date % 100;
    gps_date /= 100;
    *month = gps_date % 100;
    gps_date /= 100;
    *year = gps_date;
}


void Link::sendHeartBeat() {}

QVariantList Link::health()
{
    return health_check_array;
}


QString Link::default_map_settings()
{
    return default_graph_path;
}

void Link::save_graph_points()
{
}

void Link::connection_state()
{

    if(state_check != state_check_prev)
        ;
    else
        emit state_disconnected();

    state_check_prev = state_check;
}


QString Link::parseUTCTime(QString utcTime)
{
    // Extract hours and minutes
    int hours = utcTime.toInt() / 100;       // Divide to get hours
    int minutes = utcTime.toInt() % 100;     // Modulo to get minutes

    // Format into a readable string (e.g., "11:38")
    return QString("%1:%2")
        .arg(hours, 2, 10, QChar('0'))    // Ensure 2-digit format for hours
        .arg(minutes, 2, 10, QChar('0')); // Ensure 2-digit format for minutes
}

QString Link::simulate_time()
{
    date = QDateTime::currentDateTime();
    formattedTime = date.toString("dd.MM.yyyy,hh:mm:ss");
    formattedTimeMsg = formattedTime.toLocal8Bit();
    return formattedTimeMsg;
}

void Link::save_short()
{

}


QVariantList Link::get_error()
{
    err_array.clear();
    err_array.append(error_device);
    err_array.append(error_type);
    return err_array;
}


void Link::parse_string()
{

}

void Link::map_capture() {}

QVariantList Link::get_data()
{
    latency = 0;
    return array;
}

void Link::health_check()
{
    for (int i = 0; i < health_check_array.length(); i++) {
        if (health_check_array[i] == 0)
            valid_data = 0;
        else
            ;
    }
    health_check_array.clear();
}

void Link::set_coordinates(double initial_position_x, double initial_position_y)
{
    lat1 = initial_position_x;
    long1 = initial_position_y;
}



// Function to parse the shapefile
