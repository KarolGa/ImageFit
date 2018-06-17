#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <sstream>
using namespace std;

class PhotoData{

protected:
	int gPhoto[640][400];
	int contour[640][400];
	typedef int dimensions[400][5];
	dimensions * photo = new dimensions[640];
	


public:

	PhotoData() {
		memset(gPhoto, 0, 640 * 400 * sizeof(int));
		memset(contour, 0, 640 * 400 * sizeof(int));
		memset(photo, 0, 640 * 400 * 5 * sizeof(int));
	}
	
	void imRead(string im) {  // method reads rgb photo and creates photo array [width][height][5 fields - red, green, blue, x, y. Last 2 are just for checking if correct pixel was chose
		int red, gre, blu;
		double x, y;
		string line;
		ifstream myFile(im);
		if (myFile.is_open() ) {
			for (int i = 0; i < 400; i++) {
			for (int j = 0; j < 640; j++) {
				getline(myFile, line);
				istringstream istr(line);
				istr >> x >> y >> red >> gre >> blu; // x,y are here just for making sure we assign correct colors to correct fields
				// cout << "x=" << x << " j=" << j << " i=" << i << " y=" << y << " red=" << red << endl;
				x *= 1000;
				y *= 1000;
				photo[j][i][0] = red;
				photo[j][i][1] = gre;
				photo[j][i][2] = blu;
				photo[j][i][3] = static_cast<int>(x);
				photo[j][i][4] = static_cast<int>(y);
			}
			}
			myFile.close();
		}
	}
	


	void imGrayShow(int x, int y, int width, int height) {
		for (int i = y; i < y + height; i++) {
		for (int j = x; j < x + width; j++) {
			cout << gPhoto[i][j] << " ";
		}
			cout << endl;
		}
	}

	void imRgbShow(int x, int y, int width, int height) {
		for (int i = y; i < y+height; i++) {
		for (int j = x; j < x+width; j++) {
			cout << photo[j][i][0] << " " << photo[j][i][1] << " " << photo[j][i][2] << "| ";
		}
			cout << endl;
		}
	}



	void imDel() {
		delete[] photo;
	}

	void setGPhoto(int x, int y, int gray) {
		gPhoto[x][y] = gray;
	}

	int getRed(int x, int y) {
		return photo[x][y][0];
	}

	int getGreen(int x, int y) {
		return photo[x][y][1];
	}

	int getBlue(int x, int y) {
		return photo[x][y][2];
	}

};



class PhotoAction
	: protected PhotoData{
	
public:
	// Method converts rgb photo to gray one
	void rgb2Gray() {
		for (int i = 0; i < 400; i++) {
		for (int j = 0; j < 640; j++) {
			//Those parameters provide better bw conversion than arithmetic average, const 0.03 compensates loss, caused by floating point multiplication
			gPhoto[j][i] = static_cast<int>(0.03 + (0.2989*photo[j][i][0]) + (0.5870*photo[j][i][1]) + (0.1140*photo[j][i][2]));
		}
		}
	}

	void binarize() { //converts photo to binary system, where 1 are white backround pixels and 0 is rest of image - objects
		for (int i = 0; i < 400; i++) {
		for (int j = 0; j < 640; j++) {
			if (gPhoto[j][i] <= 254) gPhoto[j][i] = 0;
			else gPhoto[j][i] = 1;
		}
		}
	}

	void findContour() { // function finds countours of all objects on binarized photo and marks them a
		int flag = 2;
		bool gotZero, gotOne;
		for (int i = 0; i < 400; i++) {
		for (int j = 0; j < 640; j++) {
			if (gPhoto[j][i] > 1) cout << "Binarize photo" << endl;
			if(gPhoto[j][i]==1){
			gotZero = false, gotOne = false;
			for (int k = -1; k < 2; k++) {
			for (int l = -1; l < 2; l++) {
				if (gPhoto[j + k][i + l] == 0) gotZero = true;
				if (gPhoto[j + k][i + l] == 1) gotOne = true;
			}
			}
			if (gotZero == true && gotOne == true) contour[j][i]=1;
			}
		}
		}
	}
	

	void chooseObject(int objNum) { // chooses what object on photo we want use

	}

	void findDiagonal() { //finds longest diagonal on photo
	}

	void findOrinentation() { //finds orientation basing on diagonal's beggining and ending
	}

	void rotate() { // rotates photo basing on orientation of 2 photos
	}

	void findBoundingBox() { //finds x,y, width and height of object's bounding box
	}

	void scale() { // scales img1, so objects are same scale
	}

	void fit() { //adjust photo by fitting appexes of objetct on 2 photos

	}

	void createEmptyPh() { //creates array of 0, twice wider and higher

	}

};


int main()
{
	PhotoData *data = new PhotoData();
	PhotoAction *action = new PhotoAction();
	string im = "img1.dat";
	//data.imRead(im);
	//action.rgb2Gray();
	//data.imGrayShow(0,0,20,20); //  x1<x2, y1<y2, x2<640, y2<399
	//data.imDel();
	
	int los=1;
	cin >> los;

    return 0;
}