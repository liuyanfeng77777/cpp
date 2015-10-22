#include "SolutionSearch.h"
#include "stack"
 #include<math.h>
SolutionSearch::SolutionSearch(void)
{
}


SolutionSearch::~SolutionSearch(void)
{
}



/*			Student Implementation: here you need to implement the Breadth First Search Method						*/
/*																													*/
/*			Input: the first parameter is the random order of the 9 numbers, which you need to						*/
/*				   re-organize to make them into the correct order.													*/
/*				   the second parameter is actually an output. It returns or stores the moving						*/ 
/*				   path of the "empty space" that it is involved to make all the sub-images in						*/
/*				   the correct position. The integer seqence varible "solution" should store						*/
/*				   all the steps along the path																		*/
/*                                                                          										*/
/*			For example:																							*/
/*						Input:  data = {8, 4, 1, 3, 0, 2, 6, 7, 5 };												*/
/*						to make it into the correct order {0, 1, 2, 3, 4, 5, 6, 7, 8}								*/
/*						you need to make the following changes on the number 8, since the							*/
/*						number 8 represents the empty space, moving 8 to its neigboring								*/
/*						numbers equals to moving the corresponding number to the empty space.						*/
/*						Below it shows a demo of the steps:															*/
/*																													*/
/*  8 4 1   swap with 4   8 0 1   swap with 1    8 1 0   swap with 2     8 1 2   swap with 5    0 1 2				*/
/*  3 0 2  ------------>  3 4 2  ------------->  3 4 2  -------------->  3 4 8  --------------> 3 4 5  ---> End		*/
/*  6 7 5                 6 7 5                  6 7 5                   6 7 5                  6 7 8				*/
/*																													*/
/*			So from this example, the right path should be {1, 2, 5, 8}.											*/
/*			WHY? You may thought it was {4, 1, 2, 5}, since the number 8 has swapped with them in this order.		*/
/*			That is true. However, we do not care which number it swapped with, but which position the number 8		*/
/*          has gone through. As the numbers can be in any positions during different time, which give no hint		*/
/*			about where the number 8 is. In contrast, the positions are fixed. So we assume the positions are		*/
/*			in the same order as an increasing sequence:															*/
/*																													*/
/*																[0]  [1]  [2]										*/
/*											Fixed Position =    [3]  [4]  [5]										*/
/*																[6]  [7]  [8]										*/
/*																													*/											
/*			Here, I use "[]" to distinguish the positions from the numbers. So now you can see, the number 8 starts */
/*			from position [4], then swapped with number 4, which goes to the position [1]; then swapped with number */
/*			1, which goes to the position [2]; then swapped with number 2, which goes to the position [5]; finally, */
/*			swapped with number 5, which goes to the position [8]. So the path you should assign to the parameter   */
/*			"&solution" with the path sequence {1, 2, 5, 8}.	
*/
class Node
{
	

	public:
		int *matrix;
		Node *parent;
		int movement;
		int hcost;
		int gcost;
		Node(int *data, Node *p, int m, int hcostValue,int gcostValue)
	{
		matrix=data;
		parent=p;
		movement=m;
		hcost=hcostValue;
		gcost=gcostValue;
	}

		Node(){}
	bool cmp( int *dest)
	{
		//for(int i=0;i<sizeof(this->matrix);i++)
		for(int i=0;i<9;i++)
		{
			if (dest[i]!=this->matrix[i])
				return false;
				
		}
		return true;
	}
		bool greaterthan(const Node* pnode) const
		{
			if(hcost+gcost==pnode->hcost+pnode->gcost)
				return pnode->hcost+pnode->gcost>hcost+gcost;
			return hcost+gcost>pnode->hcost+pnode->gcost;	
		}

};

class NodeCostCmp
{
public:
	NodeCostCmp(){}
	bool  operator ()( const  Node* ne1,  const  Node* ne2)  const
	{
		return  ne1->greaterthan(ne2);
	} 
};



int locGet(int *a,int b)
{
	for(int i=0;i<9;i++)
	{
		if(a[i]==b)
			return i;
	}
	return 0;
}

//bool checkNotRepeat(Node a,vector<int*> &v)
//{
//	for(int i=0;i<v.size();i++)
//	{
//		bool bl=a.cmp(v[i]);
//		if(bl)
//			return false;
//		
//	}
//	return true;
//}
int hcul(int* a)
{
	int total=0;
	for(int i=0;i<9;i++)
	{
		if(a[i]!=8)
		{
			int x,y,cx,cy;
			x=i%3;
			y=i/3;
			cx=a[i]%3;
			cy=a[i]/3;
			total+=abs(x-cx)+abs(y-cy);
		}
		
	}
	return total;
}
void SolutionSearch::AStarSearch(int *data, vector<int> &solution)
{
	int goal[]={0,1,2,3,4,5,6,7,8};
	priority_queue<Node*,vector<Node*>, NodeCostCmp> q;
	int datacopy[9];
	for(int i=0;i<9;i++)
	{
		datacopy[i]=data[i];
	}
	int nodehcost=hcul(datacopy);
	int stop=-1;
	Node *emptynode=new Node();
	Node *first= new Node(datacopy,emptynode,stop,nodehcost,0);
	q.push(first);
	while(!q.empty())
	{
		Node *node=q.top();
		q.pop();
		int matrix[9];
		for(int j=0;j<9;j++)
		{
			matrix[j]=node->matrix[j];
		}
		if(node->cmp(goal))
		{
			stack<int> s;
			while(node->movement!=-1)
			{
				s.push(node->movement);
				node=node->parent;
			}
			while(!s.empty())
			{
				solution.push_back(s.top());
				s.pop();
			}
			return;
		}else{
			int l=locGet(matrix,8);
			int x,y;
			x=l%3;
			y=l/3;
			if(y<2)
			{
			y++;
			int nl=3*y+x;
			if (nl!=node->parent->movement)
				{
				int buf=matrix[l];
				matrix[l]=matrix[nl];
				matrix[nl]=buf;
				int *copy=new int[9];
				for(int j=0;j<9;j++)
					{
						copy[j]=matrix[j];
					}
				int hcost=hcul(copy);
				Node *newnode=new Node(copy,node,nl,hcost,node->gcost+1);
				q.push(newnode);
				buf=matrix[l];
				matrix[l]=matrix[nl];
				matrix[nl]=buf;
				
				}	
			y--;
			}

			if(x>0)
			{
			x--;
			int nl=3*y+x;
			if (nl!=node->parent->movement)
				{
				int buf=matrix[l];
				matrix[l]=matrix[nl];
				matrix[nl]=buf;
				int *copy=new int[9];
				for(int j=0;j<9;j++)
					{
						copy[j]=matrix[j];
					}
				int hcost=hcul(copy);
				Node *newnode=new Node(copy,node,nl,hcost,node->gcost+1);
				q.push(newnode);
				buf=matrix[l];
				matrix[l]=matrix[nl];
				matrix[nl]=buf;
				
				}	
			x++;
			}

			if(y>0)
			{
			y--;
			int nl=3*y+x;
			if (nl!=node->parent->movement)
				{
				int buf=matrix[l];
				matrix[l]=matrix[nl];
				matrix[nl]=buf;
				int *copy=new int[9];
				for(int j=0;j<9;j++)
					{
						copy[j]=matrix[j];
					}
				int hcost=hcul(copy);
				Node *newnode=new Node(copy,node,nl,hcost,node->gcost+1);
				q.push(newnode);
				buf=matrix[l];
				matrix[l]=matrix[nl];
				matrix[nl]=buf;
				
				}	
			y++;
			}

			if(x<2)
			{
			x++;
			int nl=3*y+x;
			if (nl!=node->parent->movement)
				{
				int buf=matrix[l];
				matrix[l]=matrix[nl];
				matrix[nl]=buf;
				int *copy=new int[9];
				for(int j=0;j<9;j++)
					{
						copy[j]=matrix[j];
					}
				int hcost=hcul(copy);
				Node *newnode=new Node(copy,node,nl,hcost,node->gcost+1);
				q.push(newnode);
				buf=matrix[l];
				matrix[l]=matrix[nl];
				matrix[nl]=buf;
				
				}
			x--;
			}
		}
	}
}
//void Srecurrence(vector<int> &solution,vector<int*> &repeatList,queue<Node> &q)
//{
//	int goal[]={0,1,2,3,4,5,6,7,8};
//	while(!q.empty())
//	{
//		Node nd=q.front();
//		Node *n=new Node(nd.matrix,nd.parent,nd.movement);
//		Node node=*n;
//		int data[9];
//		
//		for(int j=0;j<9;j++)
//		{
//			data[j]=node.matrix[j];
//		}
////		parentList.push_back(node);
//		q.pop();
//		if(node.cmp(goal))
//		{
//			stack<int> s;
//			while(node.movement!=-1)
//			{
//				s.push(node.movement);
//				node=*node.parent;
//			}
//			while(!s.empty())
//			{
//				solution.push_back(s.top());
//				s.pop();
//			}
//			return;
//		}else{
//			int l=locGet(data,8);
//			int x,y;
//			x=l%3;
//			y=l/3;
//			if(y<2)
//			{
//			y++;
//			int nl=3*y+x;
//			int buf=data[l];
//			data[l]=data[nl];
//			data[nl]=buf;
//			int *copy=new int[9];
//			for(int j=0;j<9;j++)
//				{
//					copy[j]=data[j];
//				}
//			Node *newn=new Node(copy,n,nl);
//			Node newNode=*newn;
//			bool bl=checkNotRepeat(newNode,repeatList);
//				if(bl)
//				{
//					q.push(newNode);
//					repeatList.push_back(copy);
//				}
//
//			buf=data[l];
//			data[l]=data[nl];
//			data[nl]=buf;
//			y--;
//			}
//
//			if(x>0)
//			{
//			x--;
//			int nl=3*y+x;
//			int buf=data[l];
//			data[l]=data[nl];
//			data[nl]=buf;
//			int *copy=new int[9];
//			for(int j=0;j<9;j++)
//			{
//				copy[j]=data[j];
//			}
//			Node *newn=new Node(copy,n,nl);
//			Node newNode=*newn;
//			bool bl=checkNotRepeat(newNode,repeatList);
//				if(bl)
//				{
//					q.push(newNode);
//					repeatList.push_back(copy);
//				}
//
//			buf=data[l];
//			data[l]=data[nl];
//			data[nl]=buf;
//			x++;
//			}
//
//			if(y>0)
//			{
//			y--;
//			int nl=3*y+x;
//			int buf=data[l];
//			data[l]=data[nl];
//			data[nl]=buf;
//			int *copy=new int[9];
//			for(int j=0;j<9;j++)
//			{
//				copy[j]=data[j];
//			}
//			Node *newn=new Node(copy,n,nl);
//			Node newNode=*newn;
//			bool bl=checkNotRepeat(newNode,repeatList);
//				if(bl)
//				{
//					q.push(newNode);
//					repeatList.push_back(copy);
//				}
//
//			buf=data[l];
//			data[l]=data[nl];
//			data[nl]=buf;
//			y++;
//			}
//
//			if(x<2)
//			{
//			x++;
//			int nl=3*y+x;
//			int buf=data[l];
//			data[l]=data[nl];
//			data[nl]=buf;
//			int *copy=new int[9];
//			for(int j=0;j<9;j++)
//			{
//				copy[j]=data[j];
//			}
//			Node *newn=new Node(copy,n,nl);
//			Node newNode=*newn;
//			bool bl=checkNotRepeat(newNode,repeatList);
//				if(bl)
//				{
//					q.push(newNode);
//					repeatList.push_back(copy);
//				}
//
//			buf=data[nl];
//			data[nl]=data[l];
//			data[l]=buf;
//			x--;
//			}
//		}
//	}
////	Srecurrence(solution,repeatList,q);
//}
//void Drecurrence(int *data, vector<int> &solution, int movement ,Node *Pnode, vector<int*> &repeatList, bool &handle )
//{
//	/*printf("The solution path is: \n");
//	for(int i = 0; i < (int)solution.size(); i++)
//	{
//		printf("%d, ", solution[i]);
//	}
//	printf("\n");*/
//	int goal[]={0,1,2,3,4,5,6,7,8};
//	int c1[9];
//	for(int i=0;i<9;i++)
//	{
//		c1[i]=data[i];
//	}
//	Node node=Node(c1, Pnode, movement);
//	repeatList.push_back(c1);
//	bool bl=node.cmp(goal);
//	if (bl)
//	{
//		stack<int> s;
//		while(node.movement!=-1)
//		{
//			s.push(node.movement);
//			node=*node.parent;
//		}
//		while(!s.empty())
//		{
//			solution.push_back(s.top());
//			s.pop();
//		}
//		handle=true;
//
//	}else
//	{
//		int l=locGet(data,8);
//		int x,y;
//		x=l%3;
//		y=l/3;		
//		if(x>0)
//		{
//		x=x-1;
//		int nl=3*y+x;
//		int buf=data[l];
//		data[l]=data[nl];
//		data[nl]=buf;
//		Node nodeCopy=Node(data,&node,-1);
//		if(checkNotRepeat(nodeCopy,repeatList))
//			{
//				Drecurrence(data, solution, nl, &node,repeatList,handle);
//				if(handle)
//					return;
//				
//			}
//
//			buf=data[nl];
//			data[nl]=data[l];
//			data[l]=buf;
//			x=x+1;
//		}
//
//		
//
//		if(y<2)
//		{
//		y=y+1;
//		int nl=3*y+x;
//		int buf=data[l];
//				data[l]=data[nl];
//				data[nl]=buf;
//		int *sbuf=node.matrix;
//		node.matrix=data;
//		Node nodeCopy=Node(data,&node,-1);
//		if(checkNotRepeat(nodeCopy,repeatList))
//			{
//				Drecurrence(data, solution, nl, &node,repeatList,handle);
//				if(handle)
//					return;
//				}
//
//			buf=data[nl];
//			data[nl]=data[l];
//			data[l]=buf;
//			y=y-1;
//		}
//		if(x<2)
//		{
//		x=x+1;
//		int nl=3*y+x;
//		int buf=data[l];
//				data[l]=data[nl];
//				data[nl]=buf;
//		int *sbuf=node.matrix;
//		node.matrix=data;
//		Node nodeCopy=Node(data,&node,-1);
//		if(checkNotRepeat(nodeCopy,repeatList))
//			{
//				Drecurrence(data, solution, nl, &node,repeatList,handle);
//				if(handle)
//					return;
//				}
//
//			buf=data[nl];
//			data[nl]=data[l];
//			data[l]=buf;
//			x=x-1;
//
//		}
//		if(y>0)
//		{
//		y=y-1;
//		int nl=3*y+x;
//		int buf=data[l];
//		data[l]=data[nl];
//		data[nl]=buf;
//		int *sbuf=node.matrix;
//		node.matrix=data;
//		Node nodeCopy=Node(data,&node,-1);
//		if(checkNotRepeat(nodeCopy,repeatList))
//			{
//				Drecurrence(data, solution, nl, &node,repeatList,handle);
//				if(handle)
//					return;
//			}
//
//			buf=data[nl];
//			data[nl]=data[l];
//			data[l]=buf;
//			y=y+1;
//		}
//	}
//	
//}
//bool SolutionSearch::breadthSearch(int *data, vector<int> &solution)
//{
//	int movement=-1;
//	Node n=Node();
//	Node node=Node(data,&n,movement);
//	queue<Node> q;
//	q.push(node);
//	vector<int*> repeatList(0);
//	int copy[9];
//	for(int i=0;i<9;i++)
//	{
//		copy[i]=data[i];
//	}
//	repeatList.push_back(copy);
//	Srecurrence(solution,repeatList,q);
//	
//	return 1;
//}


//bool SolutionSearch::depthSearch(int *data, vector<int> &solution)
//{
//	bool handle=false;
//	int movement=-1;
//	vector<int*> repeatList(0);
//	Node pnode=Node();
//	Drecurrence(data,solution,movement,&pnode,repeatList,handle);
//	//solution.push_back(7);
//	//solution.push_back(8);
//	return 1;
//}





