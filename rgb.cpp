#include "stdafx.h"
#include "rgb.h"

Rgb::Rgb() {
	red.resize(imgWidth, std::vector<int>(imgHeight));
	green.resize(imgWidth, std::vector<int>(imgHeight));
	blue.resize(imgWidth, std::vector<int>(imgHeight));
}
Rgb::~Rgb() {}
void Rgb::imRead(std::string im) {
	int re, gr, bl;
	double x, y;
	std::string line;
	std::ifstream myFile(im);
	if (myFile.is_open()) {
		for (int i = 0; i < imgHeight; i++) {
			for (int j = 0; j < imgWidth; j++) {
				getline(myFile, line);
				std::istringstream istr(line);
				istr >> x >> y >> re >> gr >> bl; // x,y are not used for now
				red[j][i] = re;
				green[j][i] = gr;
				blue[j][i] = bl;
			}
		}
		myFile.close();
	}
}

void Rgb::imRgbShow(int x, int y, int width, int height)
{
	for (int i = y; i < y + height; i++) {
		for (int j = x; j < x + width; j++) {
			cout << red[j][i] << " " << green[j][i] << " " << blue[j][i] << "| ";
		}
		cout << endl;
	}
}
void Rgb::rgb2Gray(PhotoData &data) {
	int val;
	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			//Those parameters provide better bw conversion than arithmetic average, const 0.03 compensates loss, caused by floating point multiplication
			val = static_cast<int>(0.03 + (0.2989*red[j][i]) + (0.5870*green[j][i]) + (0.1140*blue[j][i]));
			data.setImg(j, i, val);
		}
	}

}

void Rgb::rotate(Rgb &dataDst, double angle) {
		double rotatedX, rotatedY;
		double rad = angle * M_PI / 180;
		for (int i = 0; i < imgHeight; i++) {
			for (int j = 0; j < imgWidth; j++) {
				rotatedX = (j - imgWidth / 2) * cos(rad) - (i - imgHeight / 2)*sin(rad) + imgWidth / 2;
				rotatedY = (j - imgWidth / 2) * sin(rad) + (i - imgHeight / 2)*cos(rad) + imgHeight / 2;
				//cout << "X=" << dataSrc.getContourX(i) << " Y=" << dataSrc.getContourY(i) << " rotated X=" << rotatedContourX << " rotated Y=" << rotatedContourY << " objNum=" << objNum <<endl;
				dataDst.setRgb(ceil(rotatedX) + imgWidth, ceil(rotatedY) + imgHeight, getRed(j, i), getGreen(j,i), getBlue(j,i));
				dataDst.setRgb(ceil(rotatedX) + imgWidth, floor(rotatedY) + imgHeight, getRed(j, i), getGreen(j, i), getBlue(j, i));
				dataDst.setRgb(floor(rotatedX) + imgWidth, ceil(rotatedY) + imgHeight, getRed(j, i), getGreen(j, i), getBlue(j, i));
				dataDst.setRgb(floor(rotatedX) + imgWidth, floor(rotatedY) + imgHeight, getRed(j, i), getGreen(j, i), getBlue(j, i));
			}
		}
	}
void Rgb::move(Rgb &dataDst, int x, int y){
	for (int i = 0; i < 3 * imgHeight; i++) {
	for (int j = 0; j < 3 * imgWidth; j++) {
		if (j + x >= 0 && j + x < 3 * imgWidth && i + y >= 0 && i + y < 3 * imgHeight) {
			dataDst.setRgb(j + x, i + y, getRed(j, i), getGreen(j, i), getBlue(j, i));
		}
	}
	}
}
void Rgb::scale(Rgb &dataDst, double scale, int x, int y) {
	double xScaled, yScaled;
	cout << "Scaling" << endl;
	cout << "x=" << x << " y=" << y << " scale=" << scale << endl;
	for (int i = 0; i < 3 * imgHeight; i++) {
	for (int j = 0; j < 3 * imgWidth; j++) {
		xScaled = x + (j - x)*scale;
		yScaled = y + (i - y)*scale;
		if (xScaled > 0 && xScaled < 3 * imgWidth && yScaled > 0 && yScaled < 3 * imgHeight) {
			//cout << "x=" << j << " x scaled=" << xScaled << " y=" << i << " y scaled=" << yScaled << endl;
			dataDst.setRgb(ceil(xScaled), ceil(yScaled), getRed(j, i), getGreen(j, i), getBlue(j, i));
			dataDst.setRgb(ceil(xScaled), floor(yScaled), getRed(j, i), getGreen(j, i), getBlue(j, i));
			dataDst.setRgb(floor(xScaled), ceil(yScaled), getRed(j, i), getGreen(j, i), getBlue(j, i));
			dataDst.setRgb(floor(xScaled), floor(yScaled), getRed(j, i), getGreen(j, i), getBlue(j, i));
		}
	}
	}
}

void Rgb::setRgb(int x, int y, int re, int gr, int bl) {
	red[x][y] = re;
	green[x][y] = gr;
	blue[x][y] = bl;
}
int Rgb::getRed(int x, int y) { return red[x][y]; }
int Rgb::getGreen(int x, int y) { return green[x][y]; }
int Rgb::getBlue(int x, int y) { return blue[x][y]; }
