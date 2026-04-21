# Screenshot OCR

A lightweight, automated utility that performs OCR when a screenshot is taken.

Designed as a proof-of-concept for X11, it also works seamlessly under Wayland (via XWayland) by monitoring clipboard buffers.

## Features

* **Fully automated clipboard tracking** => Automatically detects changes in the clipboard (e.g., by GNOME Screenshot).
* **Multi-Language support** => Pass any Tesseract-supported language code as an argument.
* **System tray integration**
  * View detected text.
  * Save OCR results to a file.
  * Copy extracted text back to the clipboard.
* **Desktop notifications** => Uses `libnotify` to alert you when text is ready.

## Usage

```bash
$ ./screenshot_ocr [language(s)]
```

* **Examples**
  * `./screenshot_ocr slk` - Loads Slovak
  * `./screenshot_ocr eng+slk` - Loads English **&** Slovak
  * `./screenshot_ocr eng+slk+deu+...`
* If no argument is provided, it falls back to `eng`.

## Dependencies

You will need the development headers for the following:

* `Tesseract` & `Leptonica` (OCR engine)
* `OpenCV` (Image processing)
* `GTK 3.0` & `Ayatana AppIndicator` (Tray menu)
* `libnotify` (Desktop alerts)
* `X11` & `Xfixes` (Clipboard access)

## Installation & building

### Installing dependencies

#### Ubuntu / Debian

```bash
sudo apt update
sudo apt install build-essential cmake libtesseract-dev libleptonica-dev libopencv-dev libgtk-3-dev libayatana-appindicator3-dev libnotify-dev libx11-dev
```

**Language data sets** => `sudo apt install tesseract-ocr-eng` (or `tesseract-ocr-slk`, `tesseract-ocr-deu`, etc.)

#### Arch

```bash
sudo pacman -S base-devel cmake tesseract libayatana-appindicator3 leptonica opencv cmake gtk3 libnotify
```

**Language data sets** => `sudo pacman -S tesseract-data-eng` (or `tesseract-ocr-jpn`, `tesseract-ocr-rus`, etc.)

*(I could be mistaken with the dependency commands. Please let me know if so).*

### Building the project

The build system used here is **CMake**

```bash
git clone https://github.com/itzarty/screenshot_ocr
cd ./screenshot_ocr

mkdir build && cd build
cmake ..
make
```

## Compatibility

Tested and confirmed on:
* **Arch Linux**: GNOME 50 (Wayland/XWayland) - NVIDIA/Intel
* **Debian 12**: GNOME 43 (X11) - NVIDIA + AMD

### How it works

* Once launched, a worker is spun up, which looks for changes in the clipboard. (Using X11 lib and the Xfixes extension)
* When a change is detected, we check if the new data is an image and if so, the image is read and fed into Tesseract.
* Once Tesseract provides us with a result, it's announced to the rest of the app