#include "Tray.h"

Tray::Tray() {
    gtk_init(nullptr, nullptr);

    indicator = app_indicator_new("screenshot_ocr", "indicator-messages", APP_INDICATOR_CATEGORY_APPLICATION_STATUS);
    app_indicator_set_status(this->indicator, APP_INDICATOR_STATUS_ACTIVE);

    menu = gtk_menu_new();
    textItem = createItem("No text found");
    copyItem = createItem("Copy to clipboard");
    saveItem = createItem("Save to file");
    quitItem = createItem("Quit");

    g_signal_connect(copyItem, "activate", G_CALLBACK(+[](GtkWidget*, gpointer data) {
        Tray* self = static_cast<Tray*>(data);
        self->copy();
    }), this);

    g_signal_connect(saveItem, "activate", G_CALLBACK(+[](GtkWidget*, gpointer data) {
        Tray* self = static_cast<Tray*>(data);
        self->save();
    }), this);

    g_signal_connect(quitItem, "activate", G_CALLBACK(+[](GtkWidget*, gpointer data) {
        Tray* self = static_cast<Tray*>(data);
        self->quit();
    }), this);

    gtk_widget_show_all(menu);
    app_indicator_set_menu(indicator, GTK_MENU(menu));
}

GtkWidget* Tray::createItem(std::string label) {
    GtkWidget* item = gtk_menu_item_new_with_label(label.c_str());
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
    return item;
}

void Tray::setText(std::string text) {
    this->currentText = text;
    gtk_menu_item_set_label(GTK_MENU_ITEM(this->textItem), this->currentText.c_str());
}

void Tray::copy() {
    GtkClipboard* clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    gtk_clipboard_set_text(clipboard, currentText.c_str(), -1);
}

void Tray::save() {
    GtkWidget* dialog = gtk_file_chooser_dialog_new("Save OCR text", nullptr, GTK_FILE_CHOOSER_ACTION_SAVE, "_Cancel", GTK_RESPONSE_CANCEL, "_Save", GTK_RESPONSE_ACCEPT, nullptr);
    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), "ocr.txt");
    GtkFileFilter* filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "Text files");
    gtk_file_filter_add_pattern(filter, "*.txt");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char* filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        std::ofstream file(filename);
        file << currentText;
        file.close();
        g_free(filename);
    }
    gtk_widget_destroy(dialog);
}

void Tray::setQuitHandler(std::function<void()> callback) {
    this->callback = callback;
}

void Tray::quit() {
    callback();
    gtk_main_quit();
}

void Tray::run() {
    gtk_main();
}