#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
using namespace std;

class PhotoData{
	
public:
	typedef int dimensions[400][5];
		dimensions * imgRgb = new dimensions[640];
		typedef int dimen[400];
		dimen * img = new dimen[640];
		vector <int> contourX;
		vector <int> contourY;
		int objAmount;
		int wDi, hDi;
		double orientation;
		
		/*vector< vector< vector< vector<int> > > > tab;
    vector< vector< vector<int> > >tab4;
    vector< vector<int> >tab2;
    vector<int> tab3;
    tab3.push_back(5);
    tab3.push_back(2);
    tab2.push_back(tab3);
    tab4.push_back(tab2);
    tab.push_back(tab4);
    cout << tab[0][0][0][0]; //5
    cout << tab[0][0][0][1]; //2*/
		
	PhotoData(){
		memset(img, 0, 640 * 400 * sizeof(int));
		memset(imgRgb, 0, 640 * 400 * 5 * sizeof(int));
	}
	
	
	void imRead(string im) { // method reads rgb photo and creates photo array [width][height][5 fields - red, green, blue, x, y. Last 2 are just for checking if correct pixel was chose
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
				//cout << "x=" << x << " j=" << j << " i=" << i << " y=" << y << " red=" << red << endl;
				x *= 1000;
				y *= 1000;
				imgRgb[j][i][0] = red;
				imgRgb[j][i][1] = gre;
				imgRgb[j][i][2] = blu;
				imgRgb[j][i][3] = static_cast<int>(x);
				imgRgb[j][i][4] = static_cast<int>(y);
			}
			}
			myFile.close();
		}
	}
	


	void imGrayShow(int x, int y, int width, int height) { //shows part of gray or binarized image
		for (int i = y; i < y + height; i++) {
		for (int j = x; j < x + width; j++) {
			cout << img[j][i] << " ";
		}
			cout << endl;
		}
	}

	void imRgbShow(int x, int y, int width, int height) { //shows part of rgb photo
		for (int i = y; i < y+height; i++) {
		for (int j = x; j < x+width; j++) {
			cout << imgRgb[j][i][0] << " " << imgRgb[j][i][1] << " " << imgRgb[j][i][2] << "| ";
		}
			cout << endl;
		}
	}

	



	int getRed(int x, int y) { return imgRgb[x][y][0]; }

	int getGreen(int x, int y) { return imgRgb[x][y][1]; }

	int getBlue(int x, int y) { return imgRgb[x][y][2]; }

	void setImg(int x, int y, int val) { img[x][y] = val; }
	int getImg(int x, int y) { return img[x][y]; }

	void setContour(int x, int y) {
	contourX.push_back(x);
	contourY.push_back(y);
	}
	int getContourX(int pairNum) { return contourX[pairNum]; }
	int getContourY(int pairNum) { return contourY[pairNum]; }
	int getContourSize() { 
		return contourY.size(); }

	void setObjAmount(int objAmo) { objAmount = objAmo; }

	void setWDi(int wD) { wDi = wD; }
	int getWDi() { return wDi; }

	void setHDi(int hD) { hDi = hD; }
	int getHDi() { return hDi; }

	void setOrientation(double orient) {orientation = orient;}
};



class PhotoAction
	: public PhotoData {

	const double PI = 3.14159265358979323846;
	PhotoData obiekt;

public:

	// Method converts rgb photo to gray one
	void rgb2Gray(PhotoData &data) {
		int val;
		for (int i = 0; i < 400; i++) {
			for (int j = 0; j < 640; j++) {
				//Those parameters provide better bw conversion than arithmetic average, const 0.03 compensates loss, caused by floating point multiplication
				val = static_cast<int>(0.03 + (0.2989*data.getRed(j, i)) + (0.5870*data.getGreen(j, i)) + (0.1140*data.getBlue(j, i)));
				data.setImg(j, i, val);
			}
		}
	}

	void binarize(PhotoData &data) { //converts photo to binary system, where 1 are white backround pixels and 0 is rest of image - objects
		for (int i = 0; i < 400; i++) {
			for (int j = 0; j < 640; j++) {
				if (data.getImg(j, i) <= 254) data.setImg(j, i, 1);
				else data.setImg(j, i, 0);
			}
		}
	}


	
	void findContours(PhotoData &data) { // function finds contours of all objects on binarized photo and saves coordinates of contour
		bool gotZero, gotOne;
		int para = 0;
		for (int i = 1; i < 399; i++) { //this method doesnt check pixel on corners
			for (int j = 1; j < 639; j++) {
				if (data.getImg(j, i) > 1) cout << "Binarize photo" << endl;
				if (data.getImg(j, i) == 1) {
					//checking neighburhood of pixel, if there are both one and zero values that means this bixel is contour
					gotZero = false, gotOne = false;
					for (int k = -1; k < 2; k++) {
						for (int l = -1; l < 2; l++) {
							if (data.getImg(j + k, i + l) == 0) gotZero = true;
							if (data.getImg(j + k, i + l) == 1) gotOne = true;
						}
					}
					if (gotZero == true && gotOne == true) {
						data.setImg(j, i, 2);
						data.setContour(j, i);
						//cout << "wsp x kont=" << data.getContourX(para) << endl;
						para++;
					}
				}
			}
		}
	}

	
	void markContour(PhotoData &data) {//marks contours inside and outside contours of all objects found stating from top.
		int objNum = 3;
		bool allMarked, allObj=false;

		while (allObj == false) {
			allObj = true;
			allMarked = false;
			for (int i = 0; i < 400; i++) {
			for (int j = 0; j < 640; j++) {
				if (data.getImg(j, i) == 2) {
					data.setImg(j, i, objNum);
					cout << "Jestem tu" << endl;
					allObj = false;
					j = 640, i = 400;
				}
			}
			}
		
		while (allMarked == false) {
			allMarked = true;
			for (int i = 1; i < 399; i++) {
			for (int j = 1; j < 639; j++) {
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

	


	/*void chooseObject(PhotoData &data, int objNum) { // chooses what object on photo we want use and gets coordinates of object's contours, what makes finding longest diagonal faster
		int num = 0;
		bool goodObj = false;
		objNum + 2; // 1 is mark for objects, 2 is mark for conturs, objects are over 2
		if (objNum > data.objAmount) cout << "Nie ma obiektu o takim numerze, podaj numer obiektu z zakresu od 1 do " << data.objAmount;

		for (int i = 0; i < 400; i++) {
		for (int j = 0; j < 640; j++) {
			if (goodObj == false) {
				if (data.getImg(j, i) == 0) data.setImg(j, i, 0);
				else if (data.getImg(j, i) == objNum) {
					data.setImg(j, i, 1);
					goodObj = true;
				}
				else data.setImg(j, i, 0);
			}
			if (goodObj == true) {
				if(data.getImg(j, i)==0){
					data.setImg(j, i, 0);
					goodObj = false;
				}
				else if (data.getImg(j, i) == 1) data.setImg(j, i, 1); 
				else if (data.getImg(j, i) == objNum){
					data.setImg(j, i, 1);
					goodObj = false;
				}
				else cout << "Blad przy ustawianiu konturow" << endl;
				
			}
		}
		}
	}

	

	void findObjMaxDiagonal(PhotoData &data) { //finds longest diagonal on photo and gets coordinates of its ends
		int wDiag, hDiag, maxWDiag, maxHDiag;
		double diag, maxDiag = 0;
		for (int i = 0; data.getContourX(i) != 0; i++) {
		for (int j = ++i; data.getContourY(j) != 0; j++) {
			wDiag = abs(data.getContourX(i)- data.getContourX(j));
			hDiag = abs(data.getContourY(i)- data.getContourY(j));
			diag = sqrt(wDiag*wDiag + hDiag *hDiag);
			if (diag > maxDiag) {
				maxDiag = diag;
				maxWDiag = wDiag;
				maxHDiag = hDiag;
			}
		}
		}
		cout << " maxDiag=" << maxDiag;
		diagonal.setWDi(maxWDiag);
		diagonal.setHDi(maxHDiag);
	}
	
	 double deg2rad(double deg) { // degrees to radians conversion 
		double rad = deg * PI / 180;
		return rad;
	}

	double rad2deg(double rad) {// radians to degrees conversion 
		double deg = rad * 180 / PI;
		return deg;
	} 

	void findOrinentation() { //finds orientation basing on diagonal's beggining and ending
		double tang=tan(diagonal.getHDi()/diagonal.getWDi());  // radians
		//double ang = tang * 180 / pi; // radians to degrees conversion 
		setOrientation(tang);
	}
	*/
	void rotate() { // rotates photo basing on orientation of 2 photos
	}

	void findBoundingBox() { //finds x,y, width and height of object's bounding box
	}

	void scale() { // scales img1, so objects are same scale
	}

	void fit() { //adjust photo by fitting appexes of objetct on 2 photos

	} 
};

int main(int argc, char** argv)
{

	/*
	Mat image;
	image = imread("img1.png", CV_LOAD_IMAGE_COLOR);   // Read the file

	if (!image.data)                              // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	namedWindow("Display window", WINDOW_AUTOSIZE);// Create a window for display.
	imshow("Display window", image);                   // Show our image inside it.
	*/
	unsigned int x1, x2, y1, y2;
	PhotoData data;
	PhotoAction action;
	string im = "img1.dat"; // Photo 1
	data.imRead(im);
	action.rgb2Gray(data);
	x1 = 100; x2 = 100; y1 = 130; y2 = 100;
	// (x1,y1) are top left corner coordinates, x2, y2 are width and height of photo we want to show in pixels 
	if (x1+x2>640 || y1+y2>640)  cout << "Przekroczyłeś rozmiar zdjęcia";
	//data.imGrayShow(150, 80, 45, 45);
	//cout << endl << endl;

	action.binarize(data);
	data.imGrayShow(x1, y1, x2, y2); //150 lub 400, 130, 100, 50 
	cout << endl << endl;

	action.findContours(data);
	data.imGrayShow(x1, y1, x2, y2);
	cout << endl << endl;

	action.markContour(data); 
	data.imGrayShow(x1, y1, x2, y2);
	//action.chooseObject(data, 1);
	//data.imGrayShow(150, 130, 100, 50);
	//action.findObjMaxDiagonal(data);

	cin >> x1;
	waitKey(0);
    return 0;
}