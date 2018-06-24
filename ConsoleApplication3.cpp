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

class PhotoData {
	typedef int dimensions[6000][3]; //maximum resultion for photo is 1920x1080
	dimensions * imgRgb = new dimensions[6000];
	typedef int dimen[6000];
	dimen * img = new dimen[6000];
	vector <int> contourX, contourY;
	double widthDiagonal, heightDiagonal, orientation;
	int boundLeft, boundRight, boundUp, boundDown;

public:
	int imgHeight, imgWidth;
	PhotoData() {
		imgWidth = 640;
		imgHeight = 400;
		memset(img, 0, 6000 * 6000 * sizeof(int));
		memset(imgRgb, 0, 6000 * 6000 * 3 * sizeof(int));
	}

	void imRead(string im) { // method reads rgb photo and creates photo array [width][height][5 fields - red, green, blue, x, y. Last 2 are just for checking if correct pixel was chose
		int red, gre, blu;
		double x, y;
		string line;
		ifstream myFile(im);
		if (myFile.is_open()) {
			for (int i = 0; i < imgHeight; i++) {
				for (int j = 0; j < imgWidth; j++) {
					getline(myFile, line);
					istringstream istr(line);
					istr >> x >> y >> red >> gre >> blu; // x,y are not used for now
					imgRgb[j][i][0] = red;
					imgRgb[j][i][1] = gre;
					imgRgb[j][i][2] = blu;
					//x *= 1000;
					//y *= 1000;
					//imgRgb[j][i][3] = static_cast<int>(x);
					//imgRgb[j][i][4] = static_cast<int>(y);
				}
			}
			myFile.close();
		}
	}
	void pngToImg(Mat_<int>png) {
		for (int i = 0; i<png.cols; i++) {
			for (int j = 0; j<png.rows; j++) {
				img[i][j] = png(j, i);
			}
		}
	}
	void writeMatToFile(Mat& m, const char* filename){ // method writes 
		ofstream fout(filename);
		if (!fout){
			cout << "File Not Opened" << endl;  return;
		}

		for (int i = 0; i<m.rows; i++){
			for (int j = 0; j<m.cols; j++){
				fout << (int)m.at<uchar>(i, j);
			}
			fout << endl;
		}
		fout.close();
	}
	void writePngToFile() {

	}

	void imGrayShow(int x, int y, int width, int height) { //shows part of gray or binarized image
		int rot;
		cout << "If you rotated photo, type 1, else type 0: ";
		cin >> rot;
		if (rot == 1) {
			x += imgWidth;
			y += imgHeight;
		}
		
		for (int i = y; i < y + height; i++) {
			for (int j = x; j < x + width; j++) {
				cout << img[j][i] << " ";
			}
			cout << endl;
		}
	}
	void imRgbShow(int x, int y, int width, int height) { //shows part of rgb photo
		for (int i = y; i < y + height; i++) {
			for (int j = x; j < x + width; j++) {
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
		return static_cast<int>(contourX.size());
	}
		void setWidthDiagonal(double wD) { widthDiagonal = wD; }
		double getWidthDiagonal() { return widthDiagonal; }
		void setHeightDiagonal(double hD) { heightDiagonal = hD; }
		double getHeightDiagonal() { return heightDiagonal; }
		double getMaxDiagonal() {
			double maxDiagonal = sqrt(widthDiagonal*widthDiagonal + heightDiagonal * heightDiagonal);
			return maxDiagonal;
		}
	void setOrientation(double orient) { orientation = orient; }
	double getOrientation() { return orientation; }
		void setBoundLeft(int left) { boundLeft = left; }
		int getBoundLeft() { return boundLeft; }
		void setBoundRight(int right) { boundRight = right; }
		int getBoundRight() { return boundRight; }
		void setBoundUp(int up) { boundUp = up; }
		int getBoundUp() { return boundUp; }
		void setBoundDown(int down) { boundDown = down; }
		int getBoundDown() { return boundDown; }
};


class PhotoAction
	: public PhotoData {

	const double PI = 3.14159265358979323846;

public:
	void rgb2Gray(PhotoData &data) { // Method converts rgb photo to gray one
		int val;
		for (int i = 0; i < imgHeight; i++) {
			for (int j = 0; j < imgWidth; j++) {
				//Those parameters provide better bw conversion than arithmetic average, const 0.03 compensates loss, caused by floating point multiplication
				val = static_cast<int>(0.03 + (0.2989*data.getRed(j, i)) + (0.5870*data.getGreen(j, i)) + (0.1140*data.getBlue(j, i)));
				data.setImg(j, i, val);
			}
		}
	}

	void binarize(PhotoData &data) { //converts photo to binary system, where 0 are backround pixels - whites and blacks and 1 are objects - rest of image.
		for (int i = 0; i < imgHeight; i++) {
			for (int j = 0; j < imgWidth; j++) {
				if (data.getImg(j, i) > 254 || data.getImg(j, i)<1) data.setImg(j, i, 0);
				else data.setImg(j, i, 1);
			}
		}
	}

	void findContours(PhotoData &data) { // function finds contours of all objects on binarized photo and saves coordinates of contour
		bool gotZero, gotOne;
		int para = 0;
		for (int i = 1; i < imgHeight - 1; i++) { //this method doesnt check pixel at edges
			for (int j = 1; j < imgWidth - 1; j++) {
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
		bool allMarked, allObj = false;

		while (allObj == false) {
			allObj = true;
			allMarked = false;
			for (int i = 0; i < imgHeight; i++) {
				for (int j = 0; j < imgWidth; j++) {
					if (data.getImg(j, i) == 2) {
						data.setImg(j, i, objNum);
						allObj = false;
						j = imgWidth, i = imgHeight;
					}
				}
			}

			while (allMarked == false) {
				allMarked = true;
				for (int i = 1; i < imgHeight - 1; i++) {
					for (int j = 1; j < imgWidth - 1; j++) {
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
		for (int i = 0; i < data.getContourSize(); i++) {
			for (int j = i + 1; j < data.getContourSize(); j++) {
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
		data.setWidthDiagonal(maxWDiag);
		data.setHeightDiagonal(maxHDiag);
	}

	void findOrinentation(PhotoData &data) { //finds orientation using max diagonal's width and height 
		double rad = atan(data.getHeightDiagonal() / data.getWidthDiagonal());  // radians
		double ang = rad * 180 / PI; // radians to degrees conversion 
		data.setOrientation(ang);
		cout << "Orientation in degrees =" << ang << endl;
	}

	void rotate(PhotoData &dataSrc, PhotoData &dataDst, int objNum, double angle) { // method rotates all pixels from dataSrc equal to objNum by an angle and saves them moved by imgWidth and imgHeight to dataDst
		int rotatedContourX, rotatedContourY;
		objNum += 2;
		angle *= -1;
		double rad = angle * PI / 180;
		for (int i = 0; i<dataSrc.getContourSize(); i++) {
			if (dataSrc.getImg(dataSrc.getContourX(i), dataSrc.getContourY(i)) == objNum){
			rotatedContourX = static_cast<int>((dataSrc.getContourX(i) - imgWidth / 2) * cos(rad) - (dataSrc.getContourY(i) - imgHeight / 2)*sin(rad) + imgWidth/2);
			rotatedContourY = static_cast<int>((dataSrc.getContourX(i) - imgWidth / 2) * sin(rad) + (dataSrc.getContourY(i) - imgHeight / 2)*cos(rad) + imgHeight/2);
			//cout << "X=" << dataSrc.getContourX(i) << " Y=" << dataSrc.getContourY(i) << " rotated X=" << rotatedContourX << " rotated Y=" << rotatedContourY << " objNum=" << objNum <<endl;
			dataDst.setImg(rotatedContourX+imgWidth, rotatedContourY+imgHeight, objNum);
			}
		}
	}

	void findBoundingBox(PhotoData &data, int objNum) { //finds chosen object's bounding box
		int maxLeft = imgWidth, maxRight = 0, maxUp = imgHeight, maxDown = 0;
		objNum += 2;
		for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			if (data.getImg(j, i) == objNum) {
				if (j < maxLeft) maxLeft = j;
				if (j > maxRight) maxRight = j;
				if (i < maxUp) maxUp = i;
				if (i > maxDown) maxDown = i;
			}
		}
		}
		data.setBoundLeft(maxLeft);
		data.setBoundRight(maxRight);
		data.setBoundUp(maxUp);
		data.setBoundDown(maxDown);
		cout << "Bounding box: xL" << maxLeft << " xR" << maxRight << " yUp" << maxUp << " yDown" << maxDown << endl;
	}

	void findBoundingBox(PhotoData &data, int objNum, int xLeft, int xRight, int yUp, int yDown) { //finds x,y, width and height of object's bounding box
		int maxLeft = xRight, maxRight = xLeft, maxUp = yDown, maxDown = yUp;
		objNum += 2;
		for (int i = yUp; i < yDown; i++) {
		for (int j = xLeft; j < xRight; j++) {
			if (data.getImg(j, i) == objNum) {
				if (j < maxLeft) maxLeft = j;
				if (j > maxRight) maxRight = j;
				if (i < maxUp) maxUp = i;
				if (i > maxDown) maxDown = i;
			}
		}
		}
		maxLeft -= imgWidth; maxRight -= imgWidth; maxUp -= imgHeight; maxDown -= imgHeight;
		data.setBoundLeft(maxLeft);
		data.setBoundRight(maxRight);
		data.setBoundUp(maxUp);
		data.setBoundDown(maxDown);
		cout << "Bounding box: xL" << maxLeft << " xR" << maxRight << " yUp" << maxUp << " yDown" << maxDown << endl;
	}

	void replaceBlacks(){
	}

};

int main(int argc, char** argv) {
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