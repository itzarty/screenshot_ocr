#include "Worker.h"

Worker::Worker(Display *display, tesseract::TessBaseAPI* tess, std::function<void(std::string)> callback) : display(display), tess(tess), callback(callback) {
    std::thread([this](){ this->loop(); }).detach();
}

std::vector<unsigned char> Worker::getScreenshot() {
    const Atom clipboardAtom = XInternAtom(display, "CLIPBOARD", False);
    const Atom pngAtom = XInternAtom(display, "image/png", False);
    const Atom propertyAtom = XInternAtom(display, "XCLIP_PROP", False);
    const Window window = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0, 1, 1, 0, 0, 0);
    XConvertSelection(display, clipboardAtom, pngAtom, propertyAtom, window, CurrentTime);
    XEvent event;
    XNextEvent(display, &event);
    std::vector<unsigned char> result;
    if (event.type == SelectionNotify) {
        Atom actualType;
        int format;
        unsigned long nItems, bytesAfter;
        unsigned char* data = nullptr;
        XGetWindowProperty(display, window, propertyAtom, 0, (~0L), False, AnyPropertyType, &actualType, &format, &nItems, &bytesAfter, &data);
        if (data) {
            result.assign(data, data + nItems);
            XFree(data);
        }
    }
    XDestroyWindow(display, window);
    return result;
}

void Worker::loop() {
    while (!stop) {
        std::vector<unsigned char> imageData = getScreenshot();
        if (imageData != snapshot) {
            snapshot = imageData;
            if (!imageData.empty()) {
                cv::Mat image = cv::imdecode(imageData, cv::IMREAD_COLOR);
                if (!image.empty()) {
                    Pix* imagePix = pixReadMemPng(imageData.data(), imageData.size());
                    if (!imagePix) {
                        return;
                    }
                    tess->SetImage(imagePix);
                    char* text = tess->GetUTF8Text();
                    callback(text);
                    pixDestroy(&imagePix);
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void Worker::quit() {
    stop = true;
}