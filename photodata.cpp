#include "stdafx.h"
#include "photodata.h"
	PhotoData::PhotoData() {
	imgWidth = 640;
	imgHeight = 400;
	img.resize(3*imgWidth, std::vector<int>(3*imgHeight));
}
	PhotoData::~PhotoData(){}	
	void PhotoData::matToImg(cv::Mat_<int>png) {
		for (int i = 0; i<png.cols; i++) {
		for (int j = 0; j<png.rows; j++) {
			img[i][j] = png(j, i);
		}
		}
	}
	void PhotoData::imgToMat() {
		for (int i = 0; i<imgHeight; i++) {
			for (int j = 0; j<imgWidth; j++) {
				imgMat(j, i) = img[i][j];
			}
		}
	}
	void PhotoData::flipHorizontal()
	{
		int k;
		for (int i = 0; i < imgHeight/2; i++) {
		for (int j = 0; j < imgWidth/2; j++) {
			 k = img[j][i];
			img[j][i] = img[j][imgHeight - 1 - i ];
			img[j][imgHeight - 1 - i] = k;
		}
		}
	}
	void PhotoData::writeMatToFile(cv::Mat& m, const char* filename) {
		std::ofstream fout(filename);
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
	
	void PhotoData::imGrayShow(int x, int y, int width, int height, bool isRot) { 
		int rot;
		if (width > 100) {
			cout << "Width is too big, it is set max its maximum value = 100" << endl;
				width = 100;
		}

		if (isRot) {
			x += imgWidth;
			y += imgHeight;
		}

		for (int i = y + height -1; i >= y; i--) {
			for (int j = x; j < x + width; j++) {
				if (img[j][i] < 10) { cout << img[j][i] << " "; }
				else if (img[j][i] >= 10) cout << img[j][i];
			}
			cout << endl;
		}
		cout << endl;
	}
	void PhotoData::imGrayShow(int x, int y, int width, int height) {
		int rot;
		if (width > 100) {
			cout << "Width is too big, it is set max its maximum value = 100" << endl;
			width = 100;
		}
		for (int i = y + height - 1; i >= y; i--) {
			for (int j = x; j < x + width; j++) {
				cout << img[j][i] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}

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
	cv::Mat_<int> PhotoData::getImgMat() { return imgMat; }


