#pragma once
#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace std;
using namespace cv;


class PhotoData
{
	typedef int dimensions[6000][3];
	dimensions * imgRgb = new dimensions[6000];
	typedef int dimen[6000];
	dimen * img = new dimen[6000];
	vector <int> contourX, contourY;
	double widthDiagonal, heightDiagonal, orientation;
	int boundLeft, boundRight, boundUp, boundDown;

public:
	int imgHeight, imgWidth;
	PhotoData();
	~PhotoData();
	void imRead(string im);
	void pngToImg(Mat_<int>png);
	void writeMatToFile(Mat& m, const char* filename);
	void writePngToFile();
	void imGrayShow(int x, int y, int width, int height);
	void imRgbShow(int x, int y, int width, int height);

	int getRed(int x, int y);
	int getGreen(int x, int y);
	int getBlue(int x, int y);
	void setImg(int x, int y, int val);
	int getImg(int x, int y);
	void setContourCoor(int x, int y);
	int getContourX(int pairNum);
	int getContourY(int pairNum);
	int getContourSize();
	void setWidthDiagonal(double wD);
	double getWidthDiagonal();
	void setHeightDiagonal(double hD);
	double getHeightDiagonal();
	double getMaxDiagonal();
	void setOrientation(double orient);
	double getOrientation();
	void setBoundLeft(int left);
	int getBoundLeft();
	void setBoundRight(int right);
	int getBoundRight();
	void setBoundUp(int up);
	int getBoundUp();
	void setBoundDown(int down);
	int getBoundDown();
};
