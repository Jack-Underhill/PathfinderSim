/* Jack Underhill & Jack Sanger
 * CS 133, Spring 2022
 * Final Project - Pathfinding
 * The purpose of this class is to act as a parent class for the path finders which share similar
 * methods and are interacted with in the same way. Those functions being to progress a single step
 * of the path finding, to return the current checkpoints no found, and to return the current end.
 */
#ifndef _PathfinderTemplate_H
#define _PathfinderTemplate_H

#include "AnimationObject.h"
#include <unordered_set>
#include <stack>

namespace PFSim {

    // //An additional type to differentiate between the pathfinders when constructing the maze.
    // enum PathfinderType 
    // {
    //     DFS, 
    //     BFS
    // };

    class PathfinderTemplate : public AnimationObject 
    {
    public:
        // //Constructor takes the start of the maze and the checkpoints it needs to find before the end.
        // PathfinderTemplate(MazeNode* startNode, std::unordered_set<NodePosition>* checkpointsToFind);
        
        // //Empty virtual destructor. This class doesn't allocate any memory.
        // virtual ~PathfinderTemplate();

        // //Progresses the animation one tick either by
        // //returning a node to be diplayed as next in the path finding
        // //or by progressing the path finding forward one step and returning the updated node.
        // MazeNode* step();

        // //Sets the given node as a next node for the pathfinder to look at next.
        // void setNext(MazeNode* curr);

        // //Sets a given node as the final node found by the pathfinder.
        // void setEndNode(MazeNode* node);

        // //Returns the end node.
        // MazeNode* getEndNode() const;

        // //Sets the given set of checkpoints
        // //as the set of checkpoints for the pathfinder to find before the end.
        // void setCheckpoints(std::unordered_set<NodePosition>* checkpointsToFind);

        // //Returns the set of checkpoints the pathfinder needs to find before the end.
        // std::unordered_set<NodePosition>* getCheckpoints() const;

        // //Returns the animation's title to be displayed on the GUI while it runs.
        // virtual std::string getTitle() const = 0;

        // //Returns the type of animation the object is. In this case PathFind.
        // AnimationType getType() const;
        
    private:
        // MazeNode* startNode;
        // MazeNode* endNode;
        std::unordered_set<NodePosition>* checkpointsToFind;
        // std::stack<MazeNode*> stackOfNextNodes;
        
        // //Helps the step method by returning the next step for the pathfinder to take
        // //and removes it from the list of possible next steps.
        // MazeNode* nextStep();

        // //Helps the step method by progressing the path finder one step and returning the step it just
        // //took or nullptr if the pathfinder has found a target.
        // virtual MazeNode* currStep() = 0;
    };  

} // namespace PFSim

#endif