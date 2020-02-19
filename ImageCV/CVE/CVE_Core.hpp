//
//  CVE_Core.hpp
//  ImageCV
//
//  Created by 谢存 on 2020/2/19.
//  Copyright © 2020 cunxie. All rights reserved.
//

#ifndef CVE_Core_hpp
#define CVE_Core_hpp

#include <opencv2/opencv.hpp>

namespace cve {

    cv::Mat cropROIWithBoundaryDetection(const cv::Mat & sourceImage, const cv::Rect & roi);

    template<typename _Tp> cv::Rect_<_Tp> CvRectMakeWithCenterPointAndSize(const cv::Point_<_Tp> & centerPoint, _Tp width, _Tp height);

    /**
     return the nth value of sort result of the given matrix. The input mat is first reshaped to long vector, and the nth operation is carried out by std::nth_element function
     *-->  User should guarantee the sourceMat IS CONTINOUS (if not, clone it!) <--*

     @param sourceMat 1-channel matrix
     @param nth the bar
     @param flag CV_SORT_ASCENDING for ascending sort, CV_SORT_DESCENDING for descending sort.
     @return the n-th value
     */
    template<typename _Tp> _Tp nthValueOfMat(const cv::Mat_<_Tp> & sourceMat, int nth, int flag = CV_SORT_ASCENDING);

    /**
     top n-percentile value of the given mat. This method invokes nthValueOfMat internally.
     *-->  User should guarantee the sourceMat IS CONTINOUS (if not, clone it!) <--*

     @param sourceMat 1-channel matrix
     @param percentile percentile value in range [0.0, 1.0]
     @param flag CV_SORT_ASCENDING for ascending sort, CV_SORT_DESCENDING for descending sort
     @return the n-percentile value
     */
    template<typename _Tp> _Tp topNPercentileValueOfMat(const cv::Mat_<_Tp> & sourceMat, float percentile, int flag = CV_SORT_ASCENDING);
}

template<typename _Tp> cv::Rect_<_Tp> cve::CvRectMakeWithCenterPointAndSize(const cv::Point_<_Tp> & centerPoint, _Tp width, _Tp height) {
    return cv::Rect_<_Tp>(centerPoint.x - width/2, centerPoint.y - height/2, width, height);
}

template<typename _Tp> _Tp cve::nthValueOfMat(const cv::Mat_<_Tp> & sourceMat, int nth, int flag) {
    cv::Mat_<_Tp> singleRow = sourceMat.reshape(0,1);
    std::vector<_Tp> vecFromMat;
    singleRow.copyTo(vecFromMat);
    
    if (flag == CV_SORT_DESCENDING) {
        std::nth_element(vecFromMat.begin(), vecFromMat.begin() + nth, vecFromMat.end(), std::greater<_Tp>());
    } else {
        std::nth_element(vecFromMat.begin(), vecFromMat.begin() + nth, vecFromMat.end());
    }
    return vecFromMat[nth];
}

template<typename _Tp> _Tp cve::topNPercentileValueOfMat(const cv::Mat_<_Tp> & sourceMat, float percentile, int flag) {
    int nth = 1.0f * sourceMat.rows * sourceMat.cols * percentile;
    return cve::nthValueOfMat(sourceMat, nth, flag);
}

#endif /* CVE_Core_hpp */
