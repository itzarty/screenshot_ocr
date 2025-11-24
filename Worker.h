#pragma once

#include <thread>
#include <opencv2/opencv.hpp>
#include <bits/this_thread_sleep.h>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <thread>
#include <X11/Xlib.h>

class Worker {
public:
    Worker(Display *display, tesseract::TessBaseAPI* tess, std::function<void(std::string)> callback);
    void quit();
private:
    bool stop = false;
    void loop();
    std::vector<unsigned char> snapshot;
    Display* display;
    tesseract::TessBaseAPI* tess;
    std::function<void(std::string)> callback;
    void run();
    std::vector<unsigned char> getScreenshot();
};