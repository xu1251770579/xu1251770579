#include <stdlib.h>
#include <string.h>

#include "mp8.h"


/*
 * basicRecurse -- flood fill recursively from a point until reaching
 *                 white or the image border
 * INPUTS: width -- width of the input image
 *         height -- height of the input image
 *         inRed -- pointer to the input red channel (1-D array)
 *         inGreen -- pointer to the input green channel (1-D array)
 *         inBlue -- pointer to the input blue channel (1-D array)
 *         x -- current x position of the flood
 *         y -- current y position of the flood
 *         marking -- array used to mark seen positions (indexed
 *                    in the same way as the RGB input arrays)
 * OUTPUTS: marking -- marked with flooded pixels
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void 
basicRecurse (int32_t width, int32_t height,
	      const uint8_t* inRed, const uint8_t* inGreen, 
	      const uint8_t* inBlue, 
	      int32_t x, int32_t y, 
	      uint8_t* marking)
{
	//mark the marking point
	marking[x+y*width]=1;
	//check for four times and move to four different direction
	if( y-1>=0 &&( marking[x+(y-1)*width]!=1) && ((inRed[x+(y-1)*width]!=0xFF) || (inGreen[x+(y-1)*width]!=0xFF) || (inBlue[x+(y-1)*width]!=0xFF))){
		basicRecurse (width, height, inRed, inGreen, inBlue, x, y-1, marking);
	}
	if( x+1<width && ( marking[x+1+y*width]!=1) && ((inRed[x+1+y*width]!=0xFF) || (inGreen[x+1+y*width]!=0xFF) || (inBlue[x+1+y*width]!=0xFF))){
		basicRecurse (width, height, inRed, inGreen, inBlue, x+1, y, marking);
	}
	if( y+1<height && (marking[x+(y+1)*width]!=1) && ((inRed[x+(y+1)*width]!=0xFF) || (inGreen[x+(y+1)*width]!=0xFF) || (inBlue[x+(y+1)*width]!=0xFF))){
		basicRecurse (width, height, inRed, inGreen, inBlue, x, y+1, marking);
	}
	if( x-1>=0 && (marking[x-1+y*width]!=1) && ((inRed[x-1+y*width]!=0xFF) || (inGreen[x-1+y*width]!=0xFF) || (inBlue[x-1+y*width]!=0xFF))){
		basicRecurse (width, height, inRed, inGreen, inBlue, x-1, y, marking);
	}
}


/*
 * greyRecurse -- flood fill recursively from a point until reaching
 *                near-white pixels or the image border
 * INPUTS: width -- width of the input image
 *         height -- height of the input image
 *         inRed -- pointer to the input red channel (1-D array)
 *         inGreen -- pointer to the input green channel (1-D array)
 *         inBlue -- pointer to the input blue channel (1-D array)
 *         x -- current x position of the flood
 *         y -- current y position of the flood
 *         distSq -- maximum distance squared between white and boundary
 *                   pixel color
 *         marking -- array used to mark seen positions (indexed
 *                    in the same way as the RGB input arrays)
 * OUTPUTS: marking -- marked with flooded pixels
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void 
greyRecurse (int32_t width, int32_t height,
	     const uint8_t* inRed, const uint8_t* inGreen, 
	     const uint8_t* inBlue, 
	     int32_t x, int32_t y, uint32_t distSq, 
	     uint8_t* marking)
{
	//mark the marking point
	marking[x+y*width]=1;
	//check for four times and move to four different direction
	if( y-1>=0 && (marking[x+(y-1)*width]!=1) && colorsWithinDistSq (0xFF, 0xFF, 0xFF,inRed[x+(y-1)*width], inGreen[x+(y-1)*width], inBlue[x+(y-1)*width], distSq)==0){
		greyRecurse (width, height, inRed, inGreen, inBlue, x, y-1, distSq, marking);
	}
	if( x+1<width &&(marking[x+1+y*width]!=1) && colorsWithinDistSq (0xFF, 0xFF, 0xFF,inRed[x+1+y*width], inGreen[x+1+y*width], inBlue[x+1+y*width], distSq)==0){
		greyRecurse (width, height, inRed, inGreen, inBlue, x+1, y, distSq, marking);
	}
	if( y+1<height && (marking[x+(y+1)*width]!=1) && colorsWithinDistSq (0xFF, 0xFF, 0xFF,inRed[x+(y+1)*width], inGreen[x+(y+1)*width], inBlue[x+(y+1)*width], distSq)==0){
		greyRecurse (width, height, inRed, inGreen, inBlue, x, y+1, distSq, marking);
	}
	if( x-1>=0 && (marking[x-1+y*width]!=1) && colorsWithinDistSq (0xFF, 0xFF, 0xFF,inRed[x-1+y*width], inGreen[x-1+y*width], inBlue[x-1+y*width], distSq)==0){
		greyRecurse (width, height, inRed, inGreen, inBlue, x-1, y, distSq, marking);
	}
}


/*
 * limitedRecurse -- flood fill recursively from a point until reaching
 *                   pixels too different (in RGB color) from the color at
 *                   the flood start point, too far away (> 35 pixels), or
 *                   beyond the image border
 * INPUTS: width -- width of the input image
 *         height -- height of the input image
 *         inRed -- pointer to the input red channel (1-D array)
 *         inGreen -- pointer to the input green channel (1-D array)
 *         inBlue -- pointer to the input blue channel (1-D array)
 *         origX -- starting x position of the flood
 *         origY -- starting y position of the flood
 *         x -- current x position of the flood
 *         y -- current y position of the flood
 *         distSq -- maximum distance squared between pixel at origin 
 *                   and boundary pixel color
 *         marking -- array used to mark seen positions (indexed
 *                    in the same way as the RGB input arrays)
 * OUTPUTS: marking -- marked with flooded pixels
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void 
limitedRecurse (int32_t width, int32_t height,
	        const uint8_t* inRed, const uint8_t* inGreen, 
	        const uint8_t* inBlue, 
	        int32_t origX, int32_t origY, int32_t x, int32_t y, 
		uint32_t distSq, uint8_t* marking)
{
	//mark the marking point
	marking[x+y*width]=1;
	//check for four times and move to four different direction
	if( y-1>=0 && (marking[x+(y-1)*width]!=1) && colorsWithinDistSq (inRed[origX+origY*width], inGreen[origX+origY*width], inBlue[origX+origY*width],inRed[x+(y-1)*width], inGreen[x+(y-1)*width], 		   inBlue[x+(y-1)*width], distSq)==1 && (origX-x)*(origX-x) +(origY-(y-1))*(origY-(y-1))<= 35*35){
		limitedRecurse (width, height, inRed, inGreen, inBlue, origX, origY, x, y-1, distSq, marking);
	}
	if( x+1<width &&(marking[x+1+y*width]!=1) && colorsWithinDistSq (inRed[origX+origY*width], inGreen[origX+origY*width], inBlue[origX+origY*width],inRed[x+1+y*width], inGreen[x+1+y*width], 		   inBlue[x+1+y*width], distSq)==1 && (origX-(x+1))*(origX-(x+1)) +(origY-y)*(origY-y)<= 35*35){
		limitedRecurse (width, height, inRed, inGreen, inBlue, origX, origY, x+1, y, distSq, marking);
	}
	if( y+1<height && (marking[x+(y+1)*width]!=1) && colorsWithinDistSq (inRed[origX+origY*width], inGreen[origX+origY*width], inBlue[origX+origY*width],inRed[x+(y+1)*width], inGreen[x+ 		   (y+1)*width], inBlue[x+(y+1)*width], distSq)==1 && (origX-x)*(origX-x) +(origY-(y+1))*(origY-(y+1))<= 35*35){
		limitedRecurse (width, height, inRed, inGreen, inBlue, origX, origY, x, y+1, distSq, marking);
	}
	if( x-1>=0 && (marking[x-1+y*width]!=1) && colorsWithinDistSq (inRed[origX+origY*width], inGreen[origX+origY*width], inBlue[origX+origY*width],inRed[x-1+y*width], inGreen[x-1+y*width], 	   inBlue[x-1+y*width], distSq)==1 && (origX-(x-1))*(origX-(x-1)) +(origY-y)*(origY-y)<= 35*35){
		limitedRecurse (width, height, inRed, inGreen, inBlue, origX, origY, x-1, y, distSq, marking);
	}
}

