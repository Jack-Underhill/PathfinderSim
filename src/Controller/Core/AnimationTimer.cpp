#include "AnimationTimer.h"

namespace PFSim {

    AnimationTimer::AnimationTimer(AnimationType aType, int mazeLength, GeneratorType gType)
    {
        switch (aType)
        {
        case(DrawPath):
            m_Time = 18000;
            break;
        case(Reset):
            m_Time = 4000;
            break;
        case(Generate):
            setGeneratorTimer(gType);
            break;
        case(Pathfind):
            setPathfinderTimer(gType);
            break;
        }

        // scale timer to the mazelength
        m_Time *= (std::pow(DEFAULT_MAZE_LENGTH, 1.05) / std::pow(mazeLength, 1.1));
    }
    
    void AnimationTimer::run()
    {
        // //non thread(cpu) demanding (fastest speed for this, which is slow).
        // if(m_Time % 1000 == 0)
        // {
            // std::this_thread::sleep_for(std::chrono::milliseconds(m_Time / 1000)); 
        // }
        // //high thread(cpu) demanding (allows more accurate than ms)
        // else
        // {

            auto end = std::chrono::high_resolution_clock::now() + std::chrono::microseconds(m_Time);
            while(std::chrono::high_resolution_clock::now() < end);
            
        // }
    }

    void AnimationTimer::setGeneratorTimer(GeneratorType type)
    {
        switch (type)
        {
        case(Open):
            m_Time = 3500;
            break;
        case(DFSMaze):
            m_Time = 25001;
            break;
        }
    }

    void AnimationTimer::setPathfinderTimer(GeneratorType type)
    {
        if(type == Open)
        {
            m_Time = 5001;
        }
        else // maze
        {
            m_Time = 34001;
        }
    }

} // namespace PFSim