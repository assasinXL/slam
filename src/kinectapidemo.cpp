
// Standard Library
#include <iostream>
// OpenCV Header
#include<opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
// Kinect for Windows SDK Header
#include <Kinect.h>
#include<fstream>
using namespace cv;
using namespace std;
vector<Point> src;
int i = 0;
 
ofstream fout("01.txt");
void writeMatToFile(cv::Mat& m, Point p, const char* filename)
{
	//ofstream fout(filename);
 
	if (!fout)
	{
		cout << "File Not Opened" << endl;  return;
	}
 
	fout <<p.x<<" "<<p.y<<" "<< m.at<unsigned short>(p.x, p.y) << endl;
 
 
 
	fout.close();
}
 
//点击两下获取部分三维坐标
 
 
Point p;
void onMouse(int event, int x, int y, int flags, void *param)
{
	Mat *img = reinterpret_cast<Mat*>(param);
	if (event == CV_EVENT_LBUTTONDOWN)//左键按下，读取初始坐标，并在图像上该点处划圆  
	{
		i++;//统计点击的次数
 
	
 
			p.x = x;
			p.y = y;
			src.push_back(p);
			cout << p << static_cast<int>(img->at<unsigned short>(cv::Point(x, y))) << endl;
			cout << i << endl;
	
 
		//cout << image.at<unsigned short> << endl;
		//cout << p << static_cast<int>(img->at<uchar>(cv::Point(x, y))) << endl;
 
		//cout << img << endl;
	
			fout << p.x << " " << p.y << " " << static_cast<int>(img->at<unsigned short>(cv::Point(x, y))) << endl;
 
		
			//writeMatToFile(*img, p, "01.txt");
 
		
	}
 
}
//using namespace std;
int main(int argc, char** argv)
{
	// 1a. Get default Sensor
	IKinectSensor* pSensor = nullptr;
	GetDefaultKinectSensor(&pSensor);
	// 1b. Open sensor
	pSensor->Open();
	// 2a. Get frame source
	IDepthFrameSource* pFrameSource = nullptr;
	pSensor->get_DepthFrameSource(&pFrameSource);
	// 2b. Get frame description
	int        iWidth = 0;
	int        iHeight = 0;
	IFrameDescription* pFrameDescription = nullptr;
	pFrameSource->get_FrameDescription(&pFrameDescription);
	pFrameDescription->get_Width(&iWidth);
	pFrameDescription->get_Height(&iHeight);
	pFrameDescription->Release();
	pFrameDescription = nullptr;
	// 2c. get some dpeth only meta
	UINT16 uDepthMin = 0, uDepthMax = 0;
 
	pFrameSource->get_DepthMinReliableDistance(&uDepthMin);
	pFrameSource->get_DepthMaxReliableDistance(&uDepthMax);
	cout << "Reliable Distance: "
		<< uDepthMin << " – " << uDepthMax << endl;
	// perpare OpenCV
	cv::Mat mDepthImg(iHeight, iWidth, CV_16UC1);
	cv::Mat mImg8bit(iHeight, iWidth, CV_8UC1);
	cv::namedWindow("Depth Map");
	// 3a. get frame reader
	IDepthFrameReader* pFrameReader = nullptr;
	pFrameSource->OpenReader(&pFrameReader);
	// Enter main loop
	while (true)
	{
		// 4a. Get last frame
		IDepthFrame* pFrame = nullptr;
		if (pFrameReader->AcquireLatestFrame(&pFrame) == S_OK)
		{
			// 4c. copy the depth map to image
			pFrame->CopyFrameDataToArray(iWidth * iHeight,
				reinterpret_cast<UINT16*>(mDepthImg.data));
			// 4d. convert from 16bit to 8bit
 
 
			mDepthImg.convertTo(mImg8bit, CV_8U, 255.0f / uDepthMax);
 
			namedWindow("image", CV_WINDOW_AUTOSIZE);
			setMouseCallback("image", onMouse, &mDepthImg);
			imshow("image", mImg8bit);
 
			//cv::imshow("Depth Map", mImg8bit);
			// 4e. release frame
			pFrame->Release();
		}
		// 4f. check keyboard input
		if (cv::waitKey(30) == VK_ESCAPE){
			break;
		}
	}
	// 3b. release frame reader
	pFrameReader->Release();
	pFrameReader = nullptr;
	// 2d. release Frame source
	pFrameSource->Release();
	pFrameSource = nullptr;
	// 1c. Close Sensor
	pSensor->Close();
	// 1d. Release Sensor
	pSensor->Release();
	pSensor = nullptr;
	return 0;
}