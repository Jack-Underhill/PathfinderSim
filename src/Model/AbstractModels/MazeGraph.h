#ifndef _MazeGraph_h_
#define _MazeGraph_h_

#include <stack>
#include <unordered_map>
#include <unordered_set>

#include "Constants.h"

#include "MazeNode.h"
#include "AnimationObject.h"
#include "PathfinderTemplate.h"
#include "GeneratorTemplate.h"
#include "Open.h"
#include "BFS.h"
#include "ResetNodes.h"
#include "PathSolution.h"

namespace PFSim {

    class MazeGraph 
    {
    public:
        MazeGraph();
        ~MazeGraph();

        int getMazeLength() const { return m_MazeLength; }
        int getCellSize() const;
        int getCheckpointCount() { return m_CheckpointStack->size(); }
        AnimationType getAnimationType() { return m_Animation->getType(); }
        CellType getTargetFoundType();
        
        bool isAnimationComplete() const { return m_Animation->isComplete(); }
        bool isReadyForSimulation() const { return m_IsReadyForSimulation; }

        MazeNode*& updateAnimation();
        void updatePathfinderStart();
        void updateSimulationSetup();
        
        void setGenerator(GeneratorType type);
        void setPathfinder(PathfinderType type);
        void setGraphReset(); 
        void setPathSolution(); 
        void setMazeLength(int length) { m_MazeLength = length; }

        void setGeneratorOpen(int mazeLength);
        // void setGeneratorDFS();

        void setPathfinderBFS();
        // void setPathfinderDFS();

        MazeNode*& addCheckpoint();
        MazeNode*& removeTopCheckpoint();

    private:
        int m_MazeLength;
        std::unordered_map<int, MazeNode*>* m_MappedNodes;

        MazeNode* m_StartNode;
        MazeNode* m_LastTargetFound;

        AnimationObject* m_Animation;
        AnimationObject* m_PathSolution;
        PathfinderType m_Pathfinder;

        std::unordered_set<int>* m_TargetList;
        std::stack<int>* m_CheckpointStack;

        bool m_IsReadyForSimulation;

        // Creates maze node's in a grid formation each with their own position.
        // The grid formation is a square of the gui's given mazeLength of nodes.
        // Each node is mapped to its position since this function is building a disconnected graph
        // (no nodes connected).
        void buildDisconnectedGraph();

        // Deletes all the heap allocated memory (PathNodes, CellTypes, MazeNodes, Map).
        void disposeGraph();


        void findNodeToSetType(CellType type);

        void setNode(MazeNode* node, CellType type);

        void freeAllocatedAnimation();


        void generatorSetup();
        void pathfinderSetup();

        void initGeneratorOpen();
        // void initGeneratorDFS();

        void initPathfinderBFS();
        // void initPathfinderDFS();

        void initResetNodes();

        void initPathSolution();

        void initTargetList();
    };

} // namespace PFSim

#endif