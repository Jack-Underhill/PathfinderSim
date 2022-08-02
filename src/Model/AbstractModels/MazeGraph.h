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
#include "ResetNodes.h"

namespace PFSim {

    class MazeGraph 
    {
    public:
        MazeGraph();
        ~MazeGraph();

        int getMazeLength() const { return m_MazeLength; }
        int getCellSize() const;
        
        // Returns true if the current animation is complete.
        bool isAnimationComplete() const { return m_Animation->isComplete(); }

        MazeNode*& updateAnimation();

        void setGeneratorOpen(int mazeLength);
        // void setGeneratorDFS();

        void setPathfinderBFS();
        // void setPathfinderDFS();

        void findNodeToSetType(CellType type);

    private:
        int m_MazeLength;
        std::unordered_map<int, MazeNode*>* m_MappedNodes;

        MazeNode* m_StartNode;
        MazeNode* m_EndNode;         // Redundant storing the end? its identified when stepped upon... ?

        AnimationObject* m_Animation;
        PathfinderType m_Pathfinder;

        // std::unordered_set<NodePosition>* checkpointsToFind;
        // std::stack<NodePosition>* checkpointStack;

        // std::unordered_set<NodePosition>* currCheckpointsToFind;
        // MazeNode* foundTargetNode;


        // Creates maze node's in a grid formation each with their own position.
        // The grid formation is a square of the gui's given mazeLength of nodes.
        // Each node is mapped to its position since this function is building a disconnected graph
        // (no nodes connected).
        void buildDisconnectedGraph();

        // Deletes all the heap allocated memory (PathNodes, CellTypes, MazeNodes, Map).
        void disposeNodes();


        void setNode(MazeNode* node, CellType type);

        void freeAllocatedAnimation();


        void initGenerator(int mazeLength);

        void initGeneratorOpen(int mazeLength);
        // void initGeneratorDFS();

        void initPathfinder();

        void initPathfinderBFS();
        // void initPathfinderDFS();

        void initResetNodes();
    };

} // namespace PFSim

#endif