//
//  Debug.cpp
//  ImageCV
//
//  Created by 谢存 on 2020/2/18.
//  Copyright © 2020 cunxie. All rights reserved.
//

#include "Debug.hpp"

void Debug::debugDrawAre(Mat inputImg, Point2i area[]) {

    for (int x = area[0].x; x < area[1].x; x++) {
        for (int y = area[0].y; y < area[1].y; y++) {
            inputImg.ptr<Vec3b>(y)[x][0] = 0;
            inputImg.ptr<Vec3b>(y)[x][1] = 255;
            inputImg.ptr<Vec3b>(y)[x][2] = 0;
        }
    }

}

void Debug::debugDrawPoint(Mat inputImg, Point2i point) {
    inputImg.ptr<Vec3b>(point.y)[point.x][0] = 0;
    inputImg.ptr<Vec3b>(point.y)[point.x][1] = 255;
    inputImg.ptr<Vec3b>(point.y)[point.x][2] = 0;
}

void Debug::debugShow(Mat img) {
    imshow("DeBug", img);
    waitKey(0);
}
