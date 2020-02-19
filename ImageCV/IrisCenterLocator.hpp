//
//  IrisCenterLocator.hpp
//  ImageCV
//
//  Created by 谢存 on 2020/2/18.
//  Copyright © 2020 cunxie. All rights reserved.
//

#ifndef IrisCenterLocator_hpp
#define IrisCenterLocator_hpp

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>

#include "OpenCVExtensions.h"
#include "IrisTemplateGeneration.hpp"

using namespace std;
using namespace cv;

class IrisCenterLocator {
public:
    vector< vector<Mat> > ordinaryIrisTemplates;
    vector<float> ordinaryWeights;
    
    IrisCenterLocator* init();
    void setIrisRadiusRange(int irisRadiusRange[]);
    Point2i convolutionCore(Mat grayImage, vector<Mat> templates, Mat1b mask, float windowSizeRatio, float percentile, bool debug);
    Point2i localizeIrisCenter(Mat eyeImage, Point2i searchingArea[]);
    //void extractAccurateTemplateParametersFromMask(float returnValue[], Mat maskImage, Point2f irisCenter, float radius);
    //Mat DaugmanIrisCore(Mat eyeImage, vector<Point2f> eyeContour, float irisRadius, Point2f irisCenterPoint);
    //void localizeIrisCenterIn(Mat eyeImage, vector<Point2f> eyeContour, Point2f irisCenter, float irisRadius, Mat outputImage);
};

#endif /* IrisCenterLocator_hpp */
