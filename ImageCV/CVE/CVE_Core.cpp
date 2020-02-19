//
//  CVE_Core.cpp
//  ImageCV
//
//  Created by 谢存 on 2020/2/19.
//  Copyright © 2020 cunxie. All rights reserved.
//

#include "CVE_Core.hpp"

namespace cve {

    cv::Mat cropROIWithBoundaryDetection(const cv::Mat & sourceImage, const cv::Rect & roi) {
        cv::Rect intRoi = roi;
        cv::Rect imageRect = cv::Rect(0,0,sourceImage.cols, sourceImage.rows);
        cv::Rect intersection = imageRect & intRoi;
        return sourceImage(intersection).clone();
    }

}
