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
}
