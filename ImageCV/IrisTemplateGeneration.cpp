//
//  IrisTemplateGeneration.cpp
//  ImageCV
//
//  Created by 谢存 on 2020/2/18.
//  Copyright © 2020 cunxie. All rights reserved.
//

#include "IrisTemplateGeneration.hpp"

vector<Mat> IrisTemplateGeneration::configurableTopBottomTemplates(int aIrisRadiusRange[], float topBar, float bottomBar, bool bold) {
    static Mat diskStrelKernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    
    vector<Mat> irisTemplates;
    // makes the template and adds them to the template vector.
    for (int radius = aIrisRadiusRange[0]; radius < aIrisRadiusRange[1]; radius += 2) {
        // draw the circle.
        Mat ring = Mat::zeros(radius * 2 + 1, radius * 2 + 1, CV_32FC1);
        circle(ring, Point2f(radius, radius), radius, Scalar::all(1));
        if (bold) {
            dilate(ring, ring, diskStrelKernel);
        }
        
        // cut the top and bottom part.
        int upperShelterRow = (int) (1.0f * ring.rows * topBar);
        int bottomShelterRow = (int) (1.0f * ring.rows * bottomBar);
        ring(Range(0, upperShelterRow), Range::all()) = Scalar(0.0f);
        ring(Range(bottomShelterRow, ring.rows), Range::all()) = Scalar(0.0f);

        irisTemplates.push_back(ring);
    }
    return irisTemplates;
}

vector<float> IrisTemplateGeneration::getOrdinaryWeightsForTemplates() {
    vector<float> weights;
    weights.push_back(0.4);
//    weights.push_back(0.1);
//    weights.push_back(0.1);
//    weights.push_back(0.1);
//    weights.push_back(0.3);
    
    return weights;
}

vector< vector<Mat> > IrisTemplateGeneration::generateOrdinaryIrisTemplateSetWithIrisRadiusRange(int aIrisRadiusRange[]) {
    vector< vector<Mat> > allIrisTemplates;
    allIrisTemplates.push_back(IrisTemplateGeneration::generateIrisTemplatesStandard(aIrisRadiusRange));
    
    return allIrisTemplates;
}

//标准模板
vector<Mat> IrisTemplateGeneration::generateIrisTemplatesStandard(int aIrisRadiusRange[]) {
    static Mat diskStrelKernel = getStructuringElement(MORPH_RECT, Size(3,3));
    
    return configurableTopBottomTemplates(aIrisRadiusRange, 0.25, 0.75, true);
}
