/* Jack Underhill
 * Pathfinding Simulator
 * 2022
 * The purpose of this class is to act as a virtual class to be inheritated by classes which will
 * perform a step to be animated at a rate of one step per tick.
 * However the class also contains a few default functions.
 */
#ifndef _AnimationObject_H
#define _AnimationObject_H

#include <iostream>

#include "MazeNode.h"
#include "Timer.h"

namespace PFSim {

    // An additional type to differentiate between the types of animations to perform on the graph.
    enum AnimationType 
    {
        Generate,
        Pathfind,
        Reset,
        ObstacleClear,
        DrawPath
    };

    class AnimationObject 
    {
    public:
        AnimationObject() 
        { 
            m_IsComplete = false; 
            m_stepCount = 0;
            m_IsAnimating = true;
        }

        virtual ~AnimationObject() = default;

        //Returns the animation's title to be displayed on the GUI while it runs.
        virtual std::string getTitle() const = 0;

        //Returns the type of animation the object is.
        virtual AnimationType getType() const = 0;

        //Returns steps of visited cells.
        int getStepCount() const { return m_stepCount; }

        //Returns if the animation has been completed.
        bool isComplete() const { return m_IsComplete; }

        //Takes one step of animation and returns the updates node.
        virtual int step() = 0;

        //Takes the given boolean parameter and sets the complete status to that parameter.
        void setIsComplete(bool isComplete) { m_IsComplete = isComplete; }
        
        void setIsAnimating(bool isAnimating) { m_IsAnimating = isAnimating; }

    protected:
        int m_stepCount;
        bool m_IsAnimating;

    private:
        bool m_IsComplete;

    };

} // namespace PFSim

#endif