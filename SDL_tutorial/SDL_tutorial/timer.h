#pragma once

#include "SDL.h"

class Timer {
public:
    Timer();
    void start();
    void stop();
    void pause();
    void unpause();

    int get_ticks(); //Gets the timer's time
    bool is_started();
    bool is_paused();

private:
    int startTicks; //The clock time when the timer started
    int pausedTicks; //The ticks stored when the timer was paused
    bool paused;
    bool started;
};

