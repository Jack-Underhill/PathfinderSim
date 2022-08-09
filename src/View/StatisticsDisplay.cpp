#include "StatisticsDisplay.h"

namespace PFSim {
        
    StatisticsDisplay::StatisticsDisplay(sgl::GWindow*& window)
    {
        m_Generator = new DisplayCategory(window, 1);
        m_Pathfinder = new DisplayCategory(window, 2);
        m_PathSolution = new DisplayCategory(window, 3);
    }

    StatisticsDisplay::~StatisticsDisplay()
    {
        delete m_Generator;
        delete m_Pathfinder;
        delete m_PathSolution;
    }

    void StatisticsDisplay::updateTitle(AnimationType type, const std::string& title)
    {
        if(type == Generate)
        {
            setTitle(m_Generator, title);
        }
        else if(type == Pathfind)
        {
            setTitle(m_Pathfinder, title);
        }
        else if(type == DrawPath)
        {
            setTitle(m_PathSolution, title);
        }
    }

    void StatisticsDisplay::updateStepCount(AnimationType type, int count)
    {
        if(type == Generate)
        {
            setStepCount(m_Generator, count);
        }
        else if(type == Pathfind)
        {
            setStepCount(m_Pathfinder, count);
        }
        else if(type == DrawPath)
        {
            setStepCount(m_PathSolution, count);
        }
    }

    void StatisticsDisplay::updateTimeRan(AnimationType type, long long time)
    {
        if(type == Generate)
        {
            setTimeRan(m_Generator, time);
        }
        else if(type == Pathfind)
        {
            setTimeRan(m_Pathfinder, time);
        }
        else if(type == DrawPath)
        {
            setTimeRan(m_PathSolution, time);
        }
    }
    
    void StatisticsDisplay::resetPathingStats() 
    { 
        m_Pathfinder->reset(); 
        m_PathSolution->reset();
    }
    
    void StatisticsDisplay::setTitle(DisplayCategory*& category, const std::string& title)
    {
        if(category == m_Generator)
        {
            m_Generator->reset();
            m_Pathfinder->reset();
            m_PathSolution->reset();
        }

        category->updateTitle(title);
    }

    void StatisticsDisplay::setStepCount(DisplayCategory*& category, int count)
    {
        category->updateStepCount(count);
    }

    void StatisticsDisplay::setTimeRan(DisplayCategory*& category, long long time)
    {
        category->updateTimeRan(time);
    }


    /***************************************DisplayCategory***************************************/
    

    DisplayCategory::DisplayCategory(sgl::GWindow*& window, int level)
    {
        m_Window = window;
        m_CategoryLevel = level;

        m_Title = "";
        m_Count = 0;
        m_Time = 0.0;

        updateFontSize();
    }

    void DisplayCategory::updateTitle(const std::string& title)
    {
        if(title != m_Title) {
            m_Title = title;
            displayTitle();
        }
    }

    void DisplayCategory::updateStepCount(int count)
    {
        if(count != 0)
        {
            m_Count += count;
            displayCount();
        }
    }

    void DisplayCategory::updateTimeRan(long long time)
    {
        if(time != 0)
        {
            m_Time += time;
            displayTime();
        }
    }

    void DisplayCategory::reset()
    {
        m_Title = "";
        m_Count = 0;
        m_Time = 0.0;

        clearTitle();
        clearCount();
        clearTime();
    }

    void DisplayCategory::clearSection(int section)
    {
        m_Window->setColor(BACKGROUND_WINDOW_COLOR);

        double offset = DISPLAY_TOP_BUFFER / 8.0;
        double width = DISPLAY_SIZE / 3.0;
        double height = DISPLAY_TOP_BUFFER / 6.0;

        m_Window->fillRect(getX(section - 1), getY() - offset, width, height);
    }

    void DisplayCategory::displayTitle()
    {
        std::string type;
        if(m_CategoryLevel == 1)
        {
            type = "Generator: ";
        }
        else if(m_CategoryLevel == 2)
        {
            type = "Pathfinder: ";
        }
        else if(m_CategoryLevel == 3)
        {
            type = "";
        }

        std::stringstream ss;
        ss << type << m_Title;

        //repaint
        m_Window->setColor(STRING_COLOR);
        m_Window->drawString(ss.str(), getX(0), getY());
    }

    void DisplayCategory::displayCount()
    {
        clearCount();

        std::stringstream ss;
        ss << "Cells Visited: " << m_Count;
        
        //repaint
        m_Window->setColor(STRING_COLOR);
        m_Window->drawString(ss.str(), getX(1), getY());
    }

    void DisplayCategory::displayTime()
    {
        clearTime();

        std::stringstream ss;
        ss << "Time Ran: (" << m_Time << "ms)";
        
        //repaint
        m_Window->setColor(STRING_COLOR);
        m_Window->drawString(ss.str(), getX(2), getY());
    }

    double DisplayCategory::getX(int section)
    {
        double region = DISPLAY_SIZE * (section) / 3.0;
        
        double offset = 0;
        if(section > 0)
        {
            offset = DISPLAY_SIZE / 10.0;
        }

        return DISPLAY_LEFT_BUFFER + region + offset;
    }

    double DisplayCategory::getY()
    {
        double region = DISPLAY_TOP_BUFFER * (1 + m_CategoryLevel) / 5.0;
        double offset = DISPLAY_TOP_BUFFER / 10.0;

        return region - offset;
    }
    
    void DisplayCategory::updateFontSize() {
        int fontSize = DISPLAY_TOP_BUFFER / 8;

        std::string fontStr = FONT_STYLE + "-" + std::to_string(fontSize);
        m_Window->setFont(fontStr);
    }

} // namespace PFSim