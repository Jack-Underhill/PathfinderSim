#ifndef _ResetNodes_h_
#define _ResetNodes_h_

#include <unordered_map>

#include "AnimationObject.h"

namespace PFSim {

    class ResetNodes : public AnimationObject
    {
    public:
        ResetNodes(std::unordered_map<int, MazeNode*>*& mappedNodes, int mazeLength);

        //Returns the animation's title to be displayed on the GUI while it runs.
        std::string getTitle() const { return "Resetting Nodes"; }

        //Returns the type of animation the object is.
        AnimationType getType() const { return AnimationType::Reset; }

        //Takes one step of animation and returns the updates node.
        int step();

    private:
        int m_MazeLength;
        NodePosition m_CurrPos;
        std::unordered_map<int, MazeNode*>* m_MappedNodes;

    };

} // namespace PFSim

#endif