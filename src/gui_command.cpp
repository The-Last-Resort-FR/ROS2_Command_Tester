#include <cstdio>
#include <gtk/gtk.h>
#include "CommandApp.hpp"

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    
    int status = 0;
    std::shared_ptr<rclcpp::Node> node = rclcpp::Node::make_shared("gui_command");
    rclcpp::Client<custom_msg::srv::Stcommand>::SharedPtr client = node->create_client<custom_msg::srv::Stcommand>("stcommand");
    CommandApp app(node);

    status = app.Run(argc, argv);

    return 0;
}