//
//  IrisTemplateGeneration.hpp
//  ImageCV
//
//  Created by 谢存 on 2020/2/18.
//  Copyright © 2020 cunxie. All rights reserved.
//

#ifndef IrisTemplateGeneration_hpp
#define IrisTemplateGeneration_hpp

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>

#include "Debug.hpp"

using namespace std;
using namespace cv;

class IrisTemplateGeneration {
public:
    static vector<Mat> configurableTopBottomTemplates(int aIrisRadiusRange[], float topBar, float bottomBar, bool bold);
    static vector<float> getOrdinaryWeightsForTemplates();
    static vector< vector<Mat> > generateOrdinaryIrisTemplateSetWithIrisRadiusRange(int aIrisRadiusRange[]);
    static vector<Mat> generateIrisTemplatesStandard(int aIrisRadiusRange[]);
};

#endif /* IrisTemplateGeneration_hpp */
