#include "Notifications.h"

Notification::Notification(std::string name) : name(name) {
    notify_init(name.c_str());
}

Notification::~Notification() {
    notify_uninit();
}

void Notification::send(std::string title, std::string body) {
    NotifyNotification* notification = notify_notification_new(title.c_str(), body.c_str(), nullptr);
    notify_notification_show(notification, nullptr);
    g_object_unref(G_OBJECT(notification));
}

void Notification::send(std::string body) {
    send(name, body);
}