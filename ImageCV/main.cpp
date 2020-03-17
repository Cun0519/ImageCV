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
    
    // 基于当前系统的当前日期/时间
    time_t now = time(0);
    // 把 now 转换为字符串形式
    string time = ctime(&now);
    string name = "/Users/xiecun/Documents/Graduation/data/Example/" + time + ".jpg";
    
    //输入图片
    Mat inputImg = imread("/Users/xiecun/Documents/Graduation/data/Example/1.jpg", IMREAD_COLOR);
    Mat eyeImage = inputImg.clone();
    
    Point2i searchingArea[2];
    Point2i massCenter = IrisCenterLocalizationPreProcess::preProcess(inputImg, searchingArea);
    
    //通过卷积定位瞳孔中心
    IrisCenterLocator locator;
    Point2i irisCenter = locator.localizeIrisCenter(eyeImage, searchingArea);
    Debug::debugDrawCross(eyeImage, irisCenter);
    //Debug::debugShow(eyeImage);
    imwrite(name, eyeImage);
    cout << "irisCenter.x: " << irisCenter.x << "\nirisCenter.y: " << irisCenter.y << endl;
    return 0;
}
