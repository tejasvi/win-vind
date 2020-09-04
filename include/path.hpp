#ifndef _PATH_HPP
#define _PATH_HPP

#include <string>
#include <fstream>
#include <windows.h>

#include "msg_logger.hpp"
#include "i_params.hpp"

namespace Path
{
    static inline const auto _get_home_path() noexcept {
        char home_path[200] = {0} ;
        if(!GetEnvironmentVariableA("HOMEPATH", home_path, 1000)) {
            WIN_ERROR_STREAM << "cannot find %HOMEPATH% (DefaultConfig::get_home_path::GetEnviromentVariable)\n" ;
            return std::string() ;
        }
        return std::string(home_path) + '\\' ;
    }

    inline static const auto& HOME_PATH() noexcept {
        static const auto obj = _get_home_path() ;
        return obj ;
    }

    inline static const auto _get_path(std::string filename) noexcept {
        static const auto use_installer = [] {
            std::ifstream ifs{"default_config/is_installer_used"} ;
            std::string index_str{} ;
            std::getline(ifs, index_str) ;
            return index_str.front() == '1' ;
        }() ;

        if(!use_installer) {
            return "config\\" + filename ;
        }
        return HOME_PATH() + ".win-vind\\" + filename ;
    }

    inline static const auto& BINDINGS() noexcept {
        static const auto obj = _get_path("bindings.json") ;
        return obj ;
    }
    inline static const auto& SETTINGS() noexcept {
        static const auto obj = _get_path("settings.json") ;
        return obj ;
    }

    inline static const auto KEYBRD_MAP() noexcept {
        return _get_path(iParams::get_s("kb_type")) ;
    }

    namespace Default {
        inline static const auto BINDINGS() noexcept {
            return std::string("default_config/bindings.json") ;
        }
        inline static const auto SETTINGS() noexcept {
            return std::string("default_config/settings.json") ;
        }
        inline static const auto UI() noexcept {
            return std::string("default_config/ui.json") ;
        }
    }
}

#endif