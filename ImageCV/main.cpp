#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

int kmeans(Mat inputImg);
int blobDetect(Mat inputImg);
int findConnectedComponents(Mat inputImg);

int main(){
    
    //输入图片
    Mat inputImg = imread("/Users/xiecun/Documents/Graduation/data/Example/origin.jpg");
    
    //k-means
    kmeans(inputImg);
    imwrite("/Users/xiecun/Documents/Graduation/data/Example/kmeans.jpg", inputImg);
    //斑点检测
    blobDetect(inputImg);
    imwrite("/Users/xiecun/Documents/Graduation/data/Example/blobDetect.jpg", inputImg);
    //寻找连通区域
    findConnectedComponents(inputImg);
    imwrite("/Users/xiecun/Documents/Graduation/data/Example/findConnectedComponents.jpg", inputImg);
    return 0;
}

//k-means
int kmeans(Mat inputImg) {
    
    if (inputImg.empty()) {
        return 0;
    }
    
    int index = 0;
    
    int width = inputImg.cols;
    int height = inputImg.rows;
    int sampleCount = width * height;
    int dims = inputImg.channels();
    
    //Data for clustering. An array of N-Dimensional points with float coordinates is needed.
    Mat data(sampleCount, dims, CV_32F, Scalar(10));
    
    //将原始的RGB数据转换到data
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            index = row * width + col;
            Vec3b bgr = inputImg.at<Vec3b>(row, col);
            data.at<float>(index, 0) = static_cast<int>(bgr[0]);
            data.at<float>(index, 1) = static_cast<int>(bgr[1]);
            data.at<float>(index, 2) = static_cast<int>(bgr[2]);
        }
    }
    
    //Number of clusters to split the set by.
    int k = 3;
    
    //Input/output integer array that stores the cluster indices for every sample.
    Mat bestLabels;
    
    //The algorithm termination criteria, that is, the maximum number of iterations and/or the desired accuracy. The accuracy is specified as criteria.epsilon. As soon as each of the cluster centers moves by less than criteria.epsilon on some iteration, the algorithm stops.
    TermCriteria criteria = TermCriteria(TermCriteria::MAX_ITER + TermCriteria::EPS, 100, 1.0);
    
    //Flag to specify the number of times the algorithm is executed using different initial labellings. The algorithm returns the labels that yield the best compactness (see the last function parameter).
    int attempts = 3;
    
    //Flag that can take values of cv::KmeansFlags
    int flags = KMEANS_RANDOM_CENTERS;
    
    //Finds centers of clusters and groups input samples around the clusters.
    kmeans(data, k, bestLabels, criteria, attempts, flags);
    
    //聚类后每簇的bgr值之和
    int rgbSum[k];
    for (int i = 0; i < k; i++) {
        rgbSum[i] = 0;
    }
    for (int i = 0; i < bestLabels.rows * bestLabels.cols; i++) {
        rgbSum[bestLabels.at<int>(i, 0)] += data.at<float>(i, 0) + data.at<float>(i, 1) + data.at<float>(i, 2);
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
    Scalar blackWhite[] = {
        Scalar(0,0,0),
        Scalar(255,255,255)
    };
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            index = row * width + col;
            int label = bestLabels.at<int>(index, 0);
            if (label == flag) {
                inputImg.at<Vec3b>(row, col)[0] = blackWhite[0][0];
                inputImg.at<Vec3b>(row, col)[1] = blackWhite[0][1];
                inputImg.at<Vec3b>(row, col)[2] = blackWhite[0][2];
            } else {
                inputImg.at<Vec3b>(row, col)[0] = blackWhite[1][0];
                inputImg.at<Vec3b>(row, col)[1] = blackWhite[1][1];
                inputImg.at<Vec3b>(row, col)[2] = blackWhite[1][2];
            }
        }
    }
    //imshow("kmeans result", inputImg);
    //waitKey(1000);
    return 1;
}

//斑点检测
int blobDetect(Mat inputImg) {
    //参数
    SimpleBlobDetector::Params params;
    
    //By area. Extracted blobs have an area between minArea (inclusive) and maxArea (exclusive).
    params.filterByArea = true;
    params.minArea = 1;
    params.maxArea = 100;
    
    Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);
    
    //返回值
    int blobValue = 0;
    
    //迭代轮数
    int rounds = 3;
    
    for (int r = 0; r < rounds; r++) {
        
        //The detected keypoints. In the second variant of the method keypoints[i] is a set of keypoints detected in images[i] .
        vector<KeyPoint> keyPoints;
        
        //Detects keypoints in an image (first variant) or image set (second variant).
        detector -> detect(inputImg, keyPoints);
        
        int i;
        for (i = 0; i < keyPoints.size(); i++) {
            int j;
            for (j = keyPoints[i].size / 2 + 1; j >= 0; j--) {
                if (keyPoints[i].pt.y - j >= 0 && keyPoints[i].pt.x - j >= 0 && keyPoints[i].pt.y + j <= inputImg.rows && keyPoints[i].pt.x + j <= inputImg.cols) {
                    //j为去除区域的半径
                    int n, m;
                    for (n = 0; n < j * 2; n++) {
                        for (m = 0; m < j * 2; m++) {
                            inputImg.at<Vec3b>(keyPoints[i].pt.y - j + n, keyPoints[i].pt.x - j + m) = Vec3b(255, 255, 255);
                        }
                    }
                    break;
                }
            }
        }
        blobValue = (int)keyPoints.size();
    }
    //返回检测到的斑点数量
    return blobValue;
}

int findConnectedComponents(Mat inputImg) {
    
    //灰度图
    Mat grayImg;
    
    //Converts an image from one color space to another.
    cvtColor(inputImg, grayImg, CV_BGR2GRAY);
    
    //the 8-bit single-channel image to be labeled
    Mat image = grayImg;
    
    //destination labeled image
    Mat labels;
    //statistics output for each label, including the background label, see below for available statistics. Statistics are accessed via stats(label, COLUMN) where COLUMN is one of ConnectedComponentsTypes. The data type is CV_32S.
    Mat stats;
    
    //centroid output for each label, including the background label. Centroids are accessed via centroids(label, 0) for x and centroids(label, 1) for y. The data type CV_64F.
    Mat centroids;
    
    //computes the connected components labeled image of boolean image and also produces a statistics output for each label
    int nums = connectedComponentsWithStats(image, labels, stats, centroids);
    
    //原图像大小
    int imgSize = inputImg.rows * inputImg.cols;
    vector<Vec3b> colors(nums);
    for(int i = 0; i < nums; i++ ) {
        if (stats.at<int>(i, cv::CC_STAT_AREA) > (imgSize / 2)) {
            //保留背景
            colors[i] = Vec3b(255, 255, 255);
        } else if (stats.at<int>(i, cv::CC_STAT_AREA) < 100) {
            //去除小连通区域
            colors[i] = Vec3b(0, 255, 0);
        } else {
            colors[i] = Vec3b(0, 0, 0);
        }
    }
    
    for( int y = 0; y < inputImg.rows; y++ ) {
        for( int x = 0; x < inputImg.cols; x++ ) {
            int label = labels.at<int>(y, x);
            CV_Assert(0 <= label && label <= nums);
            inputImg.at<Vec3b>(y, x) = colors[label];
        }
    }

    return nums;
}
