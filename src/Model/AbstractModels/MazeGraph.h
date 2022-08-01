#ifndef _MazeGraph_h_
#define _MazeGraph_h_

#include <stack>
#include <unordered_map>
#include <unordered_set>

#include "Constants.h"

#include "MazeNode.h"
#include "AnimationObject.h"
#include "Open.h"
#include "BFS.h"

namespace PFSim {

    class MazeGraph 
    {
    public:
        MazeGraph();
        ~MazeGraph() { disposeGraph(); };

        int getMazeLength() const { return m_MazeLength; }
        int getCellSize() const;
        
        // Returns true if the current animation is complete.
        bool isAnimationComplete() const { return m_Animation->isComplete(); }       // special case of resetNodes isnt accounted for?
                                                                                     // maybe do an && m_reseter->isComplete() onto the current solution?

        MazeNode*& updateAnimation();

        void setGeneratorOpen(int mazeLength);

    private:
        int m_MazeLength;
        std::unordered_map<int, MazeNode*>* m_MappedNodes;
        MazeNode* m_StartNode;
        MazeNode* m_EndNode;

        // std::unordered_set<NodePosition>* checkpointsToFind;
        // std::stack<NodePosition>* checkpointStack;

        // std::unordered_set<NodePosition>* currCheckpointsToFind;
        // MazeNode* foundTargetNode;



        // this should contain an animation Object* for (Generators, pathfinders, Finalpath)
        AnimationObject* m_Animation;
        
        // Then contain a reset object* for (resetMaze)

        

        // Creates maze node's in a grid formation each with their own position.
        // The grid formation is a square of the gui's given mazeLength of nodes.
        // Each node is mapped to its position since this function is building a disconnected graph
        // (no nodes connected).
        void buildDisconnectedGraph();

        // Deletes all the heap allocated memory (PathNodes, CellTypes, MazeNodes, Map).
        void disposeGraph();

    };

}

#endif