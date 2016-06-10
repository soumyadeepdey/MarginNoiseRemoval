
#ifndef noiserem_pgframe_H
#define noiserem_pgframe_H

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
#include "morphologi.h"



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
	graphics=(nocc *)malloc(tncc * sizeof(nocc));

	j=0;
	for(i=0;i<tncc;i++)
	{
		if(originalcomponent[i].number_of_component >= graphicsthreashold )
		{
			graphics[i].start=originalcomponent[i].start;
			graphics[i].last=originalcomponent[i].last;
			graphics[i].number_of_component=originalcomponent[i].number_of_component;
			graphics[i].xmin=originalcomponent[i].xmin;
			graphics[i].ymin=originalcomponent[i].ymin;
			graphics[i].xmax=originalcomponent[i].xmax;
			graphics[i].ymax=originalcomponent[i].ymax;
			graphics[i].centroidx=((graphics[i].xmax-graphics[i].xmin)/2)+graphics[i].xmin;
			graphics[i].centroidy=((graphics[i].ymax-graphics[i].ymin)/2)+graphics[i].ymin;
			//printf("G comp \n");
			//j=j+1;
		}
		else
		{
		       graphics[i].start = NULL;
		}
	}
	j=0;
	
	nocc *rgraphics;
	
	rgraphics=(nocc *)malloc(tncc * sizeof(nocc));

	for(i=0;i<tncc;i++)
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


	


	for(i=0;i<tncc;i++)
	{
		if(rgraphics[i].start!=NULL)
		{	
			for(j=0;j<tncc;j++)
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
	

	for(i=0;i<tncc;i++)
	{
		if(rgraphics[i].start!=NULL)
		{
			if(rgraphics[i].xmax-rgraphics[i].xmin>0.3*row && rgraphics[i].ymax-rgraphics[i].ymin<0.1*col)
				rgraphics[i].start=NULL;
			if(rgraphics[i].xmax-rgraphics[i].xmin<0.1*row && rgraphics[i].ymax-rgraphics[i].ymin>0.8*col)
				rgraphics[i].start=NULL;
		}
	}


	for(i=0;i<tncc;i++)
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
	printf("avg-height=%d\n",avg_height);


	
/*-----------------------------------------------graphics likely to be text--------------------------------------------------------------------------*/

	for(i=0;i<tncc;i++)
	{
		if(rgraphics[i].start!=NULL)
		{
		        float th = 2 * avg_height;
			int tt = (int) th;
			//printf("height = %d and th = %d\n",rgraphics[i].xmax-rgraphics[i].xmin,tt);
			if(rgraphics[i].xmax-rgraphics[i].xmin < tt)
			{
				textpart[i].start=rgraphics[i].start;
				textpart[i].xmin=rgraphics[i].xmin;
				textpart[i].ymin=rgraphics[i].ymin;
				textpart[i].xmax=rgraphics[i].xmax;
				textpart[i].ymax=rgraphics[i].ymax;
				textpart[i].centroidx=rgraphics[i].centroidx;
				textpart[i].centroidy=rgraphics[i].centroidy;
				textpart[i].number_of_component=rgraphics[i].number_of_component;
				textpart[i].last=rgraphics[i].last;
				rgraphics[i].start=NULL;
				
			}
		}
	}



/*-------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------graphics likely to be noise--------------------------------------------------------------------------*/

	
	for(i=0;i<tncc;i++)
	{
		if(rgraphics[i].start!=NULL)
		{
			float th = 0.1*col; 
			int tt = (int) th;
			if(rgraphics[i].ymax-rgraphics[i].ymin < tt)
			{
				
				//printf("width =%d and th = %d",rgraphics[i].ymax-rgraphics[i].ymin, tt);
				rgraphics[i].start=NULL;
				//printf("Hello\n");
			}
		}
	}
	


/*-------------------------------------------------------------------------------------------------------------------------*/
	// graphics part

	
	cc *tempg;
	tempg=(cc *)malloc(sizeof(cc));
	int m,n;

// creating new image with graphics part

	int *temprgimage;
	temprgimage=(int *)malloc(row*col*sizeof(int));
	for(i=0;i<row*col;i++)
		temprgimage[i]=255;

	for(i=0;i<tncc;i++)
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
/*
	name="step/graphics.pgm";
	writeimage(name,temprgimage,row,col);
*/
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





/*-------------------------------------------------------------------------------------------------------------------------*/


	int *horizental_smoothed_image;
	int *vertical_smoothed_image;
	int *smoothed_image;// formed by anding horizental and vertical smoothed image
	

	horizental_smoothed_image=(int *)malloc(row*col*sizeof(int));
	vertical_smoothed_image=(int *)malloc(row*col*sizeof(int));
	smoothed_image=(int *)malloc(row*col*sizeof(int));
	

	horizental_smoothed_image=horizental_smoothing(new_image_textpart,avg_height/3);
	vertical_smoothed_image=vertical_smoothing(new_image_textpart,avg_height/3);
	

	/*
	name="step/hs_3.pgm";
	writeimage(name,horizental_smoothed_image,row,col);
	name="step/vs_3.pgm";
	writeimage(name,vertical_smoothed_image,row,col);
	*/


/*---------------------------------------------------------------------------------------------------------*/


	for(i=0;i<(row*col);i++)
	{
		if(horizental_smoothed_image[i]==0 || vertical_smoothed_image[i]==0)
			smoothed_image[i]=0;
		else
			smoothed_image[i]=255;
			
	}

	/*
	name="step/hs3_OR_vs3.pgm";
	writeimage(name,smoothed_image,row,col);
	*/


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
		if(words[i].xmax-words[i].xmin > 5* avg_height)
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

	/*
	name="step/word_noiseremoved1.pgm";
	writeimage(name,image_with_words,row,col);
	  */

	for(i=0;i<(row*col);i++)
	{
		if(horizental_smoothed_image[i]==0 && image_with_words[i]==0)
			image_with_words[i]=0;
		else
			image_with_words[i]=255;
	}
	
	/*
	name="step/word_noiseremoved2.pgm";
	writeimage(name,image_with_words,row,col);
	*/
	

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
	
	/*
	name="step/line.pgm";
	writeimage(name,horizental_smoothed_image,row,col);
	*/
	
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/

     int *bextractline;// boundary extraction on line
     bextractline = (int *)malloc(row*col*sizeof(int));
     bextractline = boundaryextraction(horizental_smoothed_image);
     /*
     name="step/line_boundary.pgm";
	writeimage(name,bextractline,row,col);
	*/
	

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
			tmpimg[i][j]=bextractline[i*col+j];
		}
	}

	
	
	connectedcomponent(tmpimg);
	free(tmpimg);
	free(bextractline);

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
			horizontal_smoothed_image_smallremoved[i][j]=horizental_smoothed_image[i*col+j];
			//horizental_smoothed_image[i*col+j]=255;
		}
	}

	for(i=0;i<tncc;i++)
	{
		if(line[i].start==NULL) // logic part
		{
		  for(int k=line[i].xmin;k<line[i].xmax;k++)
		  {
		    for(int l=line[i].ymin;l<line[i].ymax;l++)
		    {
		      horizontal_smoothed_image_smallremoved[k][l]=255;
		      horizental_smoothed_image[k*col+l]=255;
		    }
		    
		  }

		}
	}

	/*
	name="step/line_noiserm.pgm";
	writeimage(name,horizental_smoothed_image,row,col);
	*/


/*----------------------------------------------------------------------------------------------------------------------------------------*/

	// VERTICAL SMOOTHING ON HORIZONTAL SMOOTHING

	int *vs_on_hs;
	vs_on_hs=(int *)malloc(row*col*sizeof(int));
	vs_on_hs=vertical_smoothing(horizontal_smoothed_image_smallremoved,avg_height * 1.7); // vertically gap filling the image
	
	/*
	name="step/concentarted_text-area.pgm";
	writeimage(name,vs_on_hs,row,col);
	*/
/*-------------------------------------------------------------------------------------------------------------------------*/

	//BOUNDARY EXTRACTION ON VS_ON_HS


	int *bextract;
	bextract=(int *)malloc(row*col*sizeof(int));
	bextract=boundaryextraction(vs_on_hs);
	/*
	name="step/bextract_concentarted_text-area.pgm";
	writeimage(name,bextract,row,col);
	*/

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
			if((horizental_smoothed_image[i*col+j]==0 && binary_dst.data[i*col+j]==0)||temprgimage[i*col+j]==0)
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

	
	newparagraph=(nocc *)malloc(npar* sizeof(nocc));
	nocc *weak_text;
	weak_text=(nocc *)malloc(npar* sizeof(nocc));

	for(i=0;i<npar;i++)
	{
		weak_text[i].start=NULL;//make all paragraph as not noise
		newparagraph[i].start=paragraph[i].start;
		newparagraph[i].last=paragraph[i].last;
		newparagraph[i].number_of_component=paragraph[i].number_of_component;
		newparagraph[i].xmin=paragraph[i].xmin;
		newparagraph[i].ymin=paragraph[i].ymin;
		newparagraph[i].xmax=paragraph[i].xmax;
		newparagraph[i].ymax=paragraph[i].ymax;
		newparagraph[i].centroidx=((newparagraph[i].xmax-newparagraph[i].xmin)/2)+newparagraph[i].xmin;
		newparagraph[i].centroidy=((newparagraph[i].ymax-newparagraph[i].ymin)/2)+newparagraph[i].ymin;
		newparagraph[i].hierarchy = 1;
	}

	// for inside paragraph

	for(i=0;i<npar;i++)
	{
		if(newparagraph[i].start!=NULL)
		{
			for(j=0;j<npar;j++)
			{
			    if(i!=j)
			    {
				if(paragraph[j].xmin>=paragraph[i].xmin && paragraph[j].xmax<=paragraph[i].xmax && paragraph[j].ymin>=paragraph[i].ymin && paragraph[j].ymax<=paragraph[i].ymax)//para within para
				{
					newparagraph[j].start=NULL;
					newparagraph[j].hierarchy = -1;
				}
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
				/*
				if(newparagraph[i].xmax-newparagraph[i].xmin > row/10 && newparagraph[i].ymax-newparagraph[i].ymin < col/10)  
				{	
					newparagraph[i].start=NULL;
				}
				*/
				if(newparagraph[i].xmax-newparagraph[i].xmin < avg_height && newparagraph[i].ymax-newparagraph[i].ymin < avg_height*3)  
				{	
					newparagraph[i].start=NULL;
				}
							
				else
				{
					weak_text[i].start=paragraph[i].start;
					weak_text[i].last=newparagraph[i].last;
					weak_text[i].number_of_component=newparagraph[i].number_of_component;
					weak_text[i].xmin=newparagraph[i].xmin;
					weak_text[i].ymin=newparagraph[i].ymin;
					weak_text[i].xmax=newparagraph[i].xmax;
					weak_text[i].ymax=newparagraph[i].ymax;
					weak_text[i].centroidx=newparagraph[i].centroidx;
					weak_text[i].centroidy=newparagraph[i].centroidy;
	
					newparagraph[i].start=NULL;
				}
			}
		}
	}


	src.copyTo(color_para);

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
			para[i*col+j]=255;
		}
	}



	for(i=0;i<npar;i++)
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
				para[m*col+n]=temp->im.value;

				temp=temp->nxt;
			}
			rectangle(color_para,Point(newparagraph[i].ymin,newparagraph[i].xmin),Point(newparagraph[i].ymax,newparagraph[i].xmax),Scalar( 0,0,255),2,8);// RED (actual text part)
		}
	}
	/*
	name="step/actual-text-area.pgm";
	writeimage(name,para,row,col);
	*/


/*-------------------------------------------------------------------------------------------------------------------------*/

	// Vertical Projection on actual text area PARAGRAPH

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
	int *newoutput;
	newoutput=(int *)malloc(row*col*sizeof(int));
	int page_frame_ymax,page_frame_ymin;

	
	if(nog > 0 && (maximum_gap->size) > (1.5 *min_gap->size) )   // outside page
	{
		
	
		if(maximum_gap->y1 <(col/2) && maximum_gap->y2 < ((col/2)-(col/8)))
		{
			newrow=row;
			newcol=col-(maximum_gap->y2-(1.5*(min_gap->size)));
			page_frame_ymin=maximum_gap->y2-(min_gap->size * 1.5);
			page_frame_ymax=col-1;
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
			page_frame_ymax=maximum_gap->y1+(1.5 * min_gap->size);
			page_frame_ymin=0;
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
			page_frame_ymax=col-1;
			page_frame_ymin=0;
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
		page_frame_ymax=col-1;
		page_frame_ymin=0;
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
	/*
	name="step/new_output_image1.pgm";
	writeimage(name,newimage,newrow,newcol);
	*/
/*-------------------------------------------------------------creating result image------------------------------------------------------------*/
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

	/*
	name="temp/output_image.pgm";
	writeimage(name,resultimage,newrow,newcol);
	*/

/*-------------------------------------------------------------------------------------------------------------------------*/

//page frame

	for(i=0;i<npar;i++)
	{
		if(newparagraph[i].start!=NULL || weak_text[i].start!=NULL)
		{
			if(newparagraph[i].ymax <= page_frame_ymax && newparagraph[i].ymin >= page_frame_ymin);
			else
			{
				newparagraph[i].start=NULL;
				weak_text[i].start=NULL;
			}
		}
	}

	
		
	
/*-------------------------------------------------------------------------------------------------------------------------*/	


/*----------------------------------------------------------------layout-----------------------------------------------------------------------*/


//detected text part

	for(i=0;i<npar;i++)
	{
		if(newparagraph[i].start!=NULL) // logic part
		{
			rectangle(color_para,Point(newparagraph[i].ymin,newparagraph[i].xmin),Point(newparagraph[i].ymax,newparagraph[i].xmax),Scalar(50,128,255),3,8);// x y convention is opp
			
		}
	}



/*--------------------------------------------------checking the layout-----------------------------------------------------------------------*/

int layoutxmin,layoutxmax,layoutymin,layoutymax;
layoutxmin=row-1;
layoutxmax=0;
layoutymin=col-1;
layoutymax=0;


	for(i=0;i<npar;i++)
	{
		if(newparagraph[i].start!=NULL && newparagraph[i].ymax-newparagraph[i].ymin >= col/6)	
		{
				
			if(layoutxmin >= newparagraph[i].xmin)
				layoutxmin=newparagraph[i].xmin;
			if(layoutymin >= newparagraph[i].ymin)
				layoutymin=newparagraph[i].ymin;
			if(layoutxmax <= newparagraph[i].xmax)
				layoutxmax=newparagraph[i].xmax;
			if(layoutymax <= newparagraph[i].ymax)
				layoutymax=newparagraph[i].ymax;
				
			
		}
		
	}

printf("LAYOUT XMIN=%d\tXMAX=%d\tYMIN=%d\tYMAX=%d\n",layoutxmin,layoutxmax,layoutymin,layoutymax);


rectangle(color_para,Point(layoutymin,layoutxmin),Point(layoutymax,layoutxmax),Scalar( 255,128,160),5,8);


/*------------------------------------PROCESSING ON WEAK NOISE AND RETREIVING TEXT-------------------------------------------------*/
	
	for(i=0;i<npar;i++)
	{
	  if(weak_text[i].start!=NULL )
	  {
	    
	    if(weak_text[i].xmax<=layoutxmax && weak_text[i].ymax<=layoutymax)// weak text within layout
	    {
	      int len = weak_text[i].ymax - weak_text[i].ymin;
	      int wlen;
	      if(weak_text[i].ymax >= layoutymin && weak_text[i].ymax <= layoutymax)
		wlen = weak_text[i].ymax - layoutymin;
	      else if(weak_text[i].ymax >= layoutymax && weak_text[i].ymin <= layoutymax)
		wlen = layoutymax - weak_text[i].ymin;
	      if(wlen > 0.8 * len)
	      {
		weak_text[i].start=NULL;
		newparagraph[i].start=paragraph[i].start;
		rectangle(color_para,Point(weak_text[i].ymin,weak_text[i].xmin),Point(weak_text[i].ymax,weak_text[i].xmax),Scalar( 0,160,120),2,8);//header and footer green
	      }
	    }
	  }
	}

	for(i=0;i<npar;i++)
	{
		if(weak_text[i].start!=NULL )
		{
		    if(weak_text[i].ymax-weak_text[i].ymin > 2*avg_height)
		    {
		  
			if(((layoutymin - weak_text[i].ymax) < 3*avg_height) && ((weak_text[i].xmax>=layoutxmin) && (weak_text[i].xmin<=layoutxmax)))
			{
				weak_text[i].start=NULL;
				
				newparagraph[i].start=paragraph[i].start;
				rectangle(color_para,Point(weak_text[i].ymin,weak_text[i].xmin),Point(weak_text[i].ymax,weak_text[i].xmax),Scalar( 0,255,0),2,8);//header and footer green			
			}
			else if(((layoutymax - weak_text[i].ymin) < 3*avg_height) && ((weak_text[i].xmax>=layoutxmin) && (weak_text[i].xmin<=layoutxmax)))
			{
				weak_text[i].start=NULL;
				
				newparagraph[i].start=paragraph[i].start;
				rectangle(color_para,Point(weak_text[i].ymin,weak_text[i].xmin),Point(weak_text[i].ymax,weak_text[i].xmax),Scalar( 0,255,0),2,8);//header and footer green			
			}
			else if(((layoutxmin - weak_text[i].xmax) < 3*avg_height) && ((weak_text[i].ymax>=layoutymin) && (weak_text[i].ymin<=layoutymax)))
			{
				weak_text[i].start=NULL;
				
				newparagraph[i].start=paragraph[i].start;
				rectangle(color_para,Point(weak_text[i].ymin,weak_text[i].xmin),Point(weak_text[i].ymax,weak_text[i].xmax),Scalar( 0,255,0),2,8);//header and footer green			
			}
			else if(((weak_text[i].xmin - layoutxmax) < 3*avg_height) && ((weak_text[i].ymax>=layoutymin) && (weak_text[i].ymin<=layoutymax)))
			{
				weak_text[i].start=NULL;
				
				newparagraph[i].start=paragraph[i].start;
				rectangle(color_para,Point(weak_text[i].ymin,weak_text[i].xmin),Point(weak_text[i].ymax,weak_text[i].xmax),Scalar( 0,255,0),2,8);//header and footer green			
			}
			else if((weak_text[i].xmax <= layoutxmax && weak_text[i].xmin >= layoutxmin) && (weak_text[i].ymax <= layoutymax && weak_text[i].ymin >= layoutymin))
			{
				weak_text[i].start=NULL;
				newparagraph[i].start=paragraph[i].start;
				rectangle(color_para,Point(weak_text[i].ymin,weak_text[i].xmin),Point(weak_text[i].ymax,weak_text[i].xmax),Scalar( 128,20,160),2,8);//within page frame
			}
			else if((weak_text[i].xmin<=((row*1)/15) || weak_text[i].xmax>=((row*14)/15)) && weak_text[i].xmax-weak_text[i].xmin < avg_height *4)
			{
				weak_text[i].start=NULL;
				
				newparagraph[i].start=paragraph[i].start;
				rectangle(color_para,Point(weak_text[i].ymin,weak_text[i].xmin),Point(weak_text[i].ymax,weak_text[i].xmax),Scalar( 0,255,0),2,8);//header and footer green
			}
			/*
			else if(weak_text[i].ymax-weak_text[i].ymin < 0.3*col && (weak_text[i].xmax-weak_text[i].xmin > 0.3*row)
			{
				weak_text[i].start=NULL;
				
				newparagraph[i].start=paragraph[i].start;
			}*/
		    }
		}
	}
	
	
	
	

/*--------------------------------------------removing textual noise---------------------------------------------------------*/


	for(i=0;i<npar;i++)
	{
		if(weak_text[i].start!=NULL )
		{
			for(m=weak_text[i].xmin;m<=weak_text[i].xmax;m++)
			{
				for(n=weak_text[i].ymin;n<=weak_text[i].ymax;n++)
				{
					imgg[m][n]=255;
				}
			}
		}
		
		if(newparagraph[i].start==NULL && newparagraph[i].hierarchy == 1)
		{
			for(m=newparagraph[i].xmin;m<=newparagraph[i].xmax;m++)
			{
				for(n=newparagraph[i].ymin;n<=newparagraph[i].ymax;n++)
				{
					imgg[m][n]=255;
				}
			}
		}
	}


/*-----------------------------------------------------------------------------creating output image----------------------------------------------------------------------------*/

	int *actual_output;
	actual_output=(int *)malloc(row*col*sizeof(int));

	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{
			actual_output[i*col+j]=imgg[i][j];
		}
	}	

	name="actual_outputimage.pgm";
	writeimage(name,actual_output,row,col);

/*--------------------------------------------------checking the PAGE FRAME-----------------------------------------------------------------------*/


pgfrmxmin=row-1;
pgfrmxmax=0;
pgfrmymin=col-1;
pgfrmymax=0;


	for(i=0;i<npar;i++)
	{
		if(newparagraph[i].start!=NULL )	
		{
				
			if(pgfrmxmin >= newparagraph[i].xmin)
				pgfrmxmin=newparagraph[i].xmin;
			if(pgfrmymin >= newparagraph[i].ymin)
				pgfrmymin=newparagraph[i].ymin;
			if(pgfrmxmax <= newparagraph[i].xmax)
				pgfrmxmax=newparagraph[i].xmax;
			if(pgfrmymax <= newparagraph[i].ymax)
				pgfrmymax=newparagraph[i].ymax;
			
			number_para = number_para + 1;	
			
		}
		
	}

printf("PAGE-FRAME XMIN=%d\tXMAX=%d\tYMIN=%d\tYMAX=%d\n",pgfrmxmin,pgfrmxmax,pgfrmymin,pgfrmymax);


rectangle(color_para,Point(pgfrmymin,pgfrmxmin),Point(pgfrmymax,pgfrmxmax),Scalar( 128,0,240),5,8);





/*---------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*
	name="resultimage.png";
	imwrite(name,color_para);
*/
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

#endif