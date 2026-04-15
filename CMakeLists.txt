cmake_minimum_required(VERSION 4.0)
project(screenshot_ocr)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)

pkg_check_modules(TESSERACT REQUIRED tesseract lept)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(APPINDICATOR REQUIRED ayatana-appindicator3-0.1)
pkg_check_modules(LIBNOTIFY REQUIRED libnotify)

find_package(X11 REQUIRED)
find_package(OpenCV REQUIRED COMPONENTS core imgcodecs imgproc)

add_executable(
        screenshot_ocr
        main.cpp
        Tray.cpp
        Tray.h
        Worker.cpp
        Worker.h
        Notifications.cpp
        Notifications.h
)

target_include_directories(
        screenshot_ocr
        PRIVATE
        ${X11_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS}
        ${TESSERACT_INCLUDE_DIRS}
        ${GTK3_INCLUDE_DIRS}
        ${APPINDICATOR3_INCLUDE_DIRS}
        ${APPINDICATOR_INCLUDE_DIRS}
        ${LIBNOTIFY_INCLUDE_DIRS}
)

target_link_libraries(
        screenshot_ocr
        PRIVATE
        ${X11_LIBRARIES}
        Xfixes
        ${OpenCV_LIBRARIES}
        ${TESSERACT_LIBRARIES}
        ${GTK3_LIBRARIES}
        ${APPINDICATOR_LIBRARIES}
        ${LIBNOTIFY_LIBRARIES}
)

target_link_directories(
        screenshot_ocr
        PRIVATE
        ${TESSERACT_LIBRARY_DIRS}
        ${GTK3_LIBRARY_DIRS}
        ${APPINDICATOR3_LIBRARY_DIRS}
        ${APPINDICATOR_LIBRARY_DIRS}
)

target_compile_options(screenshot_ocr PRIVATE ${GTK3_CFLAGS_OTHER})