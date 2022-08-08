#ifndef _Timer_h_
#define _Timer_h_

#include <iostream>
#include <chrono>

namespace PFSim {

    class Timer
    {
    public:
        // Prints to the console how long, in milliseconds, that this object is in scope.
        Timer() { m_Start = std::chrono::high_resolution_clock::now(); }

        long long getElapsedTime() const
        {
            auto endTime = std::chrono::high_resolution_clock::now();

            auto start = std::chrono::time_point_cast<std::chrono::nanoseconds>(m_Start).time_since_epoch();
            auto stop = std::chrono::time_point_cast<std::chrono::nanoseconds>(endTime).time_since_epoch();

            auto duration = (stop - start).count();

            return (duration * 0.001 * 0.001);
        }

        // Console print.
        void printElapsedTime() { std::cout << "Time Ran (" << getElapsedTime() << "ms)\n"; }
        
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
    };

} // namespace PFSim

#endif