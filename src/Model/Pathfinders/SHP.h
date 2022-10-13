/* Jack Underhill & Jack Sanger
 * CS 133, Spring 2022
 * Final Project - Pathfinding
 * 
 * Shortest Hamiltonian Path (SHP)
 * Brute force solution: uses BFS to find paths between targets. Solution searches n! of the number of targets.
 * 
 * Idea is I should probably pre-set this pathfinder to run with instant animations due to being a long pf.
 * Idea when moving cells, run instantRePathing once releasing the mouse button, not while moving. (overload of computation freezes the program)
 * 
 * Idea: change internal searching method from BFS to AStar to optimize the searching speed?
 */

#ifndef _SHP_h_
#define _SHP_h_

#include <string>
#include <queue>
#include <unordered_set>
#include <stack>

#include "PathfinderTemplate.h"
#include "MazeNode.h"
#include "MinHeap.h"


namespace PFSim {
namespace Pathfinder {

    // This is really just a simple alternative stack class.
    class Sequence 
    {
    public:
        Sequence(std::stack<int>* stack)
        {
            this->stack = stack;
        }
        ~Sequence()
        {
            delete stack;
        }

        int top() { return stack->top(); } 
        void pop() { stack->pop(); } 
        bool empty() { return stack->empty(); }

    private:
        std::stack<int>* stack;
    };


    class Permutations
    {
    public:
        Permutations(int* targets, int size);
        ~Permutations();

        Sequence* top() { return Perm->top(); } 
        void pop() { Perm->pop(); } 
        bool empty() { return Perm->empty(); }

    private:
        std::stack<Sequence*>* Perm;
        
        void permutate(std::vector<int> chosen, std::vector<int> remaining, std::stack<Sequence*>* result);
    };


    class HeapPropsSHP : public HeapProps
    {
    public:
        PathSolution* path;
        int pathDistance;

        int getValue() const { return pathDistance; }

        HeapPropsSHP(PathSolution* path, int pathDistance)
        {
            this->path = path;
            this->pathDistance = pathDistance;
        }
        ~HeapPropsSHP() = default;
    };

    class MinHeapSHP : public MinHeap
    {
    public:
        MinHeapSHP() = default;
        virtual ~MinHeapSHP() = default;

        bool push(HeapProps* props);
    };


    class SHP : public PathfinderTemplate 
    {
    public:
        //Constructor takes the start, end, and a list of checkpoints the pathfinder must hit.
        SHP(MazeGraph*& graph, int target);

        //Returns the animation's title to be displayed on the GUI while it runs.
        std::string getTitle() const { return "Shortest Hamiltonian Path"; }

        PathfinderType getPathfinderType() const { return PathfinderType::SHP; }

    private:
        std::queue<MazeNode*> m_NodeQueue;
        int m_Target;

        int currStep();

        void addIfAvailable(MazeNode*& curr, MazeNode*& prev, DirectionMoved dir);
    };
    
} // namespace Pathfinder
} // namespace PFSim

#endif