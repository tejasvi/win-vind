#include "key_log.hpp"

#include <algorithm>

using namespace std ;

struct KeyLog::Impl
{
    data_t once_log ;
    explicit Impl()
    : once_log()
    {}

    explicit Impl(const data_t& codes)
    : once_log(codes)
    {}

    explicit Impl(data_t&& codes)
    : once_log(codes)
    {}

    explicit Impl(initializer_list<unsigned char>&& codes)
    : once_log(codes)
    {}
} ;

KeyLog::KeyLog()
: pimpl(make_unique<Impl>())
{}

KeyLog::KeyLog(const data_t& codes)
: pimpl(make_unique<Impl>(codes))
{}

KeyLog::KeyLog(data_t&& codes)
: pimpl(make_unique<Impl>(std::move(codes)))
{}

KeyLog::KeyLog(initializer_list<unsigned char>&& codes)
: pimpl(make_unique<Impl>(std::move(codes)))
{}

KeyLog::~KeyLog() noexcept          = default ;
KeyLog::KeyLog(KeyLog&&)            = default ;
KeyLog& KeyLog::operator=(KeyLog&&) = default ;

KeyLog::KeyLog(const KeyLog& rhs)
: pimpl(rhs.pimpl ? make_unique<Impl>(*rhs.pimpl) : make_unique<Impl>())
{}

KeyLog& KeyLog::operator=(const KeyLog& rhs)
{
    if(!(rhs.pimpl)) return *this ; //if already moved, not copy
    *pimpl = *rhs.pimpl ;
    return *this ;
}

const KeyLog::data_t& KeyLog::get() const & noexcept
{
    return pimpl->once_log ;
}

KeyLog::data_t&& KeyLog::get() && noexcept
{
    return std::move(pimpl->once_log) ;
}

KeyLog::data_t::const_iterator KeyLog::begin() const noexcept
{
    return pimpl->once_log.begin() ;
}

KeyLog::data_t::const_iterator KeyLog::end() const noexcept
{
    return pimpl->once_log.end() ;
}

KeyLog::data_t::const_iterator KeyLog::cbegin() const noexcept
{
    return pimpl->once_log.cbegin() ;
}

KeyLog::data_t::const_iterator KeyLog::cend() const noexcept
{
    return pimpl->once_log.cend() ;
}

std::size_t KeyLog::size() const noexcept
{
    return pimpl->once_log.size() ;
}

bool KeyLog::empty() const noexcept
{
    return pimpl->once_log.empty() ;
}

bool KeyLog::is_containing(const unsigned char key) const
{
    return pimpl->once_log.find(key) != pimpl->once_log.end() ;
}

bool KeyLog::operator==(const KeyLog& rhs) const
{
    if(!(rhs.pimpl)) return false ; //moved
    return pimpl->once_log == rhs.pimpl->once_log ;
}

bool KeyLog::operator!=(const KeyLog& rhs) const
{
    if(!(rhs.pimpl)) return false ; //moved
    return pimpl->once_log != rhs.pimpl->once_log ;
}

const KeyLog KeyLog::operator-(const KeyLog& rhs) const
{
    if(!(rhs.pimpl)) return *this ;
    auto diff = pimpl->once_log ;
    for(const auto k : rhs) diff.erase(k) ;
    return KeyLog(std::move(diff)) ;
}

KeyLog& KeyLog::operator-=(const KeyLog& rhs)
{
    if(rhs.pimpl) {
        for(const auto k : rhs) pimpl->once_log.erase(k) ;
    }
    return *this ;
}
