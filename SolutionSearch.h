#pragma once

#include <vector>       // std::vector
#include <queue>
using namespace std;

class SolutionSearch
{
public:
	SolutionSearch(void);
	~SolutionSearch(void);

	void AStarSearch(int *data, vector<int> &solution);
	bool breadthSearch(int *data, vector<int> &solution); //Student Implementation: you need to implement this function
	bool depthSearch(int *data, vector<int> &solution); //Student Implementation: you need to implement this function
};


