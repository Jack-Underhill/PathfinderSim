/*
    Astar will use an internal map of each nodes key to a stuct containing:

    Distance traveled(steps) from the start (G).
    Heuristic distance(displacement from current position to the end position) (H).
    Total value => (f = g + h).
    Previous node.

    Somehow have to weighting a priority queue with the lowest F value. (min-heap tree?)

    So a PQ stores all data into a min-heap tree and then it pulls the (current)min value from the heap 1 at a time into a queue. 
    (In other words its a 2 step data type (heap then queue))

    chose node.
    check/open every available move and calculate their f value, add it into the heap
    close/pop the current node.
    choose the smallest f value from the heap to be the next chosen node.
    add the rest of the queues values into a heap
    then restart from the first step with the chosen node
    ends once the end node is found

    Note: if a available move is a node that has already been addressed from another node, recalculate its f value from the current node and if its better then replace its values.

    Note: The ordering of the steps might need to be shuffled for optimization later on.

    Note: Open nodes are next nodes. and closed nodes are curr nodes
*/

/*

A* pseudocode

astar(v1, v2):
    consider every vertex to have a cost of infinity, except v1 which has a cost of 0.
    create a priority queue of vertexes, ordered by (cost+heuristic), storing only v1
    with a priority of H(v1, v2).

while the pqueue is not empty:
    dequeue a vertex v from the pqueue, and mark it as visited.
    for each of the unvisited neighbors n of v, we now know that we can reach
    this neighbor with a total cost of (v's cost + the weight of the edge from v to n).
        if the neighbor is not in the pqueue, or this is cheaper than n's current cost,
        we should enqueue the neighbor n to the pqueue with this new cost
        plus H(n, v2), and with v as its previous vertex.

when we are done, we can reconstruct the path from v2 back to v1
by following the previous pointers.

* (basically, add H(...) to costs of elements in PQ to improve PQ processing order)

*/

/* Jack Underhill
 * CS 133, Spring 2022
 * Final Project - Pathfinding
 */
#ifndef _AStar_h_
#define _AStar_h_

#include <cmath>
#include <string>
#include <vector>
#include <queue>
#include <unordered_set>

#include "PathfinderTemplate.h"
#include "MazeNode.h"
#include "MinHeap.h"

namespace PFSim {
namespace Pathfinder {

    enum DiagonalDirection
    {
        NW,
        NE, 
        SW, 
        SE
    };


    class MinHeapAStar : public MinHeap
    {
    public:
        MinHeapAStar() = default;
        virtual ~MinHeapAStar() = default;

        bool push(HeapProps* props);

    private:
        int find(int key) const;
    };


    class HeapPropsAStar : public HeapProps
    {
    public:
        MazeNode* node;     // Node's posisiton Key
        int distanceG;      // Step count from start
        int distanceH;      // Heuristic (Manhatten) distance

        int getValue() const { return distanceG + distanceH; } // F value

        HeapPropsAStar(MazeNode* node, int distanceG, int distanceH)
        {
            this->node = node;
            this->distanceG = distanceG;
            this->distanceH = distanceH;
        }
        ~HeapPropsAStar() = default;
    };
    
    
    class AStar : public PathfinderTemplate 
    {
    public:
        //Constructor takes the start, end, and a list of checkpoints the pathfinder must hit.
        AStar(MazeGraph*& graph);
        ~AStar();

        //Returns the animation's title to be displayed on the GUI while it runs.
        std::string getTitle() const { return "AStar"; }

        PathfinderType getPathfinderType() const { return PathfinderType::AStar; }

    private:
        MinHeapAStar* m_MinHeap;
        std::unordered_set<int> m_Set;

        NodePosition m_EndPosition;
        int m_MazeLength;
        int m_PrevDistanceG;
        bool m_IsDiagonalMove;

        const int NON_DIAGONAL_COST = 10;
        const int DIAGONAL_COST = 14;
        
        int currStep();
        void addIfAvailable(MazeNode*& curr, MazeNode*& prev, DirectionMoved dir);

        int heuristicEquation(int x1, int y1, int x2, int y2) const;
        int heuristicEquationWithKey(const NodePosition& pos, int positionKey) const;
        int getHeuristicDistance(const NodePosition& pos) const;

        bool openNode(MazeNode* curr);
        void closeNode();

        void addDiagonalAvailableMoves(MazeNode*& curr);
        void handleDiagonalMove(MazeNode*& curr, DiagonalDirection dir);
        bool isValidZigZagConnection(MazeNode*& curr) const;
    };
    
} // namespace Pathfinder
} // namespace PFSim

#endif