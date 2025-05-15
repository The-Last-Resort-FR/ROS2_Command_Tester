#include "CommandApp.hpp"

CommandApp::CommandApp(std::shared_ptr<rclcpp::Node> nodehandle, std::shared_ptr<rclcpp::executors::SingleThreadedExecutor> executor): mpApp(nullptr), mpNode(nodehandle), mpExecutor(executor), mStatus(0), mRow(1) {
    mpApp = gtk_application_new("ros2.gtk.gui_command", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(mpApp, "activate", G_CALLBACK (CommandApp::BuildUI), this);

    mpClient = mpNode->create_client<custom_msg::srv::Stcommand>("/send_stm_commands");
}

CommandApp::~CommandApp() {
    g_object_unref(mpApp);
}

void CommandApp::BuildUI(GtkApplication* app, gpointer user_data) {
    CommandApp* appHandle = (CommandApp*)user_data;
    std::string config_path;
    appHandle->mpNode->declare_parameter<std::string>("config_path", "");
    appHandle->mpNode->get_parameter("config_path", config_path);
    GtkBuilder* builder = gtk_builder_new();
    //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "UI file path: %s\n", (config_path + "/ressources/builder.ui").c_str());
    gtk_builder_add_from_file(builder, (config_path + "/ressources/builder.ui").c_str(), NULL);
    GObject* window = gtk_builder_get_object(builder, "window");

    gtk_window_set_application(GTK_WINDOW (window), app);
    gtk_window_set_title(GTK_WINDOW(window), "ROS2 STM32 Command Tester");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    SendCommandSructure* params = new SendCommandSructure;
    params->app = appHandle;
    params->builder = builder;

    GObject *sendButton = gtk_builder_get_object(builder, "send-button");
    g_signal_connect(sendButton, "clicked", G_CALLBACK (CommandApp::SendCommand), params);

    const char* headers[] = {
        "status", "data0", "data1", "data2", "data3", "data4", "data5", "data6", "data7"
    };
    GtkGrid* grid = GTK_GRID(gtk_builder_get_object(builder, "result"));
    for (int i = 1; i < 10; ++i) {
        GtkWidget* header = gtk_label_new(headers[i-1]);
        gtk_widget_set_halign(header, GTK_ALIGN_CENTER);
        gtk_grid_attach(grid, header, i, 0, 1, 1);
    }
    gtk_grid_set_row_spacing(grid, 4);
    gtk_grid_set_column_spacing(grid, 4);


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
    //GObject* grid = gtk_builder_get_object(builder, "result");
    GtkGrid* grid = GTK_GRID(gtk_builder_get_object(builder, "result"));

    std::string command = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(commandEntry)));
    std::string arg = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(argEntry)));
    //std::string label = gtk_label_get_text(GTK_LABEL(resultLabel));

    auto request = std::make_shared<custom_msg::srv::Stcommand::Request>();
    request->command = std::stoul(command);
    request->arg = std::stoul(arg);
    request->type = 1;

    while (!params->app->mpClient->wait_for_service(1s)) {
        if (!rclcpp::ok()) {
          RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for the service. Exiting.");
          return;
        }
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "service not available, waiting again...");
    }

    auto result = params->app->mpClient->async_send_request(request);

    if (params->app->mpExecutor->spin_until_future_complete(result) == rclcpp::FutureReturnCode::SUCCESS)
    {
        std::array<uint8_t, 9UL> r = result.get()->response;
    
        GtkWidget* text = gtk_label_new("response: ");
        gtk_widget_set_halign(text, GTK_ALIGN_START);
        gtk_grid_attach(grid, text, 0, params->app->mRow, 1, 1);
        for (size_t i = 0; i < r.size(); ++i) {
            char buff[16];
            sprintf(buff, "0x%02X", r[i]);
            GtkWidget* byteLabel = gtk_label_new(buff);
            gtk_widget_set_halign(byteLabel, GTK_ALIGN_CENTER);
            gtk_grid_attach(grid, byteLabel, i, params->app->mRow, 1, 1);
        }
        params->app->mRow++;
    }
    else
    {
        RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Failed to call stcommand");
    }


}