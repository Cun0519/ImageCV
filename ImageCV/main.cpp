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
    Mat inputImg = imread("/Users/xiecun/Documents/Graduation/data/Example/Light0.jpg", IMREAD_COLOR);
    Mat eyeImage = inputImg.clone();
    
    //去除高光
    IrisCenterLocalizationPreProcess::removeHighlights(inputImg);
    imwrite("/Users/xiecun/Documents/Graduation/data/Example/removeHighlights.jpg", inputImg);
    //k-means
    IrisCenterLocalizationPreProcess::kmeans(inputImg);
    imwrite("/Users/xiecun/Documents/Graduation/data/Example/kmeans.jpg", inputImg);
    //去除连通区域
    IrisCenterLocalizationPreProcess::removeConnectedComponents(inputImg);
    imwrite("/Users/xiecun/Documents/Graduation/data/Example/removeConnectedComponents.jpg", inputImg);
    //填充凸包获得质心
    Point2i searchingArea[2];
    IrisCenterLocalizationPreProcess::getCentroid(inputImg, searchingArea);
    imwrite("/Users/xiecun/Documents/Graduation/data/Example/getCentroid.jpg", inputImg);
    
    //通过卷积定位瞳孔中心
    IrisCenterLocator locator;
    Point2i irisCenter = locator.localizeIrisCenter(eyeImage, searchingArea);
    //Debug::debugDrawPoint(eyeImage, irisCenter);
    //Debug::debugShow(eyeImage);
    cout << "irisCenter.x: " << irisCenter.x << "\nirisCenter.y: " << irisCenter.y << endl;
    
    return 0;
}
