#include <stdio.h>
#include "RLearning.h"
#include<math.h>
#include<iostream>



int getMax(float *q_value)
{
	int rv=0;
	for(int i=1;i<4;i++)
	{
		if(q_value[rv]<=q_value[i])
			rv=i;
	}
	return rv;
}

bool ifallequal(float *q_value)
{
	for(int i=0;i<4;i++)
	{
		if(q_value[i]!=q_value[i+1])
			return false;
	}
	return true;
}

float calcurate(int i,float *q_value,int *q_times,float current)
{
	float rv=current*pow(0.7,q_times[i]);
	for(int j=0;j<4;j++)
	{
		if(j==i)
			continue;
		rv+=q_value[j]*(1-pow(0.7,q_times[j]))/3;
	//	cout<<"rv="<<rv<<"  ";
	}
	//cout<<endl;
	return rv;
}

int RLearning::getAction()
{
	int action = 0;
	int x=cur_loc.x;
	int y=cur_loc.y;
	float rate[4];
	for(int i=0;i<4;i++)
		rate[i]=0;
	bool bl=ifallequal(states[y][x].q_values);
	if(bl)
	{
		for(int i=0;i<4;i++)
			rate[i]=22.5;
	}else
	{
		int max=getMax(states[y][x].q_values);
		rate[max]=60;
		for(int i=0;i<4;i++)
		{
			if(i==max)
				continue;
			rate[i]=10;
		}
	}
	float r0=calcurate(0,rate,states[y][x].q_times,rate[0]);
	float r1=calcurate(1,rate,states[y][x].q_times,rate[1]);
	float r2=calcurate(2,rate,states[y][x].q_times,rate[2]);
	float r3=calcurate(3,rate,states[y][x].q_times,rate[3]);
	//cout<<r0<<" "<<r1<<" "<<r2<<" "<<r3<<" "<<endl;
	rate[0]=r0;
	rate[1]=r1;
	rate[2]=r2;
	rate[3]=r3;
	float rm=rand()/double(RAND_MAX)*90;
	//cout<<"rm="<<rm;
	if(rm<rate[0])
		action=0;
	else if(rate[0]<=rm&rm<rate[0]+rate[1])
		action=1;
	else if(rate[0]+rate[1]<=rm&rm<rate[2]+rate[1]+rate[0])
		action=2;
	else
		action=3;
	//cout<<"action="<<action<<" "<<endl;
    return action;
}


/*Actually this function is suggested to implemented first. This function is similar to the function you implemented as last assignment for MDP. According to the input action and the current location "cur_loc", determine the next location of the variable. To make it easier, this time we assume if you take action north, you have 100% to land to the north as long as there is another square available in the north. Only if you are already on the North bounary or the square in the north is a wall, which is at the location (1, 1), you will bounce back to current location. After the next location is determined, you need to update the corresponding q value. Finally update the variable "cur_loc" by the new location coordinate. There are three steps involved in this function:*/
void RLearning::move(int action)
{
//    Step 1 - determine the next location from the input action and the current location "cur_loc"
	int x=cur_loc.x;
	int y=cur_loc.y;
	if(action==0)
		x++;
	if(action==1)
		y++;
	if(action==2)
		x--;
	if(action==3)
		y--;
	if(x>3|x<0|y>2|y<0|(x==1&y==1))
	{
		x=cur_loc.x;
		y=cur_loc.y;
	}
	float q=states[cur_loc.y][cur_loc.x].q_values[action];
	q=q*(1-ALPHA)+ALPHA*GAMMA*states[y][x].state_value;
	states[cur_loc.y][cur_loc.x].q_values[action]=q;
	states[cur_loc.y][cur_loc.x].q_times[action]++;
	int i=getMax(states[cur_loc.y][cur_loc.x].q_values);
	states[cur_loc.y][cur_loc.x].state_value=states[cur_loc.y][cur_loc.x].q_values[i];
	cur_loc.x=x;
	cur_loc.y=y;
	
	
    
    //Step 2 - update the q values from the variables states[y][x], where (x, y) represents the current location of the circle
    
    
    //Step 3 - update the new position: changing the value of variable "cur_loc"


    
}

/********************************************************   End of your assignment for this file    *************************************************************/



RLearning::RLearning()
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
            states[y][x].q_times[0] = 0;
			states[y][x].q_times[1] = 0;
			states[y][x].q_times[2] = 0;
			states[y][x].q_times[3] = 0;
            
        }
    }
    
    /* Set the special Q value for (0, 3) and (1, 3) */
    states[0][3].q_values[0] = 1.0; //define the Q values for (0, 3)
    states[0][3].q_values[1] = 1.0;
    states[0][3].q_values[2] = 1.0;
    states[0][3].q_values[3] = 1.0;
    states[1][3].q_values[0] = -1.0; //define the Q value for (1, 3)
    states[1][3].q_values[1] = -1.0;
    states[1][3].q_values[2] = -1.0;
    states[1][3].q_values[3] = -1.0;
    
    
    /* Reset the values for the two special states: diamonds (0, 3), pitfall (1, 3). Actually there are no Q-values for these two states as these two states represents the final state of the game. Similarly, for the wall (1, 1), it does not have any state value or Q values. So make sure not to update these three states during your program*/
    states[0][3].state_value = 1.0;
    states[1][3].state_value = -1.0;
    
    
    cur_loc.x = 0;
    cur_loc.y = 2;
    
    
    
    
    
}


RLearning::~RLearning()
{


}
