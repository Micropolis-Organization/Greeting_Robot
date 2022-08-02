/********************************************************************************/
/*   Author        : Ahmed Farag                                                */
/*   Date          : 02/08/2022                                                 */
/*   Version       : V01                                                        */
/*   Description   : Greeting Robot code                                        */
/********************************************************************************/

/* Include Libiraries */
#include <HardwareSerial.h>
#include <SoftwareSerial.h>
#include <ODriveArduino.h>
#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Int16.h>
#include "STD_TYPES.h"

#include "GreetingRobot_interface.h"
#include "GreetingRobot_config.h"
#include "GreetingRobot_private.h"

// Communication with serial over serial1
HardwareSerial &odrive_serial = Serial1;

// ODrive object
ODriveArduino odrive(odrive_serial);

// Global variables to store the values of velocity

f32 Angle_Speed;
f32 Linear_Speed;
f32 Left_Wheel_Velocity;
f32 Right_Wheel_Velocity;
f32 Left_Wheel_Velocity_In_RPS;
f32 Right_Wheel_Velocity_In_RPS;
f32 Center_Of_Rotation;

// Create node handle to manage pub and sub
ros::NodeHandle nh;

// Create a standard msg for publisher
std_msgs::Int16 Sensor_One_Value;
std_msgs::Int16 Sensor_Two_Value;
std_msgs::Int16 Sensor_Three_Value;

// Create a ros publisher to publish sensors state
ros::Publisher Sensor_One("Sensor_One_State", &Sensor_One_Value);
ros::Publisher Sensor_Two("Sensor_Two_State", &Sensor_Two_Value);
ros::Publisher Sensor_Three("Sensor_Three_State", &Sensor_Three_Value);

// Call back function fro the sensor topic
void Sensor_One_Cb(const std_msgs::Int16 &Sensor_One_Value)
{
    u8 Sensor_Value = Sensor_One_Value.data;
    if (Sensor_Value == HIGH)
    {
        odrive.SetVelocity(MOTOR_ONE, LOW);
        nh.loginfo((String("Stop Sensor_Value  = ") + String(Sensor_Value).c_str()).c_str());
    }
}
// Call back function for the CMD Vel topic
void cmd_vel_cb(const geometry_msgs::Twist &cmdvel)
{
    Angle_Speed = cmdvel.angular.z;
    Linear_Speed = cmdvel.linear.x;

    if (Angle_Speed != 0.0)
    {

        Center_Of_Rotation = Linear_Speed / Angle_Speed;

        // Formula for converting the velocity cmd twist msg to motor velocity
        Left_Wheel_Velocity = -1 * (Angle_Speed * (Center_Of_Rotation - LENGTH / 2));
        Right_Wheel_Velocity = -1 * (Angle_Speed * (Center_Of_Rotation + LENGTH / 2));

        Left_Wheel_Velocity_In_RPS =  Left_Wheel_Velocity / (2 * 3.14 * WHEEL_RADIUS);
        Right_Wheel_Velocity_In_RPS = Right_Wheel_Velocity / (2 * 3.14 * WHEEL_RADIUS);
    }
    else
    {
        Left_Wheel_Velocity_In_RPS  = Right_Wheel_Velocity_In_RPS = Linear_Speed / (2 * 3.14 * WHEEL_RADIUS);
        Left_Wheel_Velocity_In_RPS  = -1 * Left_Wheel_Velocity_In_RPS;
        Right_Wheel_Velocity_In_RPS = -1 * Right_Wheel_Velocity_In_RPS;
    }
    
    odrive.SetVelocity(MOTOR_ONE, Right_Wheel_Velocity_In_RPS);
    odrive.SetVelocity(MOTOR_TWO, Left_Wheel_Velocity_In_RPS);

    nh.loginfo((String("V_l = ") + String(Left_Wheel_Velocity_In_RPS).c_str()).c_str());
    nh.loginfo((String("V_r = ") + String(Right_Wheel_Velocity_In_RPS).c_str()).c_str());
    nh.loginfo("-----------------------------------------------------------------------------------");
}

// Creating a ros subscriber for cmd_vel topic
ros::Subscriber<geometry_msgs::Twist> cmd_vel_sub("cmd_vel", &cmd_vel_cb);
ros::Subscriber<std_msgs::Int16> Sub_One("Sensor_One_State", &Sensor_One_Cb);

void GreetingRobot_PublishSensorData()
{
    Sensor_One_Value.data = digitalRead(SENSOR_ONE_INPUT_PIN);
    Sensor_One.publish(&Sensor_One_Value);

    Sensor_Two_Value.data = digitalRead(SENSOR_TWO_INPUT_PIN);
    Sensor_Two.publish(&Sensor_Two_Value);

    Sensor_Three_Value.data = digitalRead(SENSOR_THREE_INPUT_PIN);
    Sensor_Three.publish(&Sensor_Three_Value);
    nh.spinOnce();
}

void GreetingRobot_Init()
{

    /* Initialization of pins of the IR sensor */

    pinMode(SENSOR_ONE_INPUT_PIN, INPUT);
    pinMode(SENSOR_TWO_INPUT_PIN, INPUT);
    pinMode(SENSOR_THREE_INPUT_PIN, INPUT);

    // Start serial communication with oDrive on serial 1
    odrive_serial.begin(115200);
    // Initialization node function
    nh.initNode();
    // Subscriber node for cmd vel topic
    nh.subscribe(cmd_vel_sub);
    nh.subscribe(Sub_One);

    // advertise sensor  topic
    nh.advertise(Sensor_One);
    nh.advertise(Sensor_Two);
    nh.advertise(Sensor_Three);

    // start serial with ROS
    Serial.begin(115000);
}