#pragma once

#include <string>
#include <gtk/gtk.h>

class CommandApp {
private:
    GtkApplication* mpApp;
    int mStatus;
public:
    CommandApp();
    ~CommandApp();
    static void BuildUI(GtkApplication* app, gpointer user_data);
    static void SendCommand(GtkWidget* widget, gpointer user_data);
    int Run(int argc, char** argv);
};