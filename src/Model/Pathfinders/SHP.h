/* 
 * Jack Underhill
 * Summer 2022
 * Maze Generating and Pathfinding Simulator
 * 
 * Pathfinder--Shortest Hamiltonian Path(SHP)
 * 
 * This pathfinder algorithm uses a brute force implementation of BFS/AStar to find paths between 
 * targets. Total solutions to compare is n! where n = number of checkpoints. The shortest path 
 * solution out of the n! calculated possible solutions is the algorithms chosen path solution.
 * 
 * (Talk about the algorithms process)
 * Before searching nodes
 * 
 * (Talk about the files implementations: props, heap, perms, sequence, SHP)
 * This implementation of SHP uses a min heap to store the
 * 
 * (Example for file implementation, remove once completed above)
 * This implementation of AStar uses a min heap to store the open node's F values. This way the 
 * min F value (top of the heap) is always considered the next lowest-cost move. 
 * This is achieved by using a overided heap (AStarHeap) to use AStar props (AStarHeapProp). Each 
 * node opened creates an AStar props to be pushed into the AStar Heap. The props stored are: the 
 * node itself, its G distance and its H distance.
 */

/* 
 * Idea: is I should probably pre-set this pathfinder to run with instant animations due to being a long pf.
 *  
 * Idea: when moving cells, run instantRePathing once releasing the mouse button, not while moving. (overload of computation freezes the program)
 * 
 * Idea: change internal searching method from BFS to AStar to optimize the searching speed?
 *      -I kept both but also made Shp inherit those pf's.
 * 
 * Idea: disable SHP button when less than 2 cp exist (because then this alg is pointless).
 * 
 * Idea: display the solution progress somewhere(like the stat section). "Completed 13/120 solutions" -> "14/120" -> "15/120"
 * 
 * Check: to see of there is a more efficient/simple way of the current 2 classes of SHP_BFS and SHP_AStar
 */

#ifndef _SHP_h_
#define _SHP_h_

#include <string>
#include <queue>
#include <unordered_set>
#include <stack>

#include "BFS.h"
#include "AStar.h"
#include "PathfinderTemplate.h"
#include "MazeNode.h"
#include "MinHeap.h"


namespace PFSim {
namespace Pathfinder {

    /*
     * Stores unique order of checkpoints in an internal stack to be accessed 1 at a time in order 
     * of top to bottom.
     * (This is a stack class simplified to 3 functions: top, pop, empty)
     */
    class Sequence
    {
    public:
        //Takes the given stack to store.
        Sequence(std::stack<int>* stack) { this->stack = stack; }

        //Deletes the stack's allocated memory.
        ~Sequence() { delete stack; }

        //Returns the top target node's position key.
        int top() { return stack->top(); } 

        //Removes the top target node's position key.
        void pop() { stack->pop(); } 

        //Returns true if the sequence of targets is empty.
        bool empty() { return stack->empty(); }

    private:
        std::stack<int>* stack;
    };

    /*
     * Stores every permutation of the constructor given targets. Every permutation(Sequence) is 
     * stored into an internal stack to be accessed 1 at a time in order of top to bottom.
     * (This is a stack class simplified to 3 functions: top, pop, empty)
     */
    class Permutations
    {
    public:
        //Takes the given array and size to store into a stack of permutations.
        Permutations(int* targets, int size);
        
        //Deletes the stack of permutations(Sequences) allocated memory.
        ~Permutations();

        //Returns the top permutation(Sequence).
        Sequence* top() { return m_Sequence->top(); } 

        //Removes the top permutation(Sequence).
        void pop() { m_Sequence->pop(); } 

        //Returns true if the stack of permutations(Sequences) is empty.
        bool empty() { return m_Sequence->empty(); }

    private:
        std::stack<Sequence*>* m_Sequence;
        
        //Recursively stores every possible permutation(Sequence) of target keys into m_Sequence.
        void permutate(std::vector<int> chosen, std::vector<int> remaining, std::stack<Sequence*>* result);
    };

    /*
     * Properties that are stored; for each node opened from AStar's algorithm.
     */
    class SHPHeapProp : public HeapProp
    {
    public:
        PathSolution* path;
        int pathDistance;

        //Takes the given path solution and path distance to store.
        SHPHeapProp(PathSolution* path, int pathDistance)
        {
            this->path = path;
            this->pathDistance = pathDistance;
        }

        //Defualt destructor.
        ~SHPHeapProp() = default;

        //Returns pathDistance of the current path solution.
        int getValue() const { return pathDistance; }
    };

    /*
     * Overided MinHeap to be able to store AStarHeapProp and evaluate elements by 
     * each AStarHeapProp's F value.
     */
    class SHPMinHeap : public MinHeap
    {
    public:
        //Default constructor.
        SHPMinHeap() = default;

        //Defualt destructor.
        ~SHPMinHeap() = default;

        //Returns true if the given prop has been pushed into the heap.
        bool push(HeapProp* props);
    };

    /*
     * SHP Algorithm.
     */
    class SHP_BFS : public BFS
    {
    public:
        //Takes the given graph to construct BFS.
        //Also takes the given target to be stored as the target for this pathfinding algorithm.
        SHP_BFS(MazeGraph*& graph, int target);
        
        //Defualt destructor.
        ~SHP_BFS() = default;

        //Returns the animation's title.
        std::string getTitle() const { return "(SHP) Breadth First Search"; }

        //Returns the pathfinder type.
        PathfinderType getPathfinderType() const { return PathfinderType::SHP_BFS; }

    private:
        int m_Target;

        //Takes and pops the front node from the m_NodeQueue to be examined as the new currNode
        //moved into.
        //
        //If the currNode isn't the m_Target then it will updatePathfinderStep().
        //The pathfinding continues until finding the m_Target.
        //
        //Returns the closed currNode.
        int currStep() override;
    };

    /*
     * SHP Algorithm.
     */
    class SHP_AStar : public AStar
    {
    public:
        //Takes the given graph to construct AStar.
        //Also takes the given target to be stored as the target for this pathfinding algorithm.
        SHP_AStar(MazeGraph*& graph, int target);
        
        //Defualt destructor.
        ~SHP_AStar() = default;

        //Returns the animation's title.
        std::string getTitle() const { return "(SHP) AStar"; }

        //Returns the pathfinder type.
        PathfinderType getPathfinderType() const { return PathfinderType::SHP_AStar; }

    private:
        int m_Target;

        //getCurrentNodeStep() to use.
        //
        //If the currNode isn't the m_Target then it will updatePathfinderStep().
        //The pathfinding continues until finding the m_Target.
        //
        //Returns the closed currNode.
        int currStep() override;
    };
    
} // namespace Pathfinder
} // namespace PFSim

#endif