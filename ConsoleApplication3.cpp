#include "stdafx.h"
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

class PhotoData{
	
	typedef int dimensions[400][5];
		dimensions * imgRgb = new dimensions[640];
		typedef int dimen[400];
		dimen * img = new dimen[640];
		vector <int> contourX, contourY;
		double widthDiagonal, heightDiagonal;
		double orientation;

public:
	PhotoData(){
		memset(img, 0, 640 * 400 * sizeof(int));
		memset(imgRgb, 0, 640 * 400 * 5 * sizeof(int));
	}
	
	void imRead(string im) { // method reads rgb photo and creates photo array [width][height][5 fields - red, green, blue, x, y. Last 2 are just for checking if correct pixel was chose
		int red, gre, blu;
		double x, y;
		string line;
		ifstream myFile(im);
		if (myFile.is_open() ) {
			for (int i = 0; i < 400; i++) {
			for (int j = 0; j < 640; j++) {
				getline(myFile, line);
				istringstream istr(line);
				istr >> x >> y >> red >> gre >> blu; // x,y are here just for making sure we assign correct colors to correct fields
				x *= 1000;
				y *= 1000;
				imgRgb[j][i][0] = red;
				imgRgb[j][i][1] = gre;
				imgRgb[j][i][2] = blu;
				//imgRgb[j][i][3] = static_cast<int>(x);
				//imgRgb[j][i][4] = static_cast<int>(y);
			}
			}
			myFile.close();
		}
	}
	
	void imGrayShow(int x, int y, int width, int height) { //shows part of gray or binarized image
		for (int i = y; i < y + height; i++) {
		for (int j = x; j < x + width; j++) {
			cout << img[j][i] << " ";
		}
			cout << endl;
		}
	}
	void imRgbShow(int x, int y, int width, int height) { //shows part of rgb photo
		for (int i = y; i < y+height; i++) {
		for (int j = x; j < x+width; j++) {
			cout << imgRgb[j][i][0] << " " << imgRgb[j][i][1] << " " << imgRgb[j][i][2] << "| ";
		}
			cout << endl;
		}
	}

	int getRed(int x, int y) { return imgRgb[x][y][0]; }
	int getGreen(int x, int y) { return imgRgb[x][y][1]; }
	int getBlue(int x, int y) { return imgRgb[x][y][2]; }

	void setImg(int x, int y, int val) { img[x][y] = val; }
	int getImg(int x, int y) { return img[x][y]; }

	void setContourCoor(int x, int y) {
	contourX.push_back(x);
	contourY.push_back(y);
	}
	int getContourX(int pairNum) { return contourX[pairNum]; }
	int getContourY(int pairNum) { return contourY[pairNum]; }
	int getContourSize() { 
		if (contourX.size() != contourY.size()) cout << "Wrong size of contour";
		return contourX.size(); }

	void setWidthDiagonal(double wD) { widthDiagonal = wD; }
	double getWidthDiagonal() { return widthDiagonal; }

	void setHeightDiagonal(double hD) { heightDiagonal = hD; }
	double getHeightDiagonal() { return heightDiagonal; }

	void setOrientation(double orient) {orientation = orient;}
};



class PhotoAction
	: public PhotoData {

	const double PI = 3.14159265358979323846;

public:
	void rgb2Gray(PhotoData &data) { // Method converts rgb photo to gray one
		int val;
		for (int i = 0; i < 400; i++) {
			for (int j = 0; j < 640; j++) {
				//Those parameters provide better bw conversion than arithmetic average, const 0.03 compensates loss, caused by floating point multiplication
				val = static_cast<int>(0.03 + (0.2989*data.getRed(j, i)) + (0.5870*data.getGreen(j, i)) + (0.1140*data.getBlue(j, i)));
				data.setImg(j, i, val);
			}
		}
	}

	void binarize(PhotoData &data) { //converts photo to binary system, where 0 are white backround pixels and 1 are objects - rest of image.
		for (int i = 0; i < 400; i++) {
			for (int j = 0; j < 640; j++) {
				if (data.getImg(j, i) <= 254) data.setImg(j, i, 1);
				else data.setImg(j, i, 0);
			}
		}
	}

	void findContours(PhotoData &data) { // function finds contours of all objects on binarized photo and saves coordinates of contour
		bool gotZero, gotOne;
		int para = 0;
		for (int i = 1; i < 399; i++) { //this method doesnt check pixel at edges
			for (int j = 1; j < 639; j++) {
				if (data.getImg(j, i) > 1) cout << "Binarize photo" << endl;
				if (data.getImg(j, i) == 1) {
					//checking neighburhood of pixel, if there are both one and zero values that means pixel is part of contour
					gotZero = false, gotOne = false;
					for (int k = -1; k < 2; k++) {
						for (int l = -1; l < 2; l++) {
							if (data.getImg(j + k, i + l) == 0) gotZero = true;
							if (data.getImg(j + k, i + l) == 1) gotOne = true;
						}
					}
					if (gotZero == true && gotOne == true) {
						data.setImg(j, i, 2);
						data.setContourCoor(j, i);
						para++;
					}
				}
			}
		}
	}

	void markContour(PhotoData &data) {//marks inside and outside contours of all objects found stating from top
		int objNum = 3;
		bool allMarked, allObj=false;

		while (allObj == false) {
			allObj = true;
			allMarked = false;
			for (int i = 0; i < 400; i++) {
			for (int j = 0; j < 640; j++) {
				if (data.getImg(j, i) == 2) {
					data.setImg(j, i, objNum);
					allObj = false;
					j = 640, i = 400;
				}
			}
			}
		
		while (allMarked == false) {
			allMarked = true;
			for (int i = 1; i < 399; i++) {
			for (int j = 1; j < 639; j++) {
				if (data.getImg(j, i) == objNum) {
					for (int k = -1; k < 2; k++) {
					for (int l = -1; l < 2; l++) {
						if (data.getImg(j + l, i + k) == 2) {
							data.setImg(j + l, i + k, objNum);
							allMarked = false;
						}
					}
					}
				}
			}
		}
		}
		objNum++;
		}
	}

	void findMaxDiagonal(PhotoData &data, int objNum) { //finds longest diagonal on photo, gets its width and height.
		double diag, maxDiag = 0, wDiag, hDiag, maxWDiag = 0, maxHDiag = 0;
		objNum += 2; // objects start with 3
		cout << " contour size=" << data.getContourSize();
		for (int i = 0; i < data.getContourSize(); i++) {
		for (int j = i+1; j < data.getContourSize(); j++) {
			if (data.getImg(data.getContourX(i), data.getContourY(i)) == objNum && data.getImg(data.getContourX(j), data.getContourY(j)) == objNum) {
				wDiag = abs(data.getContourX(i) - data.getContourX(j));
				hDiag = abs(data.getContourY(i) - data.getContourY(j));
				diag = sqrt((wDiag*wDiag) + (hDiag *hDiag));
				if (diag > maxDiag) {
					maxDiag = diag;
					maxWDiag = wDiag;
					maxHDiag = hDiag;
				}
			}
		}
		}
		cout << " maxDiag=" << maxDiag << " width=" << maxWDiag << " height=" << maxHDiag << endl;
		data.setWidthDiagonal(maxWDiag);
		data.setHeightDiagonal(maxHDiag);
	
	} 

	void findOrinentation(PhotoData &data) { //finds orientation using max diagonal's width and height 
		double rad=atan(data.getHeightDiagonal()/data.getWidthDiagonal());  // radians
		double ang = rad * 180 / PI; // radians to degrees conversion 
		data.setOrientation(rad);
		cout << "rad=" << rad << " angle=" << ang << endl;
	}
	
	void rotate() { // rotates photo basing on orientation of 2 photos
	}

	void findBoundingBox() { //finds x,y, width and height of object's bounding box
	}

	void scale() { // scales img1, so objects are same scale
	}

	void fit() { //adjust photo by fitting appexes of objetct on 2 photos

	} 
};

int main(int argc, char** argv)
{
	
	Mat image1, image2;
	image1 = imread("img1.png", CV_LOAD_IMAGE_COLOR);   // Read the file
	image2 = imread("img2.png", CV_LOAD_IMAGE_COLOR);

	if (!image1.data || !image2.data)                              // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	namedWindow("Image 1", WINDOW_AUTOSIZE);// Create a window for display.
	imshow("Image 1", image1);  // Show our image inside it.
	namedWindow("Image 2", WINDOW_AUTOSIZE);// Create a window for display.
	imshow("Image 2", image2);  // Show our image inside it.
		waitKey(0);
	
	PhotoData data1, data2;
	PhotoAction action1, action2;
	string im1, im2;
	unsigned int x1, x2, y1, y2;	// (x1,y1) are top left corner coordinates, x2, y2 are width and height of photo we want to show in pixels
	x1 = 100; x2 = 100; y1 = 170; y2 = 100;
	if (x1 + x2>640 || y1 + y2>640)  cout << "Przekroczyłeś rozmiar zdjęcia";

	cout << "Enter name of image 1" << endl;
	cin >> im1;
	im1 = "img1.dat";
	data1.imRead(im1);
	
	cout << "Enter name of image 2" << endl;
	cin >> im2;
	im2 = "img2.dat";
	data2.imRead(im2);

	action1.rgb2Gray(data1);
	action2.rgb2Gray(data2);

	action1.binarize(data1);
	action2.binarize(data2);

	action1.findContours(data1);
	action2.findContours(data2);

	action1.markContour(data1); 
	action2.markContour(data2);

	data1.imGrayShow(x1, y1, x2, y2);
	cout << endl << endl;
	data2.imGrayShow(x1, y1, x2, y2);
	cout << endl << endl;

	cout << "Choose object from photo 1" << endl << "Choose object from photo 2" << endl;

	/*
	unsigned int x1, x2, y1, y2;	// (x1,y1) are top left corner coordinates, x2, y2 are width and height of photo we want to show in pixels
	x1 = 100; x2 = 100; y1 = 130; y2 = 100;
	if (x1 + x2>640 || y1 + y2>640)  cout << "Przekroczyłeś rozmiar zdjęcia";
	data.imGrayShow(x1, y1, x2, y2);
	cout << endl << endl;
	*/

	action1.findMaxDiagonal(data1, 1); 
	action2.findMaxDiagonal(data2, 2); // object 2 from image 2 is object 1 from image 1 - the outside contour of hash

	action1.findOrinentation(data1);
	action2.findOrinentation(data2);
	
	Mat dst;
	Point2f pc(image2.cols / 2., image2.rows / 2.);
	Mat r = getRotationMatrix2D(pc, 45, 1);
	warpAffine(image2, dst, r, image2.size());
	imwrite("rotated_im.png", dst);

	//Mat getRotationMatrix2D(Point2f center, double angle, double scale);



	cin >> im1;
    return 0;
}