//
//  CVE_Analysis.cpp
//  ImageCV
//
//  Created by 谢存 on 2020/2/19.
//  Copyright © 2020 cunxie. All rights reserved.
//

#include "CVE_Analysis.hpp"

namespace cve {
    cv::Mat removeSmallBlobsExceptLargest(const cv::Mat & bwImage) {
        std::vector<std::vector<cv::Point2i> > contours, onlyContours(1);
        std::vector<cv::Vec4i> hierarchy;
        
        findContours( bwImage.clone(), contours, hierarchy,
                     CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE );
        std::vector<double> areas(contours.size());
        if (contours.size() >0 ){
            for(size_t i= 0 ; i < contours.size() ; i ++) {
                areas[i] = contourArea(contours[i]);
            }
            
            long biggestIndex = distance(areas.begin(), max_element(areas.begin(),areas.end()));
            onlyContours[0] =contours[biggestIndex];
            cv::Mat mask(bwImage.size(),CV_8UC1,cv::Scalar::all(0));
            cv::drawContours(mask, onlyContours, -1, cv::Scalar(255), CV_FILLED);
            return mask;
        }
        return bwImage;
    }

    cv::Mat1b imageSegmentationViaIterativeKmeans(const cv::Mat& image, int kmeansIterations, float coordinateWeight, int kmeansRepeats)  {
        
        if (coordinateWeight <= 0) {
            coordinateWeight = 0.001;
        }
        
        cv::Mat1f kmeansPoints((int)image.total(), 5, 0.0f);
        image.reshape(1,(int)image.total()).convertTo(kmeansPoints.colRange(0,3), CV_32F);
        std::vector<cv::Point2f> coords(image.total());
        for(int i = 0 ; i < image.rows; i ++)
            for(int j = 0 ; j < image.cols; j ++)
                coords[i * image.cols + j] = cv::Point2f(i,j) * coordinateWeight;
        cv::Mat(coords).reshape(1, (int)coords.size()).copyTo(kmeansPoints.colRange(3,5));
        
        cv::Mat1b ratedImage(image.size(), 0);
        cv::Mat bestLabels, centers, colorsum;
        std::vector<cv::Point2i> darkerPointIndex;
        
        for(int it = 1 ; it < kmeansIterations ; it++) {
            if (kmeansPoints.rows < 2) {
                break;
            }
            cv::kmeans(kmeansPoints, 2, bestLabels, cv::TermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, kmeansRepeats, 0.001), kmeansRepeats, cv::KMEANS_PP_CENTERS, centers);
            reduce(centers.colRange(0, 3), colorsum, 1, CV_REDUCE_SUM);
            
            darkerPointIndex.clear();
            if (colorsum.at<float>(0) < colorsum.at<float>(1)) {
                cv::findNonZero(bestLabels==0, darkerPointIndex);
            }
            else {
                cv::findNonZero(bestLabels==1, darkerPointIndex);
            }
            
            for (int i = 0 ; i < darkerPointIndex.size() ; i ++) {
                int indexInInteration = darkerPointIndex[i].y;
                int r = (int) (kmeansPoints(indexInInteration, 3) / coordinateWeight);
                int c = (int) (kmeansPoints(indexInInteration, 4) / coordinateWeight);
                ratedImage(r,c) += 1;
            }
            
            cv::Mat1f temp;
            for (int  i = 0; i <darkerPointIndex.size() ; i ++) {
                temp.push_back(kmeansPoints.row(darkerPointIndex[i].y));
            }
            temp.copyTo(kmeansPoints);
        }
        
        return ratedImage;
    }

    cv::Mat fillConvexHulls(const cv::Mat & bwImage) {
        std::vector<std::vector<cv::Point2i> > contours;
        std::vector<cv::Vec4i> hierarchy;
        
        cv::findContours(bwImage.clone(), contours, hierarchy,
                     CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE );
        
        // Find the convex hull object for each contour
        std::vector<std::vector<cv::Point2i>> hull(contours.size());
        for( int i = 0; i < contours.size(); i++ )
        {
            cv::convexHull( cv::Mat(contours[i]), hull[i], false );
        }
        
        cv::Mat1b resultCanvas = cv::Mat1b::zeros(bwImage.size());
        cv::drawContours( resultCanvas, hull, -1, cv::Scalar(255), CV_FILLED);
        
        return resultCanvas;
    }

    std::vector<cv::Point2f> cve::blobMassCenter(const cv::Mat & bwImage) {
        std::vector<cv::Point2f> centers;
        
        std::vector<std::vector<cv::Point2i> > contours;
        std::vector<cv::Vec4i> hierarchy;
        
        cv::findContours(bwImage.clone() , contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE );
        
        /// Get the moments
        std::vector<cv::Moments> mu(contours.size());
        for( int i = 0; i < contours.size(); i++ ) {
            mu[i] = cv::moments( contours[i], false );
        }
        
        ///  Get the mass centers:
        for( int i = 0; i < contours.size(); i++ ) {
            centers.push_back(cv::Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00));
        }
        
        return centers;

    }
}
