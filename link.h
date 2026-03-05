#ifndef LINK_H
#define LINK_H
#include <QByteArray>
#include <QDateTime>
#include <QElapsedTimer>
#include <QObject>
#include <QRandomGenerator>
#include <QThread>
#include <QTimer>
#include <QVariant>
#include "QThread"
#include "mainwindow.h"
#include "qfloat16.h"
#include "qmutex.h"

class Link : public QObject
{
    Q_OBJECT
public:
    Link();
    ~Link();
signals:
    void data_processed(int);
    void dataUpdated(int);
    void gps_updated();
    void ahrs_updated();
    void imu_updated();
    void hull_updated();
    void system_atmos_updated();
    void encoder_updated();
    void new_data();
    void cpt_data();
    void pdf_saved();
    void img_dir_created();
    void atmos_updated();
    void heart_beat();
    void map_image_captured();
    void sonar_updated();
    void leak_updated();
    void joystick_updated();
    void dead_reckon_updated();
    void command_updated();
    void flow_updated();
    void latency_updated();
    void _invokeWriteBytes(QByteArray);
    void recevied_beat();
    void update_login();
    void login_success();
    void map_converted();
    void device_error();
    void vehicle_disarmed();
    void vehicle_armed();
    void brake_applied(int);
    void display_logged();
    void state_disconnected();
    void state_connected();
    void err_detected();
    void no_err_detected(int);
    void dir_created(QString);


public slots:
    /**
 * @brief Calculate the great-circle distance between two points (Haversine formula).
 * @param lat1 Latitude of point 1 (degrees).
 * @param lon1 Longitude of point 1 (degrees).
 * @param lat2 Latitude of point 2 (degrees).
 * @param lon2 Longitude of point 2 (degrees).
 * @return Distance in kilometers.
 */
    bool areDoublesEqual(double , double , double epsilon = 1e-9);
    QVariantList get_heartbeat();
    QVariantList get_ahrs();
    QVariantList get_gps();
    QVariantList get_imu();
    QVariantList get_atm();
    QVariantList get_sys();
    QVariantList get_son();
    QVariantList get_enc();
    QVariantList get_dead();
    QVariantList get_leak();
    QVariantList get_flow();
    QVariantList get_sub1();
    QVariantList get_err();
    QVariantList get_joy();
    void display_log(QString);
    void doWork();
    void trigger();
    void parse_string();
    void map_capture();
    QVariantList get_data();
    void health_check();
    void set_coordinates(double, double);
    void config(QByteArray);
    void graph_calculation(qfloat16 distance, qfloat16 pitch1);
    QVariantList cpt_graph();
    void get_pitch_tare();
    void get_cpt_tare();
    QVariantList get_graph_components();
    void thread_start();
    void imu_parsing();
    void AHRS_parsing();
    void gps_parsing();
    void Encoder_parsing();
    void Hull_parsing();
    void heart_beat_parsing();
    void env_parsing();
    void ping_parsing();
    double get_yaw();
    void writeToLogFile();
    void leak_parsing();
    void flow_parsing();
    void joystick_calibration();
    void export_file(const QVariantList &summary);
    void export_log();
    void create_directory();
    void send_data_to_process(QByteArray buffer);
    void data_to_be_updated(int);
    void ack_check();
    void err_check();
    void send_data(QByteArray buffer);
    void mavlink_send_buffer();
    void writeBytesThreadSafe(const char *bytes, int length);
    void login(QString, QString);
    void splitDate(uint32_t gps_date, int *day, int *month, int *year);
    void sendHeartBeat();
    QVariantList health();
    QVariantList get_error();
    QString default_map_settings();
    void save_graph_points();
    void connection_state();
    QString parseUTCTime(QString);
    QString simulate_time();
    void save_short();

private:
    QVariantList array, array2, array_3, health_check_array,stream_Array,err_array;
    QVariantList heart_array,ahrs_Array,gps_array,imu_array,env_array,sys_array,sonar_array,flow_array,leak_array,cpt_array,graph_array,encoder_array,dead_array,error_array,joy_array;

    QString arduino_Error = "Arduino Error", jetson_pc_error = "PC Error";

    QString log_heading = "Id,Device\n1,Heartbeat\n2,IMU\n3,AHRS\n4,GPS\n5,ATMOSPHERE\n6,SYSTEM ATMOSPHERE\n7,SONAR\n8,ENCODER\n9,DEAD RECKONING\n10,LEAK\n11,COMMAND\n12,JOYSTICKS\n13,FLOW SENSOR\n14,CPT\n15,ERROR\n255,ACK\n\n\n\n\n\n";
    QString error_heading = "Id,Error Type\n1,DeviceError_INIT\n2,DeviceError_HeartBeat\n3,DeviceError_DataTransferFailed\n";
    QString error_heading2 = "Date,Time,,Device,Error";
    double lat1;
    double long1;
    double hull_temp, hull_pre, hull_humidity, env_pressure = 0.0, env_depth, env_temperature = 0.0,depth_tare_value=0.0;
    float ping_distance = 0.0;
    float confidence = 0.0;
    qint64 latency, latency2;
    QVariant myVariantValue;
    QByteArray datagram, cpt_serial;
    MainWindow mainWindow;
    QByteArray UDP_DATA, init_UDP_DATA = "0,S,M,A,-5,-5,S,S,4000,10000,#";
    QString data1 = "0";
    QElapsedTimer timer;
    qint64 startTime = 0;
    qfloat16 yaw1 = 0;
    qfloat16 pitch1 = 0;
    qfloat16 pitch_actual = 0;

    qfloat16 roll1 = 0;
    qfloat16 yaw2 = 0;
    qfloat16 pitch2 = 0;
    qfloat16 roll2 = 0;
    qreal pitch_tare = 0, cpt_tare = 0;
    qfloat16 distance = 0, distance1 = 0, distance2 = 0;
    qfloat16 del_height = 0;
    qfloat16 slope_pitch;
    double D_height = 0;
    double base_D_distance = 0;
    qreal graph_y = 0.0;
    qreal graph_x = 0.0;
    double one, two, gui;
    qfloat16 current1, current2;
    QString graph_x2;
    qreal pitch_tare2;
    qreal x, y, z,w;
    qreal graph_point;
    qfloat16 prev_distance = 0;
    QString ip_Address;
    QString mod2, mod3;
    qreal graph_maxY = 0.0, graph_minY = 0.0, graph_maxX = 0.0, graph_minX = 0.0;
    qreal cpt_angle = 0.0, cpt_angley = 0.0, cpt_anglez = 0.0;
    qreal cpt_pressure = 0;
    int cpt_piston_count = 0, arm_count = 0;
    qreal cpt_distance_mm = 0.0;
    qreal cpt_minX = 0, cpt_maxX = 0,cpt_minY=0,cpt_maxY=0;
    qreal encoder_count = 0;
    double left_axis, right_axis, X, up, down, left, right;
    bool start = false;
    qreal pressure = 0.0;
    QThread *thread = nullptr;
    int value = 2000;

    // Define the range for latitude and longitude
    double minLatitude = 12.894359620117209;
    double maxLatitude = 12.893925023222678; // Adjust the upper bound as needed
    double minLongitude = 80.17690954932144;
    double maxLongitude = 80.17692421732636; // Adjust the upper bound as needed
    // Generate random latitude and longitude
    double randomLatitude;
    double randomLongitude;
    double randomDegrees, randomDouble;
    // Define the range for degrees (0 to 360)
    double minDegrees = 0.0;
    double maxDegrees = 360.0;
    // Define the range for double values
    double minDouble = 0.0;
    double maxDouble = 300.0;
    // Define the range for int values
    int minInt = 900;
    int maxInt = 3000, randomInt;

    double rpm1 = 0, rpm2 = 0;
    qfloat16 velocity1 = 0.0, velocity2 = 0.0,avg_velocity=0.0;

    uint8_t satellite = 0, HDOP = 0;
    qfloat16 vehicle_speed = 0.0;
    double altitude = 0.0;
    QString gps_time = "";

    QDateTime date,date2;
    QString formattedTime, formattedTime2,formattedTime3;
    QByteArray formattedTimeMsg,formattedTimeMsg2;

    QString from = "", to = "", selected_time = "";
    QVector<QString> twoDArray, twoDArray2, twoDArray3, twoDArray_text,errDArray;

    double initial_position_x = 0.0; //12.891718893514218;
    double initial_position_y = 0.0; //80.17360785228679;

    double changed_x = 0.0;
    double changed_y = 0.0;

    double latitude_factor = 1.0 / 111000.0;
    double longitude_factor = 1.0 / (111000.0 * cos(initial_position_x));

    double distance_tare = 0.0;
    double height_tare_value = 0.0;
    double tare_main_distance;
    double tare_main_distance2;
    double water_current = 0.0;
    uint8_t wet = 0;
    float sonar_temperature;
    uint8_t sonar_voltage;
    uint16_t sonar_pinginterval;

    //dead reckon variables
    float dr_latitude;
    float dr_longitude;
    float dr_course;
    float dr_speed;

    //leak variables
    uint8_t leak1;
    uint8_t leak2;
    uint8_t leak3;
    uint8_t leak4;

    //flow sensor variables
    uint16_t flow_rpm;
    int8_t flow_direction;

    //heartbeat
    uint8_t beat_loged;
    uint8_t beat_vehicle;

    //imu
    float accelerationX;
    float accelerationY;
    float accelerationZ;
    float gyroX;
    float gyroY;
    float gyroZ;
    float magneticX;
    float magneticY;
    float magneticZ;
    float rotationvectorX;
    float rotationvectorY;
    float rotationvectorZ;
    float linearaccelerationX;
    float linearaccelerationY;
    float linearaccelerationZ;
    float gravityX;
    float gravityY;
    float gravityZ;
    float imu_temperature;
    uint8_t systemcalibration;
    uint8_t gyrocalibration;
    uint8_t accelerometercalibration;
    uint8_t magnetometercalibration;

    //Joystick Calibration
    uint16_t chan1;
    uint16_t chan2;
    uint16_t chan3;
    uint16_t chan4;
    uint16_t chan5;
    uint16_t chan6;
    uint16_t chan7;
    uint16_t chan8;

    //Command variables
    uint8_t command_id;
    uint8_t command_param1;
    uint8_t command_param2;
    uint8_t command_param3;
    uint8_t command_param4;
    uint8_t command_param5;
    uint8_t command_param6;

    //Login variables
    const char *user_name = "";
    const char *password = "";
    bool login_status = false;

    //Joystick variables
    uint16_t y_axis = 0.0, x_axis = 0.0;
    uint16_t y_axis2 = 0.0, x_axis2 = 0.0;
    bool chan_rev = false;

    //GPS
    float satellites;
    float hdop;
    float latitude,latitude1,latitude2,home_latitude;
    float longitude,longitude1,longitude2,home_longitude;
    float age;
    uint32_t gps_date;
    uint16_t time;
    float alt;
    float course;
    float speed;
    int day, month, year;

    //CPT joystick values
    int arm = 0;
    int pis = 0;

    //writing mode variables

    QTimer *timer1;

    int valid_data = 0;

    //vehicle brake values
    int brake_value = 0;
    bool is_brake = true;

    //converted Image Path
    QString map_image_path = "";
    QString log_path = "";
    QString pdf_path="";
    QString tilesDir="";
    QString default_graph_path = "";
    QString err_path = "";
    QString err_path2 = "";
    QString err_path3 = "";
    QString err_path4 = "";
    QString err_path5 = "";


    QString graph_path="",graph_path2="";
    QString cpt_path="",cpt_path2="";

    //Logfiles
    QString logFileDir4 = "";
    QString element="",element2="",element3="",title_element="Date,Time,,Device Id,Log Status,Vehicle\n",graph_element="",graph_element2="";
    int array_count=0,temp_i=0;
    QByteArray temp_array;

    int light_intensity=0,temp_light=0,light_intensity_prev=0;
    int intensity = 0;

    //Error variables
    int error_device=0;
    int error_type = 0;
    QString error_string = "";
    //CPT VALUE
    int CPT_DEPTH_ERROR=0;

    bool state_check = false;
    bool state_check_prev = false;
    bool ott = false;
    bool plot_not=false;
    QTimer *timer4;

    QString healthy_not = "";

    bool tared_not=false;

    double env_dep_average = 0.0,env_temp_average=0.0,sys_hum_average=0.0,sys_temp_average=0.0,env_pre_average=0.0,sys_pre_average=0.0,leak_average=0.0,flow_average=0.0,sonar_distance_average=0.0,env_temp_sum=0.0,env_dept_sum=0.0,env_pre_sum=0.0,sys_temp_sum=0.0,sys_pre_sum=0.0,sys_hum_sum=0.0,flow_sum=0.0,leak_sum=0.0,sonar_distance_sum=0.0,sonar_con_sum=0.0;
    int env_d,env_p,env_t,sys_p,sys_t,sys_h,fl,le,son;
    double env_dep_average2 = 0.0,env_temp_average2=0.0,sys_hum_average2=0.0,sys_temp_average2=0.0,env_pre_average2=0.0,sys_pre_average2=0.0,leak_average2=0.0,flow_average2=0.0,sonar_distance_average2=0.0,env_temp_sum2=0.0,env_dept_sum2=0.0,env_pre_sum2=0.0,sys_temp_sum2=0.0,sys_pre_sum2=0.0,sys_hum_sum2=0.0,flow_sum2=0.0,leak_sum2=0.0,sonar_distance_sum2=0.0,sonar_con_sum2=0.0;


    // Modify the target directory to add a subdirectory for your files.
    QString logFileDir5="";
    QDateTime currentDateTime;
    bool start_once=true;
    bool ahrs_once=true;
    bool gps_once=true;
    bool imu_once=true;
    bool env_once=true;

    QTimer *timer2;
    QTimer *timer3;
    QTimer *timer5;
    QTimer *timer6;
    QTimer *timer7;
    QTimer *timer8;
    QTimer *timer9;
    QTimer *timer10;
    QVariantList data,data2,data3,data4,data5,data6,data7,data8,data9,data10;

    QMutex sendMutex,bufferMutex,parseMutex,mutex1,mutex11,mutex2,mutex3,mutex4,mutex5,mutex6,mutex7,mutex8,mutex9,mutex10,health_mutex;
};

#endif // LINK_H
