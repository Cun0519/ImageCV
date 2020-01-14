#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>

using namespace std;
using namespace cv;

int blobDetector(Mat inputImg);

int main(){
    
    //输入图片
    Mat inputImg = imread("/Users/xiecun/Documents/Graduation/data/Example/kmeans.jpg");
    
    //斑点检测
    blobDetector(inputImg)
    
    imwrite("/Users/xiecun/Documents/Graduation/data/Example/blobDetect.jpg", inputImg);
    return 0;
}

//斑点检测
int blobDetector(Mat inputImg) {
    //参数
    SimpleBlobDetector::Params params;
    //像素面积大小控制
    params.filterByArea = true;
    params.minArea = 1;
    params.maxArea = 100;
    //阈值控制
    //params.minThreshold = 10;
    //params.maxThreshold = 200;
    //形状（凸）
    //params.filterByCircularity = true;
    //params.minCircularity = 0.1;
    //形状（凹）
    //params.filterByConvexity = true;
    //params.minConvexity = 0.1;
    //形状（圆）
    //params.filterByInertia = true;
    //params.minInertiaRatio = 0.1;
    
    Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);
    
    //返回值
    int blobValue = 0;
    
    //迭代轮数
    int rounds;
    
    for (rounds = 0; rounds < 3; rounds++) {
        vector<KeyPoint> keyPoints;
        detector -> detect(inputImg, keyPoints);
        
        int i;
        for (i = 0; i < keyPoints.size(); i++) {
            int j;
            for (j = keyPoints[i].size / 2 + 1; j >= 0; j--) {
                if (keyPoints[i].pt.y - j >= 0 && keyPoints[i].pt.x - j >= 0 && keyPoints[i].pt.y + j <= inputImg.rows && keyPoints[i].pt.x + j <= inputImg.cols) {
                    //j为去除区域的半径
                    int n, m;
                    for (n = 0; n < j * 2; n++) {
                        for (m = 0; m < j * 2; m++) {
                            inputImg.at<Vec3b>(keyPoints[i].pt.y - j + n, keyPoints[i].pt.x - j + m) = Vec3b(255, 255, 255);
                        }
                    }
                    break;
                }
            }
        }
        blobValue = (int)keyPoints.size();
    }
    //返回检测到的斑点数量
    return blobValue;
}
