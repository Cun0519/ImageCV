
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
    Mat eyeImage = inputImg.clone();
    
    //k-means
    IrisCenterLocalizationPreProcess::kmeans(inputImg);
    imwrite("/Users/xiecun/Documents/Graduation/data/Example/kmeans.jpg", inputImg);
    //去除连通区域
    IrisCenterLocalizationPreProcess::removeConnectedComponents(inputImg);
    imwrite("/Users/xiecun/Documents/Graduation/data/Example/removeConnectedComponents.jpg", inputImg);
    //填充凸包获得质心
    Point2i searchingArea[2];
    IrisCenterLocalizationPreProcess::fillConvexHulltoGetCentroid(inputImg, searchingArea);
    imwrite("/Users/xiecun/Documents/Graduation/data/Example/fillConvexHulltoGetCentroid.jpg", inputImg);
    
    //通过卷积定位瞳孔中心
    IrisCenterLocator locator;
    Point2i irisCenter = locator.localizeIrisCenter(eyeImage, searchingArea);
    cout << " irisCenter.x: " << irisCenter.x << " irisCenter.y: " << irisCenter.y << endl;
    
    return 0;
}
