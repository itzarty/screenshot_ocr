#pragma once
#include <string>
#include <libnotify/notify.h>

class Notification {
public:
    Notification(std::string name);
    ~Notification();
    void send(std::string title, std::string body);
    void send(std::string body);
private:
    std::string name;
};