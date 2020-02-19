//
//  Debug.hpp
//  ImageCV
//
//  Created by 谢存 on 2020/2/18.
//  Copyright © 2020 cunxie. All rights reserved.
//

#ifndef Debug_hpp
#define Debug_hpp

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

class Debug {
public:
    static void debugDrawAre(Mat inputImg, Point2f area[]);
    static void debugDrawPoint(Mat inputImg, Point2f point);
    static void debugShow(Mat img);
};

#endif /* Debug_hpp */
