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
#include "DFSMaze.h"
#include "BFS.h"
#include "DFS.h"
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
        std::stack<MazeNode*> getNodesToSwap() { return m_NodesToSwap; }

        // PathfinderType getPathfinderType() { return ((PathfinderTemplate*)m_Animation)->getPathfinderType(); }
        GeneratorType getGeneratorType() { return m_Generated; }
        
        bool isAnimationComplete() const { return m_Animation->isComplete(); }
        bool isReadyForSimulation() const { return m_IsReadyForSimulation; }
        bool isMazeGenerated() const { return m_IsMazeGenerated; }
        bool isMouseInteractive() const { return m_IsMousePressed; }

        MazeNode*& updateAnimation();
        void updatePathfinderStart();
        void updateSimulationSetup();
        
        void setGenerator(GeneratorType type);
        void setPathfinder(PathfinderType type);
        void setGraphReset(); 
        void setPathSolution(); 
        void setMazeLength(int length) { m_MazeLength = length; }
        void setIsReadyForSimulation(bool val) { m_IsReadyForSimulation = val; }
        MazeNode*& setEndNode() { return m_MappedNodes->at( setAvailableNodeCellType(EndCell) ); }

        void setMousePressed(int x, int y);
        void setMouseReleased() { m_IsMousePressed = false; }
        void setMouseMoved(int x, int y);

        MazeNode*& addCheckpoint();
        MazeNode*& removeTopCheckpoint();

    private:
        int m_MousePositionKey;
        int m_LastMousePositionKey;
        std::stack<MazeNode*> m_NodesToSwap;

        int m_MazeLength;
        std::unordered_map<int, MazeNode*>* m_MappedNodes;

        MazeNode* m_StartNode;
        MazeNode* m_LastTargetFound;

        AnimationObject* m_Animation;
        PathSolution* m_PathSolution;
        GeneratorType m_Generated;

        std::unordered_set<int>* m_TargetList;
        std::stack<int>* m_CheckpointStack;

        bool m_IsReadyForSimulation;
        bool m_IsMazeGenerated;
        bool m_IsMousePressed;

        // Creates maze node's in a grid formation each with their own position.
        // The grid formation is a square of the gui's given mazeLength of nodes.
        // Each node is mapped to its position since this function is building a disconnected graph
        // (no nodes connected).
        void buildDisconnectedGraph();

        // Deletes all the heap allocated memory (PathNodes, CellTypes, MazeNodes, Map).
        void disposeGraph();

        bool isNewPosition(int x, int y);
        int getKeyConversion(int x, int y);


        int setAvailableNodeCellType(CellType type);

        void setNodeCellType(MazeNode* node, CellType type);

        void freeAllocatedAnimation();


        void generatorSetup();
        void pathfinderSetup();

        void initGeneratorOpen();
        void initGeneratorDFS();

        void initPathfinderBFS();
        void initPathfinderDFS();

        void initResetNodes();

        void initPathSolution();

        void initTargetList();

        void updateSwappedCheckpoints(int newKey, int oldKey);
    };

} // namespace PFSim

#endif