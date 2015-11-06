#include <iostream>
#include "ros/ros.h"
#include <termios.h>
#include <boost/thread.hpp>
#include "msg/robotcmd_motor.h"

#define KEYCODE_50 0x61
#define KEYCODE_80 0x62
#define KEYCODE_100 0x63
using namespace std;
struct termios cooked, raw;

int main(int argc, char** argv)
{ 
  int fd = 0;
  char c;
  msg::robotcmd_motor msg;
  ros::init(argc, argv, "test_robot_key_control");
  ros::NodeHandle n;
  ros::Publisher state_pub = n.advertise<msg::robotcmd_motor>("/robotcmd/motor", 1000);
  ros::Rate loop_rate(40); 
  tcgetattr(fd, &cooked);
  memcpy(&raw, &cooked, sizeof(struct termios));
  raw.c_lflag &=~ (ICANON | ECHO);
   // raw.c_cc[VEOL] = 1;
   // raw.c_cc[VEOF] = 2;
  tcsetattr(fd, TCSANOW, &raw);

  puts("Reading from keyboard");
  puts("-----------------------"); 
  puts("Please enter abc");
  puts("-----------------------");
  puts("Please enter a state: ");

  while (ros::ok())
  {
    if(read(fd, &c, 1) < 0)
    {
      perror("read():");
      exit(-1);
    }
    switch(c)
    {     
    case KEYCODE_50:
      ROS_INFO("0");
      msg.angle=0;
      state_pub.publish(msg);
     break;
    case KEYCODE_80:
      ROS_INFO("90");
      msg.angle=90;
      state_pub.publish(msg);
     break;
    case KEYCODE_100:
      ROS_INFO("180");
      msg.angle=180;
      state_pub.publish(msg);
     break;
    }
  }
  return 0;
}
