#ifndef AI_GridWorld_VisualDisplay_h
#define AI_GridWorld_VisualDisplay_h


#endif

#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/gpu/gpu.hpp>
#include "MDP.h"
#include "RLearning.h"

#define ratio 1.0 //determine the size of the image
#define margin_left 25 //the pixels start for putting text number on the square
#define margin_bottom 10

#define NUM_BUTTON 100
#define RADIUS_AGENT 20.0


using namespace std;
using namespace cv;
using namespace gpu;


/* Square Unit on the Grid World, which represents a state*/
struct Square{
    
    Point cen_pos; //the center position of the cell
    Point agent_pos; //define the position that the agent will stay in the square
    Point q_pos[4]; //the four positions to put the number for Q value in E S W N order
    float state_value; //the number will be put in the center, which is V(s)
    float q_value[4]; //the number will be put in the four corner, which is Q(s, a, s'), in E S W N order
    Point arrow_pos; //the position that the left/right/top/bottom
    
    //draw triangles color for different q values
    Point polygons[4][3];
    /*const Point* ppt[4] = {polygons[0], polygons[1], polygons[2], polygons[3]};
    const int npt[4] = {3, 3, 3, 3};*/
	
    
};


/*Buttons*/
struct Button{
    Mat org_img; //store the original image that has no click
    Mat hov_img; //store the image when the mouse hove on top of it
    Mat clk_img; //store the image when the mouse is clicking on it.
    int status; //store the status of the button that 0 - shows original image, 1 - shows hover image, 2 - shows clicked image
    Point top_let; //the position of top left corner of the image
    Point bot_rgh; //the position of bottom right corner of the image
    
};

struct GoButton : Button{
    Mat org_img_2; //as the go button is special that there are go and pause pattern on the button
    Mat hov_img_2;
    Mat clk_img_2;
    int type; //2 - go is on; 1- paus is on
};


/*The main GUI class showing the grid world map*/
class VisualDisplay{
public:
    MDP mdp; //the markov decision process class used to compute the values
    RLearning rl; //retrieve data from the RLearning class
    Mat gridworld_img; //the main image will display in the window
    Mat arrow_imgs[4]; //the arrow images
    Mat arrow_masks[4]; //the mask image for the arrow
    Mat org_gridworld[3]; //the original image loaded from file
    Mat q_mask[3][4][4]; //the triangles for update Q value color
    
    int square_length; //the size of each square in pixels
    Point grid_size; //the x and y of this point represent the grid area size
    Square square[3][4]; //3x4 squares on the image
    
    
    
    /*Control Data*/
    int add_dot_sign; //for gaming mode: there are dot on the road
    int add_ghost_sign; //for gaming mode: there is one ghost in the world
    int last_action; //for render the circle purpose
    int ghost_last_action; //for render the ghost purpose
    int idx; //the global index about number of iterations
    int motion_sign; //to distinguish the motion and staying still  0 - no moving action (purely stay).
    int motion_sign_ghost; //the same as motion_sign but control the ghost
    
    
    int game_running; //1- the game is running 0 - the game is not running
    int game_over_sign; //when the user exit the game from the red or green square

    /*Workflow control image*/
    Button button[NUM_BUTTON]; //the buttons
    Button moving_button[4]; //RL mode: the four directions for roboth moving
    GoButton go_btn; //go button for running the algorithm in a batch
    int radius;
    
    /* Pacman gaming use*/
    int dots[3][4]; //indicate whether the corresponding dots have been taken
    cv::Point circle_loc_old;  //the previous location of the pac man
    cv::Point circle_loc_cur; //the current location of the pac man

    
    
    int rl_learning; //1 - currently is learning; 0 - learing is paused
    
    
public:
    VisualDisplay();
    ~VisualDisplay();
    void loadImages();
    void run();
    void painContainer();
    void paintButtons(); //this is called by paintContainer
    
    void refreshData(); //retrieve new data from the MDP
    
    
    /*Button Check*/
    void buttonHoverCheck(int x, int y);
    void buttonClickCheck(int x, int y);
    
    /*MDP Button Callback Functions*/
    void onNext();
    void onReset();
    void onGo();
    
    /*MDP Button Callback Functions*/
    void goEast();
    void goSouth();
    void goWest();
    void goNorth();
    
    
    /*Drawing moving objects*/
    void drawCircle();
    
    
    void geterateBothActions();
    void onLearning();
};


