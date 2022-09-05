/* Jack Underhill
 * CS 133, Spring 2022
 * Final Project - Pathfinding
 * The purpose of this class is to act as a parent class for the path finders which share similar
 * methods and are interacted with in the same way. Those functions being to progress a single step
 * of the path finding, to return the current checkpoints no found, and to return the current end.
 */
#ifndef _PathfinderTemplate_H
#define _PathfinderTemplate_H

#include <unordered_set>
#include <stack>

#include "Constants.h"

#include "AnimationObject.h"
#include "MazeGraph.h"

namespace PFSim {

    class PathfinderTemplate : public AnimationObject 
    {
    public:
        //Constructor takes the start of the maze and the checkpoints it needs to find before the end.
        PathfinderTemplate(MazeGraph*& graph);
        
        //Empty virtual destructor. This class doesn't allocate any memory.
        virtual ~PathfinderTemplate();

        //Returns the node found (a checkpoint or the end).
        MazeNode* getTargetNodeFound() const { return m_TargetNodeFound; }

        //Returns the animation's title to be displayed on the GUI while it runs.
        virtual std::string getTitle() const = 0;

        //Returns the type of animation the object is. In this case PathFind.
        AnimationType getType() const;

        virtual PathfinderType getPathfinderType() const = 0;

        //Progresses the animation one tick either by
        //returning a node to be diplayed as next in the path finding
        //or by progressing the path finding forward one step and returning the updated node.
        int step();

        //Sets the given node as a next node for the pathfinder to look at next.
        void setNext(MazeNode* curr);
        
        void addAvailableMoves(MazeNode*& curr);

        virtual void addIfAvailable(MazeNode*& curr, MazeNode*& prev, DirectionMoved dir) = 0;

        bool isAvailableMove(MazeNode*& curr) const;

        bool isStillSearching() const { return m_IsStillSearching; }

        // Returns true if contained and removed.
        // Returns false if not contained.
        bool removeTargetIfContained(MazeNode*& curr);

        void setTargetList(int size, int* targets);
        int* getTargetList() { return m_TargetList; }
        int getTargetListSize() { return m_TargetListSize; }

    protected:
        int m_TargetList[CHECKPOINT_LIMIT] = {0};
        int m_TargetListSize;
        MazeNode* m_TargetNodeFound;
        bool m_IsStillSearching;

        MazeNode* getStartingPlace();
        
    private:
        std::stack<MazeNode*> stackOfNextNodes;
        MazeGraph* m_Graph;
        
        //Helps the step method by returning the next step for the pathfinder to take
        //and removes it from the list of possible next steps.
        int nextStep();

        //Helps the step method by progressing the path finder one step and returning the step it just
        //took or nullptr if the pathfinder has found a target.
        virtual int currStep() = 0;
    };  

} // namespace PFSim

#endif