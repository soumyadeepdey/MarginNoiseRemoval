
#ifndef writeimage_H
#define writeimage_H


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

/*-----------------------------------------------------------------------WRITE PGM IMAGE----------------------------------------------------------------------------*/


int writeimage(char *r, int *tempimg, int ro, int co)
{
	int i;
	unsigned char c;
	FILE *fp;
	fp=fopen(r,"w");
	fprintf(fp,"P5\n");
	fprintf(fp,"# CREATOR: Soumya Hello Creating New IMG\n");
	fprintf(fp,"%d %d\n",co,ro);
	fprintf(fp,"%d\n",maximum);
	for(i=0;i<(ro*co);i++)
	{
		c=tempimg[i];
		fprintf(fp,"%c",c);
	}
	fclose(fp);
	return 0;
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/


#endif