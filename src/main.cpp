/* Include Libiraries */
#include<Arduino.h>
#include "STD_TYPES.h"

#include "GreetingRobot_interface.h"

void setup()
{

  // Initialization function of the Robot
  GreetingRobot_Init();


}

void loop()
{

  GreetingRobot_PublishSensorData();
  delay(100);
}