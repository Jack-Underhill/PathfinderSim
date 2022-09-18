#ifndef _Prims_h_
#define _Prims_h_

#include <vector>
#include <stack>

#include "GeneratorTemplate.h"
#include "MazeNode.h"

namespace PFSim {
    
namespace Generator {
    
    class Prims : public GeneratorTemplate
    {
    public:
        Prims(MazeGraph*& graph);

        std::string getTitle() const { return "Prim's Maze"; }

        GeneratorType getGeneratorType() const { return GeneratorType::Prims; }

        int step();

    private:
        std::vector<int> currNodeList;
        std::stack<int> nextNodeList;

        int handleNextNode();
        int handleCurrNode();

        void updateNodes(MazeNode*& node);
        DirectionMoved connectWithCompletedNode(MazeNode*& node) const;
        
        void checkIsNotHandled(NodePosition& pos);
        void checkIsCompletedNode(NodePosition& pos, const DirectionMoved& dir, std::vector<DirectionMoved>& neighbors) const;
    };

} // namespace Generator

} // namespace PFSim

#endif