#include "Tray.h"
#include "Notifications.h"
#include "Worker.h"

std::vector<std::string> getLanguages() {
    std::vector<std::string> languages;
    tesseract::TessBaseAPI tess;
    tess.Init(nullptr, nullptr);
    tess.GetAvailableLanguagesAsVector(&languages);
    return languages;
}

bool validateLanguage(const char* language) {
    std::vector<std::string> languages = getLanguages();

    std::string token;
    std::istringstream tokenStream(language);
    while (std::getline(tokenStream, token, '+')) {
        if (std::ranges::find(languages, token) == languages.end()) {
            std::cerr << "Unknown language \"" << token << "\"" << std::endl;
            return false;
        }
    }

    return true;
}

int main(int argc, char **argv) {
    Display* display = XOpenDisplay(nullptr);
    if (display == nullptr) {
        std::cerr << "Cannot open X display" << std::endl;
        return 1;
    }

    const char *language = argc > 1 ? argv[1] : "eng";
    bool languageValid = validateLanguage(language);

    if (!languageValid) {
        std::cerr << "Language invalid, terminating." << std::endl;
        return 1;
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