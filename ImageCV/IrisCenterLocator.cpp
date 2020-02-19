//
//  IrisCenterLocator.cpp
//  ImageCV
//
//  Created by 谢存 on 2020/2/18.
//  Copyright © 2020 cunxie. All rights reserved.
//

#include "IrisCenterLocator.hpp"

IrisCenterLocator* IrisCenterLocator::init() {
    int _irisRadiusRange[2];
    _irisRadiusRange[0] = 30;
    _irisRadiusRange[1] = 40;
    this -> ordinaryIrisTemplates = IrisTemplateGeneration::generateOrdinaryIrisTemplateSetWithIrisRadiusRange(_irisRadiusRange);
    this -> ordinaryWeights = IrisTemplateGeneration::getOrdinaryWeightsForTemplates();
    
    return this;
}

void IrisCenterLocator::setIrisRadiusRange(int irisRadiusRange[]) {
    int _irisRadiusRange[2];
    _irisRadiusRange[0] = irisRadiusRange[0];
    _irisRadiusRange[1] = irisRadiusRange[1];
    this -> ordinaryIrisTemplates = IrisTemplateGeneration::generateOrdinaryIrisTemplateSetWithIrisRadiusRange(_irisRadiusRange);
    this -> ordinaryWeights = IrisTemplateGeneration::getOrdinaryWeightsForTemplates();
}

Point2i IrisCenterLocator::convolutionCore(Mat grayImage, vector<Mat> templates, Mat1b mask, float windowSizeRatio, float percentile, bool debug) {
    vector<cv::Mat> convResults(templates.size());
    vector<cv::Mat> convDiff(templates.size() - 1);
    
    //用不同的卷积核对原图像进行卷积
    for (int i = 0; i < templates.size(); i++) {
        Mat convolutionResult;
        filter2D(grayImage, convolutionResult, CV_32F, templates[i]);
        //矩阵归一化
        cv::normalize(convolutionResult, convolutionResult, 0.0f, 1.0f, NORM_MINMAX,CV_32FC1);
        convResults[i] = convolutionResult.clone();
        if (i > 0) {
            convDiff[i-1] = cv::Mat(convResults[i] - convResults[i - 1]);
        }
    }
    
    // find the best center in each layer
    int squareLength = 1.0f * grayImage.cols * windowSizeRatio;
    std::vector<cv::Point2i> bestCenterInEachLayer(convDiff.size());
    std::vector<float> bestCenterInEachLayerValue(convDiff.size());
    std::vector<cv::Mat1f> bestCenterInEachLayerSurrounding(convDiff.size());
    
    for (int it = 0; it < convDiff.size(); it++) {
        Mat1f sourceImage = convDiff[it];
        vector<Point2i> localMaximas = std::get<0>(cve::imageLocalMaxima(sourceImage, 1, 1, -1, mask));
        cv::Mat1f croppedGray = cve::cropROIWithBoundaryDetection(sourceImage, cve::CvRectMakeWithCenterPointAndSize(localMaximas[0], squareLength, squareLength));
        bestCenterInEachLayerValue[it] = sourceImage(localMaximas[0].y, localMaximas[0].x);
        bestCenterInEachLayer[it] = localMaximas[0];
        bestCenterInEachLayerSurrounding[it] = croppedGray;
    }
    
    // find the best layer
    int bestIndex = 0;
    float bestScore = -1.f;
    for (int i = 0; i < bestCenterInEachLayer.size(); i++) {
        cv::Mat1f tile = bestCenterInEachLayerSurrounding[i];
        float topValue = cve::topNPercentileValueOfMat(tile, percentile, CV_SORT_DESCENDING);
        cv::Mat bwImage;
        cv::threshold(tile, bwImage, topValue, 1.0f, CV_THRESH_BINARY);
        bwImage.convertTo(bwImage, CV_8UC1);
        bwImage = cve::removeSmallBlobsExceptLargest(bwImage);
        cv::Mat nonZeroCoordinates;
        findNonZero(bwImage, nonZeroCoordinates);
        cv::Point2f enclosingCenter;
        float enclosingRadius;
        cv::minEnclosingCircle(nonZeroCoordinates, enclosingCenter, enclosingRadius);
        float concavity  = 1.0f * nonZeroCoordinates.total() / (CV_PI * enclosingRadius * enclosingRadius);
        float score = topValue * concavity;
        if (bestScore < score) {
            bestScore = score;
            bestIndex = i;
        }
    }
    
    return bestCenterInEachLayer[2];
}
/*
void IrisCenterLocator::extractAccurateTemplateParametersFromMask(float returnValue[], Mat maskImage, Point2f irisCenter, float radius) {
    vector<vector<cv::Point>> contours;
    vector<Vec4i> hierarchy;
    
    findContours( maskImage, contours, hierarchy,
                 CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE );
    
    if (contours.size() == 0) {
        return;
    }
    
    cv::Point2f leftTop = cv::Point2f(contours[0][0]), rightTop = cv::Point2f(contours[0][0]), leftBottom = cv::Point2f(contours[0][0]), rightBottom = cv::Point2f(contours[0][0]);
    for (int i = 0 ; i < contours[0].size(); i ++) {
        cv::Point2f p = cv::Point2f(contours[0][i]);
        if (cv::abs(cv::norm(irisCenter - p) - radius)<2) { // on circle edge
            if (p.x < irisCenter.x && p.y < irisCenter.y && p.y < leftTop.y) {
                leftTop = p;
            }
            if (p.x > irisCenter.x && p.y < irisCenter.y && p.y < rightTop.y) {
                rightTop = p;
            }
            if (p.x < irisCenter.x && p.y > irisCenter.y && p.y > leftBottom.y) {
                leftBottom = p;
            }
            if (p.x > irisCenter.x && p.y > irisCenter.y && p.y > rightBottom.y) {
                rightBottom = p;
            }
        }
    }
    
    float topBar = 0.5 - (irisCenter.y - (leftTop.y + rightTop.y) * 0.5)  / (2 * radius);
    float bottomBar = ((leftBottom.y + rightBottom.y) * 0.5 - irisCenter.y + radius) / (2 * radius);
    bottomBar -= 0.1; // the bottom bar is often lower than actual.
    if (bottomBar < 0) {
        bottomBar = 0.9;
    }
    
    //
    returnValue[0] = topBar;
    returnValue[1] = bottomBar;
}

Mat IrisCenterLocator::DaugmanIrisCore(Mat eyeImage, vector<Point2f> eyeContour, float irisRadius, Point2f irisCenterPoint) {
}
void IrisCenterLocator::localizeIrisCenterIn(Mat eyeImage, vector<Point2f> eyeContour, Point2f irisCenter, float irisRadius, Mat outputImage) {
    
}
*/

Point2i IrisCenterLocator::localizeIrisCenter(Mat eyeImage, Point2i searchingArea[]) {
    IrisCenterLocator locator;
    
    locator.init();
    
    Mat grayImg;
    //Converts an image from one color space to another.
    cvtColor(eyeImage, grayImg, CV_BGR2GRAY);
    
    //通过mask确定搜索区域
    Mat1b mask;
    Rect rect(searchingArea[0].x, searchingArea[0].y, searchingArea[1].x - searchingArea[0].x, searchingArea[1].y - searchingArea[0].y);
    mask = Mat::zeros(grayImg.size(), CV_8UC1);
    mask(rect).setTo(255);
    
    Point2i point = locator.convolutionCore(grayImg, locator.ordinaryIrisTemplates[0], mask, 0.33f, 0.04f, false);
    Debug::debugDrawPoint(grayImg, point);
    Debug::debugShow(grayImg);
    
    return point;
}
