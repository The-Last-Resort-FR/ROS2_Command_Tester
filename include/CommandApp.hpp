#pragma once

#include <string>
#include <memory>
#include <chrono>

#include <gtk/gtk.h>
#include <rclcpp/rclcpp.hpp>

#include "custom_msg/srv/stcommand.hpp"


class CommandApp {
private:
    GtkApplication* mpApp;
    std::shared_ptr<rclcpp::Node> mpNode;
    std::shared_ptr<rclcpp::executors::SingleThreadedExecutor> mpExecutor;
    rclcpp::Client<custom_msg::srv::Stcommand>::SharedPtr mpClient;
    int mStatus;
    uint16_t mRow;
public:
    CommandApp(std::shared_ptr<rclcpp::Node> nodehandle, std::shared_ptr<rclcpp::executors::SingleThreadedExecutor> executor);
    ~CommandApp();
    static void BuildUI(GtkApplication* app, gpointer user_data);
    static void SendCommand(GtkWidget* widget, gpointer user_data);
    int Run(int argc, char** argv);
};

struct SendCommandSructure {
    CommandApp* app;
    GtkBuilder* builder;
};