#include "keystroke_repeater.hpp"

#include <chrono>
#include <iostream>
#include <random>

#include "interval_timer.hpp"

using namespace std::chrono ;

inline static const auto _const_accelerate(
        float& velocity,
        const float ms) noexcept {
    //hardcoded
    static constexpr auto MAX_ACCELERATION = 1.0f ;
    static constexpr auto MAX_VELOCITY     = 1.0f ;

    //ms -> s
    const auto t = ms / 1000 ;

    const auto delta_v = MAX_ACCELERATION * t ;
    if(velocity + delta_v < MAX_VELOCITY)
        velocity += delta_v ;
    else
        velocity = MAX_VELOCITY ;
    return velocity ;
}

inline static const auto _compute_deltat(
        const system_clock::time_point& start_time) noexcept {
    return duration_cast<milliseconds>(system_clock::now() - start_time) ;
}

inline static const auto _generate_uniform() {
    static std::random_device seed_gen ;
    static std::default_random_engine engine(seed_gen()) ;
    static std::uniform_real_distribution<float> dist(0.0f, 1.0f) ;
    return dist(engine) ;
}

//hardcoded
//static constexpr auto WAIT_TIME_FOR_STARTING   = 512ms;
static constexpr auto REPEAT_SAMPLING_DELTA_US = 25'600 ;
static constexpr auto INITIAL_VELOCITY         = 0.000'1f ;

struct KeyStrokeRepeater::Impl {
    IntervalTimer timer{REPEAT_SAMPLING_DELTA_US} ;
    float v = INITIAL_VELOCITY ;
    system_clock::time_point start_time{system_clock::now()} ;
    const milliseconds wait_time ;

    explicit Impl(const unsigned int wait_time_for_starting_ms)
    : timer(REPEAT_SAMPLING_DELTA_US),
      v(INITIAL_VELOCITY),
      start_time(system_clock::now()),
      wait_time(wait_time_for_starting_ms)
    {}
} ;

KeyStrokeRepeater::KeyStrokeRepeater(const unsigned int wait_time_for_starting_ms)
: pimpl(std::make_unique<Impl>(wait_time_for_starting_ms))
{}

KeyStrokeRepeater::~KeyStrokeRepeater() noexcept                     = default ;
KeyStrokeRepeater::KeyStrokeRepeater(KeyStrokeRepeater&&)            = default ;
KeyStrokeRepeater& KeyStrokeRepeater::operator=(KeyStrokeRepeater&&) = default ;

void KeyStrokeRepeater::reset() noexcept {
    pimpl->v = INITIAL_VELOCITY ;
    pimpl->start_time = system_clock::now() ;
}

bool KeyStrokeRepeater::is_pressed() {
    const auto dt = _compute_deltat(pimpl->start_time) ;

    if(dt < pimpl->wait_time) return false ;

    //sampling
    if(!pimpl->timer.is_passed()) return false ;

    if(_const_accelerate(pimpl->v, static_cast<float>(dt.count()))
            < _generate_uniform()) {
        return false ;
    }
    return true ;
}
