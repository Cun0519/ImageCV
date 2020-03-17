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

    /**
     Smart color image segmentation using iterative k-means clustering. This method will perform multiple binary k-means clustering on the "darker" part, so that the darkest part can be clearly seperated (e.g iris area).

     @param image the image to be segmented
     @param kmeansIterations level of segmentation. Default is 4.
     @param coordinateWeight the weight factor for pixel adjacency. 0 will perform clustering purely be color, and higher coordinateWeight will weight more on pixel adjacency. Default is 0.4f
     @param kmeansRepeats times of k-means repeating for each level. Default is 4.
     @return a ranked image. higher value mean darker.
     */
    cv::Mat1b imageSegmentationViaIterativeKmeans(const cv::Mat& image, int kmeansIterations = 4, float coordinateWeight = 0.4f, int kmeansRepeats = 4);

    /**
     Fill the convex hulls in binary image.

     @return concavity-filled binary image
     */
    cv::Mat fillConvexHulls(const cv::Mat & bwImage);

    /**
     return the mass center coordinates for the blobs in the given binary image.
     */
    std::vector<cv::Point2f> blobMassCenter(const cv::Mat & bwImage);
}

#endif /* CVE_Analysis_hpp */
