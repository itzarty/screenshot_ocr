#include "Tray.h"
#include "Notifications.h"
#include "Worker.h"

int main() {
    Display* display = XOpenDisplay(nullptr);
    if (display == nullptr) {
        std::cerr << "Cannot open X display\n";
        return 1;
    }

    tesseract::TessBaseAPI tess;
    if (tess.Init(nullptr, "eng")) {
        std::cerr << "Cannot initialize Tesseract\n";
        return 1;
    }

    Notification notification("Screenshot OCR");

    Tray tray;
    Worker worker(display, &tess, [&tray, &notification](std::string text) {
        tray.setText(text);
        notification.send(text);
    });

    tray.setQuitHandler([&worker, &display]() {
        worker.quit();
        XCloseDisplay(display);
    });

    tray.run();
}