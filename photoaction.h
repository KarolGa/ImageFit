#pragma once
#define _USE_MATH_DEFINES
#include "photodata.h"
#include <cmath>

class PhotoAction
	: public PhotoData
{
public:
	PhotoAction();
	~PhotoAction();
	void binarize(PhotoData &data); // Converts photo to binary system, where 0 are backround pixels - whites and blacks and 1 are objects - rest of image.
	void findContours(PhotoData &data); // function finds contours of all objects on binarized photo, contours pixels are 1, saves coordinates of this contour
	void markContour(PhotoData &data); //marks inside and outside contours of all objects found stating from top, where first object is marked as 3,  very high pessimistic complexity 
	void findMaxDiagonal(PhotoData &data, int objNum); //finds longest diagonal on photo, gets diagonal's width and height.
	void findOrinentation(PhotoData &data); //finds orientation using max diagonal's width and height 
	void rotate(PhotoData &dataSrc, PhotoData &dataDst, int objNum, double angle);  // method rotates chosen pixels==objNum from dataSrc equal to objNum by an angle and saves them moved by imgWidth and imgHeight to dataDst
	void rotate(PhotoData &dataSrc, PhotoData &dataDst, double angle); //method rotates all pixels from dataSrc equal to objNum by an angle and saves them moved by imgWidth and imgHeight to dataDst
	void findBoundingBox(PhotoData &data, int objNum); //finds object's bounding box
	void findBoundingBox(PhotoData &data, int objNum, int xLeft, int xRight, int yUp, int yDown); //finds rotated object's bounding box
	void move(PhotoData &dataSrc, PhotoData &dataDst, int x, int y); // translates rotated image dataSrc right by x and down y and saves thhem to dataDst
	void scale(PhotoData &dataSrc, PhotoData&dataDst, double scale, int x, int y); // rescales dataSrc img where x and y are coordinates of scaling central point, works when scale <=2
	int calculateError(PhotoData & data1, PhotoData & data2); // calculats matching error of 2  photos 
	void replaceBlacks(PhotoData &data); // replaces blacks on the photo 
};
