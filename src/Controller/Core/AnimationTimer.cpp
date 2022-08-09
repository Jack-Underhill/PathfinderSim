#include "AnimationTimer.h"

namespace PFSim {

    AnimationTimer::AnimationTimer()
    {
        m_Percentage = 50;
        m_RawTime = 1000;
    }
    
    void AnimationTimer::run()
    {
        auto end = std::chrono::high_resolution_clock::now() + std::chrono::microseconds(m_AppliedTime);
        while(std::chrono::high_resolution_clock::now() < end);
    }
    
    void AnimationTimer::updateAnimation(AnimationType aType, int mazeLength, GeneratorType gType)
    {
        switch (aType)
        {
        case(DrawPath):
            m_RawTime = 18000;
            break;
        case(Reset):
            m_RawTime = 4000;
            break;
        case(Generate):
            setGeneratorTimer(gType);
            break;
        case(Pathfind):
            setPathfinderTimer(gType);
            break;
        }

        scaleToLength(mazeLength);

        if(aType != Reset)
        {
            scaleToPercentage();
        }
        else
        {
            m_AppliedTime = m_RawTime;
        }
    }

    void AnimationTimer::updatePercentage(int value)
    {
        m_Percentage = value;
        scaleToPercentage();
    }

    void AnimationTimer::setGeneratorTimer(GeneratorType type)
    {
        if(type == Open)
        {
            m_RawTime = 3500;
        }
        else // maze
        {
            m_RawTime = 25001;
        }
    }

    void AnimationTimer::setPathfinderTimer(GeneratorType type)
    {
        if(type == Open)
        {
            m_RawTime = 5001;
        }
        else // maze
        {
            m_RawTime = 34001;
        }
    }
    
    void AnimationTimer::scaleToLength(int length)
    {
        m_RawTime *= (std::pow(DEFAULT_MAZE_LENGTH, 1.05) / std::pow(length, 1.1));
    }
    
    void AnimationTimer::scaleToPercentage()
    {
        if(m_Percentage != 50)
        {
            if(m_Percentage < 3) // capping at 3
            {
                m_Percentage = 3;
            }

            int exponent;
            if(m_Percentage > 50)
            {
                exponent = 4;
            }
            else
            {
                exponent = 2;
            }

            m_AppliedTime = m_RawTime / pow((m_Percentage / 50.0), exponent);
        }
        else //no Percentage
        {
            m_AppliedTime = m_RawTime;
        }
    }

} // namespace PFSim