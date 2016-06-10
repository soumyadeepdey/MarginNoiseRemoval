
//~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*
// Program Name : marginal_noise_removal.cpp
// 
// Project :  		DRD
// Author : 		Soumyadeep Dey
// Creation Date : 	FEB 14  -2012.  Rights Reserved
//~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*




#ifndef structures_H
#define structures_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/stat.h>
#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;


using namespace std;
/*-------------------------------------------------------- Structure Declarations--------------------------------------------------------*/

typedef struct gapping
{
	int y1;
	int y2;
	int size;
	struct gapping *nxt;
}gp;

typedef struct imagestructure
{
	int x;
	int y;
	int label;
	int value;
	int mapped;
}is;


typedef struct connectedcomponent
{
	struct imagestructure im;
	struct connectedcomponent *nxt;
}cc;



typedef struct numberofcc
{
	struct connectedcomponent *start;
	struct connectedcomponent *last;
	int number_of_component;
	int xmin;
	int ymin;
	int xmax;
	int ymax;
	int centroidx;
	int centroidy;
	int hierarchy;
}nocc;


/*-----------------------------------------------------------------------------------------------------------------*/


#endif