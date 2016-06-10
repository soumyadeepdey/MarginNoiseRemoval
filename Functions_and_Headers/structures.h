
//~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*
// Program Name : marginal_noise_removal.cpp
// 
// Project :  		DRD
// Author : 		Soumyadeep Dey
// Creation Date : 	FEB 14  -2012.  Rights Reserved
//~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*




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
}nocc;


/*-----------------------------------------------------------------------------------------------------------------*/
