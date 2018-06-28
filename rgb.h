#pragma once
#define _USE_MATH_DEFINES
#include "photodata.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using std::cin;
using std::cout;
using std::endl;

class Rgb {
	std::vector <std::vector <int>> red;
	std::vector <std::vector <int>> green;
	std::vector <std::vector <int>> blue;
	int imgWidth=640, imgHeight=400;
public:
	Rgb();
	~Rgb();
	void imRead(std::string im); // reads rgb photo 
	void imRgbShow(int x, int y, int width, int height); //shows in console part of rgb photo
	void rgb2Gray(PhotoData &data);
	void rotate(Rgb &databDst, double angle);  // method rotates chosen pixels==objNum from dataSrc equal to objNum by an angle and saves them moved by imgWidth and imgHeight to dataDst
	void move(Rgb &dataDst, int x, int y); // translates rotated image dataSrc right by x and down y and saves thhem to dataDst
	void scale(Rgb &dataDst, double scale, int x, int y); // rescales dataSrc img where x and y are coordinates of scaling central point, works when scale <=2


	void setRgb(int x, int y, int red, int green, int blue) ;
	int getRed(int x, int y);
	int getGreen(int x, int y);
	int getBlue(int x, int y);

};