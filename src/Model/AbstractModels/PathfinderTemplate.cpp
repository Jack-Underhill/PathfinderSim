/* Jack Underhill & Jack Sanger
 * CS 133, Spring 2022
 * Final Project - Pathfinding
 */
#include "PathfinderTemplate.h"

namespace PFSim {

    // PathfinderTemplate::PathfinderTemplate(MazeNode* startNode, std::unordered_set<NodePosition>* checkpointsToFind) : AnimationObject() {
    //     this->startNode = startNode;
    //     this->checkpointsToFind = checkpointsToFind;
    // }

    // PathfinderTemplate::~PathfinderTemplate() {}

    // MazeNode* PathfinderTemplate::step() {
    //     if(stackOfNextNodes.size() > 0) {
    //         return nextStep();
    //     }
    //     else {
    //         return currStep();
    //     }
    // }

    // MazeNode* PathfinderTemplate::nextStep() {
    //     MazeNode* nextNode = stackOfNextNodes.top();
    //     stackOfNextNodes.pop();
    //     return nextNode;
    // }

    // void PathfinderTemplate::setNext(MazeNode* curr) {
    //     if(curr->getType() != EndCell) {
    //         curr->setIsNext(true);
    //         curr->setIsVisited(true);
    //         stackOfNextNodes.push(curr);
    //     }
    // }

    // AnimationType PathfinderTemplate::getType() const {
    //     return Pathfind;
    // }

    // void PathfinderTemplate::setEndNode(MazeNode* node){
    //     this->endNode = node;
    // }

    // MazeNode* PathfinderTemplate::getEndNode() const{
    //     return endNode;
    // }

    // void PathfinderTemplate::setCheckpoints(std::unordered_set<NodePosition>* checkpointsToFind) {
    //     this->checkpointsToFind = checkpointsToFind;
    // }

    // std::unordered_set<NodePosition>* PathfinderTemplate::getCheckpoints() const {
    //     return checkpointsToFind;
    // }

} // namespace PFSim