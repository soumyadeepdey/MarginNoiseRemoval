
//~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*
// Program Name : marginal_noise_removal.cpp
// 
// Project :  		DRD
// Author : 		Soumyadeep Dey
// Creation Date : 	FEB 14  -2012.  Rights Reserved
//~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*

#ifndef noiserem_H
#define noiserem_H


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/stat.h>
#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

using namespace std;

#include "global_variables.h"
#include "structures.h"
#include "writeimage.h"
#include "smoothing.h"



/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
	/*-------------------------------------------------------------------------NOISE REMOVAL----------------------------------------------------------------*/


	
void noise_removal()
{


	int i,j;
	char *name;
	
	ncco=tncc; // TOTAL NUMBER OF CONNECTED COMPONENT IN ORIGINAL IMAGE 

/*------------------------------------------------------------------------------------------------*/


	// CONNECTED COMPONENT OF ORIGINAL IMAGE

	originalcomponent=(nocc *)malloc(ncco* sizeof(nocc));
	nocc *textpart; // variable which have the textpart image
	textpart=(nocc *)malloc(ncco* sizeof(nocc));
	
	for(i=0;i<ncco;i++)
	{
		originalcomponent[i].start=component[i].start;
		originalcomponent[i].last=component[i].last;
		originalcomponent[i].number_of_component=component[i].number_of_component;
		originalcomponent[i].xmin=component[i].xmin;
		originalcomponent[i].ymin=component[i].ymin;
		originalcomponent[i].xmax=component[i].xmax;
		originalcomponent[i].ymax=component[i].ymax;
	}

/*----------------------------------------------------------------------------------------------------------*/

	// FOR TEXT PART

	for(i=0;i<ncco;i++)
	{
		if(component[i].number_of_component < graphicsthreashold )
		{
			textpart[i].start=component[i].start;
			textpart[i].last=component[i].last;
			textpart[i].number_of_component=component[i].number_of_component;
			textpart[i].xmin=component[i].xmin;
			textpart[i].ymin=component[i].ymin;
			textpart[i].xmax=component[i].xmax;
			textpart[i].ymax=component[i].ymax;
			textpart[i].centroidx=((textpart[i].xmax-textpart[i].xmin)/2)+textpart[i].xmin;
			textpart[i].centroidy=((textpart[i].ymax-textpart[i].ymin)/2)+textpart[i].ymin;
		}
		else
		{
			textpart[i].start=NULL;
		}
	}

	

/*-----------------------------------------------------------------------------------------------------------------*/
	

// GRAPHICS COMPONENT CREATION

	nocc *graphics; // variable that have the graphis

	int ng=0; // NUMBER OF GRAPHICS COMPONENT
	
	for(i=0;i<tncc;i++)
	{
		if(originalcomponent[i].number_of_component >= graphicsthreashold )
		{
			ng=ng+1;
			//component[i].start=NULL;

		}
	}
	

	int tng;//TOTAL NUMBER OF GRAPHICS COMPONENT

	tng=ng;
	graphics=(nocc *)malloc(tng * sizeof(nocc));

	j=0;
	for(i=0;i<tncc;i++)
	{
		if(originalcomponent[i].number_of_component >= graphicsthreashold )
		{
			graphics[j].start=originalcomponent[i].start;
			graphics[j].last=originalcomponent[i].last;
			graphics[j].number_of_component=originalcomponent[i].number_of_component;
			graphics[j].xmin=originalcomponent[i].xmin;
			graphics[j].ymin=originalcomponent[i].ymin;
			graphics[j].xmax=originalcomponent[i].xmax;
			graphics[j].ymax=originalcomponent[i].ymax;
			graphics[j].centroidx=((graphics[j].xmax-graphics[j].xmin)/2)+graphics[j].xmin;
			graphics[j].centroidy=((graphics[j].ymax-graphics[j].ymin)/2)+graphics[j].ymin;
			j=j+1;
		}
	}
	j=0;
	
	nocc *rgraphics;
	
	rgraphics=(nocc *)malloc(tng * sizeof(nocc));

	for(i=0;i<tng;i++)
	{
			rgraphics[i].start=graphics[i].start;
			rgraphics[i].xmin=graphics[i].xmin;
			rgraphics[i].ymin=graphics[i].ymin;
			rgraphics[i].xmax=graphics[i].xmax;
			rgraphics[i].ymax=graphics[i].ymax;
			rgraphics[i].centroidx=graphics[i].centroidx;
			rgraphics[i].centroidy=graphics[i].centroidy;
			rgraphics[i].number_of_component=graphics[i].number_of_component;
			rgraphics[i].last=graphics[i].last;							
	}


	


	for(i=0;i<tng;i++)
	{
		if(rgraphics[i].start!=NULL)
		{	
			for(j=0;j<tng;j++)
			{
			
				if(graphics[j].xmin>=graphics[i].xmin && graphics[j].xmax<=graphics[i].xmax && graphics[j].ymin>=graphics[i].ymin && graphics	[j].ymax<=graphics[i].ymax && i!=j)
				{
					rgraphics[i].start=graphics[i].start;
					rgraphics[i].xmin=graphics[i].xmin;
					rgraphics[i].ymin=graphics[i].ymin;
					rgraphics[i].xmax=graphics[i].xmax;
					rgraphics[i].ymax=graphics[i].ymax;
					rgraphics[i].centroidx=graphics[i].centroidx;
					rgraphics[i].centroidy=graphics[i].centroidy;
					rgraphics[i].number_of_component=graphics[i].number_of_component+graphics[j].number_of_component;
					rgraphics[i].last=graphics[i].last;
					rgraphics[i].last->nxt=graphics[j].start;
					rgraphics[i].last=graphics[j].last;
					
				}
			}
		}
	}
	

	for(i=0;i<tng;i++)
	{
		if(rgraphics[i].start!=NULL)
		{
			if(rgraphics[i].xmax-rgraphics[i].xmin>0.3*row && rgraphics[i].ymax-rgraphics[i].ymin<0.1*col)
				rgraphics[i].start=NULL;
			if(rgraphics[i].xmax-rgraphics[i].xmin<0.1*row && rgraphics[i].ymax-rgraphics[i].ymin>0.8*col)
				rgraphics[i].start=NULL;
		}
	}


	for(i=0;i<tng;i++)
	{
		if(rgraphics[i].start!=NULL)
		{
			for(j=0;j<ncco;j++)
			{
				if(textpart[j].xmin>rgraphics[i].xmin && textpart[j].ymin>rgraphics[i].ymin && textpart[j].xmax<rgraphics[i].xmax && textpart[j].ymax<rgraphics[i].ymax && textpart[j].start!=NULL)
				{	
					rgraphics[i].last->nxt=textpart[j].start;
					rgraphics[i].last=textpart[j].last;
					rgraphics[i].number_of_component=rgraphics[i].number_of_component+textpart[j].number_of_component;
					textpart[j].start=NULL;					
				}
			}
		}
	}

/*-----------------------------------------------------------------------------------------------------------*/

	int m,n;

/*-------------------------------------------------------------------------------------------------------------------------*/


	// graphics part

	
	cc *tempg;
	tempg=(cc *)malloc(sizeof(cc));


// creating new image with graphics part

	int *temprgimage;
	temprgimage=(int *)malloc(row*col*sizeof(int));
	for(i=0;i<row*col;i++)
		temprgimage[i]=255;

	for(i=0;i<tng;i++)
	{
		if(rgraphics[i].start!=NULL) // logic part
		{
			tempg=(cc *)malloc(sizeof(cc));
			tempg=rgraphics[i].start;
			while(tempg!=NULL)
			{
				m=tempg->im.x;
				n=tempg->im.y;
				temprgimage[m*col+n]=tempg->im.value;
				tempg=tempg->nxt;
			}
		}
	}


/*-------------------------------------------------------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------*/


// creating new image with textpart


	
	cc *temp;
	temp=(cc *)malloc(sizeof(cc));
	int *(*new_image_textpart); // image text part
	new_image_textpart=(int *(*))malloc(row * sizeof(int *));
	for(i=0;i<row;i++)
	{
		new_image_textpart[i]=(int *)malloc(col * sizeof(int));
	}
	for(i=0;i<row;i++)
	{	
		for(j=0;j<col;j++)
		{
			new_image_textpart[i][j]=255;
		}
	}




	for(i=0;i<tncc;i++)
	{
		if(textpart[i].start!=NULL && textpart[i].number_of_component>3) // logic part
		{
			temp=(cc *)malloc(sizeof(cc));
			temp=textpart[i].start;
			while(temp!=NULL)
			{
				m=temp->im.x;
				n=temp->im.y;
				new_image_textpart[m][n]=temp->im.value;
				temp=temp->nxt;
			}
		}
		else
			textpart[i].start=NULL;
	}


/*-----------------------------------------------------finding average height of text part--------------------------------------------------------------------*/

	int avg_height=0;
	int ct=0;
	for(i=0;i<ncco;i++)
	{
		if(textpart[i].start!=NULL && (textpart[i].xmax-textpart[i].xmin)>7)
		{
			avg_height=avg_height+(textpart[i].xmax-textpart[i].xmin);
			ct=ct+1;
		}
	}

	avg_height=avg_height/ct;


/*-------------------------------------------------------------------------------------------------------------------------*/


	int *horizental_smoothed_image;
	int *vertical_smoothed_image;
	int *smoothed_image;// formed by anding horizental and vertical smoothed image
	

	horizental_smoothed_image=(int *)malloc(row*col*sizeof(int));
	vertical_smoothed_image=(int *)malloc(row*col*sizeof(int));
	smoothed_image=(int *)malloc(row*col*sizeof(int));
	

	horizental_smoothed_image=horizental_smoothing(new_image_textpart,avg_height/3);
	vertical_smoothed_image=vertical_smoothing(new_image_textpart,avg_height/3);
	


/*---------------------------------------------------------------------------------------------------------*/


	for(i=0;i<(row*col);i++)
	{
		if(horizental_smoothed_image[i]==0 || vertical_smoothed_image[i]==0)
			smoothed_image[i]=0;
		else
			smoothed_image[i]=255;
			
	}

	



/*-------------------------------------------------------------------------------------------------------------------------*/


	// FINDING WORDS
	
	int *(*tmpimg);

	tmpimg=(int *(*))malloc(row * sizeof(int *));
	for(i=0;i<row;i++)
	{
		tmpimg[i]=(int *)malloc(col * sizeof(int));
	}
	
	for(i=0;i<row;i++)
	{	
		for(j=0;j<col;j++)
		{
			tmpimg[i][j]=smoothed_image[i*col+j];
		}
	}

	connectedcomponent(tmpimg);
	free(tmpimg);
	
	int nwords;
	
	nwords=tncc;
	
	nocc *words;
	words=(nocc *)malloc(nwords*sizeof(nocc));
	for(i=0;i<nwords;i++)
	{
		words[i].start=component[i].start;
		words[i].last=component[i].last;
		words[i].number_of_component=component[i].number_of_component;
		words[i].xmin=component[i].xmin;
		words[i].ymin=component[i].ymin;
		words[i].xmax=component[i].xmax;
		words[i].ymax=component[i].ymax;
		words[i].centroidx=((words[i].xmax-words[i].xmin)/2)+words[i].xmin;
		words[i].centroidy=((words[i].ymax-words[i].ymin)/2)+words[i].ymin;
	}

	
	for(i=0;i<nwords;i++)
	{
		if(words[i].xmax-words[i].xmin<(avg_height))
			words[i].start=NULL;			
	}

	

	int *image_with_words;
	image_with_words=(int *)malloc(row*col*sizeof(int));
	
	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
			image_with_words[i*col+j]=255;
	}
	

	for(i=0;i<nwords;i++)
	{
		if(words[i].start!=NULL) // logic part
		{
			temp=(cc *)malloc(sizeof(cc));
			temp=words[i].start;
			while(temp!=NULL)
			{
				m=temp->im.x;
				n=temp->im.y;
				image_with_words[m*col+n]=0;
				temp=temp->nxt;
			}
		}
	}

	for(i=0;i<(row*col);i++)
	{
		if(horizental_smoothed_image[i]==0 && image_with_words[i]==0)
			image_with_words[i]=0;
		else
			image_with_words[i]=255;
	}
	




/*---------------------------------------------------------------------------------------------------------*/

// FOR LINE


	int *(*img_word);
	
	img_word=(int *(*))malloc(row * sizeof(int *));
	for(i=0;i<row;i++)
	{
		img_word[i]=(int *)malloc(col * sizeof(int));
	}
	
	for(i=0;i<row;i++)
	{	
		for(j=0;j<col;j++)
		{
			img_word[i][j]=image_with_words[i*col+j];
		}
	}

	horizental_smoothed_image=horizental_smoothing(img_word,2*avg_height/3);
	
/*---------------------------------------------------------------------------------------------------------*/


	// FINDING CONNECTED COMPONENT FROM HORIZONTAL SMOOTHED IMAGE


	tmpimg=(int *(*))malloc(row * sizeof(int *));
	for(i=0;i<row;i++)
	{
		tmpimg[i]=(int *)malloc(col * sizeof(int));
	}
	
	for(i=0;i<row;i++)
	{	
		for(j=0;j<col;j++)
		{
			tmpimg[i][j]=horizental_smoothed_image[i*col+j];
		}
	}

	name="temp/line_part.pgm";
	writeimage(name,horizental_smoothed_image,row,col);
	
	connectedcomponent(tmpimg);
	free(tmpimg);

/*-------------------------------------------------------------------------------------------------------------------------*/


	// line detection

	nocc *line;
	int nline;//number of line
	nline=tncc;

	line=(nocc *)malloc(tncc* sizeof(nocc));
	
	for(i=0;i<tncc;i++)
	{
		line[i].start=component[i].start;
		line[i].last=component[i].last;
		line[i].number_of_component=component[i].number_of_component;
		line[i].xmin=component[i].xmin;
		line[i].ymin=component[i].ymin;
		line[i].xmax=component[i].xmax;
		line[i].ymax=component[i].ymax;
		line[i].centroidx=((line[i].xmax-line[i].xmin)/2)+line[i].xmin;
		line[i].centroidy=((line[i].ymax-line[i].ymin)/2)+line[i].ymin;
	}


	// REMOVING large height LINE


	for(i=0;i<tncc;i++)
	{
		if((line[i].xmax-line[i].xmin>(avg_height*5) && (line[i].ymax-line[i].ymin < (avg_height * 2)) ))//checking
		{
			
			line[i].start=NULL;
		}
	}


	

	int *(*horizontal_smoothed_image_smallremoved);
	horizontal_smoothed_image_smallremoved=(int **)malloc(row*sizeof(int*));
	for(i=0;i<row;i++)
	{
		horizontal_smoothed_image_smallremoved[i]=(int *)malloc(col*sizeof(int));
	}
	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{
			horizontal_smoothed_image_smallremoved[i][j]=255;
			horizental_smoothed_image[i*col+j]=255;
		}
	}

	for(i=0;i<tncc;i++)
	{
		if(line[i].start!=NULL) // logic part
		{
			temp=(cc *)malloc(sizeof(cc));
			temp=line[i].start;
			while(temp!=NULL)
			{
				m=temp->im.x;
				n=temp->im.y;
				horizontal_smoothed_image_smallremoved[m][n]=temp->im.value;
				temp=temp->nxt;
			}
		}
	}


/*----------------------------------------------------------------------------------------------------------------------------------------*/

	// VERTICAL SMOOTHING ON HORIZONTAL SMOOTHING

	int *vs_on_hs;
	vs_on_hs=(int *)malloc(row*col*sizeof(int));
	vs_on_hs=vertical_smoothing(horizontal_smoothed_image_smallremoved,avg_height * 2); // vertically gap filling the image

	
/*-------------------------------------------------------------------------------------------------------------------------*/

	//BOUNDARY EXTRACTION ON VS_ON_HS


	int *bextract;
	bextract=(int *)malloc(row*col*sizeof(int));
	bextract=boundaryextraction(vs_on_hs);

/*-------------------------------------------------------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------------------------------------------------------*/


	// image with lines
	
	
	imgg=(int *(*))malloc(row * sizeof(int *)); // image noise removed
	imgg1=(int *(*))malloc(row * sizeof(int *));
	for(i=0;i<row;i++)
	{
		imgg[i]=(int *)malloc(col * sizeof(int));
		imgg1[i]=(int *)malloc(col * sizeof(int));
	}


	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{
			if((image_with_words[i*col+j]==0 && binary_dst.data[i*col+j]==0)||temprgimage[i*col+j]==0)
			{
				imgg[i][j]=0;
				imgg1[i][j]=0;
			}
			else
			{
				imgg[i][j]=255;
				imgg1[i][j]=255;
			}
		}
	}
	


/*-------------------------------------------------------------------------------------------------------------------------*/

// FOR PARAGRAPH DETECTION

	tmpimg=(int *(*))malloc(row * sizeof(int *));
	for(i=0;i<row;i++)
	{
		tmpimg[i]=(int *)malloc(col * sizeof(int));
	}
	
	for(i=0;i<row;i++)
	{	
		for(j=0;j<col;j++)
		{
			tmpimg[i][j]=bextract[i*col+j];
		}
	}
	
	connectedcomponent(tmpimg);
	free(tmpimg);


	// PARAGRAPH

	int npar;//number of paragraph
	npar=tncc;

	nocc *paragraph;

	paragraph=(nocc *)malloc(npar* sizeof(nocc));
	
	for(i=0;i<npar;i++)
	{
		paragraph[i].start=component[i].start;
		paragraph[i].last=component[i].last;
		paragraph[i].number_of_component=component[i].number_of_component;
		paragraph[i].xmin=component[i].xmin;
		paragraph[i].ymin=component[i].ymin;
		paragraph[i].xmax=component[i].xmax;
		paragraph[i].ymax=component[i].ymax;
		paragraph[i].centroidx=((paragraph[i].xmax-paragraph[i].xmin)/2)+paragraph[i].xmin;
		paragraph[i].centroidy=((paragraph[i].ymax-paragraph[i].ymin)/2)+paragraph[i].ymin;
	}

	nocc *newparagraph;
	newparagraph=(nocc *)malloc(npar* sizeof(nocc));

	for(i=0;i<npar;i++)
	{
		newparagraph[i].start=paragraph[i].start;
		newparagraph[i].last=paragraph[i].last;
		newparagraph[i].number_of_component=paragraph[i].number_of_component;
		newparagraph[i].xmin=paragraph[i].xmin;
		newparagraph[i].ymin=paragraph[i].ymin;
		newparagraph[i].xmax=paragraph[i].xmax;
		newparagraph[i].ymax=paragraph[i].ymax;
		newparagraph[i].centroidx=((newparagraph[i].xmax-newparagraph[i].xmin)/2)+newparagraph[i].xmin;
		newparagraph[i].centroidy=((newparagraph[i].ymax-newparagraph[i].ymin)/2)+newparagraph[i].ymin;
		if((paragraph[i].xmax-paragraph[i].xmin-20)<20)//noise
			newparagraph[i].start=NULL;
	}

	// for inside paragraph

	for(i=0;i<npar;i++)
	{
		if(newparagraph[i].start!=NULL)
		{
			for(j=0;j<npar;j++)
			{
				if(paragraph[j].xmin>paragraph[i].xmin && paragraph[j].xmax<paragraph[i].xmax && paragraph[j].ymin>paragraph[i].ymin && paragraph[j].ymax<paragraph[i].ymax && i!=j)//para within para
				{
					
					newparagraph[j].start=NULL;
				}
				
			}
		}
	}


	// less size of para removed

	for(i=0;i<npar;i++)
	{
		if(newparagraph[i].start!=NULL)	
		{
			if(newparagraph[i].ymax-newparagraph[i].ymin<col/6 )
			{				
				if(newparagraph[i].xmin>((row*1)/15) && newparagraph[i].xmax<((row*14)/15))
					newparagraph[i].start=NULL;
	
				if(newparagraph[i].xmax-newparagraph[i].xmin > row/10 )  
					newparagraph[i].start=NULL;

			}
		
		}
	}



	int *(*new_image_paragraph);
	int *para;
	para=(int*)malloc(row*col*sizeof(int));
	new_image_paragraph=(int * *)malloc(row * sizeof(int *));
	for(i=0;i<row;i++)
	{
		new_image_paragraph[i]=(int *)malloc(col * sizeof(int));
	}
	
	for(i=0;i<row;i++)
	{	
		for(j=0;j<col;j++)
		{
			new_image_paragraph[i][j]=255;
		}
	}



	for(i=0;i<tncc;i++)
	{
		if(newparagraph[i].start!=NULL) // logic part
		{
			temp=(cc *)malloc(sizeof(cc));
			temp=newparagraph[i].start;
			while(temp!=NULL)
			{
				m=temp->im.x;
				n=temp->im.y;
				new_image_paragraph[m][n]=temp->im.value;

				temp=temp->nxt;
			}
		}
	}



	
/*-------------------------------------------------------------------------------------------------------------------------*/

	// Vertical Projection on PARAGRAPH

	int *vp,ctr,maxvp=0;
	vp=(int*)malloc(col*sizeof(int));
	for(i=0;i<col;i++)
	{
		ctr=0;
		for(j=0;j<row;j++)
		{
			if(new_image_paragraph[j][i]==0)
				ctr=ctr+1;
		}
		vp[i]=ctr;
		if(maxvp<vp[i])
			maxvp=vp[i];
	}
	

	// GAP CALCULATION

	gp *lastgap,*tempgap,*startgap,**gap;
	startgap=NULL;
	lastgap=NULL;
	int nog,y1,y2;
	i=0;
	ctr=0;
	for(i=0;i<col;i++)
	{

		if(vp[i]==0)
		{
			ctr=ctr+1;
			tempgap=(gp *)malloc(sizeof(gp));
			tempgap->y1=i;
			tempgap->size=0;
			tempgap->nxt=NULL;	
			while(vp[i]==0 && i<col)
			{
				tempgap->size=tempgap->size+1;
				i++;
			}
			tempgap->y2=i;
			if(startgap==NULL)
			{
				startgap=tempgap;
				lastgap=tempgap;
			}
			else
			{
				lastgap->nxt=tempgap;
				lastgap=tempgap;
			}
		}
	}

	nog=ctr;

	gp *maximum_gap,*min_gap;
	if(nog!=0)
	{
	tempgap=NULL;
	
	tempgap=startgap;
	maximum_gap=startgap;
	min_gap=startgap;
	while(tempgap!=NULL)
	{
		
		if(maximum_gap->size <= tempgap->size)
		{
			maximum_gap=tempgap;
			
		}
		if(min_gap->size >= tempgap->size)
		{
			min_gap=tempgap;
			
		}	
		tempgap=tempgap->nxt;			
	}

	
	
	}
	
	else
	{
		printf("ERROR NO GAP \n");
		//exit(0);
	}
	
	
/*-------------------------------------------------------------------------------------------------------------------------*/	

	// outside page


	int newrow,newcol,npt;
	int *outputimage;
	int *newmappedgraphics;

	
	if(nog > 0 && (maximum_gap->size) > (1.5 *min_gap->size) )   // outside page
	{
		
	
		if(maximum_gap->y1 <(col/2) && maximum_gap->y2 < ((col/2)-(col/8)))
		{
			newrow=row;
			newcol=col-(maximum_gap->y2-(1.5*(min_gap->size)));
			outputimage=(int*)malloc(newrow*newcol*sizeof(int));
			newmappedgraphics=(int*)malloc(newrow*newcol*sizeof(int));
	
			for(i=0;i<newrow;i++)
			{	
				for(j=0;j<newcol;j++)
				{
					newmappedgraphics[i*newcol+j]=255;
				}
			}
			npt=maximum_gap->y2-(1.5*(min_gap->size));
			for(i=0;i<newrow;i++)
			{
				npt=maximum_gap->y2-(1.5*(min_gap->size));
				for(j=0;j<newcol && npt<col;j++)
				{
					
					outputimage[i*newcol+j]=imgg[i][npt];
					npt=npt+1;
					
				}
				npt=maximum_gap->y2-(1.5*(min_gap->size));
				for(j=0;j<newcol && npt<col;j++)
				{
					newmappedgraphics[i*newcol+j]= temprgimage[i*col+npt];
					npt=npt+1;
				}
			}
			
			for(i=0;i<newrow;i++)
			{
				for(j=0;j<newcol;j++)
				{
					if(outputimage[i*newcol+j]==0)
						outputimage[i*newcol+j]=0;
					else
						outputimage[i*newcol+j]=255;
				}
			}
			
			
		}
		else if(maximum_gap->y1 > (col/2) && maximum_gap->y1 > ((col/2)+(col/8)))
		{
			//drawing the image without texual noise
			newrow=row;
			newcol=((maximum_gap->y1)+(1.5*(min_gap->size)));
			outputimage=(int*)malloc(newrow*newcol*sizeof(int));
			newmappedgraphics=(int*)malloc(newrow*newcol*sizeof(int));
	
			for(i=0;i<newrow;i++)
			{	
				for(j=0;j<newcol;j++)
				{
					newmappedgraphics[i*newcol+j]=255;
				}
			}
			for(i=0;i<newrow;i++)
			{
				for(j=0;j<newcol;j++)
				{
					if(imgg[i][j]==0)
						outputimage[i*newcol+j]=0;
					else
						outputimage[i*newcol+j]=255;
					newmappedgraphics[i*newcol+j]= temprgimage[i*col+j];
				}
			}
			
		}
		else
		{
			printf("i dont have to remove\n");	
			newrow=row;
			newcol=col;
			outputimage=(int*)malloc(newrow*newcol*sizeof(int));
			newmappedgraphics=(int*)malloc(newrow*newcol*sizeof(int));
			for(i=0;i<newrow;i++)
			{
				for(j=0;j<newcol;j++)
				{
					if(imgg[i][j]==0)
						outputimage[i*newcol+j]= 0;
					else
						outputimage[i*newcol+j]=255;
					newmappedgraphics[i*newcol+j]= temprgimage[i*col+j];	
				}
			}
		
			
		}

	}

	else
	{
		
		newrow=row;
		newcol=col;
		outputimage=(int*)malloc(newrow*newcol*sizeof(int));
		newmappedgraphics=(int*)malloc(newrow*newcol*sizeof(int));
		for(i=0;i<newrow;i++)
		{
			for(j=0;j<newcol;j++)
			{
				if(imgg[i][j]==0)
					outputimage[i*newcol+j]= 0;
				else
					outputimage[i*newcol+j]=255;
				newmappedgraphics[i*newcol+j]= temprgimage[i*col+j];	
			}
		}
		
					
	}


	int *newimage;
	newimage=(int*)malloc(newrow*newcol*sizeof(int));


	
	for(i=0;i<newrow;i++)
	{	
		for(j=0;j<newcol;j++)
		{
			newimage[i*newcol+j]=255;
			for(m=0;m<npar;m++)
			{
				if(newparagraph[m].start!=NULL) // logic part
				{
					if((maximum_gap->size-min_gap->size) > (0.5 * min_gap->size) && nog > 0)  
					{	
						if(maximum_gap->y1 <(col/2))
						{
							npt=maximum_gap->y2-(1.5*(min_gap->size));
							if(i>=newparagraph[m].xmin && i<=newparagraph[m].xmax && j+npt>=newparagraph[m].ymin && j+npt<=newparagraph[m].ymax && outputimage[i*newcol+j]==0)
								newimage[i*newcol+j]=0;
						}
						else
						{
							if(i>newparagraph[m].xmin && i<newparagraph[m].xmax && j>newparagraph[m].ymin && j<newparagraph[m].ymax && outputimage[i*newcol+j]==0)
								newimage[i*newcol+j]=0;
						}
					}
					else
					{
						if(i>newparagraph[m].xmin && i<newparagraph[m].xmax && j>newparagraph[m].ymin && j<newparagraph[m].ymax && outputimage[i*newcol+j]==0)
							newimage[i*newcol+j]=0;
					}
				}
			}
		}
	}
	
	

	int *resultimage;
	resultimage=(int*)malloc(newrow*newcol*sizeof(int));

	for(i=0;i<newrow;i++)
	{	
		for(j=0;j<newcol;j++)
		{
			resultimage[i*newcol+j]=255;
		}
	}
	
	

	
	for(i=0;i<newrow;i++)
	{	
		for(j=0;j<newcol;j++)
		{
			if(newimage[i*newcol+j]==0 || newmappedgraphics[i*newcol+j]==0 )
				resultimage[i*newcol+j]=0;
			
		}
	}


	name="temp/output_image.pgm";
	writeimage(name,resultimage,newrow,newcol);
	
	printf("Done Noise\n");	
	
/*-------------------------------------------------------------------------------------------------------------------------*/	


	free(outputimage);
	free(newimage);
	free(resultimage);
	free(para);
	free(new_image_paragraph);
	free(originalcomponent);
	free(rgraphics);
	free(graphics);
	free(new_image_textpart);
	free(temprgimage);


}




/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

#endif