#ifndef _UI_TRANSLATOR_HPP
#define _UI_TRANSLATOR_HPP

#include <string>
#include <wx/string.h>

namespace UITrans
{
    enum class Label : unsigned char {
        SYSTRAY_PREFERENCES, //System Tray
        SYSTRAY_ABOUT,
        SYSTRAY_EXIT,

        PREF_SETTINGS, //Settings Tab
        PREF_SETTINGS_COMMON,

        PREF_SETTINGS_COMMON_GUILANG,
        PREF_SETTINGS_COMMON_GUILANG_US,
        PREF_SETTINGS_COMMON_GUILANG_JP,

        PREF_SETTINGS_COMMON_ICONSTYLE,
        PREF_SETTINGS_COMMON_ICONSTYLE_DARK,
        PREF_SETTINGS_COMMON_ICONSTYLE_LIGHT,

        PREF_SETTINGS_COMMON_KBTYPE,
        PREF_SETTINGS_COMMON_KBTYPE_US,
        PREF_SETTINGS_COMMON_KBTYPE_JP,
        PREF_SETTINGS_COMMON_KBTYPE_CUSTOM,

        PREF_SETTINGS_COMMON_RESOLUTION,

        PREF_SETTINGS_HOTKEYS,
        PREF_SETTINGS_COMMANDS,

        PREF_BINDLIST, //Bind List Tab
        PREF_BINDLIST_HOTKEYS,
        PREF_BINDLIST_COMMANDS,

        PREF_SHORTCUTAPPS, //Shortcut Apps Tab
        PREF_SHORTCUTAPPS_SHELL,
        PREF_SHORTCUTAPPS_NAME,
        PREF_SHORTCUTAPPS_PATH,
        PREF_SHORTCUTAPPS_ADD,
        PREF_SHORTCUTAPPS_DEL,

        PREF_OPTIONS, //Options Tab

        PREF_OPTIONS_OPTS,
        PREF_OPTIONS_DISPCMDS,

        PREF_OPTIONS_DISPCMDS_POS_UPPERLEFT,
        PREF_OPTIONS_DISPCMDS_POS_UPPERMID,
        PREF_OPTIONS_DISPCMDS_POS_UPPERRIGHT,
        PREF_OPTIONS_DISPCMDS_POS_MIDLEFT,
        PREF_OPTIONS_DISPCMDS_POS_CENTER,
        PREF_OPTIONS_DISPCMDS_POS_MIDRIGHT,
        PREF_OPTIONS_DISPCMDS_POS_LOWERLEFT,
        PREF_OPTIONS_DISPCMDS_POS_LOWERMID,
        PREF_OPTIONS_DISPCMDS_POS_LOWERRIGHT,

        PREF_RETURN_TO_DEFAULT, //General Buttons
        PREF_OK,
        PREF_CANCEL,
        PREF_APPLY
    } ;

    const wxString& trans(const Label index) ;
    const wxString& trans(const std::string id) ;
}

#endif