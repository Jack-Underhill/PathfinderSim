#ifndef _AnimationTimer_h_
#define _AnimationTimer_h_

#include <iostream>
#include <chrono>
#include <thread>

#include "PathfinderTemplate.h"
#include "GeneratorTemplate.h"
#include "Constants.h"

namespace PFSim {

    class AnimationTimer
    {
    public:
        AnimationTimer();
        
        void run();

        void updateAnimation(AnimationType aType, int mazeLength, GeneratorType gType = Open);
        void updatePercentage(int value);

    private:
        long long m_RawTime; // microseconds
        long long m_AppliedTime; // microseconds
        int m_Percentage;

        void setGeneratorTimer(GeneratorType type);
        void setPathfinderTimer(GeneratorType type);

        void scaleToLength(int length);
        void scaleToPercentage();
    };

} // namespace PFSim

#endif