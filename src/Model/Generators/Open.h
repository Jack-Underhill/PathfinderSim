#ifndef _Open_h_
#define _Open_h_

#include <unordered_map>

#include "GeneratorTemplate.h"

namespace PFSim {
    
namespace Generator {
    
    class Open : public GeneratorTemplate
    {
    public:
        Open(MazeGraph*& graph);

        //Returns the animation's title to be displayed on the GUI while it runs.
        std::string getTitle() const { return "Open"; }

        //Connects the unlinked neighbors of the node at the current position.
        int step();

        GeneratorType getGeneratorType() const { return GeneratorType::Open; }

    private:
        NodePosition m_CurrPos;

        //Connects all the neighbors of the given node to itself.
        void connectUnlinkedNeighbors(MazeNode*& node);
    
    };

} // namespace Generator

} // namespace PFSim


#endif