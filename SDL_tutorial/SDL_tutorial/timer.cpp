#include "timer.h"


Timer::Timer() {
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

void Timer::start() {
    started = true;
    paused = false;
    startTicks = SDL_GetTicks();
}

void Timer::stop() {
    started = false;
    paused = false;
}

void Timer::pause() {
    if( ( started == true ) && ( paused == false ) ) {
        paused = true;
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}

void Timer::unpause() {
    if( paused == true ) {
        paused = false;
        startTicks = SDL_GetTicks() - pausedTicks; //Reset the starting ticks
        pausedTicks = 0; //Reset the paused ticks
    }
}

int Timer::get_ticks() {
    if( started == true ) {
        if( paused == true ) {
            return pausedTicks; //Return the number of ticks when the timer was paused
		} else { return SDL_GetTicks() - startTicks; } //Return the current time minus the start time
    }
    return 0; //If the timer isn't running
}

bool Timer::is_started() { return started; }

bool Timer::is_paused() { return paused; }