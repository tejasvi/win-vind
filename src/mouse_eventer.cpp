#include "mouse_eventer.hpp"
#include "msg_logger.hpp"
#include <iostream>
using namespace std ;

#define MOUSEEVENTF_HWHEEL 0x01000

namespace MouseEventer
{
    bool is_click(const Button bt) noexcept {
        INPUT in ;
        in.type = INPUT_MOUSE ;
        in.mi.dx = 0 ;
        in.mi.dy = 0 ;
        in.mi.mouseData = 0 ;
        in.mi.dwFlags = MOUSEEVENTF_LEFTDOWN << bt ; //if bt is 2, converted RIGHT
        in.mi.time = 0 ;
        in.mi.dwExtraInfo = GetMessageExtraInfo() ;

        if(!SendInput(1, &in, sizeof(INPUT))) {
            Logger::error_stream << "[Error] windows.h: " \
            << GetLastError() \
            << " (win_mouse_eventer.cpp)" << endl ;
            return false ;
        }

        in.mi.dwFlags = MOUSEEVENTF_LEFTUP << bt ;
        if(!SendInput(1, &in, sizeof(INPUT))) {
            Logger::error_stream << "[Error] windows.h: " \
            << GetLastError() \
            << " (win_mouse_eventer.cpp)" << endl ;
            return false ;
        }

        return true ;
    }

    inline static bool is_down_or_up(const DWORD event, const Button bt) noexcept {
        INPUT in ;
        in.type = INPUT_MOUSE ;
        in.mi.dx = 0 ;
        in.mi.dy = 0 ;
        in.mi.mouseData = 0 ;
        in.mi.dwFlags = event << bt ;
        in.mi.time = 0 ;
        in.mi.dwExtraInfo = GetMessageExtraInfo() ;

        if(!SendInput(1, &in, sizeof(INPUT))) {
            Logger::error_stream << "[Error] windows.h: " \
            << GetLastError() \
            << " (win_mouse_eventer.cpp)" << endl ;
            return false ;
        }

        return true ;
    }

    bool is_down(const Button bt) noexcept {
        return is_down_or_up(MOUSEEVENTF_LEFTDOWN, bt) ;
    }

    bool is_up(const Button bt) noexcept {
        return is_down_or_up(MOUSEEVENTF_LEFTUP, bt) ;
    }

    inline static bool is_scroll_core(const DWORD event, const int scr_delta) noexcept {
        INPUT in ;
        in.type = INPUT_MOUSE ;
        in.mi.dx = 0 ; //amount of motion
        in.mi.dy = 0 ; //amount of motion
        in.mi.mouseData = scr_delta ;
        in.mi.dwFlags = event ;
        in.mi.time = 0 ;
        in.mi.dwExtraInfo = GetMessageExtraInfo() ;

        if(!SendInput(1, &in, sizeof(INPUT))) {
            Logger::error_stream << "[Error] windows.h: " \
            << GetLastError() \
            << "(win_mouse_eventer.cpp)" << endl ;
            return false ;
        }

        return true ;
    }

    bool is_vscroll(const int scr_delta) noexcept {
        return is_scroll_core(MOUSEEVENTF_WHEEL, scr_delta) ;
    }

    bool is_hscroll(const int scr_delta) noexcept {
        return is_scroll_core(MOUSEEVENTF_HWHEEL, scr_delta) ;
    }
    
}