
#include "config.hpp"
#include "defines.hpp"

#define DEFAULT_WRAP true
#define DEFAULT_FILE_MB 512
#define DEFAULT_FONT_SIZE 12
#define DEFAULT_SCROLL true

void Config::load(){

    // Create config directory if it does not exist
    if (!QDir(CONFIG_PATH).exists()){
        QDir().mkdir(CONFIG_PATH);
    }

    // Load and save a new config with defaults if it does not exist
    if (!QFile(CONFIG_FILE_PATH).exists()){
        this->settings = new QSettings(CONFIG_FILE_PATH, QSettings::IniFormat);
        this->settings->setValue("max_file_size_mb",QVariant(DEFAULT_FILE_MB));
        this->settings->setValue("wrap",QVariant(DEFAULT_WRAP));
        this->settings->setValue("font_size",QVariant(DEFAULT_FONT_SIZE));
        this->settings->setValue("show_scroll_bars", QVariant(DEFAULT_SCROLL));
        this->settings->sync();
    }else{
        this->settings = new QSettings(CONFIG_FILE_PATH, QSettings::IniFormat);
    }

}

int Config::max_file_size_mb(){
    return this->settings->value("max_file_size_mb", QVariant(DEFAULT_FILE_MB)).toInt();
}

bool Config::wrap(){
    return this->settings->value("wrap",QVariant(DEFAULT_WRAP)).toBool();
}

void Config::set_wrap(bool wrap){
    this->settings->setValue("wrap", QVariant(wrap));
}

int Config::font_size(){
    return this->settings->value("font_size",QVariant(DEFAULT_FONT_SIZE)).toInt();
}

bool Config::show_scroll_bars(){
    return this->settings->value("show_scroll_bars",QVariant(DEFAULT_SCROLL)).toBool();
}

void Config::save(){
    this->settings->sync();
}
