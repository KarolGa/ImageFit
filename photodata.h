#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using std::cin;
using std::cout;
using std::endl;

class PhotoData{
	std::vector <std::vector <int>> img;
	std::vector <int> contourX, contourY;
	double widthDiagonal, heightDiagonal, orientation;
	unsigned int boundLeft, boundRight, boundUp, boundDown;
	cv::Mat_<int> imgMat;
public:
	int imgHeight, imgWidth;

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
	void setOrientation(double orient);
	void setBoundLeft(int left);
	void setBoundRight(int right);

	PhotoData();
	~PhotoData();
	void matToImg(cv::Mat_<int>png); // converts Mat_<int> to img 
	void imgToMat(); // converts img to Mat_<int>
	void flipHorizontal();
	void writeMatToFile(cv::Mat& m, const char* filename);
	void writePngToFile();
	void imGrayShow(int x, int y, int width, int height, bool isRot); //shows in console part of gray or binarized image when image is moved by its width and height (so after rotation)
	void imGrayShow(int x, int y, int width, int height);  //shows in console part of gray or binarized image
	void setBoundUp(int up);
	void setBoundDown(int down);
	double getOrientation();
	double getMaxDiagonal();
	int getBoundLeft();
	int getBoundRight();
	int getBoundUp();
	int getBoundDown();
	cv::Mat_<int> getImgMat();
};
