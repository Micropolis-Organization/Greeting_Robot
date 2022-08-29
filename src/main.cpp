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

<<<<<<< HEAD
  GreetingRobot_PublishSensorData();
=======
  
  // Function to publish the readings of the sensors to ROS topics
  GreetingRobot_voidPublishSensorData();
>>>>>>> 533af2d5d70edcf01ef62142ccc86d53dfc11ce7
  delay(100);
}