//
//  MDP.cpp
//  AI_GridWorld
//
//  Created by cpsfaculty on 11/13/14.
//  Copyright (c) 2014 ___Ju Shen___. All rights reserved.
//
#include <stdio.h>

#include "MDP.h"
using namespace std;

/*************************************************************** Below are the two functions you need to implement ****************************************************/


/*Compute a Q-value for a given state and its action
  Input: state variable s; action (go to East/North/West/South), the corresponding interger value for each action is defined in the "MDP.h" Macros definition on the top
  Output: you need to update the corresponding q_values of the input state variable s
 */
void MDP::computeQValue(State &s, const int action)
{
	cout<<"x="<<s.location.x<<","<<"y="<<s.location.y<<","<<s.state_value<<endl;
	float other1=0,other2=0;
    if (action==ACTION_EAST|action==ACTION_WEST)
	{
		other1=ACTION_SOUTH;
		other2=ACTION_NORTH;
	}else
	{
		other1=ACTION_EAST;
		other2=ACTION_WEST;
	}
	float q1=0,q2=0,q3=0;
	//cout<<"cmp"<<comp(s,action)<<","<<comp(s,other1)<<","<<comp(s,other2)<<endl;
	//q1=TRANSITION_SUCCEED*(ACTION_REWARD+powf(GAMMA,idx)*comp(s,action));
	//q2=TRANSITION_FAIL/2*(ACTION_REWARD+powf(GAMMA,idx)*comp(s,other1));
	//q3=TRANSITION_FAIL/2*(ACTION_REWARD+powf(GAMMA,idx)*comp(s,other2));
	q1=TRANSITION_SUCCEED*(ACTION_REWARD+GAMMA*comp(s,action));
	q2=TRANSITION_FAIL/2*(ACTION_REWARD+GAMMA*comp(s,other1));
	q3=TRANSITION_FAIL/2*(ACTION_REWARD+GAMMA*comp(s,other2));
	float q=q1+q2+q3;
	cout<<q<<","<<q1<<","<<q2<<","<<q3<<endl<<endl;
	const float convergence=abs(s.q_values[action]-q);
	this->cur_convergence=max(convergence,cur_convergence);
	s.q_values[action]=q;
}

float MDP::comp(State &s, int realaction)
{
	int x=s.location.x,y=s.location.y;
	int rx=x,ry=y;
	if(realaction==ACTION_EAST)
	{
		x++;
	}else if(realaction==ACTION_SOUTH)		
	{
		y++;
	}else if(realaction==ACTION_WEST)
	{
		x--;
	}else
	{
		y--;
	}

	/*switch(realaction)
	{
		case 0:x++;
		case 1:y++;
		case 2:x--;
		case 3:y--;
		default:break;
	}*/
	//cout<<"realaction="<<realaction<<endl;
	if(x<0|y<0|y>2|x>3|(x==1&y==1))
	{
		//cout<<"hit the wall"<<endl;
		x=rx;
		y=ry;
	}
	float rv=this->states[y][x].state_value;
	//cout<<"state"<<y<<","<<x<<"="<<rv<<endl;
	return rv;
}

float getmax(float f1,float f2,float f3,float f4)
{
	float maxvalue=max(f1,f2);
	maxvalue=max(maxvalue,f3);
	maxvalue=max(maxvalue,f4);
	return maxvalue;
}

/*There is no return value, actually you need to use the computing result to update the state values of all the states defined as data member "State states[3][4]". Of course, you don't need to update the wall state: states[1][1], the diamond state: states[0][3], and pitfall state: states[1][3] */
void MDP::valueIteration()
{
	this->cur_convergence=0;
    for(int i=0;i<12;i++)
	{		
		int y=i/4;
		int x=i%4;
		if((x==1&y==1)|(x==3&y==0)|(x==3&y==1))
			continue;
		for(int j=0;j<4;j++)
		{
			this->computeQValue(this->states[y][x],j);
		}

	}
	for(int i=0;i<12;i++)
	{		
		int y=i/4;
		int x=i%4;
		if((x==1&y==1)|(x==3&y==0)|(x==3&y==1))
			continue;
			float f1,f2,f3,f4;
			f1=this->states[y][x].q_values[0];
			f2=this->states[y][x].q_values[1];
			f3=this->states[y][x].q_values[2];
			f4=this->states[y][x].q_values[3];

		this->states[y][x].state_value=getmax(f1,f2,f3,f4);
		
	}
}


/**********************************************************************	End of Assignment *********************************************************************/


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
