#include "xml.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>






/***************************************************//**
 *	\fn XML_IO()
 *	\brief constructor,initializes values
 ******************************************************/
IITKGP_XML_IO::XML_IO::XML_IO()
{
	cropFlag =0;
	page = new char [50000];
}
/***************************************************//**
 *	\fn ~XML_IO()
 *	\brief destructor,deletes global memory allocated
 ******************************************************/
IITKGP_XML_IO::XML_IO::~XML_IO()
{
	leftX.erase(leftX.begin(),leftX.begin()+leftX.size());
	leftY.erase(leftY.begin(),leftY.begin()+leftY.size());
	rightX.erase(rightX.begin(),rightX.begin()+rightX.size());
	rightY.erase(rightY.begin(),rightY.begin()+rightY.size());
	delete [] page;
}
/**			\fn char* readXML(char *XMLFile)
 *			\brief Parses the input XML file structure and reads the input document page to be processed. 
 *			\param char *XMLFile stores the name of the input XML file
 *			\return input document page to be processed.
 */
char*
IITKGP_XML_IO::XML_IO::readXML(char *XMLFile)
{
	cout << "\nReading input XMl";
	
	XMLparser *xmlParser = new XMLparser();
	xmlParser->readXMLdocument (XMLFile);

	XMLelement	*docElem, *pageElem, *preProcess;
	
	for (int e=0; e < xmlParser->elementList.size(); ++e) {
    	xmlParser->elementList[e]->printAttributes();
        
	if (!strcmp (xmlParser->elementList[e]->name, "DOCUMENT")) {
        docElem = xmlParser->elementList[e];

        for (int p=0; p < docElem->childs.size(); ++p) {
       	pageElem = docElem->childs[p];

	int pageElem_Childs_Size = pageElem->childs.size();

	if(pageElem_Childs_Size > 0)
	{
		preProcess = pageElem->childs[pageElem_Childs_Size-1];
		if (!strcmp(preProcess->name, "Skew") || !strcmp(preProcess->name, "TextNonText") ||
			!strcmp(preProcess->name, "Thinning") || !strcmp(preProcess->name, "Thickening") ||
			!strcmp(preProcess->name, "Binarization"))
		{
			XMLelement *preProcess_Child;
			cout << "\nLast Child is ->\t " << preProcess->name << "\n\tRead output page URI ....!!";
			for(int j=0; j < preProcess->childs.size(); j++)
			{
				if (!strcmp(preProcess->childs[j]->name, "OutputImageURI"))
				{
					preProcess_Child = preProcess->childs[j];
					page = new char [strlen(preProcess_Child->content)];
					strcpy(page,preProcess_Child->content);
					cout << "Page to be processed is ------> " << page;
					return(page);
				}
			}
		}
		else if(!strcmp(preProcess->name, "Cropping"))
		{
			cout <<"\n already a text block";exit(0);
			//cropFlag = 1;
			XMLelement *cropChild;
			cout << "\n Last child is ---->   " << preProcess->name 
			 << "\n Read input page URI and block coordinates";
			for (int m=0; m < preProcess->childs.size(); m++)
			{
				if(!strcmp(preProcess->childs[m]->name,"OutputImageURI"))
				{
					cropChild = preProcess->childs[m];
					page = new char [strlen(cropChild->content)];
					strcpy(page, cropChild->content);
					cout << "\n page to be processed \t" << page;
					return(page);
				}
			}
		}
		}
		else
		{
			page = pageElem->getAttributeValue("ImageURI");
			return page;
		}		
	}
}
}
}
/**			\fn char* readXML(char *XMLFile)
 *			\brief Parses the input XML file structure and reads the input document page to be processed. 
 *			\param char *XMLFile stores the name of the input XML file
 *			\return input document pag to be processed.
 */





/**
 * \author Soumyadeep Dey
 * \modified date - 03/03/12
 */

void
IITKGP_XML_IO::XML_IO::writemyxml(char *inputXML ,char *outputxml, char *childtype, char *input_image, char *output_image, 
				char *angel, char *algorithmname, int toplx, int toply, int btmrx, int btmry)
{
	XMLparser *xmlParser = new XMLparser();
	xmlParser->readXMLdocument (inputXML);
	XMLelement 	*docElem, *pageElem, *child, *page_frame, *pageframe_cord, *segmented_cord, *block;
	char *temp;
	
	int nblock;
	for (int e=0; e < xmlParser->elementList.size(); ++e) 
	{
		xmlParser->elementList[e]->printAttributes();
        
		if (!strcmp (xmlParser->elementList[e]->name, "DOCUMENT")) 
		{
			docElem = xmlParser->elementList[e];
			for (int p=0; p<docElem->childs.size(); ++p)
			{
            			pageElem = docElem->childs[p];
				child = pageElem->makeChildElement(childtype);
				
				if(strcmp(childtype,"Skew")==0)
				{
					child->setAttribute("SkewAngle",angel);
					child->setAttribute("AlgorithmName",algorithmname);
					XMLelement *in_Image = child->makeChildElement("InputImageURI");
					XMLelement *out_Image = child->makeChildElement("OutputImageURI");
					in_Image->setContent(input_image);
					out_Image->setContent(output_image);	
				}
				else if(strcmp(childtype,"Cropping")==0)
				{
					
					XMLelement *in_Image = child->makeChildElement("InputImageURI");
					XMLelement *out_Image = child->makeChildElement("OutputImageURI");
					in_Image->setContent(input_image);
					out_Image->setContent(output_image);
					page_frame = child->makeChildElement("BLOCK");
					temp = (char *)malloc(5000 * sizeof(char));
					pageframe_cord = page_frame->makeChildElement("topLx");
					sprintf(temp,"%d",toplx);
					printf("%s\t",temp);
					pageframe_cord->setContent(temp);
					free(temp);
					pageframe_cord = page_frame->makeChildElement("topLy");
					temp = (char *)malloc(5000 * sizeof(char));
					sprintf(temp,"%d",toply);
					printf("%s\t",temp);
					pageframe_cord->setContent(temp);
					free(temp);
					pageframe_cord = page_frame->makeChildElement("bottomRx");
					temp = (char *)malloc(5000 * sizeof(char));
					sprintf(temp,"%d",btmrx);
					printf("%s\t",temp);
					pageframe_cord->setContent(temp);
					free(temp);
					pageframe_cord = page_frame->makeChildElement("bottomRy");
					temp = (char *)malloc(5000 * sizeof(char));
					sprintf(temp,"%d",btmry);
					printf("%s\t",temp);
					pageframe_cord->setContent(temp);
					free(temp);
					
					
				}
				else if(strcmp(childtype,"TextBlock")==0)
				{
				   
				   temp = (char *)malloc(5000 * sizeof(char));
					sprintf(temp, "%d", number_para);
							child->setAttribute ("TotalNumber", temp);
							free(temp);
					XMLelement *in_Image = child->makeChildElement("InputImageURI");
					XMLelement *out_Image = child->makeChildElement("OutputImageURI");
					in_Image->setContent(input_image);
					out_Image->setContent(output_image);
					page_frame = child->makeChildElement("BLOCK");
					temp = (char *)malloc(5000 * sizeof(char));
					pageframe_cord = page_frame->makeChildElement("topLx");
					sprintf(temp,"%d",toplx);
					printf("%s\t",temp);
					pageframe_cord->setContent(temp);
					free(temp);
					pageframe_cord = page_frame->makeChildElement("topLy");
					temp = (char *)malloc(5000 * sizeof(char));
					sprintf(temp,"%d",toply);
					printf("%s\t",temp);
					pageframe_cord->setContent(temp);
					free(temp);
					pageframe_cord = page_frame->makeChildElement("bottomRx");
					temp = (char *)malloc(5000 * sizeof(char));
					sprintf(temp,"%d",btmrx);
					printf("%s\t",temp);
					pageframe_cord->setContent(temp);
					free(temp);
					pageframe_cord = page_frame->makeChildElement("bottomRy");
					temp = (char *)malloc(5000 * sizeof(char));
					sprintf(temp,"%d",btmry);
					printf("%s\t",temp);
					pageframe_cord->setContent(temp);
					free(temp);
					nblock = 1;
					for(int i=0;i<npar;i++)
					{
						if(newparagraph[i].start!= NULL )	
						{
						  
						    block = child->makeChildElement("BLOCK");
						    temp = (char *)malloc(5000 * sizeof(char));
							    sprintf(temp, "%d", nblock++);
							    block->setAttribute("Number",temp);
							    free(temp);
						  
						    segmented_cord = block->makeChildElement("topLx");
						    temp = (char *)malloc(5000 * sizeof(char));
						    toplx = newparagraph[i].xmin;
						    sprintf(temp,"%d",toplx);
						    segmented_cord->setContent(temp);
						    free(temp);
						    segmented_cord = block->makeChildElement("topLy");
						    temp = (char *)malloc(5000 * sizeof(char));
						    toply = newparagraph[i].ymin;
						    sprintf(temp,"%d",toply);
						    segmented_cord->setContent(temp);
						    free(temp);
						    segmented_cord = block->makeChildElement("bottomRx");
						    temp = (char *)malloc(5000 * sizeof(char));
						    btmrx =  newparagraph[i].xmax;
						    sprintf(temp,"%d",btmrx);
						    segmented_cord->setContent(temp);
						    free(temp);
						    segmented_cord = block->makeChildElement("bottomRy");
						    temp = (char *)malloc(5000 * sizeof(char));
						    btmry =  newparagraph[i].ymax;
						    sprintf(temp,"%d",btmry);
						    segmented_cord->setContent(temp);
						    free(temp);
						}
					}
				}
			}
			
		}
	}
	
	xmlParser->dumpInFile(outputxml);
	xmlParser->verifyAgainstSchema ("./schema_ocr.xsd");
}

