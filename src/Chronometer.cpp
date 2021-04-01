#include "Chronometer.h"

namespace blocks 
{
    bool Chronometer::hasTicks()
    {
        _timerSeconds += _stopwatch.getElapsedSeconds();
        _stopwatch.reset();
        
        if (_timerSeconds > _intervalSeconds) {
            _timerSeconds -= _intervalSeconds;
            return true;
        }

        return false;
    }
}
