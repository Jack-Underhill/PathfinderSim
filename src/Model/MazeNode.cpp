#include "MazeNode.h"

string MazeNode::reverseString(string s) {
    char* left = &(s[0]);
    char* right = &(s[s.length() - 1]);
    
    while(left < right) {
        char temp = *left;
        *left = *right;
        *right = temp;

        left++;
        right--;
    }

    return s;
}