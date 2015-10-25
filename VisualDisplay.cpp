#include "VisualDisplay.h"


void VisualDisplay::onGo()
{

  
    circle_loc_old = circle_loc_cur;
    
    int action = rl.getAction();
    
    
    
    
    rl.move(action);
    circle_loc_cur = square[rl.cur_loc.y][rl.cur_loc.x].agent_pos;
    motion_sign = action + 1;
    cv::waitKey(100);
}



void onMouse( int event, int x, int y, int flags, void* param )
{
    VisualDisplay* vd = (VisualDisplay*) param;
    
    
    
    switch( event )
    {
        case CV_EVENT_LBUTTONDOWN:
            vd->buttonClickCheck(x, y);
            break;
            
        case CV_EVENT_RBUTTONDOWN:
            break;
        case CV_EVENT_LBUTTONUP:
            
            break;
            
            
        case CV_EVENT_RBUTTONUP:
            
            break;
            
        case CV_EVENT_MOUSEMOVE:
            vd->buttonHoverCheck(x, y);
            break;
    }
    
}



void VisualDisplay::buttonHoverCheck(int x, int y)
{
    
    
    /* Moving direction button */
    for(int i = 0; i < 4; i++)
    {
        if(moving_button[i].top_let.x < x && x < moving_button[i].bot_rgh.x && moving_button[i].top_let.y < y && y < moving_button[i].bot_rgh.y)
        {
            moving_button[i].status = 1;
            
        }
        else
        {
            moving_button[i].status = 0;
        }
        
    }
    
    
    
    //Auto Learning Test
    if(go_btn.top_let.x < x && x < go_btn.bot_rgh.x && go_btn.top_let.y < y && y < go_btn.bot_rgh.y)
    {
        go_btn.status = 1;
    }
    else
    {
        go_btn.status = 0;
    }
}

void VisualDisplay::buttonClickCheck(int x, int y)
{

    
    /* For Moving Directions buttons: goEast(), goSouth(), goWest(), goNorth() */
    for(int i = 0; i < 4; i++)
    {
        
        if(moving_button[i].status == 1)
        {
            moving_button[i].status = 2;
            
            if(i == 0)
            {
                goEast();
            }
            else if(i == 1)
            {
                goSouth();
            }
            else if(i == 2)
            {
                goWest();
            }
            else if(i == 3)
            {
                goNorth();
            }
        }
    }
    
    
    /* For the go button */
    if(go_btn.status == 1 || go_btn.status == 2)
    {
        go_btn.status = 2;
        go_btn.type = (go_btn.type == 1)?2:1;
        onGo();
    }
    
}



void VisualDisplay::goEast()
{
    circle_loc_old.x = circle_loc_cur.x;
    circle_loc_old.y = circle_loc_cur.y;
    motion_sign = 1;
    
    rl.move(ACTION_EAST);
    
    
    circle_loc_cur.x = square[rl.cur_loc.y][rl.cur_loc.x].agent_pos.x;
    circle_loc_cur.y = square[rl.cur_loc.y][rl.cur_loc.x].agent_pos.y;
    last_action = GO_EAST;
}

void VisualDisplay::goSouth()
{
    circle_loc_old.x = circle_loc_cur.x;
    circle_loc_old.y = circle_loc_cur.y;
    motion_sign = 2;
    rl.move(ACTION_SOUTH);
    circle_loc_cur.x = square[rl.cur_loc.y][rl.cur_loc.x].agent_pos.x;
    circle_loc_cur.y = square[rl.cur_loc.y][rl.cur_loc.x].agent_pos.y;
    last_action = GO_SOUTH;
}
void VisualDisplay::goNorth()
{
    circle_loc_old.x = circle_loc_cur.x;
    circle_loc_old.y = circle_loc_cur.y;
    motion_sign = 4;
    rl.move(ACTION_NORTH);
    circle_loc_cur.x = square[rl.cur_loc.y][rl.cur_loc.x].agent_pos.x;
    circle_loc_cur.y = square[rl.cur_loc.y][rl.cur_loc.x].agent_pos.y;
    last_action = GO_NORTH;
    
}
void VisualDisplay::goWest()
{
    circle_loc_old.x = circle_loc_cur.x;
    circle_loc_old.y = circle_loc_cur.y;
    motion_sign = 3;
    rl.move(ACTION_WEST);
    circle_loc_cur.x = square[rl.cur_loc.y][rl.cur_loc.x].agent_pos.x;
    circle_loc_cur.y = square[rl.cur_loc.y][rl.cur_loc.x].agent_pos.y;
    last_action = GO_WEST;
}

void VisualDisplay::onReset()
{
    
}

void VisualDisplay::onNext()
{
    
}


void VisualDisplay::onLearning()
{
    
    rl_learning = (go_btn.type == 2)? 1 : 0;
    
    
}



VisualDisplay::VisualDisplay()
{
    cv::namedWindow("Grid World");
    setMouseCallback("Grid World", onMouse, this);
    
    /*The pixel position for each row (the top boundary)*/
    int row_pixel[3] = {8, 156, 304};
    
    /*The pixel position for each column (the left boundary)*/
    int col_pixel[4] = {14, 161, 307, 452};
    
    square_length = 144 * ratio; //pixel size of each square
    radius = RADIUS_AGENT * ratio;
    
    for(int i = 0; i < 3; i++)
        row_pixel[i] *= ratio;
    for(int i = 0; i < 4; i++)
        col_pixel[i] *= ratio;
    
    /*Specify the location of each triangle for q values*/
    for(int y = 0; y < 3; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            for(int j = 0; j < 4; j++)
            {
                for(int i = 0; i < 3; i++)
                {
                    square[y][x].polygons[j][i] = Point(0, 0);
                }
            }
        }
    }
    
    square[0][0].agent_pos = Point(87 * ratio, 81 * ratio);
    square[0][1].agent_pos = Point(231 * ratio, 82 * ratio);
    square[0][2].agent_pos = Point(378 * ratio, 81 * ratio);
    square[0][3].agent_pos = Point(529 * ratio, 81 * ratio);
    square[1][0].agent_pos = Point(85 * ratio, 230 * ratio);
    square[1][2].agent_pos = Point(380 * ratio, 230 * ratio);
    square[1][3].agent_pos = Point(528 * ratio, 230 * ratio);
    square[2][0].agent_pos = Point(85 * ratio, 379 * ratio);
    square[2][1].agent_pos = Point(232 * ratio, 379 * ratio);
    square[2][2].agent_pos = Point(378 * ratio, 379 * ratio);
    square[2][3].agent_pos = Point(525 * ratio, 379 * ratio);
    
    square[0][0].polygons[0][0] = Point(176, 167); //square 00
    square[0][0].polygons[0][1] = Point(312, 31);
    square[0][0].polygons[0][2] = Point(312, 304);
    
    square[0][0].polygons[1][0] = Point(171, 170);
    square[0][0].polygons[1][1] = Point(310, 310);
    square[0][0].polygons[1][2] = Point(31, 310);
    
    square[0][0].polygons[2][0] = Point(165, 167);
    square[0][0].polygons[2][1] = Point(28, 304);
    square[0][0].polygons[2][2] = Point(26, 25);
    
    square[0][0].polygons[3][0] = Point(170, 163);
    square[0][0].polygons[3][1] = Point(35, 25);
    square[0][0].polygons[3][2] = Point(309, 25);
    
    square[0][1].polygons[0][0] = Point(466, 168); //quare 01
    square[0][1].polygons[0][1] = Point(604, 31);
    square[0][1].polygons[0][2] = Point(604, 304);
    
    square[0][1].polygons[1][0] = Point(461, 172);
    square[0][1].polygons[1][1] = Point(601, 310);
    square[0][1].polygons[1][2] = Point(321, 309);
    
    square[0][1].polygons[2][0] = Point(455, 168);
    square[0][1].polygons[2][1] = Point(320, 303);
    square[0][1].polygons[2][2] = Point(320, 30);
    
    square[0][1].polygons[3][0] = Point(461, 165);
    square[0][1].polygons[3][1] = Point(600, 25);
    square[0][1].polygons[3][2] = Point(324, 25);
    
    square[0][2].polygons[0][0] = Point(760, 167); //quare 02
    square[0][2].polygons[0][1] = Point(898, 28);
    square[0][2].polygons[0][2] = Point(898, 306);
    
    square[0][2].polygons[1][0] = Point(755, 172);
    square[0][2].polygons[1][1] = Point(893, 310);
    square[0][2].polygons[1][2] = Point(613, 310);
    
    square[0][2].polygons[2][0] = Point(748, 167);
    square[0][2].polygons[2][1] = Point(612, 302);
    square[0][2].polygons[2][2] = Point(612, 31);
    
    square[0][2].polygons[3][0] = Point(754, 162);
    square[0][2].polygons[3][1] = Point(891, 24);
    square[0][2].polygons[3][2] = Point(615, 24);
    
    square[1][0].polygons[0][0] = Point(174, 462); //quare 10
    square[1][0].polygons[0][1] = Point(312, 321);
    square[1][0].polygons[0][2] = Point(312, 600);
    
    square[1][0].polygons[1][0] = Point(169, 467);
    square[1][0].polygons[1][1] = Point(306, 603);
    square[1][0].polygons[1][2] = Point(30, 603);
    
    square[1][0].polygons[2][0] = Point(165, 461);
    square[1][0].polygons[2][1] = Point(28, 596);
    square[1][0].polygons[2][2] = Point(28, 324);
    
    square[1][0].polygons[3][0] = Point(170, 455);
    square[1][0].polygons[3][1] = Point(305, 316);
    square[1][0].polygons[3][2] = Point(31, 316);
    
    
    square[1][2].polygons[0][0] = Point(762, 461); //quare 12
    square[1][2].polygons[0][1] = Point(898, 324);
    square[1][2].polygons[0][2] = Point(898, 598);
    
    square[1][2].polygons[1][0] = Point(757, 466);
    square[1][2].polygons[1][1] = Point(895, 604);
    square[1][2].polygons[1][2] = Point(617, 604);
    
    square[1][2].polygons[2][0] = Point(753, 461);
    square[1][2].polygons[2][1] = Point(610, 602);
    square[1][2].polygons[2][2] = Point(613, 321);
    
    square[1][2].polygons[3][0] = Point(758, 456);
    square[1][2].polygons[3][1] = Point(898, 316);
    square[1][2].polygons[3][2] = Point(616, 316);
    
    
    
    square[2][0].polygons[0][0] = Point(174, 755); //square 20
    square[2][0].polygons[0][1] = Point(311, 619);
    square[2][0].polygons[0][2] = Point(311, 894);
    
    square[2][0].polygons[1][0] = Point(168, 761);
    square[2][0].polygons[1][1] = Point(304, 898);
    square[2][0].polygons[1][2] = Point(32, 898);
    
    square[2][0].polygons[2][0] = Point(162, 756);
    square[2][0].polygons[2][1] = Point(25, 895);
    square[2][0].polygons[2][2] = Point(26, 620);
    
    square[2][0].polygons[3][0] = Point(168, 751);
    square[2][0].polygons[3][1] = Point(309, 610);
    square[2][0].polygons[3][2] = Point(27, 610);
    
    square[2][1].polygons[0][0] = Point(468, 756); //quare 21
    square[2][1].polygons[0][1] = Point(605, 620);
    square[2][1].polygons[0][2] = Point(605, 893);
    
    square[2][1].polygons[1][0] = Point(463, 762);
    square[2][1].polygons[1][1] = Point(601, 898);
    square[2][1].polygons[1][2] = Point(325, 898);
    
    square[2][1].polygons[2][0] = Point(458, 757);
    square[2][1].polygons[2][1] = Point(320, 894);
    square[2][1].polygons[2][2] = Point(320, 618);
    
    square[2][1].polygons[3][0] = Point(463, 751);
    square[2][1].polygons[3][1] = Point(604, 611);
    square[2][1].polygons[3][2] = Point(324, 611);
    
    square[2][2].polygons[0][0] = Point(760, 754); //quare 22
    square[2][2].polygons[0][1] = Point(898, 618);
    square[2][2].polygons[0][2] = Point(898, 893);
    
    square[2][2].polygons[1][0] = Point(755, 761);
    square[2][2].polygons[1][1] = Point(893, 898);
    square[2][2].polygons[1][2] = Point(617, 898);
    
    square[2][2].polygons[2][0] = Point(749, 755);
    square[2][2].polygons[2][1] = Point(613, 892);
    square[2][2].polygons[2][2] = Point(613, 619);
    
    square[2][2].polygons[3][0] = Point(756, 749);
    square[2][2].polygons[3][1] = Point(896, 610);
    square[2][2].polygons[3][2] = Point(614, 610);
    
    
    square[2][3].polygons[0][0] = Point(1055, 753); //quare 23
    square[2][3].polygons[0][1] = Point(1195, 614);
    square[2][3].polygons[0][2] = Point(1193, 892);
    
    square[2][3].polygons[1][0] = Point(1048, 759);
    square[2][3].polygons[1][1] = Point(1188, 898);
    square[2][3].polygons[1][2] = Point(909, 898);
    
    square[2][3].polygons[2][0] = Point(1042, 753);
    square[2][3].polygons[2][1] = Point(906, 892);
    square[2][3].polygons[2][2] = Point(906, 618);
    
    square[2][3].polygons[3][0] = Point(1047, 747);
    square[2][3].polygons[3][1] = Point(1188, 610);
    square[2][3].polygons[3][2] = Point(910, 610);
    
    
    
    for(int y = 0; y < 3; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            for(int j = 0; j < 4; j++)
            {
                for(int i = 0; i < 3; i++)
                {
                    
                    square[y][x].polygons[j][i].x = (ratio/2.0) * square[y][x].polygons[j][i].x;
                    square[y][x].polygons[j][i].y = (ratio/2.0) * square[y][x].polygons[j][i].y;
                }
            }
        }
    }
    
    
    /*The grid world area of the image (left and bottom boundary*/
    grid_size.x = 620 * ratio;
    grid_size.y = 460 * ratio;
    
    /*initialize all the squares: number positions, state value, Q value*/
    for(int y = 0; y < 3; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            if((y == 0 && x == 3) || (y == 1 && x == 3) || (y == 1 && x == 1))
                continue;
                
            //Center
            square[y][x].cen_pos.y = row_pixel[y] + square_length / 2.0 + margin_bottom * ratio;
            square[y][x].cen_pos.x = col_pixel[x] + square_length / 2.0 - margin_left * ratio;
            
            
            //East
            square[y][x].q_pos[0].y = row_pixel[y] + square_length / 2.0 + margin_bottom * ratio;
            square[y][x].q_pos[0].x = col_pixel[x] + square_length * 6.8 / 9.0 - margin_left * ratio;
            
            
            //South
            square[y][x].q_pos[1].y = row_pixel[y] + square_length * 5.0 / 6.0 + margin_bottom * ratio;
            square[y][x].q_pos[1].x = col_pixel[x] + square_length / 2.0 - margin_left * ratio;
            
            
            //West
            square[y][x].q_pos[2].y = row_pixel[y] + square_length / 2.0 + margin_bottom * ratio;
            square[y][x].q_pos[2].x = col_pixel[x] + square_length * 1.0 / 5.0 - margin_left * ratio;
            
            
            //North
            square[y][x].q_pos[3].y = row_pixel[y] + square_length * 1.0 / 6.0 + margin_bottom * ratio;
            square[y][x].q_pos[3].x = col_pixel[x] + square_length / 2.0 - margin_left * ratio;

            
            //Arrow position
            square[y][x].arrow_pos.x = col_pixel[x];
            square[y][x].arrow_pos.y = col_pixel[y];
            
            
            square[y][x].state_value = 0.00f;
            
            for(int i = 0; i < 4; i++)
                square[y][x].q_value[i] = 0.0f;
        }
    }
    
    
    
    /*initialize the control data*/
    idx = 0; //initiall set it to zero
    motion_sign = 0; //initially no motion
    last_action = 0; //initially set it to face east
    
    
    /*Initiate the button status*/
    for(int i = 1; i < 6; i++)
    {
        button[i].status = 0;
    }
    button[0].status = 2;
    go_btn.type = 1; //initially it is in ready to go state.
    
    /*Initialize the RL mode buttons*/
    for(int i = 0; i < 4; i++)
    {
        moving_button[i].status = 0;
    }
    
    
    /*Set the initial pac man loaction*/
    for(int y = 0; y < 3; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            dots[y][x] = 1;
        }
    }
    dots[0][3] = 5;
    dots[1][3] = -5;
    
    circle_loc_cur.x = square[rl.cur_loc.y][rl.cur_loc.x].agent_pos.x;
    circle_loc_cur.y = square[rl.cur_loc.y][rl.cur_loc.x].agent_pos.y;

    
    game_running = 0;
    add_dot_sign = 0;
    game_over_sign = 0;
    motion_sign_ghost = 0;
    
    
    
    /* Random Generator */
    srand(time(NULL));


}

VisualDisplay::~VisualDisplay(void)
{
}



void VisualDisplay::loadImages()
{
    org_gridworld[0] = imread("GridWorld.jpg");
    org_gridworld[1] = imread("gridworld_2.png");
    org_gridworld[2] = imread("gridWorld_cross 1.jpg");
    cv::resize(org_gridworld[0], org_gridworld[0], cv::Size(org_gridworld[0].cols * ratio, org_gridworld[0].rows * ratio));
    cv::resize(org_gridworld[1], org_gridworld[1], cv::Size(org_gridworld[1].cols * ratio, org_gridworld[1].rows * ratio));
    cv::resize(org_gridworld[2], org_gridworld[2], cv::Size(org_gridworld[2].cols * ratio, org_gridworld[2].rows * ratio));
    
    
    /* Below is for Reinforcement Learning Mode */
    for(int i = 0; i < 4; i++)
    {
        char file_name[20];
        sprintf(file_name, "arrow_%d.jpg", i);
        moving_button[i].org_img = imread(file_name);
        
        sprintf(file_name, "arrow_%d_hover.jpg", i);
        moving_button[i].hov_img = imread(file_name);
        moving_button[i].org_img.copyTo(moving_button[i].clk_img);
        
        resize(moving_button[i].org_img, moving_button[i].org_img, Size(52 * ratio, 52 * ratio));
        resize(moving_button[i].hov_img, moving_button[i].hov_img, Size(52 * ratio, 52 * ratio));
        resize(moving_button[i].clk_img, moving_button[i].clk_img, Size(52 * ratio, 52 * ratio));
        
        if(i == 0)
        {
            moving_button[i].top_let.x = grid_size.x + 120 * ratio; //render the east button
            moving_button[i].top_let.y = grid_size.y * .33;
            moving_button[i].bot_rgh.x = moving_button[i].top_let.x + moving_button[i].org_img.cols;
            moving_button[i].bot_rgh.y = moving_button[i].top_let.y + moving_button[i].org_img.rows;
        }
        
        else if(i == 1)
        {
            moving_button[i].top_let.x = grid_size.x + 70 * ratio; //render the south button
            moving_button[i].top_let.y = grid_size.y * .42;
            moving_button[i].bot_rgh.x = moving_button[i].top_let.x + moving_button[i].org_img.cols;
            moving_button[i].bot_rgh.y = moving_button[i].top_let.y + moving_button[i].org_img.rows;
        }
        
        else if(i == 2)
        {
            moving_button[i].top_let.x = grid_size.x + 20 * ratio; //render the west button
            moving_button[i].top_let.y = grid_size.y * .33;
            moving_button[i].bot_rgh.x = moving_button[i].top_let.x + moving_button[i].org_img.cols;
            moving_button[i].bot_rgh.y = moving_button[i].top_let.y + moving_button[i].org_img.rows;
        }
        
        else if(i == 3)
        {
            moving_button[i].top_let.x = grid_size.x + 70 * ratio; //render the north button
            moving_button[i].top_let.y = grid_size.y * .22;
            moving_button[i].bot_rgh.x = moving_button[i].top_let.x + moving_button[i].org_img.cols;
            moving_button[i].bot_rgh.y = moving_button[i].top_let.y + moving_button[i].org_img.rows;
        }
    
            
        
        
    }
    

    /*Load the button images*/
    button[0].org_img = imread("radio_button.png"); //button  0 (gridwall Mode)
    button[0].hov_img = imread("radio_hover.png");
    button[0].clk_img = imread("radio_select.png");
    resize(button[0].org_img, button[0].org_img, Size(20 * ratio, 20 * ratio));
    resize(button[0].hov_img, button[0].hov_img, Size(20 * ratio, 20 * ratio));
    resize(button[0].clk_img, button[0].clk_img, Size(20 * ratio, 20 * ratio));
    button[0].top_let.x = grid_size.x + 5 * ratio;
    button[0].top_let.y = grid_size.y * .04;
    button[0].bot_rgh.x = button[0].top_let.x + button[0].org_img.cols;
    button[0].bot_rgh.y = button[0].top_let.y + button[0].org_img.rows;
    
    
    button[0].org_img.copyTo(button[1].org_img); //button 1 (Gaming Mode)
    button[0].hov_img.copyTo(button[1].hov_img);
    button[0].clk_img.copyTo(button[1].clk_img);
    button[1].top_let.x = grid_size.x + 5 * ratio;
    button[1].top_let.y = grid_size.y * .11;
    button[1].bot_rgh.x = button[1].top_let.x + button[1].org_img.cols;
    button[1].bot_rgh.y = button[1].top_let.y + button[1].org_img.rows;
    
    
    
    
    button[2].org_img = imread("radio_button.png"); //Learning Mode
    button[2].hov_img = imread("radio_hover.png");
    button[2].clk_img = imread("radio_select.png");
    resize(button[2].org_img, button[2].org_img, Size(20 * ratio, 20 * ratio));
    resize(button[2].hov_img, button[2].hov_img, Size(20 * ratio, 20 * ratio));
    resize(button[2].clk_img, button[2].clk_img, Size(20 * ratio, 20 * ratio));
    button[2].top_let.x = grid_size.x + 5 * ratio; //render the check button
    button[2].top_let.y = grid_size.y * .18;
    button[2].bot_rgh.x = button[2].top_let.x + button[2].org_img.cols;
    button[2].bot_rgh.y = button[2].top_let.y + button[2].org_img.rows;
    
    
    button[3].org_img = imread("checkbox_unchecked.png"); //add dot
    button[3].hov_img = imread("checkbox_unchecked.png");
    button[3].clk_img = imread("checkbox_checked.png");
    resize(button[3].org_img, button[3].org_img, Size(18 * ratio, 18 * ratio));
    resize(button[3].hov_img, button[3].hov_img, Size(18 * ratio, 18 * ratio));
    resize(button[3].clk_img, button[3].clk_img, Size(18 * ratio, 18 * ratio));
    button[3].top_let.x = grid_size.x + 25 * ratio; //render the check button
    button[3].top_let.y = grid_size.y * .26;
    button[3].bot_rgh.x = button[3].top_let.x + button[3].org_img.cols;
    button[3].bot_rgh.y = button[3].top_let.y + button[3].org_img.rows;
    
    
    
    button[4].org_img = imread("checkbox_unchecked.png"); //add ghost
    button[4].hov_img = imread("checkbox_unchecked.png");
    button[4].clk_img = imread("checkbox_checked.png");
    resize(button[4].org_img, button[4].org_img, Size(18 * ratio, 18 * ratio));
    resize(button[4].hov_img, button[4].hov_img, Size(18 * ratio, 18 * ratio));
    resize(button[4].clk_img, button[4].clk_img, Size(18 * ratio, 18 * ratio));
    button[4].top_let.x = grid_size.x + 25 * ratio; //render the next button
    button[4].top_let.y = grid_size.y * .33;
    button[4].bot_rgh.x = button[4].top_let.x + button[4].org_img.cols;
    button[4].bot_rgh.y = button[4].top_let.y + button[4].org_img.rows;
    
    
    
    go_btn.org_img = imread("go.png"); //next button 0 (move one step forward)
    go_btn.hov_img = imread("go_hover.png");
    go_btn.clk_img = imread("pause_hover.png");
    resize(go_btn.org_img, go_btn.org_img, Size(80 * ratio, 80 * ratio));
    resize(go_btn.hov_img, go_btn.hov_img, Size(80 * ratio, 80 * ratio));
    resize(go_btn.clk_img, go_btn.clk_img, Size(80 * ratio, 80 * ratio));
    go_btn.org_img_2 = imread("pause.png"); //next button 0 (move one step forward)
    go_btn.hov_img_2 = imread("pause_hover.png");
    go_btn.clk_img_2 = imread("go_hover.png");
    resize(go_btn.org_img_2, go_btn.org_img_2, Size(80 * ratio, 80 * ratio));
    resize(go_btn.hov_img_2, go_btn.hov_img_2, Size(80 * ratio, 80 * ratio));
    resize(go_btn.clk_img_2, go_btn.clk_img_2, Size(80 * ratio, 80 * ratio));
    go_btn.top_let.x = grid_size.x + 50 * ratio; //render the next button
    go_btn.top_let.y = grid_size.y * .50;
    go_btn.bot_rgh.x = go_btn.top_let.x + go_btn.org_img.cols;
    go_btn.bot_rgh.y = go_btn.top_let.y + go_btn.org_img.rows;
    
    
}



/* Draw the buttons on the canvas */
void VisualDisplay::paintButtons()
{
    
    
    
    go_btn.top_let.x = grid_size.x + 50 * ratio; //render the next button
    go_btn.top_let.y = grid_size.y * .70;
    go_btn.bot_rgh.x = go_btn.top_let.x + go_btn.org_img.cols;
    go_btn.bot_rgh.y = go_btn.top_let.y + go_btn.org_img.rows;
    
    //render the moving buttons
    for(int i = 0; i < 4; i++)
    {
        if(moving_button[i].status == 0)
        {
            moving_button[i].org_img.copyTo(gridworld_img(cv::Range(moving_button[i].top_let.y, moving_button[i].bot_rgh.y), cv::Range(moving_button[i].top_let.x, moving_button[i].bot_rgh.x)));
        }
        else if(moving_button[i].status == 1) //render the hover image
        {
            moving_button[i].hov_img.copyTo(gridworld_img(cv::Range(moving_button[i].top_let.y, moving_button[i].bot_rgh.y), cv::Range(moving_button[i].top_let.x, moving_button[i].bot_rgh.x)));
        }
        else //render selected image
        {
            moving_button[i].clk_img.copyTo(gridworld_img(cv::Range(moving_button[i].top_let.y, moving_button[i].bot_rgh.y), cv::Range(moving_button[i].top_let.x, moving_button[i].bot_rgh.x)));
        }
    }
    
    
    
    {
        if(go_btn.status == 0 && go_btn.type ==1)
        {
            go_btn.org_img.copyTo(gridworld_img(cv::Range(go_btn.top_let.y, go_btn.bot_rgh.y), cv::Range(go_btn.top_let.x, go_btn.bot_rgh.x)));
        }
        if(go_btn.status == 1 && go_btn.type ==1)
        {
            go_btn.hov_img.copyTo(gridworld_img(cv::Range(go_btn.top_let.y, go_btn.bot_rgh.y), cv::Range(go_btn.top_let.x, go_btn.bot_rgh.x)));
        }
        else if(go_btn.status == 2 && go_btn.type == 2)
        {
            go_btn.clk_img.copyTo(gridworld_img(cv::Range(go_btn.top_let.y, go_btn.bot_rgh.y), cv::Range(go_btn.top_let.x, go_btn.bot_rgh.x)));
        }
        else if(go_btn.status == 0 && go_btn.type == 2)
        {
            go_btn.org_img_2.copyTo(gridworld_img(cv::Range(go_btn.top_let.y, go_btn.bot_rgh.y), cv::Range(go_btn.top_let.x, go_btn.bot_rgh.x)));
        }
        else if(go_btn.status == 1 && go_btn.type == 2)
        {
            go_btn.hov_img_2.copyTo(gridworld_img(cv::Range(go_btn.top_let.y, go_btn.bot_rgh.y), cv::Range(go_btn.top_let.x, go_btn.bot_rgh.x)));
        }
        else if(go_btn.status == 2 && go_btn.type == 1)
        {
            go_btn.clk_img_2.copyTo(gridworld_img(cv::Range(go_btn.top_let.y, go_btn.bot_rgh.y), cv::Range(go_btn.top_let.x, go_btn.bot_rgh.x)));
        }
    }
    
    
    
    /*Render the labels*/
    cv::putText(gridworld_img, "Reinforcement Learing", Point(button[0].top_let.x - 5 * ratio, button[0].top_let.y + 20 * ratio), FONT_HERSHEY_COMPLEX_SMALL, 0.70 * ratio, Scalar(50, 50, 50), 1, CV_AA);
    
    // cv::putText(gridworld_img, "Learning", Point(button[1].top_let.x + 25 * ratio, button[1].top_let.y + 10 * ratio), FONT_HERSHEY_COMPLEX_SMALL, 0.75 * ratio, Scalar(50, 50, 50), 1, CV_AA);
    
    /*Render the number of learnings*/
    char iteration_text[200];
    sprintf(iteration_text, "Leaning Episodes: %02d", idx);
    cv::putText(gridworld_img, iteration_text, Point(grid_size.x / 2.0 - 50 * ratio, grid_size.y + 15 * ratio), FONT_HERSHEY_COMPLEX_SMALL, 0.7 * ratio, Scalar(50, 50, 50), 1, CV_AA);
    
    
    //draw the label for automatic learning
    cv::putText(gridworld_img, "Auto Learning", Point(grid_size.x + 25 * ratio, grid_size.y * .68), FONT_HERSHEY_COMPLEX_SMALL, 0.9 * ratio, Scalar(50, 50, 50), 1, CV_AA);
    
}



void VisualDisplay::drawCircle()
{
    
    if(motion_sign == 0) //draw the still circle
    {
        circle(gridworld_img, circle_loc_cur, radius, cv::Scalar(0, 255, 255), CV_FILLED, CV_AA);
        
    }
    else //draw the moving circle
    {
        cv::Mat temp;
        gridworld_img.copyTo(temp);
        int x, y;
        x = circle_loc_old.x;
        y = circle_loc_old.y;
        int step_x = (circle_loc_cur.x - circle_loc_old.x) / 10;
        int step_y = (circle_loc_cur.y - circle_loc_old.y) / 10;
        
        /* Move to a boundary and bounce back*/
        if(motion_sign != 0 && (circle_loc_cur.x == circle_loc_old.x && circle_loc_cur.y == circle_loc_old.y))
        {
            int end_x = 0;
            int end_y = 0;
            
            if(motion_sign == 1)
            {
                end_x = circle_loc_cur.x + square_length * 3.0 / 8.0;
                end_y = circle_loc_cur.y;
            }
            else if(motion_sign == 2)
            {
                end_x = circle_loc_cur.x;
                end_y = circle_loc_cur.y + square_length * 3.0 / 8.0;
            }
            else if(motion_sign == 3)
            {
                end_x = circle_loc_cur.x - square_length * 3.0 / 8.0;
                end_y = circle_loc_cur.y;
            }
            else if(motion_sign == 4)
            {
                end_x = circle_loc_cur.x;
                end_y = circle_loc_cur.y - square_length * 3.0 / 8.0;
            }
            
            step_x = (end_x - circle_loc_old.x) / 4;
            step_y = (end_y - circle_loc_old.y) / 4;
            
            for(int i = 0; i < 4; i++)
            {
                gridworld_img.copyTo(temp);
                
                cv::circle(temp, Point(x + i * step_x, y + i * step_y), radius, cv::Scalar(0, 255, 255),  CV_FILLED, CV_AA);
                char c = cv::waitKey(10);
                
                
                cv::imshow("Grid World", temp);
                
            }
            
            step_x = -step_x;
            step_y = -step_y;
            for(int i = 0; i < 4; i++)
            {
                gridworld_img.copyTo(temp);
                
                cv::circle(temp, Point(end_x + i * step_x, end_y + i * step_y), radius, cv::Scalar(0, 255, 255),  CV_FILLED, CV_AA);
                waitKey(10);
                
                cv::imshow("Grid World", temp);
                
            }
            
        }
        /* Move to a new location */
        else
        {
            for(int i = 0; i < 10; i++)
            {
                gridworld_img.copyTo(temp);
                cv::circle(temp, Point(x + i * step_x, y + i * step_y), radius, cv::Scalar(0, 255, 255),  CV_FILLED, CV_AA);
                char c = cv::waitKey(10);
                cv::imshow("Grid World", temp);
                
            }
        }
        
        
        /* The end of current episode */
        if((rl.cur_loc.y == 0 && rl.cur_loc.x == 3) || (rl.cur_loc.y == 1 && rl.cur_loc.x == 3));
        else
        {
            temp.copyTo(gridworld_img);
        }
    }
    


}



void VisualDisplay::painContainer()
{
    /*Initially render the gridworld from the original image*/
    
    
    org_gridworld[2].copyTo(gridworld_img);
    

    
    
    /*Draw the state value or Q value on the image*/
    for(int y = 0; y < 3; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            char num_char[20];
            
            for(int i = 0; i < 4; i++)
            {
                //draw triangle areas
                if(square[y][x].q_value[i] > 0.0)
                {
                    float value = square[y][x].q_value[i] * 180.0 + 20 ;
					const Point* ppt[4] = {square[y][x].polygons[0], square[y][x].polygons[1], square[y][x].polygons[2], square[y][x].polygons[3]};
					const int npt[4] = {3, 3, 3, 3};
                    fillPoly(gridworld_img, &ppt[i], &npt[i], 1, cv::Scalar(0, value, 0), CV_AA);
                }
                else if( square[y][x].q_value[i] < 0.0)
                {
                    float value = (-square[y][x].q_value[i]) * 180.0 + 20;
					const Point* ppt[4] = {square[y][x].polygons[0], square[y][x].polygons[1], square[y][x].polygons[2], square[y][x].polygons[3]};
					const int npt[4] = {3, 3, 3, 3};
                    fillPoly(gridworld_img, &ppt[i], &npt[i], 1, cv::Scalar(0, 0, value), CV_AA);
                }
                else{
					const Point* ppt[4] = {square[y][x].polygons[0], square[y][x].polygons[1], square[y][x].polygons[2], square[y][x].polygons[3]};
					const int npt[4] = {3, 3, 3, 3};
                    fillPoly(gridworld_img, &ppt[i], &npt[i], 1, cv::Scalar(0, 0, 0), CV_AA);
                }
                
                //draw the numbers
                sprintf(num_char, "%.4f", square[y][x].q_value[i]);
                cv::putText(gridworld_img, num_char, square[y][x].q_pos[i], FONT_HERSHEY_DUPLEX , 0.40 * ratio, Scalar(255, 255, 255), 1, CV_AA);
                
                
                
            }
            
        }
    }
    
    
    /* draw the buttons */
    paintButtons();
    
    
    
  
    
    drawCircle();
    
    
}



void VisualDisplay::run()
{
    while(1)
    {
        /*In every interation, refresh the data from CSP first*/
        refreshData();
        
        /* Paint the GUI */
        painContainer();
        
        /*Control four moving direction Button Refreshing*/
        for(int i = 0; i < 4; i++)
        {
            if(moving_button[i].status == 2)
            {
                waitKey(20);
                moving_button[i].status = 1;
                break;
            }
        }
        

        imshow("Grid World", gridworld_img);
        
        
        //after showing the image, stop the motion
        motion_sign = 0;
        

        
        
        
        //RLearning use: if this is the end of current episode
        if((rl.cur_loc.y == 0 && rl.cur_loc.x == 3) || (rl.cur_loc.y == 1 && rl.cur_loc.x == 3))
        {
            
            circle_loc_cur.x = square[rl.cur_loc.y][rl.cur_loc.x].agent_pos.x;
            circle_loc_cur.y = square[rl.cur_loc.y][rl.cur_loc.x].agent_pos.y;
            int radius =  RADIUS_AGENT * ratio;
            circle(gridworld_img, circle_loc_cur, radius, cvScalar(0, 255, 255), CV_FILLED);
            imshow("Grid World", gridworld_img);
            cv::waitKey(1000);
            
            
            /* Specify the start location for the circle*/
            int y = rand() % 3;
            int x = rand() % 4;
            
            while((y == 1 && x == 1) || (y == 0 && x == 3) || (y == 1 && x == 3))
            {
                y = rand() % 3;
                x = rand() % 4;
            }
            
            rl.cur_loc.x = x;
            rl.cur_loc.y = y;
            
            circle_loc_cur.x = square[rl.cur_loc.y][rl.cur_loc.x].agent_pos.x;
            circle_loc_cur.y = square[rl.cur_loc.y][rl.cur_loc.x].agent_pos.y;
            
            idx++;
        }
        
        
        /*For the Go Button*/
        if(go_btn.type == 2 )
        {
            onGo();
        }
        
        
        char c = cv::waitKey(1);
        
        if(c == 27)
            break;
        else if(c == 'w')
        {
            circle_loc_old.x = circle_loc_cur.x;
            circle_loc_old.y = circle_loc_cur.y;
            motion_sign = 4;
            rl.move(ACTION_NORTH);
            circle_loc_cur.x = square[rl.cur_loc.y][rl.cur_loc.x].agent_pos.x;
            circle_loc_cur.y = square[rl.cur_loc.y][rl.cur_loc.x].agent_pos.y;
            last_action = GO_NORTH;
        }
        else if(c == 'd')
        {
            circle_loc_old.x = circle_loc_cur.x;
            circle_loc_old.y = circle_loc_cur.y;
            motion_sign = 1;
            rl.move(ACTION_EAST);
            circle_loc_cur.x = square[rl.cur_loc.y][rl.cur_loc.x].agent_pos.x;
            circle_loc_cur.y = square[rl.cur_loc.y][rl.cur_loc.x].agent_pos.y;
            last_action = GO_EAST;
            
        }
        else if(c == 's')
        {
            circle_loc_old.x = circle_loc_cur.x;
            circle_loc_old.y = circle_loc_cur.y;
            motion_sign = 2;
            rl.move(ACTION_SOUTH);
            circle_loc_cur.x = square[rl.cur_loc.y][rl.cur_loc.x].agent_pos.x;
            circle_loc_cur.y = square[rl.cur_loc.y][rl.cur_loc.x].agent_pos.y;
            last_action = GO_SOUTH;
        }
        else if(c == 'a')
        {
            circle_loc_old.x = circle_loc_cur.x;
            circle_loc_old.y = circle_loc_cur.y;
            motion_sign = 3;
            rl.move(ACTION_WEST);
            circle_loc_cur.x = square[rl.cur_loc.y][rl.cur_loc.x].agent_pos.x;
            circle_loc_cur.y = square[rl.cur_loc.y][rl.cur_loc.x].agent_pos.y;
            last_action = GO_WEST;
        }
    }
}


/* Transfer the data from MDP object to the VisualDisplay class*/
void VisualDisplay::refreshData()
{
    for(int y = 0; y < 3; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            square[y][x].state_value = rl.states[y][x].state_value;
            
            for(int i = 0; i < 4; i++)
            {
                square[y][x].q_value[i] = rl.states[y][x].q_values[i];
            }
        }
    }
    
}






