#include <cv.h>
#include <cvaux.h>
#include <highgui.h>
#include <cxcore.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <fstream>
using namespace cv;
using namespace std;

//Some defines we left out of the book

#define CVX_RED 	CV_RGB(0xff, 0 ,0)
#define CVX_BLACK	CV_RGB(0xff,0xff,0xff)
#define CVX_WHITE 	CV_RGB(0,0,0)

// TODO: change this number
const double SMALLEST_AREA = 1000.0;
const double BIGGEST_AREA = 9000.0;

int num_image = 0;


string file_name(String s, int i) {
	stringstream stream;
	stream  << s << i << ".tif";
	return stream.str();
}

IplImage* median(IplImage* src) {
	IplImage* dst = cvCloneImage(src);
	// some approx for number # 5
	cvSmooth(src, dst, CV_MEDIAN, 7);
	cvReleaseImage(&src);
	return dst;

}

IplImage* binary(IplImage* src) {
	IplImage* dst = cvCloneImage(src);

	cvAdaptiveThreshold(src, dst, // dst img
			255, // max_value = black
			CV_ADAPTIVE_THRESH_MEAN_C, // adaptive_method
			CV_THRESH_BINARY, // threshold_type
			91);
	// ASSUMPTION: FIX SIZE DUE TO DISTANCE FROM CAMERA

	cvReleaseImage(&src);
	return dst;

}

IplImage* inverse(IplImage* src) {
	IplImage* dst = cvCloneImage(src);

	cvThreshold(src, dst, // dst img
			100, // max_value = black
			255, // adaptive_method
			CV_THRESH_BINARY_INV);

	cvReleaseImage(&src);
	return dst;

}

IplImage* contour(IplImage* src) {


	IplImage* clone = cvCloneImage(src);
	IplImage* temp_img = cvCreateImage(cvGetSize(src), 8, 1);

	CvMemStorage* storage = cvCreateMemStorage();
	CvSeq* first_contour = NULL;


	int n1 = cvFindContours(src,
			storage,
			&first_contour, // output
			sizeof(CvContour),
			CV_RETR_LIST);

	num_image = 0;

	cout << n1 << endl;

	for (CvSeq* c = first_contour; c != NULL; c = c->h_next) {


			// make black image
			//cvZero(temp_img);
		    CvBox2D box;

		    int count = c->total; // This is number point in contour

	        // Number point must be more than or equal to 6 (for cvFitEllipse_32f).
	        if( count < 6 )
	            continue;

			CvMat* points_f = cvCreateMat(1, count, CV_32FC2);

			CvMat points_i = cvMat(1, count, CV_32SC2, points_f->data.ptr);

			cvCvtSeqToArray(c, points_f->data.ptr, CV_WHOLE_SEQ);

			cvConvert( &points_i, points_f );

			// Fits ellipse to current contour.
			box = cvFitEllipse2(points_f);


			if (box.size.height * box.size.width > SMALLEST_AREA) {

				cvSetImageROI(clone, cvRect((int) box.center.x - (box.size.width
						/ 2), (int) box.center.y - (box.size.height / 2),
						(int) box.size.width, (int) box.size.height));

				IplImage* canvas = cvCloneImage(clone);

				/* copy subimage */
				cvCopy(clone, canvas, NULL);

				// always reset the Region of Interest
				cvResetImageROI(clone);

				cvSaveImage(file_name("image", num_image).c_str(), canvas);

				cvReleaseImage(&canvas);
				num_image++;
			}

//			cvDrawContours(temp_img, c,
//					CVX_BLACK,
//					CVX_BLACK ,
//					0,
//					-1, // FILL
//					8);
//
//			regionprops(temp_img);
//
//			// TODO: remove this in final code
//			cout << num_image << " " << area << endl;
//
//			temp_img = inverse(temp_img);
//			// save imagine to file for further classifier

	}

	//cvReleaseStorage(&storage);
	cvReleaseImage(&temp_img);
	cvReleaseImage(&src);
	return clone;
}

void output(int i) {
	cout << endl << "value=" << i << endl;
}



int main(int argc, char** argv) {

	// input a file in argument
	// load image file in gray scale
	// two step in one step
	IplImage* img = 0;

	img = cvLoadImage(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	// filtered median
	img = median(img);
	// threshold for black or white
	img = binary(img);
	// contour
	img = contour(img);

	cvSaveImage("out.jpg", img);
	// release the imagine
	cvReleaseImage(&img);


	bool five = false;
	bool one = false;
	bool zero1 = false;
	bool zero2 = false;

	cout << num_image << endl;
	// binarization stuff goes below here

	for (int i = 0; i < num_image; i++) {
		string cmd = "tesseract " + file_name("image",i) +
				" out nobatch digits";

		system(cmd.c_str());


		// parse the output here
		ifstream fin("out.txt");
		if (!fin)
			continue;
		// get the output
		string num;
		getline(fin,num);
		fin.close();

		// it is empty
		if (num.empty())
			continue;

		// TODO: remove this
		cout << i << " " <<  num << endl;

		// match 2, it can only be 20
		if (num == "2") {
			output(20);
			return 0;
			if (five || one)
				exit(-1);
		}
		// match 5, save result, check result
		else if (num == "5") {
			if (zero1 || zero2) {
				output(50);
				return 0;
			}

			// insanity
			if (one)
				exit(-1);

			five = true;
		}
		//
		else if (num == "1") {
			one = true;
		}
		else if (num == "0") {
			if (zero1) // there could be an error in binarization here
			{
				if (!zero2)
					zero2 = true;
			}
			else
				zero1 = true;
		}
		else if (num == "20") {
			output(20);
			exit(1);
		}
		else if (num == "10") {
			one = true;
			if (zero1) // there could be an error in binarization here
			{
				if (!zero2)
					zero2 = true;
			}
			else
				zero1 = true;

		}
		else if (num == "100") {
			output(100);
			exit(1);
		}
		else if (num == "50") {
			output(50);
			exit(1);
		}
	}

	if (one && zero1 && zero2)
		output(100);
	else if (one && zero1)
		output(10);
	else if (one)
		output(1);
	else if (five && zero1)
		output(50);
	else if (five)
		output(5);
	else
		exit(-1);

	return 0;

}



