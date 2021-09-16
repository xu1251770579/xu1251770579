#include <stdio.h>
#include <stdlib.h> 
#include "mp4.h"
// return 0 if the regions not fit in width, return 1 if the regions fit
int32_t print_row (int32_t width, int32_t r1, int32_t r2, int32_t r3, int32_t r4, int32_t r5) {
    int32_t ini_lenth=0, R1, R2, R3, R4, R5;
//to get the least lenth of r1+r2+r3+r4+r5+interval of two non zero r -1
    if ( r1 != 0 ) { ini_lenth += (r1 + 1); }
    if ( r2 != 0 ) { ini_lenth += (r2 + 1); }
    if ( r3 != 0 ) { ini_lenth += (r3 + 1); }
    if ( r4 != 0 ) { ini_lenth += (r4 + 1); }
    if ( r5 != 0 ) { ini_lenth += (r5 + 1); }
    ini_lenth = ini_lenth - 1;
//R1-5 is the number of asterisk
    R1 = r1;
    R2 = r2;
    R3 = r3;
    R4 = r4;
    R5 = r5;
//return 0 if width is too small to put r1-5 in
    if (ini_lenth > width) {
        return 0;
    }
//the number of asterisk = r - (width -ini_lenth) and it must be at least zero
    for ( int32_t i = ini_lenth; i < width; i++) {
        if ( R1 != 0 ) { R1 -= 1; }
        if ( R2 != 0 ) { R2 -= 1; }
        if ( R3 != 0 ) { R3 -= 1; }
        if ( R4 != 0 ) { R4 -= 1; }
        if ( R5 != 0 ) { R5 -= 1; }
        }
//before r2-r4 there are a "-" before every part
        if ( r2 != 0 ) { r2 += 1; }
        if ( r3 != 0 ) { r3 += 1; }
        if ( r4 != 0 ) { r4 += 1; }
        if ( r5 != 0 ) { r5 += 1; }
// print "-" in r-R times,print "*" in R times
    for (int32_t i = 0 ; i < r1 - R1; i++ ){
        printf("-");
    }
    for (int32_t i = 0 ; i < R1 ; i++ ){
        printf("*");
    }
    for (int32_t i = 0 ; i < r2 - R2; i++ ){
        printf("-");
    }
    for (int32_t i = 0 ; i < R2; i++ ){
        printf("*");
    }
    for (int32_t i = 0 ; i < r3 - R3; i++ ){
        printf("-");
    }
    for (int32_t i = 0 ; i < R3; i++ ){
        printf("*");
    }
    for (int32_t i = 0 ; i < r4 - R4; i++ ){
        printf("-");
    }
    for (int32_t i = 0 ; i < R4; i++ ){
        printf("*");
    }
    for (int32_t i = 0 ; i < r5 - R5; i++ ){
        printf("-");
    }
    for (int32_t i = 0 ; i < R5; i++ ){
        printf("*");
    }
    for (int32_t i = 0 ; i < width - ini_lenth; i++ ){
        printf("-");
    }
    putchar(10);
    return 1;
}
