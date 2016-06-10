
//~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*
// Program Name : marginal_noise_removal.cpp
// 
// Project :  		DRD
// Author : 		Soumyadeep Dey
// Creation Date : 	FEB 14  -2012.  Rights Reserved
//~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*

#ifndef con_comp_H
#define con_comp_H

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



int cclabeling(void);
void labelingdfs(int *k,int *l);

/*-------------------------------------------------------------------CONNECTED COMPONENT--------------------------------------------------------------------------------*/


void connectedcomponent(int *(*image))
{
	int i,j,label=1,*mapping,k;
	ncc=0;
	
	is *istemp;






	tempstructuredimage=(is *(*))malloc(row * sizeof(is *));
	for(i=0;i<row;i++)
	{
		tempstructuredimage[i]=(is *)malloc(col * sizeof(is));
	}
	
	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{
			tempstructuredimage[i][j].x=i;
			tempstructuredimage[i][j].y=j;
			tempstructuredimage[i][j].label=0;
			tempstructuredimage[i][j].value=image[i][j];
		}
	}



// LABELING BY DFS
	
	int noccbyla;
	noccbyla=(1-cclabeling());



// LABELING IS PERFECT

	mapping=NULL;
	k=0;
	int *tmp,count=1;
	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{
			if(tempstructuredimage[i][j].label!=0)
			{
				if(mapping!=NULL)
				{
					for(k=0;k<count-1;k++)
					{
						if(mapping[k]==tempstructuredimage[i][j].label)
						{
							tempstructuredimage[i][j].mapped=k;
							break;
						}		
					}
					if(k==count-1)
					{
						tmp=(int*)realloc(mapping,count*sizeof(int));
						if(tmp!=NULL)
						{
							mapping=tmp;
							mapping[count-1]=tempstructuredimage[i][j].label;
							tempstructuredimage[i][j].mapped=(count-1);
							count++;
						}
						else
						{
							printf("\nERROR IN REALLOCATING MAPPING ARREY\n");
							exit(1);
						}
					}// end of k==count
				}// end of if mapping !=null
				else
				{
					tmp=(int*)realloc(mapping,count*sizeof(int));
					mapping=tmp;
					mapping[count-1]=tempstructuredimage[i][j].label;
					tempstructuredimage[i][j].mapped=(count-1);
					count++;
				}
				
			}// end of tempstructuredimage[i][j].label!=0
		}
	}// end of image

	

// MAPPING IS PERFECR TILL NOW

	tncc=count-1;

// CREATING ARREY OF STRUCTURE POINTER  and help them to uniquely mapped

	cc *cctemp,*ccstart=NULL,*temp1;
	
	component=(nocc *)malloc((count-1)* sizeof(nocc));

	for(i=0;i<(count-1);i++)
	{
		component[i].start=NULL;
		component[i].number_of_component=0;
		component[i].last=NULL;
	}
	

	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{
			if(tempstructuredimage[i][j].label!=0)
			{
				
				if(component[tempstructuredimage[i][j].mapped].start==NULL)
				{
					
					if(tempstructuredimage[i][j].mapped<0||tempstructuredimage[i][j].mapped>=count-1)
						{
							printf("error\n");
							printf("%d\t%d\t%d\t%d",tempstructuredimage[i][j].mapped,tempstructuredimage[i][j].x,tempstructuredimage[i][j].y,tempstructuredimage[i][j].label);
							exit(1);
						}
				
					
					cctemp=(cc *)malloc(sizeof(cc));
					cctemp->im.x=i;
					cctemp->im.y=j;
					cctemp->im.label=tempstructuredimage[i][j].label;
					cctemp->im.mapped=tempstructuredimage[i][j].mapped;
					cctemp->im.value=tempstructuredimage[i][j].value;
					cctemp->nxt=NULL;
					ccstart=(cc *)malloc(sizeof(cc));
					ccstart=cctemp;
					component[tempstructuredimage[i][j].mapped].start=cctemp;
					component[tempstructuredimage[i][j].mapped].last=cctemp;
					component[tempstructuredimage[i][j].mapped].number_of_component=1;
					component[tempstructuredimage[i][j].mapped].xmin=i;
					component[tempstructuredimage[i][j].mapped].ymin=j;
					component[tempstructuredimage[i][j].mapped].xmax=i;
					component[tempstructuredimage[i][j].mapped].ymax=j;
					
				}//end of if  i.e. first component of the connected component
				else
				{
					
					cctemp=(cc *)malloc(sizeof(cc));
					cctemp->im.x=i;
					cctemp->im.y=j;
					cctemp->im.label=tempstructuredimage[i][j].label;
					cctemp->im.mapped=tempstructuredimage[i][j].mapped;
					cctemp->im.value=tempstructuredimage[i][j].value;
					cctemp->nxt=NULL;
					if(component[tempstructuredimage[i][j].mapped].last->nxt==NULL)
						component[tempstructuredimage[i][j].mapped].last->nxt=cctemp;
					else
						printf("ERROR\n");
					component[tempstructuredimage[i][j].mapped].last=cctemp;
					component[tempstructuredimage[i][j].mapped].number_of_component=(component[tempstructuredimage[i][j].mapped].number_of_component)+1;
					if(component[tempstructuredimage[i][j].mapped].xmin>i)
						component[tempstructuredimage[i][j].mapped].xmin=i;
					if(component[tempstructuredimage[i][j].mapped].ymin>j)
						component[tempstructuredimage[i][j].mapped].ymin=j;
					if(component[tempstructuredimage[i][j].mapped].xmax<i)
						component[tempstructuredimage[i][j].mapped].xmax=i;
					if(component[tempstructuredimage[i][j].mapped].ymax<j)
						component[tempstructuredimage[i][j].mapped].ymax=j;

				}// end of else i.e. not 1st component of connected component
			}// end of if label

			
	
		}// end of j

	
	
	}// end of i

	free(mapping);

}


// LABELING WITH DFS

int cclabeling(void)
{
	int label=1;
	int i,j,k,l;

	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{
	
			if(tempstructuredimage[i][j].label==0&&tempstructuredimage[i][j].value==0)
			{
			
				tempstructuredimage[i][j].label=label;
				labelingdfs(&i,&j);
				label=label+1;
		
			}
			
			
		}
	}
	return(label);
	
}

void labelingdfs(int *k,int *l)
{
	int i,j,m,n;
	i=*k;
	j=*l;


	


// 8 NEIGHBOURS

/*
	int n1x,n2x,n3x,n4x,n5x,n6x,n7x,n8x;
	int n1y,n2y,n3y,n4y,n5y,n6y,n7y,n8y;
	
	n1x=i-1;
	n1y=j-1;
	n2x=i-1;
	n2y=j;
	n3x=i-1;
	n3y=j+1;
	n4x=i;
	n4y=j-1;
	n5x=i;
	n5y=j+1;
	n6x=i+1;
	n6y=j-1;
	n7x=i+1;
	n7y=j;
	n8x=i+1;
	n8y=j+1;

*/

	
	
if(tempstructuredimage[i][j].value==0&&tempstructuredimage[i][j].label!=0)
{



	if(i-1>=0&&j-1>=0)
	{
		if(tempstructuredimage[i-1][j-1].value==0&&tempstructuredimage[i-1][j-1].label==0)
		{
			//printf("N1\n");
			tempstructuredimage[i-1][j-1].label=tempstructuredimage[i][j].label;
			m=i-1;
			n=j-1;
			labelingdfs(&m,&n);
		}
	}
	if(i-1>=0)
	{
		if(tempstructuredimage[i-1][j].value==0&&tempstructuredimage[i-1][j].label==0)
		{
			//printf("N2\n");
			tempstructuredimage[i-1][j].label=tempstructuredimage[i][j].label;
			m=i-1;
			n=j;
			labelingdfs(&m,&n);
		}
	}
	if(i-1>=0&&j+1<col)
	{
		if(tempstructuredimage[i-1][j+1].value==0&&tempstructuredimage[i-1][j+1].label==0)
		{
			//printf("N3\n");
			tempstructuredimage[i-1][j+1].label=tempstructuredimage[i][j].label;
			m=i-1;
			n=j+1;
			labelingdfs(&m,&n);
		}
	}
	if(j-1>=0)
	{
		if(tempstructuredimage[i][j-1].value==0&&tempstructuredimage[i][j-1].label==0)
		{
			//printf("N4\n");
			tempstructuredimage[i][j-1].label=tempstructuredimage[i][j].label;
			m=i;
			n=j-1;
			labelingdfs(&m,&n);
		}
	}
	if(j+1<col)
	{
		if(tempstructuredimage[i][j+1].value==0&&tempstructuredimage[i][j+1].label==0)
		{
			//printf("N5\n");
			tempstructuredimage[i][j+1].label=tempstructuredimage[i][j].label;
			m=i;
			n=j+1;
			labelingdfs(&m,&n);
		}
	}
	if(i+1<row&&j-1>=0)
	{
		if(tempstructuredimage[i+1][j-1].value==0&&tempstructuredimage[i+1][j-1].label==0)
		{
			//printf("N6\n");
			tempstructuredimage[i+1][j-1].label=tempstructuredimage[i][j].label;
			m=i+1;
			n=j-1;
			labelingdfs(&m,&n);
		}
	}
	if(i+1<row)
	{
		if(tempstructuredimage[i+1][j].value==0&&tempstructuredimage[i+1][j].label==0)
		{
			//printf("N7\n");
			tempstructuredimage[i+1][j].label=tempstructuredimage[i][j].label;
			m=i+1;
			n=j;
			labelingdfs(&m,&n);
		}
	}
	if(i+1<row&&j+1<col)
	{
		if(tempstructuredimage[i+1][j+1].value==0&&tempstructuredimage[i+1][j+1].label==0)
		{
			//printf("N8\n");
			tempstructuredimage[i+1][j+1].label=tempstructuredimage[i][j].label;
			m=i+1;
			n=j+1;
			labelingdfs(&m,&n);
		}

	}

}
	
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

#endif