/*									tab:8
 *
 * mp6.c - implementation file for ECE220 "Blocky" game
 *
 * "Copyright (c) 2017-2020 by Steven S. Lumetta."
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
 * Author:	    Steve Lumetta
 * Version:	    6
 * Creation Date:   7 November 2017
 * Filename:	    mp6.c
 * History:
 *	SL	1	7 November 2017
 *		First written.
 *      JXG     2	8 November 2017
 *		Added ncurses support.
 *	SL	3	30 November 2017
 *		Polished for MP distribution.
 *	SL	4	18 February 2018
 *		Edited slightly for ECE220H S18 distribution.
 *	SL	5	6 October 2020
 *		Edited slightly for ECE220 F20 distribution.
 *	SL	6	6 October 2020
 *		Stripped down from gold version.
 */


#include <stdint.h>
#include <stdio.h>

#include "mp6.h"


/* 
 * The maximum number of blocks in one piece.  Do not change for
 * this assignment (you can do as you like with the code afterward).
 */
#define MAX_PIECE_SIZE 4

/*
 * Pieces are defined by an array of offsets from the center point.
 * Each piece consists of a set of adjacent blocks that together form
 * the piece.  The first index of the array is the piece type, the 
 * second is the orientation (0-3), the third is block index within 
 * the piece (0 to MAX_PIECE_SIZE - 1), and the fourth gives the 
 * x (0) and y (1) offsets of the block relative to the center point.
 * Note that the first block is always the center point of the piece.
 * 
 * Offsets may be replicated if a piece contains fewer than MAX_PIECE_SIZE
 * blocks.
 *
 * Clockwise rotation changes the orientation in the positive (mod 4)
 * direction, and counter-clockwise rotation changes the orientation in
 * the negative (mod 4) direction.
 */
static const int32_t piece_def[NUM_PIECE_TYPES][4][MAX_PIECE_SIZE][2] = {
    {   /* PIECE_TYPE_ELL */
	{{0, 0}, { 1,  0}, { 2,  0}, { 0,  1}},
	{{0, 0}, { 0,  1}, { 0,  2}, {-1,  0}},
	{{0, 0}, {-1,  0}, {-2,  0}, { 0, -1}},
	{{0, 0}, { 0, -1}, { 0, -2}, { 1,  0}}
    },
    {   /* PIECE_TYPE_REV_ELL */
	{{0, 0}, { 1,  0}, { 2,  0}, { 0, -1}},
	{{0, 0}, { 0,  1}, { 0,  2}, { 1,  0}},
	{{0, 0}, {-1,  0}, {-2,  0}, { 0,  1}},
	{{0, 0}, { 0, -1}, { 0, -2}, {-1,  0}}
    },
    {   /* PIECE_TYPE_LINE */
	{{0, 0}, {-1,  0}, { 1,  0}, { 2,  0}},
	{{0, 0}, { 0, -1}, { 0,  1}, { 0,  2}},
	{{0, 0}, { 1,  0}, {-1,  0}, {-2,  0}},
	{{0, 0}, { 0,  1}, { 0, -1}, { 0, -2}}
    },
    {   /* PIECE_TYPE_TEE */
	{{0, 0}, {-1,  0}, { 0, -1}, { 1,  0}},
	{{0, 0}, { 0, -1}, { 1,  0}, { 0,  1}},
	{{0, 0}, { 1,  0}, { 0,  1}, {-1,  0}},
	{{0, 0}, { 0,  1}, {-1,  0}, { 0, -1}}
    },
    {   /* PIECE_TYPE_ESS */
	{{0, 0}, { 0, -1}, { 1,  0}, { 1,  1}},
	{{0, 0}, { 1,  0}, { 0,  1}, {-1,  1}},
	{{0, 0}, { 0,  1}, {-1,  0}, {-1, -1}},
	{{0, 0}, {-1,  0}, { 0, -1}, { 1, -1}}
    },
    {   /* PIECE_TYPE_REV_ESS */
	{{0, 0}, { 0, -1}, {-1,  0}, {-1,  1}},
	{{0, 0}, { 1,  0}, { 0, -1}, {-1, -1}},
	{{0, 0}, { 0,  1}, { 1,  0}, { 1, -1}},
	{{0, 0}, {-1,  0}, { 0,  1}, { 1,  1}}
    }
};


/* 
 * empty_board -- fills a board b with empty spaces (SPACE_EMPTY)
 * INPUTS: b -- the board
 * OUTPUTS: b -- the board, filled with empty spaces
 * RETURN VALUE: 1 on success, or 0 on failure
 * SIDE EFFECTS: none
 */
int32_t
empty_board (space_type_t b[BOARD_HEIGHT][BOARD_WIDTH])
{
    //scan for all index to clear the board,i is height,j is width
    for(int32_t i = 0; i< BOARD_HEIGHT ;i++){
        for(int32_t j = 0;j<BOARD_WIDTH ;j++){
            b[i][j] = SPACE_EMPTY;
        }
    }
    //return 1 on sucess
    return 1;
}


/* 
 * mark_piece -- forcibly writes a piece into a board, ignoring the 
 *               board's current contents; you will need to understand
 *               the piece definition array (piece_def) in order to
 *               implement this function
 * INPUTS: b -- the board
 *         p -- the type of piece
 *         orient -- the orientation of the piece
 *         x -- horizontal coordinate of the piece's center point
 *         y -- vertical coordinate of the piece's center point
 *         v -- type of space to write (in the shape of the piece)
 * OUTPUTS: b -- the board, with the piece filled in with value v
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void
mark_piece (space_type_t b[BOARD_HEIGHT][BOARD_WIDTH], 
            piece_type_t p, int32_t orient, int32_t x, int32_t y,
	    space_type_t v)
{
    //scan for PIECE_SIZE i
    for (int32_t i=0; i < MAX_PIECE_SIZE;i++){
        b[piece_def[p][orient][i][1]+y][piece_def[p][orient][i][0]+x] = v;
    }
}


/* 
 * test_piece_fit -- checks whether a piece fits into a board; you will 
 *		     need to understand the piece definition array 
 *		     (piece_def) in order to implement this function
 * INPUTS: b -- the board
 *         p -- the type of piece
 *         orient -- the orientation of the piece
 *         x -- horizontal coordinate of the piece's center point
 *         y -- vertical coordinate of the piece's center point
 * OUTPUTS: none
 * RETURN VALUE: if the piece does not fit within the board, returns 
 *		 FIT_OUT_OF_BOARD; otherwise, if the piece would overwrite 
 *		 non-empty board spaces, returns FIT_NO_ROOM_THERE; 
 *		 otherwise, returns FIT_SUCCESS
 * SIDE EFFECTS: none
 */
fit_result_t
test_piece_fit (space_type_t b[BOARD_HEIGHT][BOARD_WIDTH], 
                piece_type_t p, int32_t orient, int32_t x, int32_t y)
{   
    //scan for piece size
    for (int32_t i = 0; i < MAX_PIECE_SIZE;i++){
        //test if piece out of range
        if(piece_def[p][orient][i][0]+x>=BOARD_WIDTH || piece_def[p][orient][i][0]+x<0 || piece_def[p][orient][i][1]+y>=BOARD_HEIGHT || piece_def[p][orient][i][1]+y<0){
            return FIT_OUT_OF_BOARD;
        }
        //test if the room is empty 
        if(b[piece_def[p][orient][i][1]+y][piece_def[p][orient][i][0]+x] != SPACE_EMPTY){
            return FIT_NO_ROOM_THERE;
        }
    }
    return FIT_SUCCESS;
}


/* 
 * print_board -- prints a board to the screen using period ('.') to 
 *		  represent empty spaces, percent ('%') to represent 
 *		  old blocks, and asterisk ('*') to represent the current 
 *		  piece (SPACE_BLOCK); see specification for example
 * 		  of board appearance
 * INPUTS: b -- the board
 * OUTPUTS: none
 * RETURN VALUE: none
 * SIDE EFFECTS: prints to the monitor
 */
void
print_board (space_type_t b[BOARD_HEIGHT][BOARD_WIDTH])
{
    //scan for height and width to write board
    for(int32_t i = 0; i< BOARD_HEIGHT ;i++){
        for(int32_t j = 0;j<BOARD_WIDTH ;j++){
            switch (b[i][j]){
                case SPACE_EMPTY: printf(".");
                break;
                case SPACE_FULL: printf("%%");
                break;
                case SPACE_BLOCK: printf("*");
                break;
                default: break;
            }
        }
        // when terminat the line write "\n"
        printf("\n");
    }
}


/* 
 * try_to_move_down -- tries to move the current piece down one space in 
 *                     the board; assumes that the piece is in the board
 *		       and must thus be removed before it can be put into
 *                     its new position (or back into its old position
 *                     on failure)
 * INPUTS: b -- the board
 *         p -- the type of piece
 *         orient -- the current orientation of the piece
 *         x -- current horizontal coordinate of the piece's center point
 *         y -- current vertical coordinate of the piece's center point
 * OUTPUTS: b -- the board, with the piece moved down (on success), or
 *               unchanged on failure
 * RETURN VALUE: 1 on success, 0 on failure
 * SIDE EFFECTS: none
 */
int32_t
try_to_move_down (space_type_t b[BOARD_HEIGHT][BOARD_WIDTH], 
                  piece_type_t p, int32_t orient, int32_t x, int32_t y)
{   
    //first remove the piece
    remove_piece(b,p,orient,x,y);
    //test if the piece fit,if fit move it and return 1
    if (FIT_SUCCESS==test_piece_fit(b,p,orient,x,y+1)){
        add_piece(b,p,orient,x,y+1);
        return 1;
    }
    else{
        //not fit put it back and return 0
        add_piece(b,p,orient,x,y);
        return 0;
    }
}


/* 
 * try_to_move_left -- tries to move the current piece one space left in
 *                     the board; assumes that the piece is in the board
 *		       and must thus be removed before it can be put into
 *                     its new position (or back into its old position
 *                     on failure)
 * INPUTS: b -- the board
 *         p -- the type of piece
 *         orient -- the current orientation of the piece
 *         x -- current horizontal coordinate of the piece's center point
 *         y -- current vertical coordinate of the piece's center point
 * OUTPUTS: b -- the board, with the piece moved left (on success), or
 *               unchanged on failure
 * RETURN VALUE: 1 on success, 0 on failure
 * SIDE EFFECTS: none
 */
int32_t
try_to_move_left (space_type_t b[BOARD_HEIGHT][BOARD_WIDTH], 
                  piece_type_t p, int32_t orient, int32_t x, int32_t y)
{
    //first remove the piece
    remove_piece(b,p,orient,x,y);
    //test if the piece fit,if fit move it and return 1
    if (FIT_SUCCESS == test_piece_fit(b,p,orient,x-1,y)){
        add_piece(b,p,orient,x-1,y);
        return 1;
    }
    else{
        //not fit put it back and return 0
        add_piece(b,p,orient,x,y);
        return 0;
    }
}


/* 
 * try_to_move_right -- tries to move the current piece one space right in
 *                     the board; assumes that the piece is in the board
 *		       and must thus be removed before it can be put into
 *                     its new position (or back into its old position
 *                     on failure)
 * INPUTS: b -- the board
 *         p -- the type of piece
 *         orient -- the current orientation of the piece
 *         x -- current horizontal coordinate of the piece's center point
 *         y -- current vertical coordinate of the piece's center point
 * OUTPUTS: b -- the board, with the piece moved right (on success), or
 *               unchanged on failure
 * RETURN VALUE: 1 on success, 0 on failure
 * SIDE EFFECTS: none
 */
int32_t
try_to_move_right (space_type_t b[BOARD_HEIGHT][BOARD_WIDTH], 
                   piece_type_t p, int32_t orient, int32_t x, int32_t y)
{
    //first remove the piece
    remove_piece(b,p,orient,x,y);
    //test if the piece fit,if fit move it and return 1
    if (FIT_SUCCESS == test_piece_fit(b,p,orient,x+1,y)){
        add_piece(b,p,orient,x+1,y);
        return 1;
    }
    else{
        //not fit put it back and return 0
        add_piece(b,p,orient,x,y);
        return 0;
    }
}


/* 
 * try_to_rotate_clockwise -- 
 *	tries to rotate the current piece clockwise in the board; assumes 
 *	that the piece is in the board and must thus be removed before it 
 *	can be put into its new position (or back into its old position on
 *	failure)
 * INPUTS: b -- the board
 *         p -- the type of piece
 *         orient -- the current orientation of the piece
 *         x -- current horizontal coordinate of the piece's center point
 *         y -- current vertical coordinate of the piece's center point
 * OUTPUTS: b -- the board, with the piece rotated clockwise (on success), 
 *		 or unchanged on failure
 * RETURN VALUE: 1 on success, 0 on failure
 * SIDE EFFECTS: none
 */
int32_t
try_to_rotate_clockwise (space_type_t b[BOARD_HEIGHT][BOARD_WIDTH], 
                         piece_type_t p, int32_t orient, int32_t x, int32_t y)
{
    //first remove the piece
    remove_piece(b,p,orient,x,y);
    int32_t orient_incre;
    //increment the orient and if orient == 3 then orient back to 0
    if(orient == 3){
        orient_incre = 0;
    }else{
        orient_incre = orient + 1;
    }
    //test if the piece fit,if fit rotate it and return 1
    if (FIT_SUCCESS == test_piece_fit(b,p,orient_incre,x,y)){
        add_piece(b,p,orient_incre,x,y);
        return 1;
    }
    else{
        //not fit put it back and return 0
        add_piece(b,p,orient,x,y);
        return 0;
    }
}


/* 
 * try_to_rotate_cc -- 
 *	tries to rotate the current piece counter-clockwise in the board; 
 *	assumes that the piece is in the board and must thus be removed 
 *	before it can be put into its new position (or back into its old 
 *	position on failure)
 * INPUTS: b -- the board
 *         p -- the type of piece
 *         orient -- the current orientation of the piece
 *         x -- current horizontal coordinate of the piece's center point
 *         y -- current vertical coordinate of the piece's center point
 * OUTPUTS: b -- the board, with the piece rotated counter-clockwise (on 
 *		 success), or unchanged on failure
 * RETURN VALUE: 1 on success, 0 on failure
 * SIDE EFFECTS: none
 */
int32_t
try_to_rotate_cc (space_type_t b[BOARD_HEIGHT][BOARD_WIDTH], 
                  piece_type_t p, int32_t orient, int32_t x, int32_t y)
{
    //first remove the piece
    remove_piece(b,p,orient,x,y);
    int32_t orient_decre;
    //decrement the orient and if orient == 0 then orient move to 3
    if(orient == 0){
        orient_decre = 3;
    }else{
        orient_decre = orient +-1;
    }
    remove_piece(b,p,orient,x,y);
    //test if the piece fit,if fit rotate it and return 1
    if (FIT_SUCCESS == test_piece_fit(b,p,orient_decre,x,y)){
        add_piece(b,p,orient_decre,x,y);
        return 1;
    }
    else{
        //not fit put it back and return 0
        add_piece(b,p,orient,x,y);
        return 0;
    }
}


/* 
 * is_row_full -- checks whether a row in a board is filled with SPACE_FULL 
 * INPUTS: b -- the board
 *	   row -- which row to check
 * OUTPUTS: none
 * RETURN VALUE: 1 if the specified row is full, or 0 otherwise
 * SIDE EFFECTS: none
 */
int32_t
is_row_full (space_type_t b[BOARD_HEIGHT][BOARD_WIDTH], int row)
{
    //test if the line is full 
    for(int32_t i=0; i < BOARD_WIDTH;i++){
        if(b[row][i] != SPACE_FULL){
            //not full return 0
            return 0;
        }
    }
    // full return 1
    return 1;
}


/* 
 * remove_full_rows -- removes all rows filled with SPACE_FULL from a 
 *                     board, moving rows above them downward and adding 
 *                     new empty rows at the top of the board
 * INPUTS: b -- the board
 * OUTPUTS: b -- the board, with full rows removed
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void
remove_full_rows (space_type_t b[BOARD_HEIGHT][BOARD_WIDTH])
{
    //check for all row if the row is full 
    for(int32_t i = 0; i< BOARD_HEIGHT; i++){
        if(1 == is_row_full ( b,i)){
            //copy the row above to move downward
            for( int32_t k=0; k < i;k++){
                for(int32_t j =0; j< BOARD_WIDTH;j++){
                    b[i-k][j] = b[i-k-1][j];
                }
            }
            //add one empty line on the top
            for(int32_t x = 0;x<BOARD_WIDTH;x++){
                b[0][x]=SPACE_EMPTY;
            }
        }
    }

}

