#include "Ticker.h"

namespace blocks 
{
    bool Ticker::hasTicks()
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
