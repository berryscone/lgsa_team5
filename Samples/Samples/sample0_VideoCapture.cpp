#include <iostream>
using namespace std;

#include "sample0_VideoCapture.h"
#include "opencv2/opencv.hpp"
using namespace cv;


int run_sample0()
{
    static const string file_name = R"(C:\Users\blowi\OneDrive\22Y_SA_Course\Team Project\beaver1.avi)";

    Mat frame;
    //--- INITIALIZE VIDEOCAPTURE
    VideoCapture cap;
    // open the default camera using default API
    // cap.open(0);
    // OR advance usage: select any API backend
    int deviceID = 0;             // 0 = open default camera
    int apiID = cv::CAP_ANY;      // 0 = autodetect default API
    // open selected camera using selected API
    // cap.open(deviceID, apiID);
    cap.open(file_name);
    
    // check if we succeeded
    if (!cap.isOpened()) {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }

    //--- GRAB AND WRITE LOOP
    cout << "Start grabbing" << endl
        << "Press any key to terminate" << endl;
    for (;;)
    {
        // wait for a new frame from camera and store it into 'frame'
        cap.read(frame);
        // check if we succeeded
        if (frame.empty()) {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }

        // Show properties
        cout << "------------------------------------------------------------" << endl;
        cout << "CAP_PROP_POS_MSEC:" << cap.get(CAP_PROP_POS_MSEC) << endl;
        cout << "CAP_PROP_POS_FRAMES:" << cap.get(CAP_PROP_POS_FRAMES) << endl;
        cout << "CAP_PROP_FPS:" << cap.get(CAP_PROP_FPS) << endl;
        cout << "CAP_PROP_BRIGHTNESS :" << cap.get(CAP_PROP_BRIGHTNESS) << endl;
        cout << "CAP_PROP_EXPOSURE :" << cap.get(CAP_PROP_EXPOSURE) << endl;
        cout << "CAP_PROP_POS_MSEC:" << cap.get(CAP_PROP_POS_MSEC) << endl;

        // show live and wait for a key with timeout long enough to show images
        imshow("Live", frame);
        if (waitKey(5) >= 0)
            break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}