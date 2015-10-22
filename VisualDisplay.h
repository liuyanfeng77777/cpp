#pragma once
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>

using namespace cv;
#include "SolutionSearch.h"

struct Square{ //this structure define a square region on the container (image) by using two points
	cv::Point top_left;
	cv::Point bottom_right;
};



class VisualDisplay
{
private:
	int *original_states;
	int *current_states;
	vector<int> path; //the path for solution
	int target_pos; //Store the position of the 8-piece
	

	cv::Mat image;
	cv::Mat piece[9];
	cv::Mat container; //the image that actually show the pieces and board
	Square piece_positions[9]; //define the position on the container that each piece will lay out.
							   //here it has the order of 0, 1, 2, ..., 8. So initially, it is assign the position value in the constructor

	SolutionSearch solution_finder; //the most important component that is used to find the possible solutions

	//Below are used for mouse actions
	int selected_piece; //whether there is any piece has been selected -1 means no selection, 0-7 means a particular piece is selected
	std::vector<int> moving_directions[9];

	
	
public:
	VisualDisplay(void);
	~VisualDisplay(void);
	void paintContainer(); //draw all the sub-images onto a single image

	void load(char* file_name, int* data, int pos); //load the image file to the image object
	void run(); //the main function of the class that shows different pieces of the image and support user's interaction
	friend void onMouse( int event, int x, int y, int flags, void* param );

	void swap(int candidate);
	void generateVisualMotion(int candidate, int target);
	void showSolution(); 
	void VisualDisplay::showPath();
};

