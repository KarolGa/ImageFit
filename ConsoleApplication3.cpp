#include "stdafx.h"
#include "photodata.h""
#include "photoaction.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;


int main() {
	PhotoData dataRot1, dataRot2, dataPng1, dataPng2;
	PhotoAction action;
	int objNumb = 1;

	Mat_<int> image1gray, image2gray;
	image1gray = imread("img1.png", CV_LOAD_IMAGE_GRAYSCALE), image2gray = imread("img2.png", CV_LOAD_IMAGE_GRAYSCALE);
	dataPng1.pngToImg(image1gray); 	dataPng2.pngToImg(image2gray);
		action.binarize(dataPng1); action.binarize(dataPng2);
		action.findContours(dataPng1); action.findContours(dataPng2);
		action.markContour(dataPng1); action.markContour(dataPng2);
		action.findMaxDiagonal(dataPng1, objNumb); 	action.findMaxDiagonal(dataPng2, objNumb);
		cout << "Photo 1:" << endl;	action.findOrinentation(dataPng1);
		cout << "Photo 2:" << endl; action.findOrinentation(dataPng2);
		double angle = dataPng1.getOrientation() -  dataPng2.getOrientation();
		double scale = dataPng1.getMaxDiagonal() / dataPng2.getMaxDiagonal();
		cout << "Angle=" << angle << " scale=" << scale << endl;
	action.rotate(dataPng2, dataRot2, objNumb, angle);
	cout << "PhotoPng1: bb" << endl;
	action.findBoundingBox(dataPng1, objNumb);
	cout << "PhotoPngRot2 bb:" << endl;
	action.findBoundingBox(dataRot2, objNumb, 0, 3 * dataRot2.imgWidth, 0, 3 * dataRot2.imgHeight);
	int xTranslation = dataPng1.getBoundRight() - dataRot2.getBoundRight();
	int yTranslation = (dataPng1.getBoundDown() - dataRot2.getBoundDown());
	cout << " x translation=" << xTranslation << "; y translation=" << yTranslation << endl;
	
	Mat image1color = imread("img1.png", CV_LOAD_IMAGE_UNCHANGED), image2color = imread("img2.png", CV_LOAD_IMAGE_UNCHANGED);
	if (!image1color.data || !image2color.data)                              // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}
	namedWindow("Image 1", WINDOW_AUTOSIZE);// Creates a window for display.
	imshow("Image 1", image1color);  // Shows our image inside it.
	namedWindow("Image 2", WINDOW_AUTOSIZE);// Creates a window for display.
	imshow("Image 2", image2color);  // Shows our image inside it.
		Point2f pc(image2color.cols / 2, image2color.rows / 2), bb(dataRot2.getBoundRight(), dataRot2.getBoundDown());
		Mat matrixRotate = getRotationMatrix2D(pc, angle, 1);
		warpAffine(image2color, image2color, matrixRotate, image2color.size());
		namedWindow("Image2Rotated", WINDOW_AUTOSIZE);
		imshow("Image2Rotated", image2color);
	Mat matMove = (Mat_<double>(2, 3) << 1, 0, xTranslation, 0, 1, yTranslation);
	warpAffine(image2color, image2color, matMove, image2color.size());
	namedWindow("Image2Moved", WINDOW_AUTOSIZE);
	imshow("Image2Moved", image2color);
		Mat matrixScale = getRotationMatrix2D(bb, 0, scale);
		warpAffine(image2color, image2color, matrixScale, image2color.size());
		namedWindow("Image2Scaled", WINDOW_AUTOSIZE);
		imshow("Image2Scaled", image2color);

	//Tu uzyje metody, ktora wywali czarne miejsca na zdjeciu i zapisze finalny obraz

	waitKey(0);
	cout << "Confirm closing application by entering any character" << endl;
	int whatev;
	cin >> whatev;
	return 0;
}