#include "calibrate.hpp"
#include <iostream>
#include <camera_calibration/cal.h>
#include <std_srvs/Empty.h>
#include <ros/ros.h>
using namespace cv;
using namespace std;


bool matrix(camera_calibration::cal::Request  &req, camera_calibration::cal::Response &res){ 
	res.data.clear();
	int width,height,squareSize,frames,u=0,v=0;   //variable u and v have been used for converting 2d array to 1D array
	char *argv[2];
	double calib_mat[3][3],distor_mat[5];
	Calibrate cal;
	width = 8;
	height = 5;
	squareSize = 28;
	frames = 20;
	argv[1]="/home/rohit/Downloads/Webcam";       //argv[1] is the absolute directory path
	
	
	cal.run(argv,width,height,squareSize,frames);
	
	
	for(int i=0; i<9; i++){
		res.data.push_back(cal.cameraMatrix.at<double>(v,u));
		if(u==2){
			v++; u=-1;
		}
		u++;
	}
	
	ROS_INFO("sending back response");
	cout<<endl<<"calibration_matrix::"<<endl;
	
	for(int i=0; i<9; i++){
		cout<<res.data[i]<<"  ";
	}
	
	for(int i=0; i<5; i++){
			res.dist.push_back(cal.distCoeffs.at<double>(i));
			
	}
	
	cout<<endl<<"distortion coeffs:: "<<endl;
	
	for(int i=0; i<5; i++){
		cout<<res.dist[i]<<"  ";
	}	
	
	cout<<endl;
	//cout<<"camera matrix ="<<endl<<cal.cameraMatrix<<endl<<endl;
	//cout<<"distortion coeffs ="<<endl<<cal.distCoeffs<<endl<<endl;
	
	return true;
}



int main (int argc,char* argv[]){
	ros::init(argc, argv, "cal_matrix");
	ros::NodeHandle n;

	ros::ServiceServer service = n.advertiseService("cal_matrix", matrix);
	ROS_INFO("Ready to Give calibration matrix and distortion matrix");
	ros::spin();
	return 0;
}

