#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>

using namespace std;
using namespace cv;

int main(){
    Mat inputImg = imread("/Users/xiecun/Documents/Graduation/data/1578538705811R_3_10_.jpg");

    //SimpleBlobDetector::Params
    //参数
    SimpleBlobDetector::Params params;
    //像素面积大小控制
    params.filterByArea = true;
    params.minArea = 1;
    params.maxArea = 50000;
    //阈值控制
    //params.minThreshold = 10;
    //params.maxThreshold = 200;
    //形状（凸）
    //params.filterByCircularity = false;
    //params.minCircularity = 0.7;
    //形状（凹）
    //params.filterByConvexity = true;
    //params.minConvexity = 0.9;
    //形状（园）
    //params.filterByInertia = false;
    //params.minInertiaRatio = 0.5;

    Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);
    vector<KeyPoint> keyPoints;
    detector -> detect(inputImg, keyPoints);
    Mat outputImg;
    drawKeypoints(inputImg, keyPoints, outputImg, Scalar(0, 0, 255));
    imshow("keyPoints", outputImg);
    waitKey(10000);
    return 0;
}
