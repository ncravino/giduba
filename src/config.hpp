#pragma once

#include <QDir>
#include <QSettings>

/*!
 \class Config
 \brief The Config class deals with loading and saving application configurations

 Currently implemented configurations are:
 - max_file_size_mb: the maximum file size in MB
 - wrap: if we want line wrapping on by default
 - font_size: the editor font size
 - show_scroll_bars: if we want scroll bars to always show (except the horizontal when wrap is on)

 Only wrap can be set at runtime via edit menu.

 */
class Config {
   QSettings * settings;

public:

    /*!
    * Load configuration file from $HOME/.config/giduba/giduba.conf
    */
    void load();

    /*!
    * Save configuration file from $HOME/.config/giduba/giduba.conf
    */
    void save();

    /*!
    * The maximum input file size in megabytes
    */
    int max_file_size_mb();

    /*!
    * The line wrapping mode
    */
    bool wrap();

    /*!
    * Set the line wrapping mode
    * This can be set in the edit menu during runtime
    */
    void set_wrap(bool);

    /*!
    * The default font size
    */
    int font_size();

    /*!
    * Always show scrolling bars or just when needed?
    * horizontal scroll bar will be hidden when wrapping is on.
    */
    bool show_scroll_bars();

};
