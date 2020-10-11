#ifndef _CHANGE_MODE_HPP
#define _CHANGE_MODE_HPP
#include "binded_func_with_creator.hpp"

struct Change2Normal : public BindedFuncWithCreator<Change2Normal>
{
    static void sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger, const bool vclmodeout=true) ;
    static const std::string sname() noexcept ;
} ;


struct Change2Insert : public BindedFuncWithCreator<Change2Insert>
{
    static void sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger, const bool vclmodeout=true) ;
    static const std::string sname() noexcept ;
} ;


struct Change2Visual : public BindedFuncWithCreator<Change2Visual>
{
    static void sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger, const bool vclmodeout=true) ;
    static const std::string sname() noexcept ;
} ;


struct Change2Editor : public BindedFuncWithCreator<Change2Editor>
{
    static void sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger, const bool vclmodeout=true) ;
    static const std::string sname() noexcept ;
} ;
#endif
