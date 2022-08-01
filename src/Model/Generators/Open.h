#ifndef _Open_h_
#define _Open_h_

#include <unordered_map>

#include "GeneratorTemplate.h"

namespace PFSim {
    
namespace Generator {
    
    class Open : public GeneratorTemplate
    {
    public:
        Open(std::unordered_map<int, MazeNode*>*& mappedNodes, int mazeLength);

        //Returns the animation's title to be displayed on the GUI while it runs.
        std::string getTitle() const;

        //Connects the unlinked neighbors of the node at the current position.
        MazeNode*& step();

    private:
        NodePosition m_CurrPos;

        //Connects all the neighbors of the given node to itself.
        void connectUnlinkedNeighbors(MazeNode*& node);
    
    };

} // namespace Generator

} // namespace PFSim


#endif