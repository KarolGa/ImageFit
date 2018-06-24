#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "photodata.h""
using namespace std;



	PhotoData::PhotoData() {
	imgWidth = 640;
	imgHeight = 400;
	memset(img, 0, 6000 * 6000 * sizeof(int));
	memset(imgRgb, 0, 6000 * 6000 * 3 * sizeof(int));
}
	PhotoData::~PhotoData()
{
}
	void PhotoData::imRead(string im) { // method reads rgb photo and creates photo array [width][height][5 fields - red, green, blue, x, y. Last 2 are just for checking if correct pixel was chose
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
	void PhotoData::pngToImg(Mat_<int>png) {
		for (int i = 0; i<png.cols; i++) {
			for (int j = 0; j<png.rows; j++) {
				img[i][j] = png(j, i);
			}
		}
	}
	void PhotoData::writeMatToFile(Mat& m, const char* filename) { // method writes 
		ofstream fout(filename);
		if (!fout) {
			cout << "File Not Opened" << endl;  return;
		}

		for (int i = 0; i<m.rows; i++) {
			for (int j = 0; j<m.cols; j++) {
				fout << (int)m.at<uchar>(i, j);
			}
			fout << endl;
		}
		fout.close();
	}
	void PhotoData::writePngToFile() {

	}

	void PhotoData::imGrayShow(int x, int y, int width, int height) { //shows part of gray or binarized image
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
	void PhotoData::imRgbShow(int x, int y, int width, int height) { //shows part of rgb photo
		for (int i = y; i < y + height; i++) {
			for (int j = x; j < x + width; j++) {
				cout << imgRgb[j][i][0] << " " << imgRgb[j][i][1] << " " << imgRgb[j][i][2] << "| ";
			}
			cout << endl;
		}
	}

	int PhotoData::getRed(int x, int y) { return imgRgb[x][y][0]; }
	int PhotoData::getGreen(int x, int y) { return imgRgb[x][y][1]; }
	int PhotoData::getBlue(int x, int y) { return imgRgb[x][y][2]; }
	void PhotoData::setImg(int x, int y, int val) { img[x][y] = val; }
	int PhotoData::getImg(int x, int y) { return img[x][y]; }
	void PhotoData::setContourCoor(int x, int y) {
		contourX.push_back(x);
		contourY.push_back(y);
	}
	int PhotoData::getContourX(int pairNum) { return contourX[pairNum]; }
	int PhotoData::getContourY(int pairNum) { return contourY[pairNum]; }
	int PhotoData::getContourSize() {
		if (contourX.size() != contourY.size()) cout << "Wrong size of contour";
		return static_cast<int>(contourX.size());
	}
	void PhotoData::setWidthDiagonal(double wD) { widthDiagonal = wD; }
	double PhotoData::getWidthDiagonal() { return widthDiagonal; }
	void PhotoData::setHeightDiagonal(double hD) { heightDiagonal = hD; }
	double PhotoData::getHeightDiagonal() { return heightDiagonal; }
	double PhotoData::getMaxDiagonal() {
		double maxDiagonal = sqrt(widthDiagonal*widthDiagonal + heightDiagonal * heightDiagonal);
		return maxDiagonal;
	}
	void PhotoData::setOrientation(double orient) { orientation = orient; }
	double PhotoData::getOrientation() { return orientation; }
	void PhotoData::setBoundLeft(int left) { boundLeft = left; }
	int PhotoData::getBoundLeft() { return boundLeft; }
	void PhotoData::setBoundRight(int right) { boundRight = right; }
	int PhotoData::getBoundRight() { return boundRight; }
	void PhotoData::setBoundUp(int up) { boundUp = up; }
	int PhotoData::getBoundUp() { return boundUp; }
	void PhotoData::setBoundDown(int down) { boundDown = down; }
	int PhotoData::getBoundDown() { return boundDown; }



