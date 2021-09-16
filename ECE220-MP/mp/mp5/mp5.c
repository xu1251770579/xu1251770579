/*									tab:8
 *
 * main.c - skeleton source file for ECE220 picture drawing program
 *
 * "Copyright (c) 2018 by Charles H. Zega, and Saransh Sinha."
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice and the following
 * two paragraphs appear in all copies of this software.
 * 
 * IN NO EVENT SHALL THE AUTHOR OR THE UNIVERSITY OF ILLINOIS BE LIABLE TO 
 * ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL 
 * DAMAGES ARISING OUT  OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, 
 * EVEN IF THE AUTHOR AND/OR THE UNIVERSITY OF ILLINOIS HAS BEEN ADVISED 
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * THE AUTHOR AND THE UNIVERSITY OF ILLINOIS SPECIFICALLY DISCLAIM ANY 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE 
 * PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND NEITHER THE AUTHOR NOR
 * THE UNIVERSITY OF ILLINOIS HAS ANY OBLIGATION TO PROVIDE MAINTENANCE, 
 * SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 * Author:	    Charles Zega, Saransh Sinha
 * Version:	    1
 * Creation Date:   12 February 2018
 * Filename:	    mp5.h
 * History:
 *	CZ	1	12 February 2018
 *		First written.
 */
#include "mp5.h"
#include <stdio.h>
#include <stdlib.h>
#include <png.h>
/*
	You must write all your code only in this file, for all the functions!
*/



/* 
 *  near_horizontal
 *	 
 *	 
 *	
 *	
 * INPUTS: x_start,y_start -- the coordinates of the pixel at one end of the line
 * 	   x_end, y_end    -- the coordinates of the pixel at the other end
 * OUTPUTS: draws a pixel to all points in between the two given pixels including
 *          the end points
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */
// this function draw a line with slope in [-1,1]
int32_t
near_horizontal(int32_t x_start, int32_t y_start, int32_t x_end, int32_t y_end){
	/* Your code goes here! */
	int32_t temp_x,temp_y,x_step=0;
	//to make x_start always less than x_end(so we make (x_start, y_start) be the (x1,y1))
	if ( x_start > x_end ){
		temp_x = x_end;
		x_end = x_start; 
		x_start = temp_x;
		temp_y = y_end;
		y_end = y_start;
		y_start = temp_y;
	}
	//sgn =the sigh of the y_end - y_start 
	int32_t sgn = ( 0 < (y_end - y_start) ? 1:-1);
	if (y_end == y_start){
		sgn = 0;
	}
	//possible is return, and won't stop the drawing
	int32_t possible = 1;

	//scan the line by x
	for( int32_t i = (x_end - x_start); i >= 0; i-- ){
		int32_t x,y;
		x = x_step + x_start;
		y = ((2*(y_end - y_start)*(x - x_start) + sgn*(x_end - x_start))/(2*(x_end - x_start))) + y_start;
		if (1 != draw_dot(x,y)){
			possible=0;
		}
		//increment the x_step
		x_step++;
	}
	return possible;
}


/* 
 *  near_vertical
 *	 
 *	 
 *	
 *	
 * INPUTS: x_start,y_start -- the coordinates of the pixel at one end of the line
 * 	   x_end, y_end    -- the coordinates of the pixel at the other end
 * OUTPUTS: draws a pixel to all points in between the two given pixels including
 *          the end points
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */
// this function draw a line with slope in k<-1 k>1
int32_t
near_vertical(int32_t x_start, int32_t y_start, int32_t x_end, int32_t y_end){
	/* Your code goes here! */
	int32_t temp_x,temp_y,y_step=0;
	
	//to make y_start always less than y_end(so we make (x_start, y_start) be the (x1,y1))
	if ( y_start > y_end ){
		temp_x = x_end;
		x_end = x_start; 
		x_start = temp_x;
		temp_y = y_end;
		y_end = y_start;
		y_start = temp_y;
	}
	//sgn =the sigh of the y_end - y_start
	int32_t sgn = ( 0 < (x_end - x_start) ? 1:-1);
	if(x_start == x_end){
		sgn = 0;
		//if x_s = x_e y_s = y_e,draw a dot
		if(y_start == y_end){
			return draw_dot(x_start,y_start);
		}
	}
	//possible is return, and won't stop the drawing
	int32_t possible = 1;
	//scan the line by y
	for( int32_t i = (y_end - y_start); i >= 0; i-- ){
		int32_t x,y;
		y = y_step + y_start;
		x = ((2*(x_end - x_start)*(y - y_start) + sgn*(y_end - y_start))/(2*(y_end - y_start))) + x_start;
		if (1 != draw_dot(x,y)){
			possible=0;
		}
		//increment the step
		y_step++;
	}
	return possible;
}

/* 
 *  draw_line
 *	 
 *	 
 *	
 *	
 * INPUTS: x_start,y_start -- the coordinates of the pixel at one end of the line
 * 	   x_end, y_end    -- the coordinates of the pixel at the other end
 * OUTPUTS: draws a pixel to all points in between the two given pixels including
 *          the end points
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */
//this funtion choose which function to call 

int32_t
draw_line(int32_t x_start, int32_t y_start, int32_t x_end, int32_t y_end){
	/* Your code goes here! */
	int32_t possible = 1;
	//when x_start == x_end, the line is vertical or the dot(so it call function near_vertical)
	if (x_start == x_end){
		if(near_vertical(x_start,y_start,x_end,y_end) != 1){
			possible = 0;		
		} 
	}
	//this means when -1<=k<=1 we call the near_horizontal
	else if(0 == ((y_start - y_end)/(x_start - x_end)) || y_start - y_end == x_start - x_end || y_end - y_start == x_start - x_end){
		if(1 != near_horizontal(x_start,y_start,x_end,y_end)){
			possible = 0;
		}	
	}
	//when k>=1 || k<=-1
	else{
		if(near_vertical(x_start,y_start,x_end,y_end) != 1){
			possible = 0;		
		} 
	}

	return possible;
}


/* 
 *  draw_rect
 *	 
 *	 
 *	
 *	
 * INPUTS: x,y -- the coordinates of the of the top-left pixel of the rectangle
 *         w,h -- the width and height, respectively, of the rectangle
 * OUTPUTS: draws a pixel to every point of the edges of the rectangle
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
draw_rect(int32_t x, int32_t y, int32_t w, int32_t h){
	/* Your code goes here! */
	int32_t possible = 1;
	//if the width <=0 or h<0 return 0 and don't draw anything
	if((w<=0) || (h < 0)){
		return 0;
	}
	//draw four line
	if(1 != draw_line(x,y,(x+w),y)){
		possible = 0;
	}
	if(1 != draw_line(x,y,x,(y+h))){
		possible = 0;
	}
	if(1 != draw_line((x+w),(y+h),(x+w),y)){
		possible = 0;
	}
	if(1 != draw_line((x+w),(y+h),x,(y+h))){
		possible = 0;
	}


	return possible;
}


/* 
 *  draw_triangle
 *	 
 *	 
 *	
 *	
 * INPUTS: x_A,y_A -- the coordinates of one of the vertices of the triangle
 *         x_B,y_B -- the coordinates of another of the vertices of the triangle
 *         x_C,y_C -- the coordinates of the final of the vertices of the triangle
 * OUTPUTS: draws a pixel to every point of the edges of the triangle
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
draw_triangle(int32_t x_A, int32_t y_A, int32_t x_B, int32_t y_B, int32_t x_C, int32_t y_C){
	/* Your code goes here! */
	int32_t possible = 1;
	//draw three lines
	if(1 != draw_line(x_A,y_A,x_B,y_B)){
		possible = 0;
	}
	if(1 != draw_line(x_B,y_B,x_C,y_C)){
		possible = 0;
	}
	if(1 != draw_line(x_C,y_C,x_A,y_A)){
		possible = 0;
	}


	return possible;
}

/* 
 *  draw_parallelogram
 *	 
 *	 
 *	
 *	
 * INPUTS: x_A,y_A -- the coordinates of one of the vertices of the parallelogram
 *         x_B,y_B -- the coordinates of another of the vertices of the parallelogram
 *         x_C,y_C -- the coordinates of another of the vertices of the parallelogram
 * OUTPUTS: draws a pixel to every point of the edges of the parallelogram
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
draw_parallelogram(int32_t x_A, int32_t y_A, int32_t x_B, int32_t y_B, int32_t x_C, int32_t y_C){
	/* Your code goes here! */
	int32_t possible = 1;
	//draw four lines
	if(1 != draw_line(x_A,y_A,x_B,y_B)){
		possible = 0;
	}
	if(1 != draw_line(x_B,y_B,x_C,y_C)){
		possible = 0;
	}
	if(1 != draw_line(x_A,y_A,(x_C + x_A - x_B),(y_C + y_A - y_B))){
		possible = 0;
	}
	if(1 != draw_line(x_C,y_C,(x_C + x_A - x_B),(y_C + y_A - y_B))){
		possible = 0;
	}


	return possible;
}


/* 
 *  draw_circle
 *	 
 *	 
 *	
 *	
 * INPUTS: x,y -- the center of the circle
 *         inner_r,outer_r -- the inner and outer radius of the circle
 * OUTPUTS: draws a pixel to every point whose distance from the center is
 * 	    greater than or equal to inner_r and less than or equal to outer_r
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
draw_circle(int32_t x, int32_t y, int32_t inner_r, int32_t outer_r){
	/* Your code goes here!*/
	int32_t possible = 1;
	//make sure the inner_r are >0 and outer_r larger than inner_r
	if((0 > inner_r) || (inner_r >outer_r) ){
		return 0;
	}
	//scan the square of (2*outer_r)*(2*outer)
	for(int32_t i = 0; (2*outer_r) >= i; i++){
		for(int32_t j = 0; (2*outer_r) >= j; j++){
			int32_t distance_square;
			//test if the distance is in the (inner_r,outer_r)
			distance_square =(i-outer_r)*(i-outer_r)+(j-outer_r)*(j-outer_r);
			if ((distance_square >= (inner_r*inner_r)) && (distance_square <= (outer_r*outer_r))){
				if(1 != draw_dot(x+i-outer_r,y+j-outer_r)){
					possible = 0;				
				}
			}	
		}
	}


	return possible;
}


/* 
 *  rect_gradient
 *	 
 *	 
 *	
 *	
 * INPUTS: x,y -- the coordinates of the of the top-left pixel of the  rectangle
 *         w,h -- the width and height, respectively, of the rectangle
 *         start_color -- the color of the far left side of the rectangle
 *         end_color -- the color of the far right side of the rectangle
 * OUTPUTS: fills every pixel within the bounds of the rectangle with a color
 *	    based on its position within the rectangle and the difference in
 *          color between start_color and end_color
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
rect_gradient(int32_t x, int32_t y, int32_t w, int32_t h, int32_t start_color, int32_t end_color){
	/* Your code goes here! */
	int32_t possible = 1;
	//test the impossible situation
	if((w<=0) || (h < 0)){
		return 0;
	}
	//scan by x
	for (int32_t i =0; i <= w; i++){
		int32_t current_color,current_red,current_green,current_blue,start_red,end_red,start_blue,end_blue,start_green,end_green;
		//pick the eight bits that represent each color and move to first eight bits
		start_red = (start_color & 0x000000FF);
		end_red = (end_color & 0x000000FF);
		start_blue = ((start_color & 0x0000FF00) >> 8);
		end_blue = ((end_color & 0x0000FF00) >> 8);
		start_green = ((start_color & 0x00FF0000) >> 16);
		end_green = ((end_color & 0x00FF0000) >> 16);
		//calculate the sgn for each color 
		int32_t sgn_r = ( 0 < (end_red - start_red) ? 1:-1);
		int32_t sgn_b = ( 0 < (end_blue - start_blue) ? 1:-1);
		int32_t sgn_g = ( 0 < (end_green - start_green) ? 1:-1);
		//calculate the average of each color and turn back to the 32bits color
		current_red = (((2*i*(end_red-start_red))+(sgn_r*w))/(2*w))+start_red;
		current_blue = (((((2*i*(end_blue-start_blue))+(sgn_b*w))/(2*w))+start_blue) << 8);
		current_green = (((((2*i*(end_green-start_green))+(sgn_g*w))/(2*w))+start_green) << 16);
		current_color = current_red + current_blue + current_green;
		set_color(current_color);
		//draw
		if(1 != draw_line((x+i),y,(x+i),(y+h))){
			possible = 0;
		}
	}


	return possible;
}



/* 
 *  near_dothorizontal
 *	 
 *	 
 *	
 *	
 * INPUTS: x_start,y_start -- the coordinates of the pixel at one end of the line
 * 	   x_end, y_end    -- the coordinates of the pixel at the other end
 * OUTPUTS: draws a dotted line to all points in between the two given pixels including
 *          the end points
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
near_dothorizontal(int32_t x_start, int32_t y_start, int32_t x_end, int32_t y_end){
	/* Your code goes here! */
	int32_t temp_x,temp_y,x_step=0;
	//to make x_start always less than x_end(so we make (x_start, y_start) be the (x1,y1))
	if ( x_start > x_end ){
		temp_x = x_end;
		x_end = x_start; 
		x_start = temp_x;
		temp_y = y_end;
		y_end = y_start;
		y_start = temp_y;
	}
	int32_t sgn = ( 0 < (y_end - y_start) ? 1:-1);
	if (y_end == y_start){
		sgn = 0;
	}
	int32_t possible = 1;
	for( int32_t i = (x_end - x_start); i >= 0; i-- ){
		int32_t x,y;
		x = x_step + x_start;
		y = ((2*(y_end - y_start)*(x - x_start) + sgn*(x_end - x_start))/(2*(x_end - x_start))) + y_start;
		//test to draw the dotted line 
		if ((x+y)%2 == 1){
			if (1 != draw_dot(x,y)){
			possible=0;
			}
		}
		x_step++;
	}
	return possible;
}


/* 
 *  near_dotvertical
 *	 
 *	 
 *	
 *	
 * INPUTS: x_start,y_start -- the coordinates of the pixel at one end of the line
 * 	   x_end, y_end    -- the coordinates of the pixel at the other end
 * OUTPUTS: draws a dotline to all points in between the two given pixels including
 *          the end points
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
near_dotvertical(int32_t x_start, int32_t y_start, int32_t x_end, int32_t y_end){
	/* Your code goes here! */
	int32_t temp_x,temp_y,y_step=0;
	
	//to make y_start always less than y_end(so we make (x_start, y_start) be the (x1,y1))
	if ( y_start > y_end ){
		temp_x = x_end;
		x_end = x_start; 
		x_start = temp_x;
		temp_y = y_end;
		y_end = y_start;
		y_start = temp_y;
	}
	int32_t sgn = ( 0 < (x_end - x_start) ? 1:-1);
	if(x_start == x_end){
		sgn = 0;
		if(y_start == y_end){
			return draw_dot(x_start,y_start);
		}
	}
	int32_t possible = 1;
	for( int32_t i = (y_end - y_start); i >= 0; i-- ){
		int32_t x,y;
		y = y_step + y_start;
		x = ((2*(x_end - x_start)*(y - y_start) + sgn*(y_end - y_start))/(2*(y_end - y_start))) + x_start;
		//test to draw the dot
		if ((x+y)%2 == 1){
			if (1 != draw_dot(x,y)){
			possible=0;
			}
		}
		y_step++;
	}
	return possible;
}

/* 
 *  draw_line
 *	 
 *	 
 *	
 *	
 * INPUTS: x_start,y_start -- the coordinates of the pixel at one end of the line
 * 	   x_end, y_end    -- the coordinates of the pixel at the other end
 * OUTPUTS: draws a dotline to all points in between the two given pixels including
 *          the end points
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
draw_dotline(int32_t x_start, int32_t y_start, int32_t x_end, int32_t y_end){
	/* Your code goes here! */
	//just same as draw_line
	int32_t possible = 1;
	if (x_start == x_end){
		if(near_dotvertical(x_start,y_start,x_end,y_end) != 1){
			possible = 0;		
		} 
	}
	else if(0 == ((y_start - y_end)/(x_start - x_end)) || y_start - y_end == x_start - x_end || y_end - y_start == x_start - x_end){
		if(1 != near_dothorizontal(x_start,y_start,x_end,y_end)){
			possible = 0;
		}	
	}
	else{
		if(near_dotvertical(x_start,y_start,x_end,y_end) != 1){
			possible = 0;		
		} 
	}

	return possible;
}

/* 
 *  draw_dotparallelogram
 *	 
 *	 
 *	
 *	
 * INPUTS: x_A,y_A -- the coordinates of one of the vertices of the parallelogram
 *         x_B,y_B -- the coordinates of another of the vertices of the parallelogram
 *         x_C,y_C -- the coordinates of another of the vertices of the parallelogram
 * OUTPUTS: draws a dot shadow in the parallelogram
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
draw_dotparallelogram(int32_t x_A, int32_t y_A, int32_t x_B, int32_t y_B, int32_t x_C, int32_t y_C){
	/* Your code goes here! */
	//for different type of parallelogram use different scan x,y(just like the near_horizaontal and near_vertical)
	int32_t possible = 1,x,y;
	//call the vertical so use y to scan
	if (x_A == x_C){
		for(y= y_A; y_C >= y;y++ ){
			int32_t sgn = ( 0 < (x_C - x_A) ? 1:-1);
			if(x_A == x_C){
				sgn = 0;
			}
			x = ((2*(x_C - x_A)*(y - y_A) + sgn*(y_C - y_A))/(2*(y_C - y_A))) + x_A;
			if(1 != draw_dotline(x,y,x+x_B-x_A,y+y_B-y_A)){
				possible = 0;
			}
		} 
	}
	//if slope in [-1,1] use x
	else if(0 == ((y_A - y_C)/(x_A - x_C)) || y_A - y_C == x_A - x_C || y_C - y_A == x_A - x_C){
		for(x=x_A; x_C >= x; x++){
			int32_t sgn = ( 0 < (y_C - y_A) ? 1:-1);
			if (y_C == y_A){
				sgn = 0;
			}
			y = ((2*(y_C - y_A)*(x - x_A) + sgn*(x_C - x_A))/(2*(x_C - x_A))) + y_A;
			if(1 != draw_dotline(x,y,x+x_B-x_A,y+y_B-y_A)){
				possible = 0;
			}
		}
	}
	//use y to scan
	else{
		for(y= y_A; y_C >= y;y++ ){
			int32_t sgn = ( 0 < (x_C - x_A) ? 1:-1);
			if(x_A == x_C){
				sgn = 0;
			}
			x = ((2*(x_C - x_A)*(y - y_A) + sgn*(y_C - y_A))/(2*(y_C - y_A))) + x_A;
			if(1 != draw_dotline(x,y,x+x_B-x_A,y+y_B-y_A)){
				possible = 0;
			}
		} 
	}

	return possible;
}


/* 
 *  draw_picture
 *	 
 *	 
 *	
 *	
 * INPUTS: none
 * OUTPUTS: alters the image by calling any of the other functions in the file
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */


int32_t
draw_picture(){
	/* Your code goes here! */
//set the black background
set_color(0);
for (int32_t i=0;i<320;i++){
	draw_line(0,i,624,i);
}
//draw the special picture
set_color(0xFFFFFF);
draw_line(149,5,211,5);draw_line(211,5,205,17);draw_line(205,17,155,17);draw_line(155,17,162,28);
draw_line(162,28,199,28);draw_line(149,5,79,128);draw_line(155,17,91,128);draw_line(162,28,104,128);
draw_line(79,128,91,128);draw_line(125,207,79,128);draw_line(131,195,91,128);draw_line(137,184,104,128);
draw_line(131,195,137,184);draw_line(187,207,125,207);draw_line(181,195,131,195);draw_line(175,184,137,184);
draw_line(187,207,181,195);draw_line(200,185,187,207);draw_line(200,161,181,195);draw_line(188,161,175,184);
draw_line(200,161,188,161);draw_line(220,194,200,161);draw_line(213,205,200,185);draw_line(207,216,194,196);
draw_line(233,217,239,227);draw_line(227,228,233,238);draw_line(220,239,226,250);draw_line(239,227,233,238);
draw_line(239,227,278,227);draw_line(233,238,283,238);draw_line(226,250,290,250);draw_line(290,250,283,238);
draw_line(297,194,278,227);draw_line(309,194,283,238);draw_line(322,194,290,250);draw_line(309,194,297,194);
draw_line(297,194,239,95);draw_line(309,194,244,83);draw_line(322,194,251,72);draw_line(244,83,251,72);
draw_line(174,95,239,95);draw_line(168,83,244,83);draw_line(162,72,251,72);draw_line(168,83,174,95);
draw_line(174,95,156,128);draw_line(168,83,143,128);draw_line(162,72,130,128);draw_line(130,128,143,128);
draw_line(168,150,156,128);draw_line(161,161,143,128);draw_line(142,151,130,128);draw_line(168,150,161,161);
draw_line(111,139,135,139);draw_line(124,161,161,161);draw_line(142,151,118,151);
draw_line(85,139,72,139);draw_line(92,151,79,151);draw_line(98,161,85,161);draw_line(85,161,79,151);
draw_line(41,196,72,139);draw_line(53,196,79,151);draw_line(66,196,85,161);draw_line(41,196,53,196);
draw_line(41,196,72,251);draw_line(53,196,80,240);draw_line(66,196,86,229);draw_line(80,240,86,229);
draw_line(213,251,72,251);draw_line(207,240,80,240);draw_line(200,229,86,229);draw_line(213,251,207,240);
draw_line(213,251,258,172);draw_line(207,240,245,172);draw_line(200,229,233,172);draw_line(245,172,233,172);
draw_line(226,116,258,172);draw_line(220,127,245,172);draw_line(213,139,233,172);draw_line(226,116,220,127);
draw_line(226,116,201,116);draw_line(220,127,182,127);draw_line(213,139,189,139);draw_line(182,127,189,139);
draw_line(213,95,201,116);draw_line(200,95,182,127);draw_line(226,95,214,115);
draw_line(214,72,219,61);draw_line(226,72,232,61);draw_line(239,72,244,61);draw_line(219,61,232,61);
draw_line(199,28,219,61);draw_line(205,17,232,61);draw_line(211,5,244,61);
//draw the shadow
draw_dotparallelogram(156,17,162,28,96,120);
draw_dotparallelogram(91,128,97,118,130,194);
draw_dotparallelogram(168,85,174,95,143,128);
draw_dotparallelogram(143,128,148,118,161,160);
draw_dotparallelogram(79,152,85,162,53,196);
draw_dotparallelogram(58,204,66,195,79,240);
draw_dotparallelogram(201,162,207,172,181,195);
draw_dotparallelogram(201,184,207,172,213,205);
draw_dotparallelogram(227,228,233,217,233,238);
draw_dotparallelogram(220,127,226,116,245,172);
draw_dotparallelogram(252,160,257,172,208,239);
draw_dotparallelogram(245,84,251,72,309,194);
draw_dotparallelogram(316,183,322,193,283,238);
draw_dotparallelogram(205,16,211,6,238,72);
draw_dotparallelogram(232,61,244,61,226,71);
draw_dotparallelogram(212,96,225,95,201,115);
	return 0;
}
