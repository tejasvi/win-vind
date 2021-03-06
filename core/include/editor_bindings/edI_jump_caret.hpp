#ifndef _EDI_JUMP_CARET_HPP
#define _EDI_JUMP_CARET_HPP

#include "binded_func_with_creator.hpp"

struct EdiJumpCaret2BOL : public BindedFuncWithCreator<EdiJumpCaret2BOL>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) ;
    static const std::string sname() noexcept ;

    bool is_for_moving_caret() const noexcept override ;
} ;

struct EdiJumpCaret2EOL : public BindedFuncWithCreator<EdiJumpCaret2EOL>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) ;
    static const std::string sname() noexcept ;

    bool is_for_moving_caret() const noexcept override ;
} ;

struct EdiNJumpCaret2Line_DfBOF : public BindedFuncWithCreator<EdiNJumpCaret2Line_DfBOF>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) ;
    static const std::string sname() noexcept ;

    bool is_for_moving_caret() const noexcept override ;
} ;

struct EdiNJumpCaret2Line_DfEOF : public BindedFuncWithCreator<EdiNJumpCaret2Line_DfEOF>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) ;
    static const std::string sname() noexcept ;

    bool is_for_moving_caret() const noexcept override ;
} ;
#endif
