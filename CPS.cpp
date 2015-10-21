#include <stdio.h>

#include "CSP.h"

/************************************** Below are the three core functions***************************************/

class arcdomain
{
public:
	int x,y;
	vector<int> *domain;
	arcdomain(){}
	arcdomain(int i,int j,vector<int> *v)
	{
		x=i;
		y=j;
		domain=v;
	}
	
};
/*Check whether current state satisfy the constraints*/
bool CSP::goalCheck(const State state)
{
	for(int i = 0; i < 81; i++)
        {
            int y = i / 9;
            int x = i % 9;
			if(state.values[y][x]==0)
				return false;

		}
	for(int i = 0; i < 81; i++)
        {
            int y = i / 9;
            int x = i % 9;
			//check slash
			int yy=y%3;
			int xx=x%3;
			if(xx==0&&yy==0)
				if(state.values[y+1][x+1]==state.values[y][x]|state.values[y+2][x+2]==state.values[y][x])
					return false;
			
			if(xx==2&&yy==0)
				if(state.values[y+1][x-1]==state.values[y][x]|state.values[y+2][x-2]==state.values[y][x])
					return false;
			if(xx==1&&yy==1)
				if(state.values[y-1][x+1]==state.values[y][x]|state.values[y+1][x-1]==state.values[y][x])
					return false;
			/*
			if(xx==2&&yy==2)
				if(state.values[y-1][x-1]==state.values[y][x]|state.values[y-2][x-2]==state.values[y][x])
					return false;
			if(xx==0&&yy==2)
				if(state.values[y-1][x+1]==state.values[y][x]|state.values[y-2][x+2]==state.values[y][x])
					return false;
			*/
			//check Y axis
			for(int j=0;j<9;j++)
			{
				if(state.values[i][x]==state.values[y][x])
					if(i!=y)
						return false;
			}
			//check x axis
			for(int j=0;j<9;j++)
			{
				if(state.values[y][i]==state.values[y][x])
					if(i!=x)
						return false;
			}

		}
	
    
    return true;
}

bool domaincheck(Variable &v,int dest)
{
	if(v.assignement!=0)
		return false;
	vector<int>::iterator itr = v.domain.begin();
	while (itr != v.domain.end())
		{
			if (*itr == dest)
			{
				v.domain.erase(itr);
				return true;
			}
		++itr;
		}
	return false;
}

/*Update Domain for the forward checking*/
void CSP::updateDomain(const State state)
{
	for(int i = 0; i < 81; i++)
	{
		int y = i / 9;
        int x = i % 9;
		if(state.values[y][x]==0)
		{
			this->variables[y][x].domain.clear();
			for(int i=1;i<=9;i++)
			{
				this->variables[y][x].domain.push_back(i);
			}
		}	
	}

  for(int i = 0; i < 81; i++)
        {
			
            int y = i / 9;
            int x = i % 9;
			int value=this->variables[y][x].assignement;
			if(value!=0)
			{
				int yy=y%3;
				int xx=x%3;
				if(xx==0&&yy==0)
				{
										
					domaincheck(this->variables[y+1][x+1],value);				
					domaincheck(this->variables[y+2][x+2],value);
						
				
				}
				if(xx==1&&yy==1)
				{
					domaincheck(this->variables[y+1][x+1],value);
					domaincheck(this->variables[y-1][x-1],value);
					domaincheck(this->variables[y-1][x+1],value);
					domaincheck(this->variables[y+1][x-1],value);
				}
					
				if(xx==2&&yy==2)
				{
					domaincheck(this->variables[y-1][x-1],value);
					domaincheck(this->variables[y-2][x-2],value);
				}
					
				if(xx==0&&yy==2)
				{
					domaincheck(this->variables[y-1][x+1],value);
					domaincheck(this->variables[y-2][x+2],value);
				}
				
				if(xx==2&&yy==0)
				{
					domaincheck(this->variables[y+1][x-1],value);
					domaincheck(this->variables[y+2][x-2],value);
				}

				for(int j=0;j<9;j++)
				{
					domaincheck(this->variables[j][x],value);
				}
			//check x axis
				for(int j=0;j<9;j++)
				{
					domaincheck(this->variables[y][j],value);
				}
				
			}
			
			
		}
}

bool arccheck(vector<int> h,Variable &t)
{
	if(t.assignement!=0)
		return false;
	bool sign;
	bool rvalue=false;
	vector<int>::iterator titr = t.domain.begin();
	while (titr != t.domain.end())
		{
			sign=true;
			vector<int>::iterator hitr = h.begin();
			if(hitr==h.end())
				return false;
			while (hitr != h.end())
			{
				if (*hitr!= *titr)
				{
					sign=false;
					break;
					
				}
				hitr++;
			}
			if(sign)
			{
				rvalue=true;
				titr=t.domain.erase(titr);
				if(titr==t.domain.end())
					return true;
			}
			titr++;
		}
	return rvalue;
}

/*Arc consistency use*/
void CSP::arcConsistency(const State state)
{
	for(int i = 0; i < 81; i++)
	{
		int y = i / 9;
        int x = i % 9;
		if(state.values[y][x]==0)
		{
			this->variables[y][x].domain.clear();
			for(int i=1;i<=9;i++)
			{
				this->variables[y][x].domain.push_back(i);
			}
		}	
	}
	int count=0;
	for(int i = 0; i < 81; i++)
        {
			count++;
			
            int y = i / 9;
            int x = i % 9;
			
			if(this->variables[y][x].assignement!=0)
			{
				queue<arcdomain> q;
				int fyy=y%3;
				int fxx=x%3;
				if(fxx==0&fyy==0)
				{
						if(domaincheck(variables[y+1][x+1],this->variables[y][x].assignement))
						{
							arcdomain newdomain(x+1,y+1,&this->variables[y+1][x+1].domain);
							q.push(newdomain);
						}
						if(domaincheck(variables[y+2][x+2],this->variables[y][x].assignement))
						{
							arcdomain newdomain(x+2,y+2,&this->variables[y+2][x+2].domain);
							q.push(newdomain);
						}
				}
				if(fxx==1&fyy==1)
				{
						if(domaincheck(variables[y+1][x+1],this->variables[y][x].assignement))
						{
							arcdomain newdomain(x+1,y+1,&this->variables[y+1][x+1].domain);
							q.push(newdomain);
						}
						if(domaincheck(variables[y-1][x-1],this->variables[y][x].assignement))
						{
							arcdomain newdomain(x-1,y-1,&this->variables[y+1][x+1].domain);
							q.push(newdomain);
						}
						if(domaincheck(variables[y-1][x+1],this->variables[y][x].assignement))
						{
							arcdomain newdomain(x+1,y-1,&this->variables[y-1][x+1].domain);
							q.push(newdomain);
						}
						if(domaincheck(variables[y+1][x-1],this->variables[y][x].assignement))
						{
							arcdomain newdomain(x-1,y+1,&this->variables[y+1][x-1].domain);
							q.push(newdomain);
						}
				}
				if(fxx==2&fyy==2)
				{
						if(domaincheck(variables[y-1][x-1],this->variables[y][x].assignement))
						{
							arcdomain newdomain(x-1,y-1,&this->variables[y-1][x-1].domain);
							q.push(newdomain);
						}
						if(domaincheck(variables[y-2][x-2],this->variables[y][x].assignement))
						{
							arcdomain newdomain(x-2,y-2,&this->variables[y-2][x-2].domain);
							q.push(newdomain);
						}
				}
				if(fxx==0&fyy==2)
				{
						if(domaincheck(variables[y-1][x+1],this->variables[y][x].assignement))
						{
							arcdomain newdomain(x+1,y-1,&this->variables[y-1][x+1].domain);
							q.push(newdomain);
						}
						if(domaincheck(variables[y-2][x+2],this->variables[y][x].assignement))
						{
							arcdomain newdomain(x+2,y-2,&this->variables[y-2][x+2].domain);
							q.push(newdomain);
						}
				}
				if(fxx==2&fyy==0)
				{
						if(domaincheck(variables[y+1][x-1],this->variables[y][x].assignement))
						{
							arcdomain newdomain(x-1,y+1,&this->variables[y+1][x-1].domain);
							q.push(newdomain);
						}
						if(domaincheck(variables[y+2][x-2],this->variables[y][x].assignement))
						{
							arcdomain newdomain(x-2,y+2,&this->variables[y+2][x-2].domain);
							q.push(newdomain);
						}
				}
				for(int j=0;j<9;j++)
				{
					if(domaincheck(variables[y][j],this->variables[y][x].assignement))
					{
						arcdomain newdomain(j,y,&this->variables[y][j].domain);
						q.push(newdomain);
					}
				}
				for(int j=0;j<9;j++)
				{
					if(domaincheck(variables[j][x],this->variables[y][x].assignement))
					{
						arcdomain newdomain(x,j,&this->variables[j][x].domain);
						q.push(newdomain);
					}
				}	
				while(!q.empty())
				{
					arcdomain domain=q.front();
					q.pop();
					x=domain.x;
					y=domain.y;
					int yy=domain.y%3;
					int xx=domain.x%3;
					if(xx==0&&yy==0)
					{
							if(arccheck(*domain.domain,this->variables[y+1][x+1]))
							{
						
								arcdomain newdomain(x+1,y+1,&this->variables[y+1][x+1].domain);
								q.push(newdomain);
							}
							if(arccheck(*domain.domain,this->variables[y+2][x+2]))
							{
						
								arcdomain newdomain(x+2,y+2,&this->variables[y+2][x+2].domain);
								q.push(newdomain);
							}
					}
					if(xx==1&&yy==1)
					{
							if(arccheck(*domain.domain,this->variables[y+1][x+1]))
							{
						
								arcdomain newdomain(x+1,y+1,&this->variables[y+1][x+1].domain);
								q.push(newdomain);
							}
							if(arccheck(*domain.domain,this->variables[y-1][x-1]))
							{
						
								arcdomain newdomain(x-1,y-1,&this->variables[y-1][x-1].domain);
								q.push(newdomain);
							}
							if(arccheck(*domain.domain,this->variables[y-1][x+1]))
							{
						
								arcdomain newdomain(x+1,y-1,&this->variables[y-1][x+1].domain);
								q.push(newdomain);
							}
							if(arccheck(*domain.domain,this->variables[y+1][x-1]))
							{
						
								arcdomain newdomain(x-1,y+1,&this->variables[y+1][x-1].domain);
								q.push(newdomain);
							}
					}
					if(xx==2&&yy==2)
					{
							if(arccheck(*domain.domain,this->variables[y-1][x-1]))
							{
						
								arcdomain newdomain(x-1,y-1,&this->variables[y-1][x-1].domain);
								q.push(newdomain);
							}
							if(arccheck(*domain.domain,this->variables[y-2][x-2]))
							{
						
								arcdomain newdomain(x-2,y-2,&this->variables[y-2][x-2].domain);
								q.push(newdomain);
							}
					}
					if(xx==2&&yy==0)
					{
							if(arccheck(*domain.domain,this->variables[y+1][x-1]))
							{
						
								arcdomain newdomain(x-1,y+1,&this->variables[y+1][x-1].domain);
								q.push(newdomain);
							}
							if(arccheck(*domain.domain,this->variables[y+2][x-2]))
							{
						
								arcdomain newdomain(x-2,y+2,&this->variables[y+2][x-2].domain);
								q.push(newdomain);
							}
					}
					if(xx==0&&yy==2)
					{
							if(arccheck(*domain.domain,this->variables[y-1][x+1]))
							{
								arcdomain newdomain(x+1,y-1,&this->variables[y-1][x+1].domain);
								q.push(newdomain);
							}
							if(arccheck(*domain.domain,this->variables[y-2][x+2]))
							{
								arcdomain newdomain(x+2,y-2,&this->variables[y-2][x+2].domain);
								q.push(newdomain);
							}
					}
					
				for(int j=0;j<9;j++)
				{
					if(j==x)
						continue;
					if(arccheck(*domain.domain,this->variables[y][j]))
					{
						arcdomain newdomain(j,y,&this->variables[y][j].domain);
						q.push(newdomain);
					}
				}
				for(int j=0;j<9;j++)
				{
					if(j==y)
						continue;
					if(arccheck(*domain.domain,this->variables[j][x]))
					{
						arcdomain newdomain(x,j,&this->variables[j][x].domain);
						q.push(newdomain);
					}
				}
				}
			}
		}
}


/************************************************	End of Assignment ***********************************************/




CSP::CSP()
{
    /*Initially assign the domain, assignment for each variable and initialize the current state*/
    for(int y = 0; y < 9; y++)
    {
        for(int x = 0; x < 9; x++)
        {
            variables[y][x].assignement = 0; //Initialize the assignment
            
            /*Initialize the domain*/
            for(int i = 1; i <= 9; i++)
            {
                variables[y][x].domain.push_back(i);
            }
            
            cur_state.values[y][x] = 0; //Initizlize the current state
            
        }
    }
    
    alg_opt = 1; //initially set it as back track
    
    srand(time(NULL));
    random = 0;
}


CSP::~CSP()
{
    
}




void CSP::setData(int *data)
{
    for(int y = 0; y < 9; y++)
    {
        for(int x = 0; x < 9; x++)
        {
            int idx = y * 9 + x;
            variables[y][x].assignement = data[idx]; //Initialize the assignment
            cur_state.values[y][x] = data[idx]; //Initizlize the current state
            
        }
    }
}

void CSP::clearData()
{
    /*Initially assign the domain, assignment for each variable and initialize the current state*/
    for(int y = 0; y < 9; y++)
    {
        for(int x = 0; x < 9; x++)
        {
            variables[y][x].assignement = 0; //Initialize the assignment
            
            /*Initialize the domain*/
            variables[y][x].domain.clear();
            for(int i = 1; i <= 9; i++)
            {
                variables[y][x].domain.push_back(i);
            }
            
            cur_state.values[y][x] = 0; //Initizlize the current state
            
        }
    }
    
    /*Check whether a random domain is use*/
    if(random == 1)
        reshuffleDomain();
    
    repeating_list.clear();
    while(!assigned_variables.empty())
    {
        assigned_variables.pop();
        repeating_list.clear();
    }
    
}


void CSP::reshuffleDomain()
{
    for(int i = 0; i < 81; i++)
    {
        int y = i / 9;
        int x = i % 9;
        
        std::random_shuffle( variables[y][x].domain.begin(), variables[y][x].domain.end() );
    }
}

void CSP::sortDomain()
{
    for(int i = 0; i < 81; i++)
    {
        int y = i / 9;
        int x = i % 9;
        
        std::sort( variables[y][x].domain.begin(), variables[y][x].domain.end() );
    }
}

/*Cancel last assignment*/
int CSP::goBack(int *chosen_cell)
{
    if(assigned_variables.size() > 0)
    {
        int cur_id = assigned_variables.top(); /*Remove last options*/
        assigned_variables.pop(); //pop out last option
        int y = cur_id / 9;
        int x = cur_id % 9;
        
        variables[y][x].assignement = 0; //assign the cell to zero
        cur_state.values[y][x] = 0; //update the assignment
        *chosen_cell = cur_id;
        
       // printf("(%d, %d)\n", y, x);
        if(alg_opt == 2)
        {
            updateDomain(cur_state);
        }
        else if (alg_opt == 3)
        {
            arcConsistency(cur_state);
        }
        
    }
    
    return goalCheck(cur_state);
    
}


bool CSP::arcCheckingOrder(int *chosen_cell)
{
    arcConsistency(cur_state);
    
    
    
    /*First go through all the variables and do backtrack if there is no empty domain */
    for(int i = 0; i < 81; i++)
    {
        int y = i / 9;
        int x = i % 9;
        
        
        if(cur_state.values[y][x] == 0 && variables[y][x].domain.size() == 0)
        {
            int available_assignemnt = 0; //an indicatior whether there are possible possible varaibles to be re-assigned
            while (available_assignemnt == 0) {
                int cur_id = assigned_variables.top();
                int y = cur_id / 9;
                int x = cur_id % 9;
                variables[y][x].assignement = 0;
                cur_state.values[y][x] = 0;
                arcConsistency(cur_state);
                
                
                for(int i = 0; i < variables[y][x].domain.size(); i++)
                {
                    State temp_state;
                    temp_state = cur_state;
                    temp_state.values[y][x] = variables[y][x].domain[i];
                    if (std::find(repeating_list.begin(), repeating_list.end(), temp_state)==repeating_list.end()) //if not in the repeating list
                    {
                        cur_state = temp_state;
                        variables[y][x].assignement = variables[y][x].domain[i];
                        repeating_list.push_back(temp_state);
                        available_assignemnt = 1;
                        *chosen_cell = cur_id;
                        arcConsistency(cur_state);
                        return false; //get out of the current varaible assignment
                    }
                }
                
                if(available_assignemnt == 0) //if all the domain values have been tried for current variable
                {
                    variables[y][x].assignement = 0;
                    cur_state.values[y][x] = 0;
                    assigned_variables.pop();
                    
                }
            }
            
        }
        
    }
    
    /*If there is no variable that has empty domain, then assign variable here*/
    /*First go through all the variables and do backtrack if there is no empty domain */
    int count = 0;
    while (count < 81)
    {
        /*Find the index of minimum number of domain*/
        int min_idx = 0;
        int min_num = 10; //because the maximum number of domain is 10
        for(int i = 0; i < 81; i++)
        {
            int y = i / 9;
            int x = i % 9;
            if(cur_state.values[y][x] == 0 && variables[y][x].domain.size() > 0)
            {
                if (variables[y][x].domain.size() < min_num) {
                    min_idx = i;
                    min_num = variables[y][x].domain.size();
                }
            }
        }
        
        int y = min_idx / 9;
        int x = min_idx % 9;
        
        /*If there is any varable has not been assigned yet, assign it and return it*/
        if(cur_state.values[y][x] == 0 && variables[y][x].domain.size() > 0)
        {
            /*Find the smalles number in domain to assign it. Here no update domain for bracktrack*/
            int id_min = 0;
            cur_state.values[y][x] = variables[y][x].domain[id_min];
            variables[y][x].assignement = variables[y][x].domain[id_min];
            assigned_variables.push(min_idx); //push the variable into stack, which will be used for backtrack (or DFS)
            repeating_list.push_back(cur_state); //make this state into the repeat_list
            *chosen_cell = 9 * y + x;
            
            arcConsistency(cur_state); //Every time modify the assignment update the domain
            
            return false;
        }
        
        count++;
        
    }
    
    if(goalCheck(cur_state))
    {
        printf("find the goal\n");
        return true;
    }
	else
	{
		int available_assignemnt = 0; //an indicatior whether there are possible varaibles to be re-assigned
		while (available_assignemnt == 0) {
			int cur_id = assigned_variables.top();
			int y = cur_id / 9;
			int x = cur_id % 9;
			variables[y][x].assignement = 0;
			cur_state.values[y][x] = 0;
			arcConsistency(cur_state);
			for(int i = 0; i < variables[y][x].domain.size(); i++)
			{
				State temp_state;
				temp_state = cur_state;
				temp_state.values[y][x] = variables[y][x].domain[i];
				if (std::find(repeating_list.begin(), repeating_list.end(), temp_state)==repeating_list.end()) //if not in the repeating list
				{
					cur_state = temp_state;
					variables[y][x].assignement = variables[y][x].domain[i];
					repeating_list.push_back(cur_state);
					available_assignemnt = 1;
					*chosen_cell = cur_id;
					break; //get out of the current varaible assignment
				}
			}

			if(available_assignemnt == 0) //if all the domain values have been tried for current variable
			{

				assigned_variables.pop();

			}
		}

		return false;
	}
    return false;
    
}



/*arcChecking without ordering*/
bool CSP::arcChecking(int *chosen_cell)
{
    arcConsistency(cur_state);
    
    
    
    /*First go through all the variables and do backtrack if there is no empty domain */
    for(int i = 0; i < 81; i++)
    {
        int y = i / 9;
        int x = i % 9;
        
        if(cur_state.values[y][x] == 0 && variables[y][x].domain.size() == 0)
        {
            int available_assignemnt = 0; //an indicatior whether there are possible possible varaibles to be re-assigned
            while (available_assignemnt == 0) {
                int cur_id = assigned_variables.top();
                int y = cur_id / 9;
                int x = cur_id % 9;
                variables[y][x].assignement = 0;
                cur_state.values[y][x] = 0;
                arcConsistency(cur_state);
                
                
                for(int i = 0; i < variables[y][x].domain.size(); i++)
                {
                    State temp_state;
                    temp_state = cur_state;
                    temp_state.values[y][x] = variables[y][x].domain[i];
                    if (std::find(repeating_list.begin(), repeating_list.end(), temp_state)==repeating_list.end()) //if not in the repeating list
                    {
                        cur_state = temp_state;
                        variables[y][x].assignement = variables[y][x].domain[i];
                        repeating_list.push_back(temp_state);
                        available_assignemnt = 1;
                        *chosen_cell = cur_id;
                        arcConsistency(cur_state);
                        return false; //get out of the current varaible assignment
                    }
                }
                
                if(available_assignemnt == 0) //if all the domain values have been tried for current variable
                {
                    variables[y][x].assignement = 0;
                    cur_state.values[y][x] = 0;
                    assigned_variables.pop();
                    
                }
            }
            
        }
    }
    
    /*If there is no variable that has empty domain, then assign variable here*/
    for(int i = 0; i < 81; i++)
    {
        int y = i / 9;
        int x = i % 9;
        
        /*If there is any varable has not been assigned yet, assign it and return it*/
        if(cur_state.values[y][x] == 0 && variables[y][x].domain.size() > 0)
        {
            /*Find the smalles number in domain to assign it. Here no update domain for bracktrack*/
            int id_min = 0;
            cur_state.values[y][x] = variables[y][x].domain[id_min];
            variables[y][x].assignement = variables[y][x].domain[id_min];
            assigned_variables.push(i); //push the variable into stack, which will be used for backtrack (or DFS)
            repeating_list.push_back(cur_state); //make this state into the repeat_list
            *chosen_cell = 9 * y + x;
            
            arcConsistency(cur_state); //Every time modify the assignment update the domain
            
            return false;
        }
        
    }
    
    if(goalCheck(cur_state))
    {
        printf("find the goal\n");
        return true;
    }
	else
	{
		int available_assignemnt = 0; //an indicatior whether there are possible varaibles to be re-assigned
		while (available_assignemnt == 0) {
			int cur_id = assigned_variables.top();
			int y = cur_id / 9;
			int x = cur_id % 9;
			variables[y][x].assignement = 0;
			cur_state.values[y][x] = 0;
			arcConsistency(cur_state);
			for(int i = 0; i < variables[y][x].domain.size(); i++)
			{
				State temp_state;
				temp_state = cur_state;
				temp_state.values[y][x] = variables[y][x].domain[i];
				if (std::find(repeating_list.begin(), repeating_list.end(), temp_state)==repeating_list.end()) //if not in the repeating list
				{
					cur_state = temp_state;
					variables[y][x].assignement = variables[y][x].domain[i];
					repeating_list.push_back(cur_state);
					available_assignemnt = 1;
					*chosen_cell = cur_id;
					break; //get out of the current varaible assignment
				}
			}

			if(available_assignemnt == 0) //if all the domain values have been tried for current variable
			{

				assigned_variables.pop();

			}
		}

		return false;
	}
    return false;

}



/*Forward Checking algorithm*/
bool CSP::forwardChecking(int *chosen_cell)
{
    updateDomain(cur_state); //the first step is based on current setting to update the domain
    
    
    
    /*First go through all the variables and do backtrack whether there is an empty domain */
    for(int i = 0; i < 81; i++)
    {
        int y = i / 9;
        int x = i % 9;
        
        if(cur_state.values[y][x] == 0 && variables[y][x].domain.size() == 0)
        {
            int available_assignemnt = 0; //an indicatior whether there are possible possible varaibles to be re-assigned
            while (available_assignemnt == 0) {
                int cur_id = assigned_variables.top();
                int y = cur_id / 9;
                int x = cur_id % 9;
                variables[y][x].assignement = 0;
                cur_state.values[y][x] = 0;
                updateDomain(cur_state);
                for(int i = 0; i < variables[y][x].domain.size(); i++)
                {
                    State temp_state;
                    temp_state = cur_state;
                    temp_state.values[y][x] = variables[y][x].domain[i];
                    if (std::find(repeating_list.begin(), repeating_list.end(), temp_state)==repeating_list.end()) //if not in the repeating list
                    {
                        cur_state = temp_state;
                        variables[y][x].assignement = variables[y][x].domain[i];
                        repeating_list.push_back(temp_state);
                        available_assignemnt = 1;
                        *chosen_cell = cur_id;
                        updateDomain(cur_state);
                        return false; //get out of the current varaible assignment
                    }
                }
                
                if(available_assignemnt == 0) //if all the domain values have been tried for current variable
                {
                    variables[y][x].assignement = 0;
                    cur_state.values[y][x] = 0;
                    assigned_variables.pop();
                    
                }
            }
            
        }
    }
    
    /*If there is no variable that has empty domain, then assign variable here*/
    for(int i = 0; i < 81; i++)
    {
        int y = i / 9;
        int x = i % 9;
        
        /*If there is any varable has not been assigned yet, assign it and return it*/
        if(cur_state.values[y][x] == 0 && variables[y][x].domain.size() > 0)
        {
            /*Find the smalles number in domain to assign it. Here no update domain for bracktrack*/
            int id_min = 0;
            cur_state.values[y][x] = variables[y][x].domain[id_min];
            variables[y][x].assignement = variables[y][x].domain[id_min];
            assigned_variables.push(i); //push the variable into stack, which will be used for backtrack (or DFS)
            repeating_list.push_back(cur_state); //make this state into the repeat_list
            *chosen_cell = 9 * y + x;
            
            updateDomain(cur_state); //Every time modify the assignment update the domain
            
            return false;
        }

    }
    
    if(goalCheck(cur_state))
    {
        printf("find the goal\n");
        return true;
    }else
	{
		int available_assignemnt = 0; //an indicatior whether there are possible varaibles to be re-assigned
		while (available_assignemnt == 0) {
			int cur_id = assigned_variables.top();
			int y = cur_id / 9;
			int x = cur_id % 9;
			variables[y][x].assignement = 0;
			cur_state.values[y][x] = 0;
			updateDomain(cur_state);
			for(int i = 0; i < variables[y][x].domain.size(); i++)
			{
				State temp_state;
				temp_state = cur_state;
				temp_state.values[y][x] = variables[y][x].domain[i];
				if (std::find(repeating_list.begin(), repeating_list.end(), temp_state)==repeating_list.end()) //if not in the repeating list
				{
					cur_state = temp_state;
					variables[y][x].assignement = variables[y][x].domain[i];
					repeating_list.push_back(cur_state);
					available_assignemnt = 1;
					*chosen_cell = cur_id;
					break; //get out of the current varaible assignment
				}
			}

			if(available_assignemnt == 0) //if all the domain values have been tried for current variable
			{

				assigned_variables.pop();

			}
		}

		return false;
	}
    return false;
    
}


/*Forward Checking algorithm*/
bool CSP::forwardCheckingOrder(int *chosen_cell)
{

    updateDomain(cur_state); //the first step is based on current setting to update the domain
    
    
    
    /*First go through all the variables and do backtrack whether there is an empty domain */
    for(int i = 0; i < 81; i++)
    {
        int y = i / 9;
        int x = i % 9;
        
        if(cur_state.values[y][x] == 0 && variables[y][x].domain.size() == 0)
        {
            int available_assignemnt = 0; //an indicatior whether there are possible possible varaibles to be re-assigned
            while (available_assignemnt == 0) {
                int cur_id = assigned_variables.top();
                int y = cur_id / 9;
                int x = cur_id % 9;
                variables[y][x].assignement = 0;
                cur_state.values[y][x] = 0;
                updateDomain(cur_state);
                for(int i = 0; i < variables[y][x].domain.size(); i++)
                {
                    State temp_state;
                    temp_state = cur_state;
                    temp_state.values[y][x] = variables[y][x].domain[i];
                    if (std::find(repeating_list.begin(), repeating_list.end(), temp_state)==repeating_list.end()) //if not in the repeating list
                    {
                        cur_state = temp_state;
                        variables[y][x].assignement = variables[y][x].domain[i];
                        repeating_list.push_back(temp_state);
                        available_assignemnt = 1;
                        *chosen_cell = cur_id;
                        updateDomain(cur_state);
                        return false; //get out of the current varaible assignment
                    }
                }
                
                if(available_assignemnt == 0) //if all the domain values have been tried for current variable
                {
                    variables[y][x].assignement = 0;
                    cur_state.values[y][x] = 0;
                    assigned_variables.pop();
                    
                }
            }
            
        }
    }
    
    
    int count = 0;
    while (count < 81)
    {
        /*Find the index of minimum number of domain*/
        int min_idx = 0;
        int min_num = 10; //because the maximum number of domain is 10
        for(int i = 0; i < 81; i++)
        {
            int y = i / 9;
            int x = i % 9;
            if(cur_state.values[y][x] == 0 && variables[y][x].domain.size() > 0)
            {
                if (variables[y][x].domain.size() < min_num) {
                    min_idx = i;
                    min_num = variables[y][x].domain.size();
                }
            }
        }
        
        int y = min_idx / 9;
        int x = min_idx % 9;
        
        /*If there is any varable has not been assigned yet, assign it and return it*/
        if(cur_state.values[y][x] == 0 && variables[y][x].domain.size() > 0)
        {
            /*Find the smalles number in domain to assign it. Here no update domain for bracktrack*/
            int id_min = 0;
            cur_state.values[y][x] = variables[y][x].domain[id_min];
            variables[y][x].assignement = variables[y][x].domain[id_min];
            assigned_variables.push(min_idx); //push the variable into stack, which will be used for backtrack (or DFS)
            repeating_list.push_back(cur_state); //make this state into the repeat_list
            *chosen_cell = 9 * y + x;
            
            updateDomain(cur_state); //Every time modify the assignment update the domain
            
            return false;
        }
        
        count++;
    }
    
    if(goalCheck(cur_state))
    {
        printf("find the goal\n");
        return true;
    }
	else
	{
		int available_assignemnt = 0; //an indicatior whether there are possible varaibles to be re-assigned
		while (available_assignemnt == 0) {
			int cur_id = assigned_variables.top();
			int y = cur_id / 9;
			int x = cur_id % 9;
			variables[y][x].assignement = 0;
			cur_state.values[y][x] = 0;
			updateDomain(cur_state);
			for(int i = 0; i < variables[y][x].domain.size(); i++)
			{
				State temp_state;
				temp_state = cur_state;
				temp_state.values[y][x] = variables[y][x].domain[i];
				if (std::find(repeating_list.begin(), repeating_list.end(), temp_state)==repeating_list.end()) //if not in the repeating list
				{
					cur_state = temp_state;
					variables[y][x].assignement = variables[y][x].domain[i];
					repeating_list.push_back(cur_state);
					available_assignemnt = 1;
					*chosen_cell = cur_id;
					break; //get out of the current varaible assignment
				}
			}

			if(available_assignemnt == 0) //if all the domain values have been tried for current variable
			{

				assigned_variables.pop();

			}
		}

		return false;
	}

    return false;
    
}



/*Back Track to solve the proble*/
bool CSP::backTrack(int *chosen_cell)
{
    
  
    
    for(int i = 0; i < 81; i++)
    {
        int y = i / 9;
        int x = i % 9;
        
        
        /*If there is any varable has not been assigned yet, assign it and break*/
        if(cur_state.values[y][x] == 0)
        {
            
            /*Find the smalles number in domain to assign it. Here no update domain for bracktrack*/
            int id_min = 0;
            cur_state.values[y][x] = variables[y][x].domain[id_min];
            variables[y][x].assignement = variables[y][x].domain[id_min];
            assigned_variables.push(i); //push the variable into stack, which will be used for backtrack (or DFS)
            repeating_list.push_back(cur_state); //make this state into the repeat_list
            *chosen_cell = 9 * y + x;
            return false;
            
        }
    }
    
    /*If all the the variable are assigned*/
    {
        if(assigned_variables.size() == 0)//reset all the variables if there are no any varaibles assigned yet
        {
            for(int i = 0; i < 81; i++)
            {
                assigned_variables.push(i);
            }
        }
        
        if(goalCheck(cur_state))
        {
            printf("find the goal\n");
            return true;
        }
        else
        {
            int available_assignemnt = 0; //an indicatior whether there are possible varaibles to be re-assigned
            while (available_assignemnt == 0) {
                int cur_id = assigned_variables.top();
                int y = cur_id / 9;
                int x = cur_id % 9;
                
                
                for(int i = 0; i < variables[y][x].domain.size(); i++)
                {
                    State temp_state;
                    temp_state = cur_state;
                    temp_state.values[y][x] = variables[y][x].domain[i];
                    if (std::find(repeating_list.begin(), repeating_list.end(), temp_state)==repeating_list.end()) //if not in the repeating list
                    {
                        cur_state = temp_state;
                        variables[y][x].assignement = variables[y][x].domain[i];
                        repeating_list.push_back(cur_state);
                        available_assignemnt = 1;
                        *chosen_cell = cur_id;
                        break; //get out of the current varaible assignment
                    }
                }
                
                if(available_assignemnt == 0) //if all the domain values have been tried for current variable
                {
                    variables[y][x].assignement = 0;
                    cur_state.values[y][x] = 0;
                    assigned_variables.pop();
                    
                }
            }
            
            return false;
        }
    }
}
