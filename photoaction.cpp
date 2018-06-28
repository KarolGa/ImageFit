#include "stdafx.h"
#include "photoaction.h"

PhotoAction::PhotoAction(){}

PhotoAction::~PhotoAction(){}

void PhotoAction::binarize(PhotoData &data) {
	for (int i = 0; i < imgHeight; i++) {
	for (int j = 0; j < imgWidth; j++) {
		if (data.getImg(j, i) > 254 || data.getImg(j, i)<1) data.setImg(j, i, 0);
		else data.setImg(j, i, 1);
	}
	}
}

void PhotoAction::findContours(PhotoData &data) { 
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

void PhotoAction::markContour(PhotoData &data) {
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

void PhotoAction::findMaxDiagonal(PhotoData &data, int objNum) { 
	double diag, maxDiag = 0, wDiag, hDiag, maxWDiag = 0, maxHDiag = 0;
	//objNum += 2; // objects start with 3
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

void PhotoAction::findOrinentation(PhotoData &data) { 
	double rad = atan(data.getHeightDiagonal() / data.getWidthDiagonal());  // radians
	double ang = rad * 180 / M_PI; // radians to degrees conversion 
	data.setOrientation(ang);
}

void PhotoAction::rotate(PhotoData &dataSrc, PhotoData &dataDst, double angle) {
	double rotatedX, rotatedY;
	double rad = angle * M_PI / 180;
	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++){
			rotatedX = (j - imgWidth / 2) * cos(rad) - (i - imgHeight / 2)*sin(rad) + imgWidth / 2;
			rotatedY = (j - imgWidth / 2) * sin(rad) + (i - imgHeight / 2)*cos(rad) + imgHeight / 2;
			//cout << "X=" << dataSrc.getContourX(i) << " Y=" << dataSrc.getContourY(i) << " rotated X=" << rotatedContourX << " rotated Y=" << rotatedContourY << " objNum=" << objNum <<endl;
			dataDst.setImg(ceil(rotatedX) + imgWidth, ceil(rotatedY) + imgHeight, dataSrc.getImg(j, i));
			dataDst.setImg(ceil(rotatedX) + imgWidth, floor(rotatedY) + imgHeight, dataSrc.getImg(j, i));
			dataDst.setImg(floor(rotatedX) + imgWidth, ceil(rotatedY) + imgHeight, dataSrc.getImg(j, i));
			dataDst.setImg(floor(rotatedX) + imgWidth, floor(rotatedY) + imgHeight, dataSrc.getImg(j, i));

	}
	}
}
void PhotoAction::rotate(PhotoData &dataSrc, PhotoData &dataDst, int objNum, double angle) {
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

void PhotoAction::findBoundingBox(PhotoData &data, int objNum) { 
	int maxLeft = imgWidth, maxRight = 0, maxUp = imgHeight, maxDown = 0;
	//objNum += 2;
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
}

void PhotoAction::findBoundingBox(PhotoData &data, int objNum, int xLeft, int xRight, int yUp, int yDown) { //finds x,y, width and height of object's bounding box
	int maxLeft = xRight, maxRight = xLeft, maxUp = yDown, maxDown = yUp;
	//objNum += 2;
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
}

void PhotoAction::move(PhotoData &dataSrc, PhotoData &dataDst, int x, int y)
{
	for (int i = 0; i < 3 * imgHeight; i++) {
	for (int j = 0; j < 3 * imgWidth; j++) {
		if(j + x >= 0 && j + x < 3*imgWidth && i + y >= 0 && i + y < 3*imgHeight ){
		dataDst.setImg(j + x, i + y, dataSrc.getImg(j, i));
		}
	}
	}

}

void PhotoAction::scale(PhotoData & dataSrc, PhotoData & dataDst, double scale, int x, int y){
	double xScaled, yScaled;
	cout << "Scaling" << endl;
	cout << "x=" << x << " y=" << y << " scale=" << scale << endl;
	for (int i = 0; i < 3 * imgHeight; i++) {
	for (int j = 0; j < 3 * imgWidth; j++) {
		xScaled = x + (j-x)*scale;
		yScaled = y + (i-y)*scale;
		if (xScaled > 0 && xScaled < 3 * imgWidth && yScaled > 0 && yScaled < 3 * imgHeight && dataSrc.getImg(j,i)!=0) {
		//cout << "x=" << j << " x scaled=" << xScaled << " y=" << i << " y scaled=" << yScaled << endl;
			dataDst.setImg(ceil(xScaled), ceil(yScaled), dataSrc.getImg(j,i));
			dataDst.setImg(ceil(xScaled), floor(yScaled), dataSrc.getImg(j, i));
			dataDst.setImg(floor(xScaled), ceil(yScaled), dataSrc.getImg(j, i));
			dataDst.setImg(floor(xScaled), floor(yScaled), dataSrc.getImg(j, i));
		}
	}
	}
}

int PhotoAction::calculateError(PhotoData & data1, PhotoData & data2) {
	double correctPixels = 0;
	for (int i = 1; i < imgHeight - 1; i++) { //this method doesnt check pixel at edges
	for (int j = 1; j < imgWidth - 1; j++) {
		if (data1.getImg(j, i) > 0 && data2.getImg(j, i) > 0 || data1.getImg(j, i) == 0 && data2.getImg(j, i) == 0) {
		correctPixels++;
		}
	}
	}
	int error =static_cast<int>(round(100 * correctPixels / (imgHeight * imgWidth)));
	return error;
}

void PhotoAction::replaceBlacks(PhotoData &data) {
	}
