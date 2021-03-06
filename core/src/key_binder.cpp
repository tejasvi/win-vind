#include "key_binder.hpp"

#include <algorithm>
#include <array>
#include <deque>
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <windows.h>

#include "binded_func.hpp"
#include "disable_gcc_warning.hpp"
#include <nlohmann/json.hpp>
#include "easy_click.hpp"
#include "enable_gcc_warning.hpp"

#include "bindings_lists.hpp"
#include "change_mode.hpp"
#include "i_params.hpp"
#include "key_absorber.hpp"
#include "key_logger.hpp"
#include "key_matcher.hpp"
#include "mode_manager.hpp"
#include "msg_logger.hpp"
#include "path.hpp"
#include "utility.hpp"
#include "virtual_cmd_line.hpp"
#include "virtual_key_fwd.hpp"
#include "vkc_converter.hpp"

#undef max

using namespace std ;

namespace KeyBinder
{
    static std::vector<BindedFunc::shp_t> g_func_list{} ;
    static std::unordered_set<unsigned char> g_unbinded_syskeys{} ;

    void init() {
        g_func_list.clear() ;
        g_func_list = BindingsLists::get() ;

        g_unbinded_syskeys.clear() ;
        g_unbinded_syskeys = VKCConverter::get_all_sys_vkc() ;

        EsyClk::initialize() ;
    }

    using ModeManager::Mode ;
    static const std::unordered_map<Mode, const char*> g_modeidxs {
        {Mode::Normal,          "guin"},
        {Mode::Insert,          "guii"},
        {Mode::Visual,          "guiv"},
        {Mode::Command,         "cmd"},
        {Mode::EdiNormal,       "edin"},
        {Mode::EdiInsert,       "edii"},
        {Mode::EdiVisual,       "ediv"},
        {Mode::EdiLineVisual,   "edivl"},
    } ;

    template <typename T>
    inline auto _parse_strcmd(T&& cmdstr) {
        auto get_specode = [](std::string k) {
            if(k == "space")   return ' ' ;
            if(k == "hbar")    return '-' ;
            if(k == "gt")      return '>' ;
            if(k == "lt")      return '<' ;
            return static_cast<char>(0) ;
        } ;
        KeyMatcher::cmd_t cmd ;

        for(std::size_t i = 0 ; i < cmdstr.length() ; i ++) {
            const auto onechar = cmdstr[i] ;
            if(onechar != '<') {
                //ascii
                if(auto vkc = VKCConverter::get_vkc(onechar)) { //ex) a
                    cmd.emplace_back(1, vkc) ;
                    continue ;
                }

                //shifted ascii
                if(auto vkc = VKCConverter::get_shifted_vkc(onechar)) { //ex) A (A is divided to a and SHIFT)
                    cmd.push_back(KeyMatcher::keyset_t{vkc, VKC_SHIFT}) ;
                    continue ;
                }

                ERROR_PRINT(onechar + std::string(" of ") + cmdstr + "\tis invalid ascii key code") ;
                continue ;
            }

            auto pairpos = cmdstr.find('>', i + 1) ;
            if(pairpos == std::string::npos) {
                throw std::runtime_error("command is bad syntax. " + cmdstr +  " does not have a greater-than sign (>)") ;
            }

            KeyMatcher::keyset_t keyset{} ;
            const auto keystrset = Utility::split(cmdstr.substr(i + 1, pairpos - i - 1), "-") ;
            for(auto code = keystrset.begin() ; code != keystrset.end() ; code ++) {
                if(code != keystrset.begin() && code->length() == 1) { //ascii code
                    //ascii
                    if(auto vkc = VKCConverter::get_vkc(code->front())) {
                        keyset.push_back(vkc) ;
                        continue ;
                    }

                    //shifted ascii
                    if(auto vkc = VKCConverter::get_shifted_vkc(code->front())) {
                        keyset.push_back(vkc) ;
                        keyset.push_back(VKC_SHIFT) ;
                        continue ;
                    }
                }

                auto lowercode = Utility::A2a(*code) ;

                //if the cmd is same as some mode's key (e.g. <guin>, <edin>),
                //its pointer use same pointer to target mode.
                for(const auto& target_index : g_modeidxs) {
                    if(lowercode == target_index.second) {
                        throw target_index.first ;
                    }
                }

                if(lowercode == "any") {
                    keyset.push_back(VKC_OPTIONAL) ;
                    continue ;
                }
                if(lowercode == "num") {
                    keyset.push_back(VKC_OPTNUMBER) ;
                    continue ;
                }

                if(auto ascii = get_specode(lowercode)) {
                    if(auto vkc = VKCConverter::get_vkc(ascii)) {
                        keyset.push_back(vkc) ;
                        continue ;
                    }
                    if(auto vkc = VKCConverter::get_shifted_vkc(ascii)) {
                        keyset.push_back(vkc) ;
                        keyset.push_back(VKC_SHIFT) ;
                        continue ;
                    }

                    ERROR_PRINT(*code  + " is not supported. (" + Path::BINDINGS() + ")") ;
                    continue ;
                }

                if(const auto vkc = VKCConverter::get_sys_vkc(lowercode)) {
                    keyset.push_back(vkc) ;

                    //If a system key is bindied as a single command.
                    if(keystrset.size() == 1) {
                        g_unbinded_syskeys.erase(vkc) ;
                    }
                    continue ;
                }

                ERROR_PRINT(*code + "\t of " + cmdstr + " is invalid system key code") ;
            }

            cmd.push_back(std::move(keyset)) ;

            i = pairpos ;
            continue ;
        }
        return cmd ;
    }

    void load_config() {
        std::ifstream ifs{Path::BINDINGS()} ;
        nlohmann::json jp ;
        ifs >> jp ;
        if(jp.empty()) {
            throw std::runtime_error(Path::BINDINGS() + " is empty.") ;
        }

        if(!jp.is_array()) {
            throw std::runtime_error("The root element of " + Path::BINDINGS() + " should be array.") ;
        }

        constexpr auto mode_num = static_cast<unsigned char>(Mode::NUM) ;

        std::array<KeyMatcher::shp_t, mode_num> matcher_list ;

        //if JSON's data is "edin": ["<guin>"], index_links[edin-index] = guin-index
        std::array<unsigned char, mode_num> index_links ;

        if(g_func_list.empty()) {
            throw std::logic_error("KeyBinder has no defined BindFunc.") ;
        }

        //initialize the ignoring key list
        g_unbinded_syskeys = VKCConverter::get_all_sys_vkc() ;

        //create name lists of BindidFunc
        std::unordered_map<std::string, BindedFunc::shp_t> funclist ;
        for(auto& func : g_func_list) {
            funclist[func->name()] = func ;
        }

        for(auto& obj : jp) {
            try {
                auto& func = funclist.at(obj.at("name")) ;
                if(!obj.is_object()) {
                    ERROR_PRINT("The child of root-array should be object. (" \
                            + Path::BINDINGS() + ", name: " + obj["name"].get<std::string>() + ").") ;
                }

                matcher_list.fill(nullptr) ;
                index_links.fill(static_cast<unsigned char>(Mode::None)) ;

                for(const auto& index : g_modeidxs) {
                    try {
                        const auto& cmds = obj.at(index.second) ;
                        if(!cmds.is_array()) {
                            ERROR_PRINT("The command lists should be array (" \
                                    + func->name() + "/" + index.second + ").") ;
                            continue ;
                        }
                        if(cmds.empty()) {
                            continue ;
                        }
                        KeyMatcher::cmdlist_t cmdlist ;

                        for(std::string cmdstr : cmds) {
                            if(cmdstr.empty()) continue ;
                            KeyMatcher::cmd_t cmd ;
                            try {
                                cmd = _parse_strcmd(cmdstr) ;
                            }
                            catch(const std::runtime_error& e) {
                                ERROR_PRINT(func->name() + "::" + index.second \
                                        + " in " + Path::BINDINGS() + " " + e.what()) ;
                                continue ;
                            }
                            catch(const Mode m) {
                                index_links[static_cast<unsigned char>(index.first)] \
                                    = static_cast<unsigned char>(m) ;
                                cmdlist.clear() ;
                                break ;
                            }
                            cmdlist.push_back(cmd) ;
                        }

                        if(cmdlist.empty()) continue ;

                        //create KeyMatcher for one mode
                        matcher_list[static_cast<unsigned char>(index.first)] \
                            = std::make_shared<KeyMatcher>(std::move(cmdlist)) ;
                    }
                    catch(const std::out_of_range& e) {
                        ERROR_PRINT(e.what()) ;
                        continue ;
                    }
                }

                //If there are some key-bindings fields of the mode having <mode-name> (e.q. <guin>, <edin>) in bindings.json ,
                //they are copied key-bindings from the first mode in json-array to them.
                //Ex) "guin": ["<Esc>", "happy"]
                //    "edin": ["<guin>", "<guii>"]    -> same as "guin"'s key-bindings(<Esc>, "happy")
                for(std::size_t i = 0 ; i < index_links.size() ; i ++) {
                    const auto link_idx = index_links[i] ;
                    if(link_idx == static_cast<unsigned char>(Mode::None)) 
                        continue ;

                    matcher_list[i] = matcher_list[link_idx] ;
                }

                for(std::size_t i = 0 ; i < matcher_list.size() ; i ++) {
                    func->register_matcher(static_cast<ModeManager::Mode>(i), matcher_list[i]) ;
                }
            }
            catch(const std::out_of_range& e) {
                ERROR_PRINT(std::string(e.what()) + ". The following syntax is invalid." + obj.dump()) ;
                continue ;
            }
        }

        //post process
        JumpCursorUtility::load_config() ;
        ExAppUtility::load_config() ;
    }

    bool is_invalid_log(const KeyLog& log, const InvalidPolicy ip) {

        if(log.empty()) return true ;

        auto must_ignore = [&log](auto&& set) {
            return std::all_of(log.cbegin(), log.cend(), [&set](const auto& key) {
                return set.find(key) != set.end() ;
            }) ;
        } ;

        switch(ip) {
            case None: {
                return false ;
            }
            case AllSystemKey: {
                static const auto system_keys = VKCConverter::get_all_sys_vkc() ;
                return must_ignore(system_keys) ;
            }
            case UnbindedSystemKey: {
                return must_ignore(g_unbinded_syskeys) ;
            }
            default: {
                return false ;
            }
        }
    }

    //This function regards as other functions is stronger than the running function.
    //If the 2nd argument is not passed, it regards as not processing.
    const BindedFunc::shp_t find_func(
            const KeyLogger& lgr,
            const BindedFunc::shp_t& running_func,
            const bool full_scan,
            ModeManager::Mode mode) {

        unsigned int most_matched_num  = 0 ;
        BindedFunc::shp_t matched_func = nullptr ;

        auto choose = [&most_matched_num, &matched_func](auto& func, auto num) {
            if(num > most_matched_num) {
                most_matched_num = num ;
                matched_func     = func ;
            }
            else if(num == most_matched_num && func->is_callable()) {
                //On same matching level, the callable function is the strongest.
                matched_func = func ;
            }
        } ;

        if(!running_func) { //lower cost version
            if(full_scan) {
                for(const auto& func : g_func_list)
                    choose(func, func->validate_if_fullmatch(lgr, mode)) ;
            }
            else {
                for(const auto& func : g_func_list)
                    choose(func, func->validate_if_match(lgr, mode)) ;
            }
            return matched_func ;
        }

        unsigned int matched_num ;
        if(full_scan) {
            for(const auto& func : g_func_list) {
                matched_num = func->validate_if_fullmatch(lgr, mode) ;
                if(running_func == func) continue ;
                choose(func, matched_num) ;
            }
        }
        else {
            for(const auto& func : g_func_list) {
                matched_num = func->validate_if_match(lgr, mode) ;
                if(running_func == func) continue ;
                choose(func, matched_num) ;
            }
        }

        //New matched function is given priority over running func.
        if(matched_func)
            return matched_func ;

        if(running_func->is_callable())
            return running_func ;

        return nullptr ;
    }

    const BindedFunc::shp_t find_func_byname(const std::string& name) {
            for(const auto& func : g_func_list) {
                if(func->name() == name) return func ;
            }
            return nullptr ;
    }

    void call_matched_funcs() {
        static KeyLogger l_logger{} ;
        static BindedFunc::shp_t l_running_func       = nullptr ;
        static unsigned int l_repeat_num              = 0 ;
        static bool l_must_release_key_after_repeated = false ;

        static const KeyLog c_nums {
            VKC_0, VKC_1, VKC_2, VKC_3, VKC_4,
            VKC_5, VKC_6, VKC_7, VKC_8, VKC_9
        } ;

        using Utility::remove_from_back ;

        if(!KyLgr::log_as_vkc(l_logger)) {
            if(!l_running_func) {
                remove_from_back(l_logger, 1) ;
                return ;
            }
            l_running_func->process(false, 1, &l_logger, nullptr) ;
            remove_from_back(l_logger, 1) ;
            return ;
        }

        if(l_repeat_num != 0) {
            if(l_logger.back().is_containing(VKC_ESC)) {
                l_repeat_num = 0 ;
                VirtualCmdLine::reset() ;
            }
        }

        //Note
        //it ignores solo system keys.
        //Ex)
        //  ______________________________________________________
        // |                |                       |             |
        // |   input keys   |        Shift          |  Shift + t  |
        // |                | (unbinded key only)   |             | 
        // |----------------|-----------------------|-------------|
        // |   behavior     |        ignore         |    pass     |
        // |________________|_______________________|_____________|
        //
        if(is_invalid_log(l_logger.back(), InvalidPolicy::UnbindedSystemKey)) {
            remove_from_back(l_logger, 1) ;
            l_running_func = nullptr ;

            if(l_must_release_key_after_repeated) {
                l_must_release_key_after_repeated = false ;
            }
            return ;
        }

        // Note about l_must_release_key_after_repeated:
        // false : same as default.
        // true  : wait until some unbinded sytem keys are inputed or no keys is inputed.
        // 
        // This behavior is needed to prohibit following case.
        // Ex)
        //  ________________________________________________________________________________________
        // |                            |      |         |                   |                      |
        // |         input keys         |  2   |  Shift  |      Shift + j    |         j            |
        // |----------------------------|------|---------|-------------------|----------------------|
        // | called func name (without) |  -   |    -    |  edi_n_remove_EOL | edi_move_caret_down  |
        // |----------------------------|------|---------|-------------------|----------------------|
        // | called func name (with)    |  -   |    -    |  edi_n_remove_EOL |          -           |
        // |____________________________|______|_________|___________________|______________________|
        //
        if(l_must_release_key_after_repeated) {
            remove_from_back(l_logger, 1) ;
            l_running_func = nullptr ;
            return ;
        }
        auto topvkc = *(l_logger.back().begin()) ;

        //If some numbers has inputed, ignore commands binded by numbers.
        if(l_repeat_num != 0) {
            l_logger.back() -= c_nums ;
        }

        auto matched_func = find_func(l_logger, l_running_func) ;

        if(!matched_func) {
            if(!VKCConverter::is_number(topvkc)) {
                //If inputed non-numeric key, reset the repeat number.
                if(l_repeat_num != 0) {
                    l_repeat_num = 0 ;
                    VirtualCmdLine::reset() ;
                }
            }
            else {
                static constexpr auto max = std::numeric_limits<unsigned int>::max() / 10 ;
                using ModeManager::Mode ;
                auto mode = ModeManager::get_mode() ;
                if(l_repeat_num < max && //Whether it is not out of range?
                        mode != Mode::Insert && mode != Mode::EdiInsert) {
                    l_repeat_num = l_repeat_num * 10 + VKCConverter::to_number(topvkc) ;
                    VirtualCmdLine::cout(std::to_string(l_repeat_num)) ;
                }
            }

            l_logger.clear() ;
            l_running_func = nullptr ;
            return ;
        }

        if(matched_func->is_callable()) {
            l_running_func = matched_func ;

            if(l_repeat_num == 0) {
                l_running_func->process(true, 1, &l_logger, nullptr) ;
            }
            else {
                VirtualCmdLine::reset() ;
                l_running_func->process(true, l_repeat_num, &l_logger, nullptr) ;
                l_repeat_num = 0 ;
                l_must_release_key_after_repeated = true ;
            }

            l_logger.clear() ;
            return ;
        }
    }
}
