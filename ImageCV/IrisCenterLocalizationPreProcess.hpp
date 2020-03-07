//
//  IrisCenterLocalizationPreProcess.hpp
//  ImageCV
//
//  Created by 谢存 on 2020/2/18.
//  Copyright © 2020 cunxie. All rights reserved.
//

#ifndef IrisCenterLocalizationPreProcess_hpp
#define IrisCenterLocalizationPreProcess_hpp

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/photo.hpp>

#include "Debug.hpp"

using namespace std;
using namespace cv;

class IrisCenterLocalizationPreProcess {
public:
    static void removeHighlights(Mat inputImg);
    static void kmeans(Mat inputImg);
    static int removeConnectedComponents(Mat inputImg);
    static Point2i getCentroid(Mat inputImg, Point2i searchingArea[]);
};

#endif /* IrisCenterLocalizationPreProcess_hpp */
