# ROS2_Command_Tester

A GUI to test commands, meant to be used with [ROS2_STM32_Comm](https://github.com/The-Last-Resort-FR/ROS2_STM32_Comm)

## Dependencies

- ROS2 Humble
- Gtk4
- [ROS2_Custom_Msg](https://github.com/The-Last-Resort-FR/ROS2_Custom_Msg)

## Building

Install ROS2 Humble [tutorial here](https://docs.ros.org/en/humble/Installation.html)  
This repo needs the custom messages of [ROS2_Custom_Msg](https://github.com/The-Last-Resort-FR/ROS2_Custom_Msg)
Install the dependencies  
```bash
sudo apt update && sudo apt install libgtk-4-1 libgtk-4-dev  
```
Clone the repository  
```bash
git clone https://github.com/The-Last-Resort-FR/ROS2_Command_Tester.git
git clone https://github.com/The-Last-Resort-FR/ROS2_Custom_Msg.git
```
Build ROS2_Custom_Msg first then ROS2_Command_Tester
```bash
cd ROS2_Custom_Msg
colcon build
source install/setup.sh
cd ../ROS2_Command_Tester
colcon build --symlink-install
```
Source the environement of ROS2_Command_Tester
```bash
source install/setup.bash
```

## Usage

Run the app  
```bash
ros2 run gui_command gui_command 
```
  
You need ROS2_STM32_Comm's node to be started and an STM32 board running STM32_USB_Comm for it to work properly

## Version

- V0.0.0 : GUI
- V0.1.0 : Communication with command server node 

## TODO

- ComboBox or Dropdown for command type