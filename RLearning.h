//
//  RLearning.h
//  AI_GridWorld
//
//  Created by cpsfaculty on 11/23/14.
//  Copyright (c) 2014 ___Ju Shen___. All rights reserved.
//

#ifndef AI_GridWorld_RLearning_h
#define AI_GridWorld_RLearning_h


#endif

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <ctime>        // std::time

#define GO_EAST 0
#define GO_SOUTH 1
#define GO_WEST 2
#define GO_NORTH 3

#define ALPHA 0.5 //the weights for current value and new sample
#define GAMMA 0.9

using namespace std;
using namespace cv;

//A structure defines a state variable
struct RLState{
    Point location; //the 2D location of this state, e.g. (0, 0), (0, 2), (2, 3)...
    //the location defines the ID for the state
    float state_value;
    float q_values[4]; //four possible values in four directions, these four values have the same order as the action defined above: q_values[0] corresponds to action East; q_values[1] corresponds to action South; q_values[2] corresponds to action West; q_values[3] corresponds to action North
    int q_times[4];
};


class RLearning {
    
public:
    RLearning();
    ~RLearning();
    void move(int action); //Gridworld: the action taken for the agent (update the new location, update the Q-value)
    
    int getAction(); //according to current location of the circle, determine the next action to move by using exploration function
    
    
    
    RLState states[3][4]; //the main variables that define the state for each cell in the gridwall. Actually, there are three state varibles that you do not need to update their state values and Q values: states[1][1], states[0][3], states[1][3], which represent the wall, diamond reward, and pitfall.
    
    Point cur_loc; //record the location of the agent
    
    Point ghost_cur_loc; //record the location of the ghost

};
