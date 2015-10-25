#include <stdio.h>

#include "MDP.h"

MDP::MDP()
{
    /*Initialize all the state with 0.0 state_value and 0.0 Q_values*/
    for(int y = 0; y < 3; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            states[y][x].location.x = x; //specify the location for this state
            states[y][x].location.y = y;
            
            states[y][x].state_value = 0.0; //define the state value
            states[y][x].q_values[0] = 0.0; //define the Q value
            states[y][x].q_values[1] = 0.0;
            states[y][x].q_values[2] = 0.0;
            states[y][x].q_values[3] = 0.0;
        }
    }
    
    /* Reset the values for the two special states: diamonds (0, 3), pitfall (1, 3). Actually there are no Q-values for these two states as these two states represents the final state of the game. Similarly, for the wall (1, 1), it does not have any state value or Q values. So make sure not to update these three states during your program*/
    states[0][3].state_value = 1.0;
    states[1][3].state_value = -1.0;
    
    
    
    /*Set the current convergence to a big number initially*/
    cur_convergence = 100; //the reason this value is set to a big value is to ensure
    
    
}


MDP::~MDP()
{
    
}





/*There is no return value, actually you need to use the computing result to update the state value and q values of the variables states[9] */
void MDP::valueIteration()
{
    cur_convergence = 0; //everytime before checking the converngence, cur_convergence is used to record the biggest change of all the Q values for all the state. So initiall, it is made to 0. Than replaced by any bigger value
    
    /*Compute the Q values*/
    for(int y = 0; y < 3; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            if(!((y == 0 && x == 3) || (y == 1 && x == 1) || (y == 1 && x == 3)))
            {
                /*Compute the Q values for four actions*/
                computeQValue(states[y][x], ACTION_EAST);
                computeQValue(states[y][x], ACTION_SOUTH);
                computeQValue(states[y][x], ACTION_WEST);
                computeQValue(states[y][x], ACTION_NORTH);
            }
        }
    }
    //printf("current converngce: %.6f\n", cur_convergence);
    
    /*Update the State Values*/
    for(int y = 0; y < 3; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            
            if(!((y == 0 && x == 3) || (y == 1 && x == 1) || (y == 1 && x == 3)))
            {
                //assign a big value iniatially
                states[y][x].state_value = -1000.0;
                
                for(int i = 0; i < 4; i++)
                {
                    if(states[y][x].state_value < states[y][x].q_values[i])
                    {
                        states[y][x].state_value = states[y][x].q_values[i];
                    }
                }
            }
        }
    }
}




/*Compute a Q-value for a given state and its action
 Input: state variable s; action (go to East/North/West/South), the corresponding interger value for each action is defined in the "MDP.h" Macros definition on the top
 Output: you need to update the corresponding q_values of the input state variable s
 */
void MDP::computeQValue(State &s, const int action)
{
    int x_new, y_new; //the new location coordiate
    float potential_values[3]; // the three potential values you may get by taking this action. The first element potential_values[0] represents the successful transition which has the prabability 0.8; the other two represents going to the unexpected directions
    
    const int x = s.location.x; //for easy access, x and y are created to store the location of the state
    const int y = s.location.y;
    
    
    // Go to East
    if(action == ACTION_EAST)
    {
        /*Case 1: compute the value for arrives at East */
        x_new = x + 1;
        y_new = y;
        
        //if the current state is on the right boundary (actually this only happens when the state is at (2, 3)
        if(x == 3)
        {
            x_new = x; //bounce back
        }
        
        //if the current state is right to the wall
        if(y == 1 && x == 0)
        {
            x_new = x;
        }
        potential_values[0] = TRANSITION_SUCCEED * (ACTION_REWARD + GAMMA * states[y_new][x_new].state_value);
        
        
        /*Case 2: it goes to east but arrives in the north */
        x_new = x;
        y_new = y - 1;
        
        //if the current state is on the up boundary
        if(y == 0)
        {
            y_new = y;
        }
        
        //if the current state is at the bottom of the wall
        if(y == 2 && x == 1)
        {
            y_new = y;
        }
        potential_values[1] = TRANSITION_FAIL * .5 * (ACTION_REWARD + GAMMA * states[y_new][x_new].state_value);
        
        
        /*Case 3: if goes to east but arrives in the south */
        x_new = x;
        y_new = y + 1;
        
        //if the current state is on the bottom boundary
        if(y == 2)
        {
            y_new = y;
        }
        
        //if the current state is at the top of the wall
        if(y == 0 && x == 1)
        {
            y_new = y;
        }
        potential_values[2] = TRANSITION_FAIL * .5 * (ACTION_REWARD + GAMMA * states[y_new][x_new].state_value);
        
        
        /*Fianlly update the Q value of current state*/
        float temp_cur = potential_values[0] + potential_values[1] + potential_values[2];
        
        float cur_diff = s.q_values[ACTION_EAST] - temp_cur;
        cur_diff = (cur_diff < 0) ? (-cur_diff) : (cur_diff);
        if(cur_diff > cur_convergence)
            cur_convergence =cur_diff;
        
        s.q_values[ACTION_EAST] = temp_cur;
        
    }
    
    
    // Go to South
    else if(action == ACTION_SOUTH)
    {
        /*Case 1: compute the value for arrives at South */
        x_new = x;
        y_new = y + 1;
        
        //if the current state is on the bottom boundary
        if(y == 2)
        {
            y_new = y; //bounce back
        }
        
        //if the current state is at the top of the wall
        if(y == 0 && x == 1)
        {
            y_new = y;
        }
        potential_values[0] = TRANSITION_SUCCEED * (ACTION_REWARD + GAMMA * states[y_new][x_new].state_value);
        
        
        /*Case 2: it goes to south but arrives in the east */
        x_new = x + 1;
        y_new = y;
        
        //if the current state is on the right boundary
        if(x == 3)
        {
            x_new = x;
        }
        
        //if the current state is on the left of the wall
        if(y == 1 && x == 0)
        {
            x_new = x;
        }
        potential_values[1] = TRANSITION_FAIL * .5 * (ACTION_REWARD + GAMMA * states[y_new][x_new].state_value);
        
        
        /*Case 3: if goes to south but arrives in the west */
        x_new = x - 1;
        y_new = y;
        
        //if the current state is on the left boundary
        if(x == 0)
        {
            x_new = x;
        }
        
        //if the current state is on the right of the wall
        if(y == 1 && x == 2)
        {
            x_new = x;
        }
        potential_values[2] = TRANSITION_FAIL * .5 * (ACTION_REWARD + GAMMA * states[y_new][x_new].state_value);
        
        
        /*Fianlly update the Q value of current state*/
        float temp_cur = potential_values[0] + potential_values[1] + potential_values[2];
        
        float cur_diff = s.q_values[ACTION_SOUTH] - temp_cur;
        cur_diff = (cur_diff < 0) ? (-cur_diff) : (cur_diff);
        if(cur_diff > cur_convergence)
            cur_convergence =cur_diff;
        
        s.q_values[ACTION_SOUTH] = temp_cur;
    }
    
    
    
    // Go to West
    if(action == ACTION_WEST)
    {
        /*Case 1: compute the value for arrives at West */
        x_new = x - 1;
        y_new = y;
        
        //if the current state is on the left boundary
        if(x == 0)
        {
            x_new = x; //bounce back
        }
        
        //if the current state is on the right to the wall
        if(y == 1 && x == 2)
        {
            x_new = x;
        }
        potential_values[0] = TRANSITION_SUCCEED * (ACTION_REWARD + GAMMA * states[y_new][x_new].state_value);
        
        
        /*Case 2: it goes to west but arrives in the north */
        x_new = x;
        y_new = y - 1;
        
        //if the current state is on the up boundary
        if(y == 0)
        {
            y_new = y;
        }
        
        //if the current state is at the bottom of the wall
        if(y == 2 && x == 1)
        {
            y_new = y;
        }
        potential_values[1] = TRANSITION_FAIL * .5 * (ACTION_REWARD + GAMMA * states[y_new][x_new].state_value);
        
        
        /*Case 3: if goes to west but arrives in the south */
        x_new = x;
        y_new = y + 1;
        
        //if the current state is on the bottom boundary
        if(y == 2)
        {
            y_new = y;
        }
        
        //if the current state is at the top of the wall
        if(y == 0 && x == 1)
        {
            y_new = y;
        }
        potential_values[2] = TRANSITION_FAIL * .5 * (ACTION_REWARD + GAMMA * states[y_new][x_new].state_value);
        
        
        /*Fianlly update the Q value of current state*/
        float temp_cur = potential_values[0] + potential_values[1] + potential_values[2];
        
        float cur_diff = s.q_values[ACTION_WEST] - temp_cur;
        cur_diff = (cur_diff < 0) ? (-cur_diff) : (cur_diff);
        if(cur_diff > cur_convergence)
            cur_convergence =cur_diff;
        
        s.q_values[ACTION_WEST] = temp_cur;
        
    }
    
    
    // Go to North
    else if(action == ACTION_NORTH)
    {
        /*Case 1: compute the value for arrives at North */
        x_new = x;
        y_new = y - 1;
        
        //if the current state is on the top boundary
        if(y == 0)
        {
            y_new = y; //bounce back
        }
        
        //if the current state is at the bottom of the wall
        if(y == 2 && x == 1)
        {
            y_new = y;
        }
        potential_values[0] = TRANSITION_SUCCEED * (ACTION_REWARD + GAMMA * states[y_new][x_new].state_value);
        
        
        /*Case 2: it goes to south but arrives in the east */
        x_new = x + 1;
        y_new = y;
        
        //if the current state is on the right boundary
        if(x == 3)
        {
            x_new = x;
        }
        
        //if the current state is on the left of the wall
        if(y == 1 && x == 0)
        {
            x_new = x;
        }
        potential_values[1] = TRANSITION_FAIL * .5 * (ACTION_REWARD + GAMMA * states[y_new][x_new].state_value);
        
        
        /*Case 3: if goes to south but arrives in the west */
        x_new = x - 1;
        y_new = y;
        
        //if the current state is on the left boundary
        if(x == 0)
        {
            x_new = x;
        }
        
        //if the current state is on the right of the wall
        if(y == 1 && x == 2)
        {
            x_new = x;
        }
        potential_values[2] = TRANSITION_FAIL * .5 * (ACTION_REWARD + GAMMA * states[y_new][x_new].state_value);
        
        
        /*Fianlly update the Q value of current state*/
        float temp_cur = potential_values[0] + potential_values[1] + potential_values[2];
        
        float cur_diff = s.q_values[ACTION_NORTH] - temp_cur;
        cur_diff = (cur_diff < 0) ? (-cur_diff) : (cur_diff);
        if(cur_diff > cur_convergence)
            cur_convergence =cur_diff;
        
        s.q_values[ACTION_NORTH] = temp_cur;
        
    }
    
}



/*Reset the current computed state and Q values*/
void MDP::resetData()
{
    /*Initialize all the state with 0.0 state_value and 0.0 Q_values*/
    for(int y = 0; y < 3; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            states[y][x].location.x = x; //specify the location for this state
            states[y][x].location.y = y;
            
            states[y][x].state_value = 0.0; //define the state value
            states[y][x].q_values[0] = 0.0; //define the Q value
            states[y][x].q_values[1] = 0.0;
            states[y][x].q_values[2] = 0.0;
            states[y][x].q_values[3] = 0.0;
        }
    }
    
    /* Reset the values for the two special states: diamonds (0, 3), pitfall (1, 3). Actually there are no Q-values for these two states as these two states represents the final state of the game. Similarly, for the wall (1, 1), it does not have any state value or Q values. So make sure not to update these three states during your program*/
    states[0][3].state_value = 1.0;
    states[1][3].state_value = -1.0;
    
    
    
    /*Set the current convergence to a big number initially*/
    cur_convergence = 100; //the reason this value is set to a big value is to ensure
    
    
}
