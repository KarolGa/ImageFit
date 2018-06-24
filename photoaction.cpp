#include "stdafx.h"
#include <iostream>
#include "photoaction.h"
using namespace std;



PhotoAction::PhotoAction()
{
}


PhotoAction::~PhotoAction()
{
}



void PhotoAction::rgb2Gray(PhotoData &data) { // Method converts rgb photo to gray one
	int val;
	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			//Those parameters provide better bw conversion than arithmetic average, const 0.03 compensates loss, caused by floating point multiplication
			val = static_cast<int>(0.03 + (0.2989*data.getRed(j, i)) + (0.5870*data.getGreen(j, i)) + (0.1140*data.getBlue(j, i)));
			data.setImg(j, i, val);
		}
	}
}

void PhotoAction::binarize(PhotoData &data) { //converts photo to binary system, where 0 are backround pixels - whites and blacks and 1 are objects - rest of image.
	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			if (data.getImg(j, i) > 254 || data.getImg(j, i)<1) data.setImg(j, i, 0);
			else data.setImg(j, i, 1);
		}
	}
}

void PhotoAction::PhotoAction::findContours(PhotoData &data) { // function finds contours of all objects on binarized photo and saves coordinates of contour
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
				if (gotZero && gotOne) {
					data.setImg(j, i, 2);
					data.setContourCoor(j, i);
					para++;
				}
			}
		}
	}
}

void PhotoAction::markContour(PhotoData &data) {//marks inside and outside contours of all objects found stating from top
	int objNum = 3;
	bool allMarked, allObj = false;

	while (!allObj) {
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

		while (!allMarked) {
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

void PhotoAction::findMaxDiagonal(PhotoData &data, int objNum) { //finds longest diagonal on photo, gets its width and height.
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

void PhotoAction::findOrinentation(PhotoData &data) { //finds orientation using max diagonal's width and height 
	double rad = atan(data.getHeightDiagonal() / data.getWidthDiagonal());  // radians
	double ang = rad * 180 / M_PI; // radians to degrees conversion 
	data.setOrientation(ang);
	cout << "Orientation in degrees =" << ang << endl;
}

void PhotoAction::rotate(PhotoData &dataSrc, PhotoData &dataDst, int objNum, double angle) { // method rotates all pixels from dataSrc equal to objNum by an angle and saves them moved by imgWidth and imgHeight to dataDst
	int rotatedContourX, rotatedContourY;
	objNum += 2;
	angle *= -1;
	double rad = angle * M_PI / 180;
	for (int i = 0; i<dataSrc.getContourSize(); i++) {
		if (dataSrc.getImg(dataSrc.getContourX(i), dataSrc.getContourY(i)) == objNum) {
			rotatedContourX = static_cast<int>((dataSrc.getContourX(i) - imgWidth / 2) * cos(rad) - (dataSrc.getContourY(i) - imgHeight / 2)*sin(rad) + imgWidth / 2);
			rotatedContourY = static_cast<int>((dataSrc.getContourX(i) - imgWidth / 2) * sin(rad) + (dataSrc.getContourY(i) - imgHeight / 2)*cos(rad) + imgHeight / 2);
			//cout << "X=" << dataSrc.getContourX(i) << " Y=" << dataSrc.getContourY(i) << " rotated X=" << rotatedContourX << " rotated Y=" << rotatedContourY << " objNum=" << objNum <<endl;
			dataDst.setImg(rotatedContourX + imgWidth, rotatedContourY + imgHeight, objNum);
		}
	}
}

void PhotoAction::findBoundingBox(PhotoData &data, int objNum) { //finds chosen object's bounding box
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

void PhotoAction::findBoundingBox(PhotoData &data, int objNum, int xLeft, int xRight, int yUp, int yDown) { //finds x,y, width and height of object's bounding box
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

void PhotoAction::replaceBlacks() {
}
