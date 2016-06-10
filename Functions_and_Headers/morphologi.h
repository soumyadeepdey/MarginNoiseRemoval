


//~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*
// Program Name : marginal_noise_removal.cpp
// 
// Project :  		DRD
// Author : 		Soumyadeep Dey
// Creation Date : 	FEB 14  -2012.  Rights Reserved
//~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*



#ifndef morphologi_H
#define morphologi_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/stat.h>
#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

#include "global_variables.h"
#include "structures.h"

using namespace std;

/* --------------------------------------------------------- MORPHOLOGICAL OPERATIONS------------------------------------------------------------------*/


/*-------------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------- DILATION WITH 4 NEIGHBOURHOOD---------------------------------------------------------------------------*/

int *dilation(int *image)
{
	int *(*im),*(*im1);
	int i,j;
	int *tempimage;
	tempimage=(int*)malloc(row*col*sizeof(int));
	im=(int *(*))malloc(row * sizeof(int *));
	for(i=0;i<row;i++)
	{
		im[i]=(int *)malloc(col * sizeof(int));
	}
	im1=(int *(*))malloc(row * sizeof(int *));
	for(i=0;i<row;i++)
	{
		im1[i]=(int *)malloc(col * sizeof(int));
	}
	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{
			im[i][j]=image[i*col+j];
			im1[i][j]=image[i*col+j];
		}
	}

	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{
			if(im[i][j]==0)
			{
				if(i-1>=0)
					im1[i-1][j]=0;
				if(i+1<row)
					im1[i+1][j]=0;
				if(j-1>=0)
					im1[i][j-1]=0;
				if(j+1<col)
					im1[i][j+1]=0;
			}
			else
				im1[i][j]=255;
		}
	}


	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{
			tempimage[i*col+j]=im1[i][j];
		}
	}

	free(im);
	free(im1);

	return (tempimage);
	free(tempimage);


	
}


/*-------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------EROTION WITH 4 NEIGHBOURHOOD-------------------------------------------------------------*/

int *erosion(int *image)
{
	int *(*im);
	int i,j;
	int *tempimage;
	tempimage=(int*)malloc(row*col*sizeof(int));
	im=(int *(*))malloc(row * sizeof(int *));
	for(i=0;i<row;i++)
	{
		im[i]=(int *)malloc(col * sizeof(int));
	}
	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{
			im[i][j]=image[i*col+j];
		}
	}
	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{
			if(im[i][j]==0)
			{
				if(i-1<0||i+1>=row||j-1<0||j+1>=col)
					tempimage[i*col+j]=255;
				else if(im[i-1][j]==0&&im[i+1][j]==0&&im[i][j-1]==0&&im[i][j+1]==0)
					tempimage[i*col+j]=0;
				else
					tempimage[i*col+j]=255;
			}
			else
				tempimage[i*col+j]=255;
		}
	}

	return (tempimage);
	free(tempimage);
	free(im);
		
}

/*-------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------- BOUNDARY EXTRACTION--------------------------------------------------*/

int *boundaryextraction(int *image)
{
	int *(*im);
	int i,j,k;
	im=(int *(*))malloc(row * sizeof(int *));
	for(i=0;i<row;i++)
	{
		im[i]=(int *)malloc(col * sizeof(int));
	}
	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{
			im[i][j]=image[i*col+j];
		}
	}
	int *erodedimage;
	int *extractedimage;
	extractedimage=(int*)malloc(row*col*sizeof(int));
	erodedimage=(int*)malloc(row*col*sizeof(int));
	erodedimage=erosion(image);
	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{
			if(im[i][j]==erodedimage[i*col+j])
				extractedimage[i*col+j]=255;
			else
				extractedimage[i*col+j]=0;
		}
	}


	
	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{
			im[i][j]=extractedimage[i*col+j];
		}
	}

	free(im);
	free(erodedimage);

	return(extractedimage);
	
	free(extractedimage);
	
}

/*-------------------------------------------------------------------------------------------------------------------------*/


#endif
