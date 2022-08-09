#ifndef _Prims_h_
#define _Prims_h_

#include <unordered_map>

#include "AnimationObject.h"
#include "GeneratorTemplate.h"

namespace PFSim {
    
namespace Generator {
    
    class Prims : public GeneratorTemplate
    {
    public:
        Prims(MazeNode*& startNode, std::unordered_map<int, MazeNode*>*& mappedNodes, int mazeLength);
        // ~Prims();

        std::string getTitle() const { return "Prim's"; }

        GeneratorType getGeneratorType() const { return GeneratorType::Prims; }

        int step();

    private:
        
    };

} // namespace Generator

} // namespace PFSim

#endif