#pragma once
#include <iostream>
#include "photodata.h"
using namespace std;


class PhotoAction
	: public PhotoData
{
	const int M_PI = 3.14159;

public:
	PhotoAction();
	~PhotoAction();

	void rgb2Gray(PhotoData &data);

	void binarize(PhotoData &data);

	void findContours(PhotoData &data);

	void markContour(PhotoData &data);

	void findMaxDiagonal(PhotoData &data, int objNum);

	void findOrinentation(PhotoData &data);

	void rotate(PhotoData &dataSrc, PhotoData &dataDst, int objNum, double angle);

	void findBoundingBox(PhotoData &data, int objNum);

	void findBoundingBox(PhotoData &data, int objNum, int xLeft, int xRight, int yUp, int yDown);

	void replaceBlacks();


};
