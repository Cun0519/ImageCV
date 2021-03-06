//
//  IrisCenterLocalizationPreProcess.cpp
//  ImageCV
//
//  Created by 谢存 on 2020/2/18.
//  Copyright © 2020 cunxie. All rights reserved.
//

#include "IrisCenterLocalizationPreProcess.hpp"

//画质优化
void IrisCenterLocalizationPreProcess::qualityOptimization(Mat inputImg) {
    
    CV_Assert(!inputImg.empty());

}

//preProcess
Point2i IrisCenterLocalizationPreProcess::preProcess(Mat inputImg, Point2i searchingArea[]) {
    
    CV_Assert(!inputImg.empty());
    
    const int iterationLevel = 5;
    cv::Mat1b ratedImage = cve::imageSegmentationViaIterativeKmeans(inputImg, iterationLevel);
    
    double maxAreaRatio = 0.025 * ratedImage.cols * ratedImage.cols * CV_PI / ratedImage.size().area();
    double minAreaRatio = maxAreaRatio * 0.4;
    cv::Mat irisArea;
    double currentMinArea = 1e10;
    for (int i = 2; i < iterationLevel; i++) {
        cv::Mat currentLayer = ratedImage >= i;
        double areaValue = cv::sum(currentLayer)[0] / 255.0 / ratedImage.size().area();
        if (areaValue > minAreaRatio && areaValue < maxAreaRatio && areaValue < currentMinArea) {
            irisArea = currentLayer;
            currentMinArea = areaValue;
        }
    }
    if (irisArea.empty()) {
        irisArea = ratedImage == 3;
    }
    
    irisArea = cve::removeSmallBlobsExceptLargest(irisArea);
    irisArea = cve::fillConvexHulls(irisArea);
    cv::Point2f massCenter = cve::blobMassCenter(irisArea)[0];
    
    //计算瞳孔中心搜索区域的搜索长度
    double searchingLength = sqrt(inputImg.cols);
    searchingArea[0].x = round(massCenter.x - searchingLength);
    searchingArea[0].y = round(massCenter.y - searchingLength);
    searchingArea[1].x = round(massCenter.x + searchingLength);
    searchingArea[1].y = round(massCenter.y + searchingLength);
    
    //Debug::debugDrawAre(inputImg, searchingArea);
    //Debug::debugShow(inputImg);
    
    return massCenter;
}

//k-means
void IrisCenterLocalizationPreProcess::iterationKmeans(Mat inputImg) {

    CV_Assert(!inputImg.empty());

    int index = 0;
    int width = inputImg.cols;
    int height = inputImg.rows;
    int sampleCount = width * height;
    int dims = inputImg.channels();

    //Data for clustering. An array of N-Dimensional points with float coordinates is needed.
    Mat data(sampleCount, dims, CV_32F);

    //将原始的RGB数据转换到data
    for (int row = 0; row < height; row++) {
        index = row * width;
        for (int col = 0; col < width; col++) {
            Vec3b bgr = inputImg.at<Vec3b>(row, col);
            data.ptr<float>(index)[0] = bgr[0];
            data.ptr<float>(index)[1] = bgr[1];
            data.ptr<float>(index)[2] = bgr[2];
            index++;
        }
    }

    int k = 3;
    Mat bestLabels;
    TermCriteria criteria = TermCriteria(TermCriteria::MAX_ITER + TermCriteria::EPS, 100, 1.0);
    int attempts = 3;
    int flags = KMEANS_RANDOM_CENTERS;

    kmeans(data, k, bestLabels, criteria, attempts, flags);

    //聚类后每簇的bgr值之和
    int rgbSum[k];
    for (int i = 0; i < k; i++) {
        rgbSum[i] = 0;
    }
    for (int i = 0; i < bestLabels.rows * bestLabels.cols; i++) {
        rgbSum[bestLabels.at<int>(i, 0)] += data.ptr<float>(i)[0] + data.ptr<float>(i)[1] + data.ptr<float>(i)[2];
    }
    //找出bgr值之和的最小值
    int num = rgbSum[0];
    int flag = 0;
    for (int i = 0; i < k; i++) {
        if (rgbSum[i] < num) {
            num = rgbSum[i];
            flag = i;
        }
    }

    //显示图像分割结果
    //把样本数据点转换回去
    for (int row = 0; row < height; row++) {
        index = row * width;
        for (int col = 0; col < width; col++) {
            int label = bestLabels.at<int>(index, 0);
            if (label == flag) {
                inputImg.ptr<Vec3b>(row)[col] = Vec3b(255, 255, 255);
            } else {
                inputImg.ptr<Vec3b>(row)[col] = Vec3b(0, 0, 0);
            }
            index++;
        }
    }
}

//去除连通区域
int IrisCenterLocalizationPreProcess::removeConnectedComponents(Mat inputImg) {

    CV_Assert(!inputImg.empty());

    /*
     第一轮去除虹膜外部连通域
     */

    //灰度图
    Mat grayImg;

    //Converts an image from one color space to another.
    cvtColor(inputImg, grayImg, CV_BGR2GRAY);

    Mat image;
    image = grayImg;

    //destination labeled image
    Mat labels;
    //statistics output for each label, including the background label, see below for available statistics. Statistics are accessed via stats(label, COLUMN) where COLUMN is one of ConnectedComponentsTypes. The data type is CV_32S.
    Mat stats;

    //centroid output for each label, including the background label. Centroids are accessed via centroids(label, 0) for x and centroids(label, 1) for y. The data type CV_64F.
    Mat centroids;

    //computes the connected components labeled image of boolean image and also produces a statistics output for each label
    int nums_0 = connectedComponentsWithStats(image, labels, stats, centroids);

    //原图像大小
    int statArea_0[nums_0];
    for (int i = 0; i < nums_0; i++) {
        statArea_0[i] = stats.at<int>(i, CC_STAT_AREA);
    }
    //对连通域面积进行排序
    sort(statArea_0, statArea_0 + nums_0);
    //背景区域
    int backGroundSize = statArea_0[nums_0 - 1];
    //虹膜区域
    int irisSize = statArea_0[nums_0 - 2];
    vector<Vec3b> colors_0(nums_0);
    for(int i = 0; i < nums_0; i++ ) {
        if (stats.at<int>(i, CC_STAT_AREA) == backGroundSize) {
            //保留背景
            colors_0[i] = Vec3b(0, 0, 0);
        } else if (stats.at<int>(i, CC_STAT_AREA) == irisSize) {
            //保留核心区域
            colors_0[i] = Vec3b(255, 255, 255);
        } else {
            //第一轮去除外围连通域
            colors_0[i] = Vec3b(0, 0, 0);

        }
    }

    for( int y = 0; y < inputImg.rows; y++ ) {
        for( int x = 0; x < inputImg.cols; x++ ) {
            int label = labels.at<int>(y, x);
            CV_Assert(0 <= label && label <= nums_0);
            inputImg.at<Vec3b>(y, x) = colors_0[label];
        }
    }

    //cout << "第一轮连通域： " << nums_0 << endl;

    /*
    第二轮去除虹膜外部连通域
    */

    //Converts an image from one color space to another.
    cvtColor(inputImg, grayImg, CV_BGR2GRAY);

    threshold(grayImg, image, 0, 255, THRESH_BINARY_INV);

    int nums_1 = connectedComponentsWithStats(image, labels, stats, centroids);

    //原图像大小
    int statArea_1[nums_1];
    for (int i = 0; i < nums_1; i++) {
        statArea_1[i] = stats.at<int>(i, CC_STAT_AREA);
    }
    //对连通域面积进行排序
    sort(statArea_1, statArea_1 + nums_1);
    //背景区域
    backGroundSize = statArea_1[nums_1 - 1];
    //虹膜区域
    irisSize = statArea_1[nums_1 - 2];
    vector<Vec3b> colors_1(nums_1);
    for(int i = 0; i < nums_1; i++ ) {
        if (stats.at<int>(i, CC_STAT_AREA) == backGroundSize) {
            //保留背景
            colors_1[i] = Vec3b(0, 0, 0);
        } else if (stats.at<int>(i, CC_STAT_AREA) == irisSize) {
            //保留核心区域
            colors_1[i] = Vec3b(255, 255, 255);
        } else {
            //第二轮去除内部连通域
            colors_1[i] = Vec3b(255, 255, 255);

        }
    }

    for( int y = 0; y < inputImg.rows; y++ ) {
        for( int x = 0; x < inputImg.cols; x++ ) {
            int label = labels.at<int>(y, x);
            CV_Assert(0 <= label && label <= nums_1);
            inputImg.at<Vec3b>(y, x) = colors_1[label];
        }
    }

    return nums_0 + nums_1;
}

//获取质心区域
Point2i IrisCenterLocalizationPreProcess::getCentroid(Mat inputImg, Point2i searchingArea[]) {

    CV_Assert(!inputImg.empty());

    Mat grayImg;
    //Converts an image from one color space to another.
    cvtColor(inputImg, grayImg, CV_BGR2GRAY);

    //Detected contours. Each contour is stored as a vector of points (e.g. std::vector<std::vector<cv::Point> >).
    vector< vector <Point> > contours;

    //Contour retrieval mode
    int mode = RETR_CCOMP;

    //Contour approximation method
    int method = CHAIN_APPROX_NONE;

    //寻找轮廓
    findContours(grayImg, contours, mode, method);

    CV_Assert(contours.size() > 0);

    //Output convex hull. It is either an integer vector of indices or vector of points. In the first case, the hull elements are 0-based indices of the convex hull points in the original array (since the set of convex hull points is a subset of the original point set). In the second case, hull elements are the convex hull points themselves.
    //vector <vector <Point> > hull(contours.size());

    //填充凸包
    //Finds the convex hull of a point set.
    //convexHull(Mat(contours[0]), hull[0]);
    //fillConvexPoly(inputImg, hull[0], Scalar(255, 255, 255), LINE_8);

    //求质心
    int sumX = 0, sumY = 0;
    for (int i = 0; i < contours[0].size(); i++) {
        sumX += contours[0][i].x;
        sumY += contours[0][i].y;
    }
    Point2i centroid;
    centroid.x = round(sumX / contours[0].size());
    centroid.y = round(sumY / contours[0].size());

    //计算瞳孔中心搜索区域的搜索长度
    double searchingLength = sqrt(contourArea(contours[0])) / 4;
    searchingArea[0].x = round(centroid.x - searchingLength / 2.0);
    searchingArea[0].y = round(centroid.y - searchingLength / 2.0);
    searchingArea[1].x = round(centroid.x + searchingLength / 2.0);
    searchingArea[1].y = round(centroid.y + searchingLength / 2.0);

    return centroid;

}
