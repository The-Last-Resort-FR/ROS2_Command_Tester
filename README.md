# ROS2_Command_Tester

A GUI to test commands, meant to be used with [ROS2_STM32_Comm](https://github.com/The-Last-Resort-FR/ROS2_STM32_Comm)

## Dependencies

- ROS2 Humble
- Gtk4

## Building

Install ROS2 Humble [tutorial here](https://docs.ros.org/en/humble/Installation.html)  
Install the dependencies  
```bash
sudo apt update && sudo apt install libgtk-4-1 libgtk-4-dev  
```
Clone the repository  
```bash
git clone https://github.com/The-Last-Resort-FR/ROS2_Command_Tester.git
```
Build  
```bash
cd ROS2_Command_Tester
colcon build
```

## Usage

Run the app  
```bash
ros2 run gui_command gui_command 
```
  
You need ROS2_STM32_Comm's node to be started and an STM32 board running STM32_USB_Comm for it to work properly

## Version

- V0.0.0 : GUI

## TODO

- Communication with the command server node