#pragma once

#include <string>
#include <fstream>
#include <functional>
#include <libayatana-appindicator/app-indicator.h>

class Tray {
public:
    Tray();
    void setText(const std::string &text);
    void run();
    void setQuitHandler(std::function<void()> callback);
private:
    AppIndicator *indicator;
    std::string currentText;

    GtkWidget* textItem;
    GtkWidget* copyItem;
    GtkWidget* saveItem;
    GtkWidget* quitItem;
    GtkWidget* menu;

    void copy();
    void save();
    void quit();
    GtkWidget* createItem(const std::string &label, void (*itemCallback)(GtkWidget*, gpointer));

    std::function<void()> callback;
};