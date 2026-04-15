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
    bool found = false;
    for (int i = 0; i < 10; ++i) {
        if (XCheckTypedWindowEvent(display, window, SelectionNotify, &event)) {
            found = true;
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    std::vector<unsigned char> result;
    if (found && event.xselection.property != None) {
        Atom actualType;
        int format;
        unsigned long nItems, bytesAfter;
        unsigned char* data = nullptr;
        XGetWindowProperty(display, window, propertyAtom, 0, (~0L), False, AnyPropertyType, &actualType, &format, &nItems, &bytesAfter, &data);
        if (data) {
            result.assign(data, data + nItems);
            XFree(data);
            XDeleteProperty(display, window, propertyAtom);
        }
    }
    XDestroyWindow(display, window);
    return result;
}

void Worker::loop() {
    int xfixesEventBase, xfixesErrorBase;

    if (!XFixesQueryExtension(display, &xfixesEventBase, &xfixesErrorBase)) {
        std::cerr << "XFixes extension not available" << std::endl;
        return;
    }

    Atom clipboard = XInternAtom(display, "CLIPBOARD", False);
    XFixesSelectSelectionInput(display, DefaultRootWindow(display), clipboard, XFixesSetSelectionOwnerNotifyMask);

    XEvent event;
    while (!stop) {
        if (XPending(display)) {
            XNextEvent(display, &event);
            if (event.type == xfixesEventBase + XFixesSelectionNotify) {
                XFixesSelectionNotifyEvent* sev = (XFixesSelectionNotifyEvent*)&event;
                std::vector<unsigned char> imageData = getScreenshot();

                if (!imageData.empty()) {
                    Pix* imagePix = pixReadMemPng(imageData.data(), imageData.size());
                    if (imagePix) {
                        tess->SetImage(imagePix);
                        char* text = tess->GetUTF8Text();
                        if (text) {
                            callback(std::string(text));
                            delete[] text;
                        }
                        pixDestroy(&imagePix);
                    }
                }
            }
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
}

void Worker::quit() {
    stop = true;
}