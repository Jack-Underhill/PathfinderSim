#ifndef _ClearObstacles_h_
#define _ClearObstacles_h_

#include "AnimationObject.h"
#include "ResetNodes.h"

namespace PFSim {

    class ClearObstacles : public ResetNodes
    {
    public:
        ClearObstacles(MazeGraph*& graph) : ResetNodes(graph)
        {
        }

        //Returns the animation's title to be displayed on the GUI while it runs.
        std::string getTitle() const { return "Clearing Obstacles"; }

        //Returns the type of animation the object is.
        AnimationType getType() const { return AnimationType::ObstacleClear; }

        //Takes one step of animation and returns the updates node.
        int step() override;
    };

} // namespace PFSim

#endif