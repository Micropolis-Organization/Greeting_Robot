#!/bin/bash
gnome-terminal -- roscore
sleep 1
gnome-terminal -- rosrun rosserial_python serial_node.py /dev/ttyACM0 _baud:=115200
sleep 1
gnome-terminal -- rosrun teleop_twist_keyboard teleop_twist_keyboard.py

