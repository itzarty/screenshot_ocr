#include "Tray.h"
#include "Notifications.h"
#include "Worker.h"

std::vector<std::string> getLanguages() {
    std::vector<std::string> languages;
    tesseract::TessBaseAPI tess;
    tess.Init(nullptr, nullptr);
    tess.GetAvailableLanguagesAsVector( &languages );
    return languages;
}

int main(int argc, char **argv) {
    Display* display = XOpenDisplay(nullptr);
    if (display == nullptr) {
        std::cerr << "Cannot open X display" << std::endl;
        return 1;
    }

    std::vector<std::string> languages = getLanguages();

    const char *language = argc > 1 ? argv[1] : "eng";
    if (std::find(languages.begin(), languages.end(), language) == languages.end()) {
        std::cerr << "Unknown language \"" << language << R"(". Falling back to "eng".)" << std::endl;
        language = "eng";
    }

    tesseract::TessBaseAPI tess;

    if (tess.Init(nullptr, language)) {
        std::cerr << "Couldn't initialize Tesseract." << std::endl;
        return 1;
    }

    Notification notification("Screenshot OCR");

    Tray tray;
    Worker worker(display, &tess, [&tray, &notification](const std::string &text) {
        tray.setText(text);
        notification.send(text);
    });

    tray.setQuitHandler([&worker, &display]() {
        worker.quit();
        XCloseDisplay(display);
    });

    tray.run();
}