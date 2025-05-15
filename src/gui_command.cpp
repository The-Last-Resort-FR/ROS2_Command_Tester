#include <cstdio>
#include <gtk/gtk.h>
#include "CommandApp.hpp"

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    
    int status = 0;
    std::shared_ptr<rclcpp::Node> node = rclcpp::Node::make_shared("gui_command");
    
    auto executor = std::make_shared<rclcpp::executors::SingleThreadedExecutor>();
    CommandApp app(node, executor);
    executor->add_node(node);


    status = app.Run(1, argv);
    return status;
}