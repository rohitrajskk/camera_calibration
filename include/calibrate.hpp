#ifndef CALIBRATE_HPP
#define CALIBRATE_HPP
#include <iostream>
#include <sstream>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;


class Calibrate
{

public:
Calibrate();

Mat cameraMatrix, distCoeffs;
//int width1,height1,squareSize1;
class Settings
{
public:
    //Settings() : goodInput(false) {}
    enum Pattern { NOT_EXISTING, CHESSBOARD, CIRCLES_GRID, ASYMMETRIC_CIRCLES_GRID };
    enum InputType {INVALID, CAMERA, VIDEO_FILE, IMAGE_LIST};
    void write (string in,int width,int height,int size,int frames){
    boardSize.width=width;            // The size of the board -> Number of items by width and height
    boardSize.height=height;
    calibrationPattern=CHESSBOARD;// One of the Chessboard, circles, or asymmetric circle pattern
    squareSize=size;          // The size of a square in your defined unit (point, millimeter,etc).
    nrFrames=frames;              // The number of frames to use from the input for calibration
    aspectRatio=0;         // The aspect ratio
    delay=50;                 // In case of a video input
    bwritePoints=true;         //  Write detected feature points
    bwriteExtrinsics=true;     // Write extrinsic parameters
    calibZeroTangentDist=true; // Assume zero tangential distortion
    calibFixPrincipalPoint=false;// Fix the principal point at the center
    flipVertical=false;          // Flip the captured images around the horizontal axis
    outputFileName = "out.xml";      // The name of the file where to write
    showUndistorsed=true;       // Show undistorted images after calibration
    input = in;               // The input ->
    inputType = IMAGE_LIST;
    interprate();
}
bool readStringList(){
    DIR           *d;
    imageList.clear();
    string src;
    src = input;
    char* Src = new char[src.length() + 1];
    strcpy(Src,src.c_str());
    struct dirent *dir;
    d = opendir(Src);
    src = src + "/";
    if (d)
    {
    while ((dir = readdir(d)) != NULL)
    {
     if(dir->d_name[0]!='.'){
     imageList.push_back(src + dir->d_name);
     }
     }
    
    closedir(d);
    return true;
  }
return false;
}
void interprate()
    {
        goodInput = true;
        if (boardSize.width <= 0 || boardSize.height <= 0)
        {
            cerr << "Invalid Board size: " << boardSize.width << " " << boardSize.height << endl;
            goodInput = false;
        }
        if (squareSize <= 10e-6)
        {
            cerr << "Invalid square size " << squareSize << endl;
            goodInput = false;
        }
        if (nrFrames <= 0)
        {
            cerr << "Invalid number of frames " << nrFrames << endl;
            goodInput = false;
        }

        if (!sizeof(input))      // Check for valid input
                inputType = INVALID;
        else
        {
            if (input[0] >= '0' && input[0] <= '9')
            {
                stringstream ss(input);
                ss >> cameraID;
                inputType = CAMERA;
            }
            else
            {
                if (readStringList())
                    {
                        inputType = IMAGE_LIST;
                        nrFrames = (nrFrames < (int)imageList.size()) ? nrFrames : (int)imageList.size();
                    }
                else
                    inputType = VIDEO_FILE;
            }
            if (inputType == CAMERA)
                inputCapture.open(cameraID);
            if (inputType == VIDEO_FILE)
                inputCapture.open(input);
            if (inputType != IMAGE_LIST && !inputCapture.isOpened())
                    inputType = INVALID;
        }
        if (inputType == INVALID)
        {
            cerr << " Inexistent input: " << input;
            goodInput = false;
        }

        flag = 0;
        if(calibFixPrincipalPoint) flag |= CV_CALIB_FIX_PRINCIPAL_POINT;
        if(calibZeroTangentDist)   flag |= CV_CALIB_ZERO_TANGENT_DIST;
        if(aspectRatio)            flag |= CV_CALIB_FIX_ASPECT_RATIO;


        //calibrationPattern = NOT_EXISTING;
        //if (!patternToUse.compare("CHESSBOARD")) calibrationPattern = CHESSBOARD;
        //if (!patternToUse.compare("CIRCLES_GRID")) calibrationPattern = CIRCLES_GRID;
        //if (!patternToUse.compare("ASYMMETRIC_CIRCLES_GRID")) calibrationPattern = ASYMMETRIC_CIRCLES_GRID;
        //if (calibrationPattern == NOT_EXISTING)
          //  {
            //    cerr << " Inexistent camera calibration mode: " << patternToUse << endl;
             //   goodInput = false;
            //}warning: missing whitespace after the macro name [enabled by default]

        atImageList = 0;

    }

Mat nextImage()
    {
       Mat result;/*
        if( inputCapture.isOpened() )
        {
            Mat view0;
            inputCapture >> view0;
            view0.copyTo(result);
        }*/
       if( atImageList < (int)imageList.size() ){
            result = imread(imageList[atImageList], CV_LOAD_IMAGE_COLOR);
            cout << imageList[atImageList]<<endl;
            atImageList++;}
        return result;
    }

public:
    Size boardSize;            // The size of the board -> Number of items by width and height
    Pattern calibrationPattern;// One of the Chessboard, circles, or asymmetric circle pattern
    float squareSize;          // The size of a square in your defined unit (point, millimeter,etc).
    int nrFrames;              // The number of frames to use from the input for calibration
    float aspectRatio;         // The aspect ratio
    int delay;                 // In case of a video input
    bool bwritePoints;         //  Write detected feature points
    bool bwriteExtrinsics;     // Write extrinsic parameters
    bool calibZeroTangentDist; // Assume zero tangential distortion
    bool calibFixPrincipalPoint;// Fix the principal point at the center
    bool flipVertical;          // Flip the captured images around the horizontal axis
    string outputFileName;      // The name of the file where to write
    bool showUndistorsed;       // Show undistorted images after calibration
    string input;               // The input ->



    int cameraID;
    vector<string>imageList;
    int atImageList;
    VideoCapture inputCapture;
    bool goodInput;
    InputType inputType;
    int flag;

};//end of class Settings
public:
void help();
double computeReprojectionErrors( const vector<vector<Point3f> >& objectPoints,
                                         const vector<vector<Point2f> >& imagePoints,
                                         const vector<Mat>& rvecs, const vector<Mat>& tvecs,
                                         const Mat& cameraMatrix , const Mat& distCoeffs,
                                         vector<float>& perViewErrors);
void calcBoardCornerPositions(Size boardSize, float squareSize, vector<Point3f>& corners,
                                     Settings::Pattern patternType /*= Settings::CHESSBOARD*/);
bool runCalibration( Settings& s, Size& imageSize, Mat& cameraMatrix, Mat& distCoeffs,
                            vector<vector<Point2f> > imagePoints, vector<Mat>& rvecs, vector<Mat>& tvecs,
                            vector<float>& reprojErrs,  double& totalAvgErr);
void saveCameraParams( Settings& s, Size& imageSize, Mat& cameraMatrix, Mat& distCoeffs,
                              const vector<Mat>& rvecs, const vector<Mat>& tvecs,
                              const vector<float>& reprojErrs, const vector<vector<Point2f> >& imagePoints,
                              double totalAvgErr );
bool runCalibrationAndSave(Settings& s, Size imageSize, Mat&  cameraMatrix, Mat& distCoeffs,vector<vector<Point2f> > imagePoints );
int run(char* argv[],int width,int height,int size,int frames);
 

};//end of class Calibrate
#endif


















