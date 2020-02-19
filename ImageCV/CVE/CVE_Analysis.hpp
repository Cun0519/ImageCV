//
//  CVE_Analysis.hpp
//  ImageCV
//
//  Created by 谢存 on 2020/2/19.
//  Copyright © 2020 cunxie. All rights reserved.
//

#ifndef CVE_Analysis_hpp
#define CVE_Analysis_hpp

#include <opencv2/opencv.hpp>

namespace cve {
    /**
     remove the small blobs (or noise dots) and leaving only the largest blob

     @param bwImage binary image to be processed
     @return a binary image containing only the largest blob
     */
    cv::Mat removeSmallBlobsExceptLargest(const cv::Mat & bwImage);
}

#endif /* CVE_Analysis_hpp */
