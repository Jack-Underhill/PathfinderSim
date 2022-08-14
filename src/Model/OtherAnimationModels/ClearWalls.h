#ifndef _ClearWalls_h_
#define _ClearWalls_h_

#include "AnimationObject.h"
#include "ResetNodes.h"

namespace PFSim {

    class ClearWalls : public ResetNodes
    {
    public:
        ClearWalls(std::unordered_map<int, MazeNode*>*& mappedNodes, int mazeLength);

        //Returns the animation's title to be displayed on the GUI while it runs.
        std::string getTitle() const { return "Clearing Walls"; }

        //Returns the type of animation the object is.
        AnimationType getType() const { return AnimationType::WallClear; }

        //Takes one step of animation and returns the updates node.
        int step() override;
    };

} // namespace PFSim

#endif