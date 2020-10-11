#ifndef _EXTERNAL_APP_HPP
#define _EXTERNAL_APP_HPP

#include "binded_func_with_creator.hpp"

namespace ExAppUtility
{
    void load_config() ;
}

struct StartShell : public BindedFuncWithCreator<StartShell>
{
    static void sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct StartAnyApp : public BindedFuncWithCreator<StartAnyApp>
{
    static void sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

#endif
