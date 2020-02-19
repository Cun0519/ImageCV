//
//  CVE_Image.cpp
//  ImageCV
//
//  Created by 谢存 on 2020/2/19.
//  Copyright © 2020 cunxie. All rights reserved.
//

#include "CVE_Image.hpp"

namespace cve {

    std::vector<cv::Point2i> imageLocalMaxima(const cv::Mat& input, int nLocMax, float minDistBtwLocMax, float threshold, cv::InputArray mask) {
        cv::Mat1f playground;
        input.convertTo(playground, CV_32FC1);
        std::vector<cv::Point2i> peakPoints;
        std::vector<double> maxValues;
        
        if (threshold == -1) {
            double temp = 0;
            cv::minMaxLoc(playground, &temp, NULL, NULL, NULL, mask);
            threshold = temp;
        }
        for (int i = 0 ; i < nLocMax; i++) {
            cv::Point2i location;
            double maxVal;
            cv::minMaxLoc(playground, NULL, &maxVal, NULL, &location, mask);
            if (maxVal >= threshold) {
                peakPoints.push_back(location);
                maxValues.push_back(maxVal);
                cv::circle(playground, location, minDistBtwLocMax, cv::Scalar::all(threshold), -1);
            } else
                break;
        }
        
        return peakPoints;
    }
}
