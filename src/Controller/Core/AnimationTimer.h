#ifndef _AnimationTimer_h_
#define _AnimationTimer_h_

#include <iostream>
#include <chrono>
#include <thread>

#include "PathfinderTemplate.h"
#include "GeneratorTemplate.h"
#include "Constants.h"
#include "Timer.h"

namespace PFSim {

    class AnimationTimer : public Timer
    {
    public:
        AnimationTimer(AnimationType aType, int mazeLength, GeneratorType gType = Open);
        
        void run();

        // void updateMultiplier();

    private:
        long long m_Time; // microseconds
        // int m_Multiplier;

        void setGeneratorTimer(GeneratorType type);
        void setPathfinderTimer(GeneratorType type);
    };

} // namespace PFSim

#endif