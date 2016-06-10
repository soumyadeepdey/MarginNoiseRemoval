

//~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*
// Program Name : marginal_noise_removal.cpp
// 
// Project :  		DRD
// Author : 		Soumyadeep Dey
// Creation Date : 	FEB 14  -2012.  Rights Reserved
//~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*



#ifndef smoothing_H
#define smoothing_H

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


/*-------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------SMOOTHING OPERATIONS----------------------------------------------------------------*/


/*----------------------------------------------------------HORIZONTAL SMOOTHING----------------------------------------------------------------*/

int *horizental_smoothing(int *(*image),int whitespace)
{
	int i,j,k;
	
	int *(*hsmoothedimage);
	hsmoothedimage=(int *(*))malloc(row * sizeof(int *));
	for(i=0;i<row;i++)
	{
		hsmoothedimage[i]=(int *)malloc(col * sizeof(int));
	}
	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{
			hsmoothedimage[i][j]=image[i][j];
		}
	}

	
	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{			
			if(image[i][j]==0)
			{
				for(k=j+1;k<(j+1+whitespace);k++)
				{
					if(k<col)
					{					
						hsmoothedimage[i][k]=0;
					}
					else 
						break;
					
				}
			}
		}
	}

	int *tempimage;
	tempimage=(int *)malloc(row*col*sizeof(int));
	for(i=0;i<row;i++)
	{	
		for(j=0;j<col;j++)
		{
			tempimage[i*col+j]=hsmoothedimage[i][j];
		}
	}
	



	free(hsmoothedimage);

	return (tempimage);
	
}


/*---------------------------------------------------------VERTICAL SMOOTHING----------------------------------------------------------------*/

int *vertical_smoothing(int *(*image),int whitespace)
{
	int i,j,k;
	
	int *(*vsmoothedimage);
	vsmoothedimage=(int *(*))malloc(row * sizeof(int *));
	for(i=0;i<row;i++)
	{
		vsmoothedimage[i]=(int *)malloc(col * sizeof(int));
	}
	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{
			vsmoothedimage[i][j]=image[i][j];
		}
	}

	
	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{			
			if(image[i][j]==0)
			{
				for(k=i+1;k<(i+1+whitespace);k++)
				{
					if(k<row)
					{					
						vsmoothedimage[k][j]=0;
					}
					else 
						break;
					
				}
			}
		}
	}

	int *tempimage;
	tempimage=(int *)malloc(row*col*sizeof(int));
	for(i=0;i<row;i++)
	{	
		for(j=0;j<col;j++)
		{
			tempimage[i*col+j]=vsmoothedimage[i][j];
		}
	}
	
	free(vsmoothedimage);
	return (tempimage);	
}

#endif