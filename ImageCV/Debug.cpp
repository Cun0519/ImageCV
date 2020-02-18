//
//  Debug.cpp
//  ImageCV
//
//  Created by 谢存 on 2020/2/18.
//  Copyright © 2020 cunxie. All rights reserved.
//

#include "Debug.hpp"

void Debug::debugDrawSearchingAre(Mat inputImg, Point2f searchingArea[]) {

    for (int x = round(searchingArea[0].x); x < round(searchingArea[1].x); x++) {
        for (int y = round(searchingArea[0].y); y < round(searchingArea[1].y); y++) {
            inputImg.ptr<Vec3b>(y)[x][0] = 0;
            inputImg.ptr<Vec3b>(y)[x][1] = 255;
            inputImg.ptr<Vec3b>(y)[x][2] = 0;
        }
    }

}

void Debug::debugShow(Mat img) {
    imshow("DeBug", img);
    waitKey(0);
}
