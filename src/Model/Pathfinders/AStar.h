/* 
 * Jack Underhill
 * Summer 2022
 * Maze Generating and Pathfinding Simulator
 * 
 * Pathfinder--A* search algorithm(AStar)
 * 
 * This pathfinder algorithm makes the lowest-cost path from the start node to the closest target 
 * node. This means the algorithm prioritizes choosing moves that advance itself in getting closer 
 * to the target; before choosing alternate moves.
 * 
 * The lowest-cost move is calculated by this function: f(n) = g(n) + h(n).
 *      f(n) = F value      (Total path cost)
 *      g(n) = G distance   (Step count from start)
 *      h(n) = H distance   (Heuristic (Manhatten) distance to the target)
 * 
 * Before every (next lowest-cost) move, every neighbor node that hasn't been calculated/open yet 
 * needs to be calculated and added into the open list. (Neighbors are considered one step away)
 * Then from the open list, the next lowest-cost move is selected and removed from the open 
 * list to be marked completed/closed. Repeat.
 * 
 * This implementation of AStar uses a min heap to store the open node's F values. This way the 
 * min F value (top of the heap) is always considered the next lowest-cost move. 
 * This is achieved by using a overided heap (AStarHeap) to use AStar props (AStarHeapProp). Each 
 * node opened creates an AStar props to be pushed into the AStar Heap. The props stored are: the 
 * node itself, its G distance and its H distance.
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

    /*
     * Enum representations for all 4 diagonal directions.
     */
    enum DiagonalDirection
    {
        NW,
        NE, 
        SW, 
        SE
    };

    /*
     * Properties that are stored; for each node opened from AStar's algorithm.
     */
    class AStarHeapProp : public HeapProp
    {
    public:
        MazeNode* node;     // Node's posisiton Key
        int distanceG;      // Step count from start
        int distanceH;      // Heuristic (Manhatten) distance

        //Takes the node itself and its G & H distances.
        AStarHeapProp(MazeNode* node, int distanceG, int distanceH)
        {
            this->node = node;
            this->distanceG = distanceG;
            this->distanceH = distanceH;
        }

        //Defualt destructor.
        ~AStarHeapProp() = default;

        //Returns F value of the current node (G distance + H distance).
        int getValue() const { return distanceG + distanceH; }
    };

    /*
     * Overided MinHeap to be able to store AStarHeapProp and evaluate elements by 
     * each AStarHeapProp's F value.
     */
    class AStarHeap : public MinHeap
    {
    public:
        //Defualt constructor.
        AStarHeap() = default;
        
        //Defualt destructor.
        ~AStarHeap() = default;
        
        //Returns true if the given prop has been pushed into the heap.
        //
        //Returns false if not pushed into the heap; 
        //the given prop contained a node that is already in one of the heap-stored props, 
        //and the given copy doesn't optimize the node's G-distance.
        bool push(HeapProp* prop);

        //Returns a vector list of all the HeapProps that are tied for the minimum value.
        std::vector<HeapProp*> getTiedHeapProps();

    private:
        //Searches through the whole m_PropVector to see if the current given node key already 
        //exists in the list.
        //Returns the index of the found duplicate, else returns -1.
        int find(int key) const;

        //Returns true if the given prop's G-distance is less than the heap-stored prop's 
        //(at the given index) G-distance. The G-distance of the heap-stored prop is replaced with 
        //the given one and then bubbled up into place inside the heap.
        //
        //Else returns false and no heap updating.
        bool handleDuplicate(HeapProp* prop, int index);

        //Adds HeapProp at currIndex to the referenced list if its value is equal to the given value.
        //If so then the left and right children are recursively called in place of the currIndex.
        void getTiedHeapPropsHelper(std::vector<HeapProp*>& list, int currIndex, int value); 
    };
    
    /*
     * Astar Pathfinding Algorithm.
     */
    class AStar : public PathfinderTemplate 
    {
    public:
        //Takes the given graph to store: 
        //--the starting node for this pathfinding algorithm.
        //--the end node's position.
        //--the maze's length.
        //
        //The rest of the member variables are then initialized by their own. 
        AStar(MazeGraph*& graph);

        ~AStar();

        //Returns the animation's title.
        std::string getTitle() const { return "AStar"; }

        //Returns the pathfinder type.
        PathfinderType getPathfinderType() const { return PathfinderType::AStar; }

    protected:
        //Handles/updates tied top nodes (if there is).
        //Takes and closes the top node from the m_MinHeap to be returned.
        MazeNode* getCurrentNodeStep();
        
        //addIfAvailable() for each (N,W,S,E,NE,NW,SE,SW) neighboring nodes into the m_MinHeap.
        //If the m_MinHeap is still empty after, then set the algorithm to be no longer searching.
        void updatePathfinderStep(MazeNode*& currNode);

    private:
        AStarHeap* m_MinHeap;

        NodePosition m_EndPosition;
        int m_MazeLength;
        int m_PrevDistanceG;
        bool m_IsDiagonalMove;

        const int NON_DIAGONAL_COST = 10;
        const int DIAGONAL_COST = 14;

        //getCurrentNodeStep() to pass into PathfinderTemplate::currStepTemplate(),
        //and then return the currNode's position key.
        int currStep();

        //If it is available then: 
        //set the curr node to next (if not already),
        //set the given moved direction to the given curr node (if successfully opened curr node),
        //set the curr node's parent node to be the given prev node (if successfully opened curr node).
        void addIfAvailable(MazeNode*& curr, MazeNode*& prev, DirectionMoved dir);

        //Returns the H-distance cost from position 1 to position 2. 
        //The heuristic equation is simular to manhattan distance but prioritizes zigzaging rather 
        //than moving full x-direction then full y-direction.
        //
        //Each diagonal move cost (1 x-move and 1 y-move) = 14.
        //Each non-diagonal move cost (1 x-move or 1 y-move) = 10.
        int heuristicEquation(int x1, int y1, int x2, int y2) const;

        //Converts the pos and positionKey to (x,y) coordinates to pass into heuristicEquation().
        //Returns the H-distance cost from pos to positionKey.
        int heuristicEquationWithKey(const NodePosition& pos, int positionKey) const;                    // rename variable names

        //If there are checkpoints remaining: H-distance to each checkpoint is calculated and the 
        //smallest one is returned.
        //Else the H-distance to the end node is calculated and returned.
        int getHeuristicDistance(const NodePosition& pos) const;

        //Open/push the given curr node into the heap. The G-distance cost and the H-distance cost 
        //is calculated into the node's properties to be pushed all together.
        //Returns true if the node was opened, else false.
        bool openNode(MazeNode* curr);

        //Close/pop the top node in the heap, and set it to be no longer next and now visited.
        void closeNode(MazeNode* node);

        //Call handleDiagonalMove() for all 4 diagonal moves (NE, NW, SE, SW).
        void addDiagonalAvailableMoves(MazeNode*& curr);

        //If the given diagonal directed move returns true from isValidZigZagConnection(), 
        //then pass the move into addIfAvailable().
        void handleDiagonalMove(MazeNode*& curr, DiagonalDirection dir);

        //Returns true if the given curr node is not a wallcell and is open, else returns false.
        bool isValidZigZagConnection(MazeNode*& curr) const;

        //Tied nodes for the top of the m_MinHeap need to be broken when deciding for which target
        //to move towards; or else steps would be wasted moving towards every target equally away.
        //So a tiebreaker is only called when there are multiple active targets and there are
        //multiple nodes tied for the top of the m_MinHeap. 
        void handleTies();
        
        //With the given listOfTiedNodes, every node from there is copied and removed from the
        //heap, and then added back into the heap with the given indexOfTieBreaker (the node chosen
        //from the tie) to always be at the top of the heap.
        void updateHeapWithTieBreaker(std::vector<HeapProp*>& listOfTiedNodes, int indexOfTieBreaker);

        //Goes through the given list of tied HeapProps and returns the index of the node with the
        //smallest H-distance.
        int getIndexOfLowestH(std::vector<HeapProp*>& list);

        //Replace the given index of the given list with a new allocated HeapProp that is a copy of
        //the old HeapProp (the one being replaced).
        void copyHeapProp(std::vector<HeapProp*>& list, int index);
    };
    
} // namespace Pathfinder
} // namespace PFSim

#endif