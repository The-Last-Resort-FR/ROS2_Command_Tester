#include <cstdio>
#include <gtk/gtk.h>
#include "CommandApp.hpp"

int main(int argc, char **argv) {
    int status = 0;
    CommandApp app;

    status = app.Run(argc, argv);
    
    return status;
}