#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>

using namespace std;
using namespace cv;

int kmeans(Mat inputImg);
int blobDetect(Mat inputImg);

int main(){
    
    //输入图片
    Mat inputImg = imread("/Users/xiecun/Documents/Graduation/data/Example/origin.jpg");
    
    //k-means
    kmeans(inputImg);
    imwrite("/Users/xiecun/Documents/Graduation/data/Example/kmeans.jpg", inputImg);
    //斑点检测
    blobDetect(inputImg);
    imwrite("/Users/xiecun/Documents/Graduation/data/Example/blobDetect.jpg", inputImg);
    return 0;
}

//k-means
int kmeans(Mat inputImg) {
    
    if (inputImg.empty()) {
        return 0;
    }
    
    int width = inputImg.cols;
    int height = inputImg.rows;
    int sampleCount = width * height;
    int dims = inputImg.channels();
    int index = 0;
    Mat img(sampleCount, dims, CV_32F, Scalar(10));
    //RGB数据转换到样本数据
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            index = row * width + col;
            Vec3b bgr = inputImg.at<Vec3b>(row, col);
            img.at<float>(index, 0) = static_cast<int>(bgr[0]);
            img.at<float>(index, 1) = static_cast<int>(bgr[1]);
            img.at<float>(index, 2) = static_cast<int>(bgr[2]);
        }
    }
    
    //聚类簇数
    int clusterCount = 3;
    //
    Mat labels;
    //迭代停止规则
    TermCriteria criteria = TermCriteria(TermCriteria::MAX_ITER + TermCriteria::EPS, 100, 1.0);
    //尝试轮数
    int attempts = 3;
    
    //运行Kmeans
    kmeans(img, clusterCount, labels, criteria, attempts, KMEANS_RANDOM_CENTERS);
    
    //聚类后每簇的bgr值之和
    int rgbSum[clusterCount];
    for (int i = 0; i < clusterCount; i++) {
        rgbSum[i] = 0;
    }
    for (int i = 0; i < labels.rows * labels.cols; i++) {
        rgbSum[labels.at<int>(i, 0)] += img.at<float>(i, 0) + img.at<float>(i, 1) + img.at<float>(i, 2);
    }
    //找出bgr值之和的最小值
    int num = rgbSum[0];
    int flag = 0;
    for (int i = 0; i < clusterCount; i++) {
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
            int label = labels.at<int>(index, 0);
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
    //像素面积大小控制
    params.filterByArea = true;
    params.minArea = 1;
    params.maxArea = 100;
    //阈值控制
    //params.minThreshold = 10;
    //params.maxThreshold = 200;
    //形状（凸）
    //params.filterByCircularity = true;
    //params.minCircularity = 0.1;
    //形状（凹）
    //params.filterByConvexity = true;
    //params.minConvexity = 0.1;
    //形状（圆）
    //params.filterByInertia = true;
    //params.minInertiaRatio = 0.1;
    
    Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);
    
    //返回值
    int blobValue = 0;
    
    //迭代轮数
    int rounds;
    
    for (rounds = 0; rounds < 3; rounds++) {
        vector<KeyPoint> keyPoints;
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
