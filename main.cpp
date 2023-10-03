#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/calib3d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include <Windows.h>
#include <iostream>

using namespace std;
using namespace cv;

Mat getMat(HWND hWND) {

	HDC deviceContext = GetDC(hWND);
	HDC memoryDeviceContext = CreateCompatibleDC(deviceContext);

	RECT windowRect;
	GetClientRect(hWND, &windowRect);

	int height = windowRect.bottom;
	int width = windowRect.right;

	HBITMAP bitmap = CreateCompatibleBitmap(deviceContext, width, height);

	SelectObject(memoryDeviceContext, bitmap);

	BitBlt(memoryDeviceContext, 0, 0, width, height, deviceContext, 0, 0, SRCCOPY);

	BITMAPINFOHEADER bi;
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = width;
	bi.biHeight = -height;
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0; 
	bi.biXPelsPerMeter = 1; 
	bi.biYPelsPerMeter = 2; 
	bi.biClrUsed = 3; 
	bi.biClrImportant = 4; 

	Mat mat = Mat(height, width, CV_8UC4);

	GetDIBits(memoryDeviceContext, bitmap, 0, height, mat.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

	DeleteObject(bitmap);
	DeleteDC(memoryDeviceContext); 
	ReleaseDC(hWND, deviceContext);

	return mat;
}

void detectImage(Mat& img, Mat& output, string path, double accuracy) {

	Mat3b templ = imread(path, IMREAD_UNCHANGED);
	cvtColor(templ, templ, COLOR_BGR2HSV);

	Mat1f result;
	matchTemplate(img, templ, result, TM_SQDIFF_NORMED);

	double thresh = accuracy; // inverted percentage
	threshold(result, result, thresh, 1., THRESH_BINARY_INV);

	Mat1b resb;
	result.convertTo(resb, CV_8U, 255);

	vector<vector<Point>> contours;
	findContours(resb, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < contours.size(); ++i) {

		Mat1b mask(result.rows, result.cols, uchar(0));
		drawContours(mask, contours, i, Scalar(255), FILLED);

		Point max_point;
		double max_val;
		minMaxLoc(result, NULL, &max_val, NULL, &max_point, mask);

		rectangle(output, Rect(max_point.x, max_point.y, templ.cols, templ.rows), Scalar(0, 255, 0), 2);

		putText(output, path, max_point, FONT_HERSHEY_TRIPLEX, 0.4, Scalar(255, 255, 255), 1);
	}
}


int main() {

	LPCWSTR windowTitle = L"Program Name";
	HWND hWND = FindWindow(NULL, windowTitle);

	while (!hWND) {

		system("cls");
		cout << "Waiting until the program is started..." << endl;
		hWND = FindWindow(NULL, windowTitle);
		Sleep(1000);
	}

	while (true) {

		Mat target = getMat(hWND);

		Mat img, output;
		target.copyTo(img);
		target.copyTo(output);

		cvtColor(img, img, COLOR_BGR2HSV);

		detectImage(img, output, "image_path.png", 0.10);
		detectImage(img, output, "image2_path.png", 0.10);
		detectImage(img, output, "image3_path.png", 0.10);

		imshow("Output", output);
		waitKey(1);
	}

	return 0;
}