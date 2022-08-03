/********************************************************************************/
/*   Author        : Ahmed Farag                                                */
/*   Date          : 02/08/2022                                                 */
/*   Version       : V01                                                        */
/*   Description   : Greeting Robot code                                        */
/********************************************************************************/

/* Include Libiraries */
#include  <Arduino.h>
#include "STD_TYPES.h"

#include "GreetingRobot_interface.h"

void setup()
{

  // Initialization function of the Robot
  GreetingRobot_voidInit();



}

void loop()
{

  while (1)
  {
    /* code */
    Serial.println(digitalRead(10));
  }
  
  // Function to publish the readings of the sensors to ROS topics
  // GreetingRobot_voidPublishSensorData();
  delay(100);
}