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
    rclcpp::Client<custom_msg::srv::Stcommand>::SharedPtr mpClient;
    int mStatus;
public:
    CommandApp(std::shared_ptr<rclcpp::Node> nodehandle);
    ~CommandApp();
    static void BuildUI(GtkApplication* app, gpointer user_data);
    static void SendCommand(GtkWidget* widget, gpointer user_data);
    int Run(int argc, char** argv);
};

struct SendCommandSructure {
    CommandApp* app;
    GtkBuilder* builder;
};