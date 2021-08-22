//
//  main.cpp
//  HelloVC
//
//  Created by Sean Fowler on 12/20/18.
//  Copyright © 2018 Sean Fowler. All rights reserved.
//

#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/objdetect/objdetect.hpp"

using namespace std;
using namespace cv;

int main(int argc, const char * argv[])
{
    //create the cascasde classifier object used for the face detection
    CascadeClassifier face_cascade;
    
    //use the haarcascade_frontalface.alt.xml library
    face_cascade.load("haarcascade_frontalface_alt.xml");
    
    
    //Setup video capture device (camera)
    VideoCapture stream1;
    stream1.open(0);
    
    string windowName = "Hello OpenCV";
    cout << "Hello, OpenCV!" << endl;
    
    if (!stream1.isOpened()){
        cout << "Not Open." << endl;
    }
    
    
//    int iLowH = 110;
//    int iLowS = 150;
//    int iLowV = 150;
//
//    int iHighH = 130;
//    int iHighS = 255;
//    int iHighV = 255;
    
    
    int iLowH = 0;
    int iLowS = 0;
    int iLowV = 0;
    
    int iHighH = 255;
    int iHighS = 255;
    int iHighV = 255;
    
    int iLastX = -1;
    int iLastY = -1;
    
    //Capture a temporary image from the camera
    Mat imgTmp;
    stream1.read(imgTmp);
    
    
    //Create a black image with the size as the camera output
    Mat imgLines = Mat::zeros( imgTmp.size(), CV_8UC3 );
    
    
    
    namedWindow("Processed Image", WINDOW_AUTOSIZE);
    namedWindow("Original", WINDOW_AUTOSIZE);
    
    moveWindow("Original", 900,900);
    
    
    while (true){
        Mat cameraFrame;
        Mat imgHSV;
        Mat processedImage;
        
        stream1.read(cameraFrame);
        
        cvtColor(cameraFrame, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
        
        
        
        Mat imgProcessed;
        
        inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgProcessed); //Threshold the image
        
        //morphological opening (remove small objects from the foreground)
//        erode(imgProcessed, imgProcessed, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
//        dilate( imgProcessed, imgProcessed, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        
        //morphological closing (fill small holes in the foreground)
//        dilate( imgProcessed, imgProcessed, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
//        erode(imgProcessed, imgProcessed, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        
        
        cv::flip(cameraFrame,cameraFrame,1);
        cv::flip(imgProcessed,imgProcessed,1);
        
        
        //Calculate the moments of the Processed image
        Moments oMoments = moments(imgProcessed);
        
        double dM01 = oMoments.m01;
        double dM10 = oMoments.m10;
        double dArea = oMoments.m00;
        
        // if the area <= 10000, I consider that the there are no object in the image and it's because of the noise, the area is not zero
        if (dArea > 10000)
        {
            //calculate the position of the ball
            int posX = dM10 / dArea;
            int posY = dM01 / dArea;
            
            if (iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0)
            {
                //Draw a red line from the previous point to the current point
                line(imgLines, Point(posX, posY), Point(iLastX, iLastY), Scalar(0,255,0), 2);
            }
            
            iLastX = posX;
            iLastY = posY;
        }
        
        
        
        imshow("Processed Image", imgProcessed); //show the thresholded image
        
        cameraFrame = cameraFrame + imgLines;
        imshow("Original", cameraFrame); //show the original image
        //imshow("test", cameraFrame);
        
        if( (waitKey(10) & 255) == 27 ) break;
    }
    
    return 0;
}
