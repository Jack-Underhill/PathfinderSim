#ifndef _StatisticsDisplay_h_
#define _StatisticsDisplay_h_

#include <sstream>

#include "gwindow.h"

#include "Constants.h"

#include "AnimationObject.h"
#include "PathSolution.h"

namespace PFSim {

    class DisplayCategory;

    class StatisticsDisplay
    {
    public: 
        StatisticsDisplay(sgl::GWindow*& window);
        ~StatisticsDisplay();

        void updateTitle(AnimationType type, const std::string& title);
        void updateStepCount(AnimationType type, int count);
        void updateTimeRan(AnimationType type, long long time);

        void resetPathingStats();
        void resetPathSolution();

    private: 
        DisplayCategory* m_Generator;
        DisplayCategory* m_Pathfinder;
        DisplayCategory* m_PathSolution;

        void setTitle(DisplayCategory*& category, const std::string& title);
        void setStepCount(DisplayCategory*& category, int count);
        void setTimeRan(DisplayCategory*& category, long long time);
    };
    

    class DisplayCategory
    {
    public: 
        DisplayCategory(sgl::GWindow*& window, int level);
        ~DisplayCategory() = default;

        void updateTitle(const std::string& title);
        void updateStepCount(int count);
        void updateTimeRan(long long time);

        void reset();

    private: 
        sgl::GWindow* m_Window;
        int m_CategoryLevel;
        std::string m_Title;
        int m_Count;
        long long m_Time;
        
        void clearTitle() { clearSection(1); }
        void clearCount() { clearSection(2); }
        void clearTime()  { clearSection(3); }

        void clearSection(int section);

        void displayTitle();
        void displayCount();
        void displayTime();

        double getX(int section);
        double getY();

        void updateFontSize();  // rn only used in construction
    };

} // namespace PFSim

#endif