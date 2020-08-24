#include "key_logger.hpp"
#include "key_absorber.hpp"
#include "vkc_converter.hpp"
#include "utility.hpp"
#include "virtual_key_fwd.hpp"

#include <iostream>

using namespace std ;

struct KeyLogger::Impl
{
    data_t logs ;
    static KeyLog past_log ;

    explicit Impl() : logs() {}

    ~Impl() noexcept {
        logs.clear() ;
    }

    Impl(Impl&&) = delete ;
    Impl& operator=(Impl&&) = delete ;
    Impl(const Impl&) = delete ;
    Impl& operator=(const Impl&) = delete ;
} ;

KeyLog KeyLogger::Impl::past_log{} ;


KeyLogger::KeyLogger() noexcept
: pimpl(make_unique<Impl>())
{}

KeyLogger::~KeyLogger() noexcept = default ;

size_t KeyLogger::size() const noexcept
{
    return pimpl->logs.size() ;
}

bool KeyLogger::is_empty() const noexcept
{
    return pimpl->logs.empty() ;
}

void KeyLogger::clear() noexcept
{
    pimpl->logs.clear() ;
}

void KeyLogger::remove_from_top(const std::size_t num_from_top) noexcept
{
    if(pimpl->logs.empty()) return ;
    pimpl->logs.erase(pimpl->logs.begin(), pimpl->logs.begin() + num_from_top) ;
}

void KeyLogger::remove_from_back(std::size_t num_from_back) noexcept
{
    if(pimpl->logs.empty()) return ;
    if(pimpl->logs.size() < num_from_back) {
        num_from_back = pimpl->logs.size() ;
    }
    pimpl->logs.erase(pimpl->logs.end() - num_from_back, pimpl->logs.end()) ;
}

KeyLogger::data_t::const_iterator KeyLogger::begin() const noexcept
{
    return pimpl->logs.begin() ;
}

KeyLogger::data_t::const_iterator KeyLogger::end() const noexcept
{
    return pimpl->logs.end() ;
}

KeyLogger::data_t::const_iterator KeyLogger::cbegin() const noexcept
{
    return pimpl->logs.cbegin() ;
}

KeyLogger::data_t::const_iterator KeyLogger::cend() const noexcept
{
    return pimpl->logs.cend() ;
}

const KeyLog KeyLogger::back() const noexcept
{
    if(pimpl->logs.empty()) {
        return std::move(KeyLog{}) ;
    }

    return pimpl->logs.back() ;
}

const KeyLog KeyLogger::at(const size_t i) const
{
    return pimpl->logs.at(i) ;
}

bool KeyLogger::is_changed_and_update()
{
    auto log = KeyAbsorber::get_downed_list() ;
    pimpl->logs.push_back(log) ;

    if(pimpl->past_log == log) {
        return false ;
    }

    pimpl->past_log = std::move(log) ; //update past log
    return true ;
}

//regard inputed key log as ascii charactor
bool KeyLogger::is_changed_and_inputc()
{
    auto log = KeyAbsorber::get_downed_list() ;
    const auto result = pimpl->past_log != log ;
    const auto diff = log - pimpl->past_log ;
    pimpl->past_log = log ;

    if(log.is_including(VKC_SHIFT)) {
        auto data = diff.get() ;
        data.push_back(VKC_SHIFT) ;
        if(log.is_including(VKC_LSHIFT)) {
            data.push_back(VKC_LSHIFT) ;
        }
        if(log.is_including(VKC_RSHIFT)) {
            data.push_back(VKC_RSHIFT) ;
        }

        Utility::remove_deplication(data) ;
        //construct KeyLog inside logs directly from std::vector
        pimpl->logs.emplace_back(std::move(data)) ;
        return result ;
    }

    pimpl->logs.push_back(std::move(diff)) ;
    return result ;
}

const string KeyLogger::get_str() const noexcept
{
    string str{} ;
    if(pimpl->logs.empty()) {
        return str ;
    }

    for(const auto& log : pimpl->logs) {
        if(log.is_including(VKC_SHIFT)) {
            //shifted ascii
            for(const auto vkc : log) {
                const auto c = VKCConverter::get_shifted_ascii(vkc) ;
                if(c != 0) str.push_back(c) ;
            }
            continue ;
        }
        for(const auto vkc : log) {
            const auto c = VKCConverter::get_ascii(vkc) ;
            if(c != 0) str.push_back(c) ;
        }
    }
    return str ;
}