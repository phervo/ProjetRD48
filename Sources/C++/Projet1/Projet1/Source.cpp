#define _CRT_SECURE_NO_WARNINGS
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>

using namespace cv;
using namespace std;

/// Global Variables
Mat src; Mat hsv; Mat hue;
int bins = 25;

/// Function Headers
void Hist_and_Backproj(int, void*);
void RunHist_and_BackProj(char *);
void InvertMyFace(char *);
void DetectMyFace(char *);
int DetectSkinTone(char *);
void SkinDetection(char*, char*, char*);
int EgaliserGris(String);
int EgaliserCouleur(String);
int TestSeparationHSV(char*);
int TestDectectionPeauHSV(char*);
int TestLab(char*);
int Test(char*);

int main(int argc, char *argv[])
{
	char *imagename = "images/v1.jpg";
	//InvertMyFace(imagename);
	//DetectMyFace(imagename);
	//DetectSkinTone(imagename);
	//RunHist_and_BackProj(imagename);
	//TestSeparationHSV(imagename);
	//TestDectectionPeauHSV(imagename);
	//Test(imagename);

	return 0;
}



int Test(char* imagename){
	Mat monImage = imread(imagename, CV_LOAD_IMAGE_UNCHANGED);
	Mat res;
	vector<Mat> channels;
	vector<Mat> channels2;
	split(monImage,channels);
	Mat une = channels[0];
	Mat deux = channels[1];
	Mat trois = channels[2];
	Mat unee, deuxe, troise;
	equalizeHist(une, unee);
	equalizeHist(deux, deuxe);
	equalizeHist(trois, troise);
	/*channels2[0] = unee;
	channels2[1] = deuxe;
	channels2[2] = troise;
	merge(channels2, res);
	
	String nomFenetre = "Mon image";
	namedWindow(nomFenetre, CV_WINDOW_NORMAL);
	imshow(nomFenetre, res);
	waitKey(0);
	destroyWindow(nomFenetre);*/

	//Mat imgroi = monImage(Rect(10, 20, 100, 100));
	//GaussianBlur(imgroi, imgroi, Size(5, 5), 1.2, 1.2);
	return 0;
}




int TestLab(char* imagename){
	Mat monImage = imread(imagename, CV_LOAD_IMAGE_UNCHANGED);
	Mat monImageHSV = Mat(monImage.rows, monImage.cols, CV_8UC3);

	cvtColor(monImage, monImageHSV, CV_BGR2Lab);
	Mat l, a, b;

	namedWindow("L");
	namedWindow("A");
	namedWindow("B");

	vector<Mat> channels;
	split(monImageHSV, channels);

	l = channels[0];
	a = channels[1];
	b = channels[2];
	imshow("L", l); moveWindow("L", 0, 0);
	imshow("A", a); moveWindow("A", 660, 0);
	imshow("B", b); moveWindow("B", 0, 460);

	waitKey(0);
	destroyAllWindows();
	return 0;
}



int TestSeparationHSV(char* imagename){
	Mat monImage = imread(imagename, CV_LOAD_IMAGE_UNCHANGED);
	Mat monImageHSV = Mat(monImage.rows, monImage.cols, CV_8UC3);
	Mat hue, saturation, value;
	cvtColor(monImage, monImageHSV, CV_BGR2HSV);

	namedWindow("Hue");
	namedWindow("Saturation");
	namedWindow("Value");

	vector<Mat> channels;
	split(monImageHSV, channels);

	hue = channels[0];
	saturation = channels[1];
	value = channels[2];
	imshow("Hue", hue); moveWindow("Hue", 0, 0);
	imshow("Saturation", saturation); moveWindow("Saturation", 660, 0);
	imshow("Value", value); moveWindow("Value", 0, 460);

	waitKey(0);
	destroyAllWindows();
	return 0;
}

int TestDectectionPeauHSV(char* imagename){
	Mat monImage = imread(imagename, CV_LOAD_IMAGE_UNCHANGED);
	Mat monImageHSV = Mat(monImage.rows, monImage.cols, CV_8UC3);
	cvtColor(monImage, monImageHSV, CV_BGR2HSV);
	Mat hsv_mask = Mat(monImage.rows, monImage.cols, CV_8UC1);

	Scalar hsv_min = Scalar(0, 10, 60, 0);
	Scalar hsv_max = Scalar(20, 150, 255, 0);
	inRange(monImageHSV, hsv_min, hsv_max, hsv_mask);

	namedWindow("Image");
	namedWindow("Skin");

	imshow("Image", monImage); moveWindow("Image", 0, 0);
	imshow("Skin", hsv_mask); moveWindow("Skin", 660, 0);

	waitKey(0);
	destroyAllWindows();
	return 0;
}

void RunHist_and_BackProj(char* imagename)
{
	/// Read the image
	src = imread(imagename, 1);
	/// Transform it to HSV
	cvtColor(src, hsv, COLOR_BGR2HSV);

	/// Use only the Hue value
	hue.create(hsv.size(), hsv.depth());
	int ch[] = { 0, 0 };
	mixChannels(&hsv, 1, &hue, 1, ch, 1);

	/// Create Trackbar to enter the number of bins
	const char* window_image = "Source image";
	namedWindow(window_image, CV_WINDOW_NORMAL);
	createTrackbar("* Hue  bins: ", window_image, &bins, 180, Hist_and_Backproj);
	Hist_and_Backproj(0, 0);

	/// Show the image
	imshow(window_image, src);

	/// Wait until user exits the program
	waitKey(0);
}

void Hist_and_Backproj(int, void*)
{
	MatND hist;
	int histSize = MAX(bins, 2);
	float hue_range[] = { 0, 180 };
	const float* ranges = { hue_range };

	/// Get the Histogram and normalize it
	calcHist(&hue, 1, 0, Mat(), hist, 1, &histSize, &ranges, true, false);
	normalize(hist, hist, 0, 255, NORM_MINMAX, -1, Mat());

	/// Get Backprojection
	MatND backproj;
	calcBackProject(&hue, 1, 0, hist, backproj, &ranges, 1, true);

	/// Draw the backproj
	namedWindow("BackProj", CV_WINDOW_NORMAL);
	imshow("BackProj", backproj);

	/// Draw the histogram
	int w = 400; int h = 400;
	int bin_w = cvRound((double)w / histSize);
	Mat histImg = Mat::zeros(w, h, CV_8UC3);

	for (int i = 0; i < bins; i++)
	{
		rectangle(histImg, Point(i*bin_w, h), Point((i + 1)*bin_w, h - cvRound(hist.at<float>(i)*h / 255.0)), Scalar(0, 0, 255), -1);
	}

	imshow("Histogram", histImg);

}

void InvertMyFace(char *imagename){
	IplImage* img = 0;
	int height, width, step, channels;
	uchar *data;
	int i, j, k;

	// load an image
	img = cvLoadImage(imagename);
	if (!img){
		printf("Could not load image file: %s\n", imagename);
		exit(0);
	}

	// get the image data
	height = img->height;
	width = img->width;
	step = img->widthStep;
	channels = img->nChannels;
	data = (uchar *)img->imageData;
	printf("Processing a %dx%d image with %d channels with step %d\n", height, width, channels, step);

	// create a window
	cvNamedWindow("InvertMyFace", CV_WINDOW_NORMAL);
	cvMoveWindow("InvertMyFace", 100, 100);    // to move the window to position 100 100.

	// invert the image
	for (i = 0; i<height; i++)
	for (j = 0; j<width; j++)
	for (k = 0; k<channels; k++)  //loop to read for each channel
		data[i*step + j*channels + k] = 255 - data[i*step + j*channels + k];    //inverting the image

	// show the image
	cvShowImage("InvertMyFace", img);




	// wait for a key
	cvWaitKey(0);

	// release the image
	cvReleaseImage(&img);
	cvDestroyWindow("InvertMyFace");

}

void DetectMyFace(char *imagename)
{
	//http://niksonpaul.wordpress.com/2011/04/25/face-detection-using-opencv-c/
	// image structure in opencv
	IplImage *inImg = 0;
	// face detector classifer 
	CvHaarClassifierCascade *clCascade = 0;
	CvMemStorage *mStorage = 0;
	CvSeq *faceRectSeq;

	inImg = cvLoadImage(imagename);

	mStorage = cvCreateMemStorage(0);
	clCascade = (CvHaarClassifierCascade *)cvLoad("haarcascade_frontalface_default.xml", 0, 0, 0);

	if (!inImg || !mStorage || !clCascade)
	{
		printf("Initilization error : %s", (!inImg) ? "cant load image" : (!clCascade) ?
			"cant load haar cascade" :
			"unable to locate memory storage");

		return;
	}

	faceRectSeq = cvHaarDetectObjects(inImg, clCascade, mStorage,
		1.2,
		3,
		CV_HAAR_DO_CANNY_PRUNING,
		cvSize(25, 25));

	// window name 
	const char *winName = "DetectMyFace";
	cvNamedWindow(winName, CV_WINDOW_AUTOSIZE);

	for (int i = 0; i < (faceRectSeq ? faceRectSeq->total : 0); i++)
	{

		CvRect *r = (CvRect*)cvGetSeqElem(faceRectSeq, i);
		CvPoint p1 = { r->x, r->y };
		CvPoint p2 = { r->x + r->width, r->y + r->height };

		// printf(" %d %d %d %d\n", r->x, r->y, r->width, r->height);
		cvRectangle(inImg, p1, p2, CV_RGB(0, 255, 0), 1, 4, 0);
	}

	cvShowImage(winName, inImg);
	cvWaitKey(0);
	// destroy the view window
	cvDestroyWindow(winName);

	// release the variables 
	cvReleaseImage(&inImg);
	if (clCascade) cvReleaseHaarClassifierCascade(&clCascade);
	if (mStorage)  cvReleaseMemStorage(&mStorage);
}

int DetectSkinTone(char *imagename){
	//http://answers.opencv.org/question/3300/skin-detection/
	Mat src;
	src = imread(imagename);
	if (src.empty())
		return -1;
	blur(src, src, Size(3, 3));
	Mat hsv;
	cvtColor(src, hsv, CV_BGR2HSV);
	Mat bw;
	//inRange(hsv, Scalar(0, 40, 60), Scalar(20, 150, 255), bw);
	inRange(hsv, Scalar(0, 10, 60), Scalar(20, 150, 255), bw);
	imshow("src", src);
	imshow("dst", bw);
	waitKey(0);
}

void SkinDetection(char* filePath, char* savePath, char* extension)
{
	//http://aspilham.blogspot.fr/2011/01/face-detection-based-on-skin-colour.html
}

int EgaliserGris(String momImage_nom){
	Mat monImage = imread(momImage_nom, CV_LOAD_IMAGE_UNCHANGED);

	if (monImage.empty()){
		cout << "Erreur: l'image " << momImage_nom << " n'a pas pu être chargée" << endl;
		return -1;
	}
	vector<Mat> channels;
	Mat img_hist_equalized;
	cvtColor(monImage, monImage, CV_BGR2GRAY); //change the color image to grayscale image
	equalizeHist(monImage, img_hist_equalized); //equalize the histogram

	String nomFenetre = "Mon image";
	namedWindow(nomFenetre, CV_WINDOW_NORMAL);
	imshow(nomFenetre, img_hist_equalized);
	waitKey(0);
	destroyWindow(nomFenetre);
}

int EgaliserCouleur(String momImage_nom){
	Mat monImage = imread(momImage_nom, CV_LOAD_IMAGE_UNCHANGED);

	if (monImage.empty()){
		cout << "Erreur: l'image " << momImage_nom << " n'a pas pu être chargée" << endl;
		return -1;
	}
	vector<Mat> channels;
	Mat img_hist_equalized;
	cvtColor(monImage, img_hist_equalized, CV_BGR2YCrCb); //change the color image from BGR to YCrCb format

	split(img_hist_equalized, channels); //split the image into channels

	equalizeHist(channels[0], channels[0]); //equalize histogram on the 1st channel (Y)

	merge(channels, img_hist_equalized); //merge 3 channels including the modified 1st channel into one image

	cvtColor(img_hist_equalized, img_hist_equalized, CV_YCrCb2BGR); //change the color image from YCrCb to BGR format (to display image properly)

	String nomFenetre = "Mon image";
	namedWindow(nomFenetre, CV_WINDOW_NORMAL);
	imshow(nomFenetre, img_hist_equalized);
	waitKey(0);
	destroyWindow(nomFenetre);
}

