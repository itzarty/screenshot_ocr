#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <functional>
#include <libayatana-appindicator/app-indicator.h>

class Tray {
public:
    Tray();
    void setText(std::string text);
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
    GtkWidget* createItem(std::string label);

    std::function<void()> callback;
};