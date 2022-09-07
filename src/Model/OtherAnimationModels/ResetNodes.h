#ifndef _ResetNodes_h_
#define _ResetNodes_h_

#include <unordered_map>

#include "AnimationObject.h"
#include "MazeGraph.h"

namespace PFSim {

    class ResetNodes : public AnimationObject
    {
    public:
        ResetNodes(MazeGraph*& graph);

        //Returns the animation's title to be displayed on the GUI while it runs.
        std::string getTitle() const { return "Resetting Nodes"; }

        //Returns the type of animation the object is.
        AnimationType getType() const { return AnimationType::Reset; }

        //Takes one step of animation and returns the updates node.
        virtual int step();

        void resetDirection();

    protected:
        int m_MazeLength;
        NodePosition m_CurrPos;
        int m_lastPositionKey;
        std::unordered_map<int, MazeNode*>* m_MappedNodes;

    };

} // namespace PFSim

#endif