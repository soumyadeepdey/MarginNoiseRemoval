
//~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*
// Program Name : marginal_noise_removal.cpp
// 
// Project :  		DRD
// Author : 		Soumyadeep Dey
// Creation Date : 	FEB 14  -2012.  Rights Reserved
//~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*



/*-------------------------------------------------- Binarization-----------------------------------------------------------------*/


int *binarization()
{
  /* 0: Binary
     1: Binary Inverted
     2: Threshold Truncated
     3: Threshold to Zero
     4: Threshold to Zero Inverted
   */

	/// Convert the image to Gray
  	

	//adaptiveThreshold(  src_gray, binary_dst, maximum_BINARY_value, ADAPTIVE_THRESH_GAUSSIAN_C,  threshold_type,  blockSize, 20);
	
	binary_threshold_value = 120;
      threshold( src_gray,  binary_dst,  binary_threshold_value, maximum_BINARY_value,threshold_type );


	int *tempimg,i,j,k;
	tempimg=(int*)malloc(src.rows*src.cols*(sizeof(int)));
	for(i=0;i<(binary_dst.rows*binary_dst.cols);i++)
	{
		tempimg[i]=binary_dst.data[i];
	}

	
	 return (tempimg);
	
}

/*-------------------------------------------------------------------------------------------------------------------------*/
