
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>

#include "Debug.hpp"
#include "IrisCenterLocalizationPreProcess.hpp"
#include "IrisCenterLocator.hpp"

using namespace std;
using namespace cv;

int main() {
    
    //输入图片
    Mat inputImg = imread("/Users/xiecun/Documents/Graduation/data/Example/origin.jpg", IMREAD_COLOR);
    
    //k-means
    IrisCenterLocalizationPreProcess::kmeans(inputImg);
    imwrite("/Users/xiecun/Documents/Graduation/data/Example/kmeans.jpg", inputImg);
    //去除连通区域
    IrisCenterLocalizationPreProcess::removeConnectedComponents(inputImg);
    imwrite("/Users/xiecun/Documents/Graduation/data/Example/removeConnectedComponents.jpg", inputImg);
    //填充凸包获得质心
    Point2f searchingArea[2];
    IrisCenterLocalizationPreProcess::fillConvexHulltoGetCentroid(inputImg, searchingArea);
    imwrite("/Users/xiecun/Documents/Graduation/data/Example/fillConvexHulltoGetCentroid.jpg", inputImg);
    //绘制搜索区域
    inputImg = imread("/Users/xiecun/Documents/Graduation/data/Example/origin.jpg", IMREAD_COLOR);
    //Debug::debugDrawSearchingAre(inputImg, searchingArea);
    //imwrite("/Users/xiecun/Documents/Graduation/data/Example/drawSearchingAre.jpg", inputImg);
    
    IrisCenterLocator locator;
    int irisRadiusRange[2];
    irisRadiusRange[0] = 33;
    irisRadiusRange[1] = 38;
    locator.setIrisRadiusRange(irisRadiusRange);
    Mat grayImg;
    //Converts an image from one color space to another.
    cvtColor(inputImg, grayImg, CV_BGR2GRAY);
    Mat1b temp;
    Point2i point = locator.convolutionCore(grayImg, locator.ordinaryIrisTemplates[0], temp, 1, 1, false);
    Debug::debugDrawPoint(grayImg, point);
    Debug::debugShow(grayImg);
    return 0;
}
