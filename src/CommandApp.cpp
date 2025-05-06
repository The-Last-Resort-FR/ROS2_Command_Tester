#include "CommandApp.hpp"

CommandApp::CommandApp(std::shared_ptr<rclcpp::Node> nodehandle): mpApp(nullptr), mpNode(nodehandle), mStatus(0) {
    mpApp = gtk_application_new("ros2.gtk.gui_command", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(mpApp, "activate", G_CALLBACK (CommandApp::BuildUI), this);

    mpClient = mpNode->create_client<custom_msg::srv::Stcommand>("stcommand");
}

CommandApp::~CommandApp() {
    g_object_unref(mpApp);
}

void CommandApp::BuildUI(GtkApplication* app, gpointer user_data) {

    GtkBuilder* builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "ressources/builder.ui", NULL);
    GObject* window = gtk_builder_get_object(builder, "window");

    gtk_window_set_application(GTK_WINDOW (window), app);
    gtk_window_set_title(GTK_WINDOW(window), "ROS2 STM32 Command Tester");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    SendCommandSructure* params = new SendCommandSructure;
    params->app = (CommandApp*)user_data;
    params->builder = builder;

    GObject *sendButton = gtk_builder_get_object(builder, "send-button");
    g_signal_connect(sendButton, "clicked", G_CALLBACK (CommandApp::SendCommand), params);


    gtk_widget_set_visible(GTK_WIDGET (window), true);
    // params and builder are lost but we'll use it until the end of the app anyway
}

int CommandApp::Run(int argc, char** argv) {
    mStatus = g_application_run (G_APPLICATION (mpApp), argc, argv);
    return mStatus;
}

void CommandApp::SendCommand(GtkWidget* widget, gpointer user_data) {
    using namespace std::chrono_literals;
    (void)widget;
    SendCommandSructure* params = (SendCommandSructure*)user_data;

    GtkBuilder* builder = params->builder;
    GObject* commandEntry = gtk_builder_get_object(builder, "command");
    GObject* argEntry = gtk_builder_get_object(builder, "argument");
    GObject* resultLabel = gtk_builder_get_object(builder, "result");

    std::string command = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(commandEntry)));
    std::string arg = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(argEntry)));
    std::string label = gtk_label_get_text(GTK_LABEL(resultLabel));

    auto request = std::make_shared<custom_msg::srv::Stcommand::Request>();
    request->command = std::stoul(command);
    request->arg = std::stoul(arg);

    while (!params->app->mpClient->wait_for_service(1s)) {
        if (!rclcpp::ok()) {
          RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for the service. Exiting.");
          return;
        }
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "service not available, waiting again...");
    }

    auto result = params->app->mpClient->async_send_request(request);

    if (rclcpp::spin_until_future_complete(params->app->mpNode, result) == rclcpp::FutureReturnCode::SUCCESS)
    {
        char buff[256];
        uint8_t* cast = (uint8_t*)&(result.get()->response);
        uint8_t curr = 0;
        sprintf(buff, "Response:\t");
        curr += sizeof("Response:\t");
        for(uint8_t i = 0; i < 9; i++) {
            sprintf(buff + curr, "%X ", *(cast+i));
            curr += 3;
        }

        std::string newlabel = label + buff + "\n";
        gtk_label_set_label(GTK_LABEL(resultLabel), newlabel.c_str());
    }
    else
    {
        RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Failed to call stcommand");
    }


}