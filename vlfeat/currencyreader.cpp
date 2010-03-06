#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>

#include<string>
#include<iostream>
#include<sstream>
#include<fstream>
#include<iomanip>
#include<cmath>
#include<algorithm>
#include<vector>
using namespace std;

extern "C" {
//#include "vl/generic.h"
#include "vl/sift.h"
}
#include "Image.h"
//#include "Exception.h"

const int COEFFICIENT_ARRAY_WIDTH = 9;
const int MAX_CORRESPONDENCES = 50;
const int MAXDESCRIPTORS = 3000;
const int threshold = 28000;
const double PI = 3.141592654;
Image I1 ;
Image I2 ;

struct descriptor
{
	vl_sift_pix *d;
	float x1;
	float x2;
	descriptor* matches[20];
	int numMatches;
};

void convertToGrayscale(Image img, vl_sift_pix *gray );
static long imin(long a, long b) {return (a < b) ? a : b;}
int ssd( descriptor* d1, int size1, descriptor* d2, int size2 );
int sift( descriptor* d, float* pixels, Image I );				//returns the number of keypoints

vl_sift_pix* initializeImage( string name, Image& image )
{
	ifstream ifs(name.c_str(), ios_base::in | ios_base::binary) ;
    if(!ifs) {
	cout << "Could not open file";
      //throw Exception("Could not open a file") ;
    }
	ifs>>image;
	vl_sift_pix* gray = new vl_sift_pix[image.getHeight()*image.getWidth()];
	convertToGrayscale(image, gray );
	return gray;
}

int main(int argc, char *argv[]) 
{
	//for (int i=0; argv[1][i] != 0; i++)
		//image1 += argv[1][i];
	const int numTests = 10;
	string command = "convert " + (string)argv[1] + " test.ppm";
	system(command.c_str());
	string image1 = "test.ppm"; 
	vl_sift_pix* gray1 = initializeImage( image1, I1 );

	string baseName = "libnn.ppm";
	char buffer1[33];		//tens place
	char buffer2[33];		//ones place
	string testNames[numTests];
	vl_sift_pix* gray[numTests];
	Image testIms[numTests];
	for(int i=0; i < numTests; i++ )		//Assumes that numTests < 100
	{
		if( i < 10 )
			//itoa((int)0, buffer1,10);
			sprintf(buffer1, "%d", 0);
		else
			//itoa(i/10, buffer1,10);
			sprintf(buffer1, "%d", i/10);
		//itoa(i%10, buffer2,10);
		sprintf(buffer2, "%d", i%10);
		baseName[3] = buffer1[0];
		baseName[4] = buffer2[0];
		testNames[i] = baseName;
		gray[i] =  initializeImage( baseName, testIms[i] );
		convertToGrayscale(testIms[i], gray[i] );
	}
	
	descriptor* d1 = new descriptor[MAXDESCRIPTORS];
	for( int k=0; k<MAXDESCRIPTORS; k++ )				//Allocate the descriptors for the first image
	{
		d1[k].x1 = 0;
		d1[k].x2 = 0;
		d1[k].d = new vl_sift_pix[128];
		d1[k].numMatches = 0;
	}

/////////
	descriptor* dtests[numTests];
	int counts[numTests];
	int numMatch[numTests];
	int count1 = 0;
	count1 = sift( d1, gray1, I1);				//sift and store the descriptors in d1, and return the # of descriptors
	for ( int i=0; i < count1; i++ )			//Normalize the values
	{
		for ( int k=0; k < 128; k++ )
		{
			double x = 512.0 * d1[i].d[k];
			x = (x < 255.0) ? x : 255.0 ;
			d1[i].d[k] = x;
		}
	}
	for( int i=0; i < numTests; i++ )
	{
		dtests[i] = new descriptor[MAXDESCRIPTORS];
		for( int k=0; k<MAXDESCRIPTORS; k++ )				//Allocate the descriptors for the test images
		{
			dtests[i][k].x1 = 0;
			dtests[i][k].x2 = 0;
			dtests[i][k].d = new vl_sift_pix[128];
			dtests[i][k].numMatches = 0;
		}
		counts[i] = sift( dtests[i], gray[i], testIms[i]);				//sift and store the descriptors in d2, and return the # of descriptors
		for ( int j=0; j < counts[i]; j++ )			//Normalize the values of the 2nd image
		{
			for ( int k=0; k < 128; k++ )
			{
				double x = 512.0 * dtests[i][j].d[k];
				x = (x < 255.0) ? x : 255.0 ;
				dtests[i][j].d[k] = x;
			}
		}
		numMatch[i] = ssd( d1, count1, dtests[i], counts[i] );		//take the sum of squared differences
	}
	//Find the test image with the most matches
	int largestMatch = numMatch[0];
	int matchIndex = 0;
	for( int i=1; i <numTests; i++ )
	{
		if( numMatch[i] > largestMatch )
		{
			largestMatch = numMatch[i];
			matchIndex = i;
		}
	}
	bool match = false;
	if( (float)largestMatch/(float)counts[matchIndex] >= 0.075 )
		match = true;

	if(match)
	{
		switch(matchIndex)
		{
		case 0:
		case 1:
			cout<< 1;
			return 1;
		case 2:
		case 3:
			cout<< 5;
			return 5;
		case 4:
		case 5:
			cout<< 10;
			return 10;
		case 6:
		case 7:
			cout<< 20;
			return 20;
		case 8:
		case 9:
			cout<< 50;
			return 50;
		case 10:
		case 11:
			cout << 100;
			return 100;
		}
	}
	else
	{
		cout<< 0;
		return 0;
	}
			

	/*ssd( d1, count1, dtests[matchIndex], counts[matchIndex] );
	for ( int i=0; i<count1; i++ )			
	{
		for ( int j=0; j<  d1[i].numMatches; j++ )
		{
			int image1x = d1[i].x1;
			int image1y = d1[i].x2;
			int image2x = d1[i].matches[j]->x1;
			int image2y = d1[i].matches[j]->x2;

			unsigned char* pt = I1.getPixelPt( image1x, image1y);
			pt[0] = 255;
			pt[1] = 0;
			pt[2] = 0;

			unsigned char* pt2 = testIms[matchIndex].getPixelPt( image2x, image2y);
			pt2[0] = 255;
			pt2[1] = 0;
			pt2[2] = 0;
		}
	}

	string outFile = "testmatch.ppm";	
	ofstream ofile(outFile.c_str(), ios_base::out | ios_base::binary);
	ofile<<I1;
	ofile.close();

	//testNames[matchIndex].resize(testNames[matchIndex].size()-4);
	string outFile2 = "libmatch.ppm";	
	ofstream ofile2(outFile2.c_str(), ios_base::out | ios_base::binary);
	ofile2<<testIms[matchIndex];
	ofile2.close();*/



	/////////////DEallocation///////////////////

	for( int k=0; k<MAXDESCRIPTORS; k++ )				//DEallocate the descriptors for the first image
	{
		delete [] d1[k].d;
	}
	delete [] d1;

	for(int i=0; i< numTests; i++)
	{
		for( int k=0; k<MAXDESCRIPTORS; k++ )				//DEallocate the descriptors for the second image
		{
			delete [] dtests[i][k].d;
		}
			delete [] dtests[i];
	}
	return 0;
}

void convertToGrayscale(Image img, vl_sift_pix *gray )
{
	if( img.getDataSize() == img.getHeight()*img.getWidth() )		//already grayscale, since pixelsize = 1
		for ( int i=0; i<img.getHeight()*img.getWidth(); i++ )
			gray[i] = (float)*(img.getDataPt()+i);
	else															//converting rgb to grayscale
		for ( int i=0; i < img.getHeight()*img.getWidth(); i++ )
			gray[i] = (float)( 0.3 * *(img.getDataPt()+3*i) + 0.59 * *(img.getDataPt()+3*i+1) + 0.11 * *(img.getDataPt()+3*i+2) );
}


int sift( descriptor* d, float* pixels, Image I )				//returns the number of keypoints
{
	VlSiftFilt *sift = vl_sift_new(I.getWidth(),I.getHeight(),-1,3,0);
	vl_sift_process_first_octave(sift, pixels) ;			//process over the first octave
	int count = 0;
	do
	{
		vl_sift_detect(sift);
		const VlSiftKeypoint* key = vl_sift_get_keypoints(sift);
		for(int i=0; i<sift->nkeys; i++)
		{
			double angles[4];
			int numOrients = vl_sift_calc_keypoint_orientations(sift,angles,key+i);			
			for(int j=0;j<numOrients; j++)
			{
				vl_sift_calc_keypoint_descriptor(sift,(d[count].d),key+i,angles[j]);		//calculate the descriptor
				d[count].x1=(key+i)->x;
				d[count].x2=(key+i)->y;
				count++;
			}
		}
	}while( vl_sift_process_next_octave(sift) != VL_ERR_EOF );		//iterate over the rest of the octaves
	vl_sift_delete(sift);
	return count;
}


int ssd( descriptor* d1, int size1, descriptor* d2, int size2 )
{
	int totalMatches = 0;
	for ( int i=0; i < size1; i++ )
	{
		d1[i].numMatches=0;
		for ( int j=0; j < size2; j++ )
		{
			float sum = 0;
			for ( int k=0; k < 128; k++ )
			{
				sum += (float)(d1[i].d[k] - d2[j].d[k]) * (float)(d1[i].d[k] - d2[j].d[k]);		//square the differences
			}
			if ( sum < threshold )
			{
				d1[i].matches[d1[i].numMatches] = &d2[j];
				d1[i].numMatches++;
				totalMatches++;
				break;
			}
		}
	}

	return totalMatches;
}
