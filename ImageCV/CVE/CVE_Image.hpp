//
//  CVE_Image.hpp
//  ImageCV
//
//  Created by 谢存 on 2020/2/19.
//  Copyright © 2020 cunxie. All rights reserved.
//

#ifndef CVE_Image_hpp
#define CVE_Image_hpp

#include <opencv2/opencv.hpp>

namespace cve {

    /**
     Identify the local maxima(s) in the given image.

     @param input A 1-channel gray image or float image.
     @param nLocMax The maximum number of local maxima to be identified.
     @param minDistBtwLocMax The minimum pixel distance between two local maximas.
     @param threshold The threshold value of local maxima. If negative(< -1), the threshold = minValue(input).
     @return A vector of pixel coordinate(s) (in Pixel2i type) of the identified local maxima(s).
     */
    std::vector<cv::Point2i> imageLocalMaxima(const cv::Mat & input, int nLocMax = 1, float minDistBtwLocMax = 1, float threshold = -1, cv::InputArray mask = cv::noArray());
}

#endif /* CVE_Image_hpp */
