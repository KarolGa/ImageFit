#include "stdafx.h"
#include "photodata.h""
#include "photoaction.h"
#include "rgb.h"

int main() {
	PhotoData dataImg1, dataImg2, dataImg2c, dataRot, dataRotc, dataMov, dataMovc, dataSca, dataScac;
	PhotoAction action;
	Rgb rgb1, rgb2, rgb2c;
	int objNumb1 = 1, objNumb2 = 2;
	char okay = 'n';
	std::string imName1, imName2;
	cout << "Application has been created to analyze and compare 2 .dat photos with similar non-white objects placed on white background and translating first "; 
	cout << "image into second by rotating, moving and scaling it" << endl; 

	cout << "Insert name of 1st photo" << endl; cin >> imName1;
	cout << "Insert name of 2nd photo" << endl; cin >> imName2;
	if (imName1[imName1.length() - 1] == 'g') {
		cv::Mat_<int> image1gray, image2gray;
		image1gray = cv::imread(imName1, CV_LOAD_IMAGE_GRAYSCALE), image2gray = cv::imread(imName2, CV_LOAD_IMAGE_GRAYSCALE);
		dataImg1.matToImg(image1gray); 	dataImg2.matToImg(image2gray);
	}
	else{
		rgb1.imRead(imName1); rgb2.imRead(imName2); rgb2c.imRead(imName2);
		rgb1.rgb2Gray(dataImg1); rgb2.rgb2Gray(dataImg2); rgb2.rgb2Gray(dataImg2c);
	}
	//finding objects 
	action.binarize(dataImg1); action.binarize(dataImg2); action.binarize(dataImg2c);
	action.findContours(dataImg1); action.findContours(dataImg2);
	action.markContour(dataImg1); action.markContour(dataImg2);
	
	int x, y, width = 100, height = 100;
	while(okay!='y'){
		cout << "Choose coordinates of up left corner of part photo to show" << endl << "x="; cin >> x;
		cout << "y="; cin >> y;
		if (0 <= x < dataImg1.imgWidth - width && 0 <= y < dataImg1.imgHeight - height) {
			cout << "Photo 1:" << endl;
			dataImg1.imGrayShow(x, y, width, height);
			cout << "Photo 2:" << endl;
			dataImg2.imGrayShow(x, y, width, height);
			cout << "Did you find contour of object you are looking for? If yes insert: y, if no type: n" << endl;
			cin >> okay;
		}
		else { cout << "You chose incorrect part of image" << endl; }
	}
	
	cout << "Choose number of contour from photo 1: " << endl; cin >> objNumb1;
	cout << "Choose number of contour from photo 2: " << endl; cin >> objNumb2;
	// objNumb1 = 3; objNumb2 = 4;
	action.findMaxDiagonal(dataImg1, objNumb1); action.findMaxDiagonal(dataImg2, objNumb2);
	action.findOrinentation(dataImg1); action.findOrinentation(dataImg2);
	double angle = dataImg1.getOrientation() -  dataImg2.getOrientation();
	double scale = dataImg1.getMaxDiagonal() / dataImg2.getMaxDiagonal();
	cout << "Angle difference=" << angle << "deg" << endl << "Scale difference=" << scale << endl;

	action.findBoundingBox(dataImg1, objNumb1);
	action.rotate(dataImg2, dataRot, angle);
	action.findBoundingBox(dataRot, objNumb2, 0, 3 * dataRot.imgWidth, 0, 3 * dataRot.imgHeight);
	int xTranslation = dataImg1.getBoundRight() - (dataRot.getBoundRight() + dataRot.imgWidth);
	int yTranslation = dataImg1.getBoundDown() - (dataRot.getBoundDown() + dataRot.imgHeight);
	cout << "x translation=" << xTranslation << "; y translation=" << yTranslation << endl;
	// translating orginal photo
	action.rotate(dataImg2c, dataRotc, angle);
	action.move(dataRotc, dataMovc, xTranslation, yTranslation);
	action.scale(dataMovc, dataScac, scale, dataImg1.getBoundRight(), dataImg1.getBoundDown());
	dataImg1.imGrayShow(180, 110, 100, 150);
	dataScac.imGrayShow(180, 110, 100, 150);
	int error = action.calculateError(dataImg1, dataScac);
	cout << "Matching error is " << error  << "%" << endl;
	/*
	if (imName1[imName1.length() - 1] == 'g'){
		cout << "Lets see this translation on image?" << endl;	
		/*
		cv::Mat_<int> image1gray, image2gray;
		image1gray = cv::imread(imName1, CV_LOAD_IMAGE_GRAYSCALE), image2gray = cv::imread(imName2, CV_LOAD_IMAGE_GRAYSCALE);
		dataPng1.matToImg(image1gray); 	dataPng2.matToImg(image2gray);
		//dataPng1.imgToMat(); /// to nie chce przejść!
		cv::Mat_<int> jakis;
		jakis = dataPng1.getImgMat();
		cv::namedWindow("Image 2", cv::WINDOW_AUTOSIZE);
		imshow("Image 2", jakis);
		* /
	
		cv::Mat image1color = cv::imread("img1.png", CV_LOAD_IMAGE_UNCHANGED), image2color = cv::imread("img2.png", CV_LOAD_IMAGE_UNCHANGED);
		if (!image1color.data || !image2color.data)                              // Check for invalid input
		{
			cout << "Could not open or find the image" << std::endl;
			return -1;
		}
		cv::namedWindow("Image 1", cv::WINDOW_AUTOSIZE);// Creates a window for display.
		imshow("Image 1", image1color);  // Shows our image inside it.
		cv::namedWindow("Image 2", cv::WINDOW_AUTOSIZE);
		imshow("Image 2", image2color);

		cv::Point2f pc(image2color.cols / 2, image2color.rows / 2), bb(dataImg1.getBoundRight(), dataImg1.getBoundDown());
		cv::Mat matrixRotate = getRotationMatrix2D(pc, angle, 1);
		warpAffine(image2color, image2color, matrixRotate, image2color.size());
		cv::Mat matMove = (cv::Mat_<double>(2, 3) << 1, 0, xTranslation, 0, 1, yTranslation);
		warpAffine(image2color, image2color, matMove, image2color.size());
		cv::Mat matrixScale = getRotationMatrix2D(bb, 0, scale);
		warpAffine(image2color, image2color, matrixScale, image2color.size());
		cv::namedWindow("Image2Fitted", cv::WINDOW_AUTOSIZE);
		imshow("Image2Fitted", image2color);	
		//Tu uzyje metody, ktora wywali czarne miejsca na zdjeciu i zapisze finalny obraz
		cv::waitKey(0);
	}*/
	cout << "Confirm closing application by entering any character" << endl;
	int whatev;
	cin >> whatev;
	return 0;
}