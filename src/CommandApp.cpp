#include "CommandApp.hpp"

CommandApp::CommandApp(): mpApp(nullptr), mStatus(0) {
    mpApp = gtk_application_new("ros2.gtk.gui_command", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(mpApp, "activate", G_CALLBACK (CommandApp::BuildUI), NULL);
}

CommandApp::~CommandApp() {
    g_object_unref(mpApp);
}

void CommandApp::BuildUI(GtkApplication* app, gpointer user_data) {
    (void)user_data;

    GtkBuilder* builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "ressources/builder.ui", NULL);
    GObject* window = gtk_builder_get_object(builder, "window");

    gtk_window_set_application(GTK_WINDOW (window), app);
    gtk_window_set_title(GTK_WINDOW(window), "ROS2 STM32 Command Tester");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);


    GObject *sendButton = gtk_builder_get_object(builder, "send-button");
    g_signal_connect(sendButton, "clicked", G_CALLBACK (CommandApp::SendCommand), builder);


    gtk_widget_set_visible(GTK_WIDGET (window), true);
}

int CommandApp::Run(int argc, char** argv) {
    mStatus = g_application_run (G_APPLICATION (mpApp), argc, argv);
    return mStatus;
}

void CommandApp::SendCommand(GtkWidget* widget, gpointer user_data) {
    (void)widget;

    GtkBuilder* builder = GTK_BUILDER((GObject*)user_data);
    GObject* commandEntry = gtk_builder_get_object(builder, "command");
    GObject* argEntry = gtk_builder_get_object(builder, "argument");
    GObject* resultLabel = gtk_builder_get_object(builder, "result");

    std::string command = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(commandEntry)));
    std::string arg = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(argEntry)));
    std::string label = gtk_label_get_text(GTK_LABEL(resultLabel));

    std::string newlabel = label + " " + command + " " + arg;
    gtk_label_set_label(GTK_LABEL(resultLabel), newlabel.c_str());
}