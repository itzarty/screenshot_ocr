cmake_minimum_required(VERSION 4.0)
project(screenshot_ocr)

set(CMAKE_CXX_STANDARD 23)

find_package(X11 REQUIRED)
include_directories(${X11_INCLUDE_DIRS})

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(PkgConfig REQUIRED)
pkg_check_modules(TESSERACT REQUIRED tesseract lept)
include_directories(${TESSERACT_INCLUDE_DIRS})
link_directories(${TESSERACT_LIBRARY_DIRS})

pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
include_directories(${GTK3_INCLUDE_DIRS} ${APPINDICATOR3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS} ${APPINDICATOR3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})

pkg_check_modules(APPINDICATOR REQUIRED ayatana-appindicator3-0.1)
include_directories(${APPINDICATOR_INCLUDE_DIRS})
link_directories(${APPINDICATOR_LIBRARY_DIRS})

pkg_check_modules(LIBNOTIFY REQUIRED libnotify)
include_directories(${LIBNOTIFY_INCLUDE_DIRS})

add_executable(screenshot_ocr
        main.cpp
        Tray.cpp
        Tray.h
        Worker.cpp
        Worker.h
        Notifications.cpp
        Notifications.h
)

target_link_libraries(screenshot_ocr ${X11_LIBRARIES} ${OpenCV_LIBRARIES} ${TESSERACT_LIBRARIES} ${GTK3_LIBRARIES} ${APPINDICATOR_LIBRARIES} ${LIBNOTIFY_LIBRARIES})
