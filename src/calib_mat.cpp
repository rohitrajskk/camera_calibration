#include "calibrate.hpp"
#include <iostream>
#include <camera_calibration/cal.h>
#include <std_srvs/Empty.h>
#include <ros/ros.h>
using namespace cv;
using namespace std;


bool matrix(camera_calibration::cal::Request  &req, camera_calibration::cal::Response &res){ 
	res.data.clear();
	int width,height,squareSize,frames,u=0,v=0;
	char *argv[2];
	
	double calib_mat[3][3],distor_mat[5];
	Calibrate cal;
	width = 8;
	height = 5;
	squareSize = 28;
	frames = 20;
	argv[1]="/home/rohit/Downloads/Webcam";       //argv[1] is the absolute directory path
	cal.run(argv,width,height,squareSize,frames);
	Mat mat =cal.cameraMatrix;
	
	for(int i=0; i<3; i++){
		for(int j=0; j<3; j++){
			calib_mat[i][j]=mat.at<double>(i,j);
			
		}
	}
	
	
	for(int i=0; i<9; i++){
		res.data.push_back (calib_mat[v][u]);
		if(u==2){
			v++; u=-1;
		}
		u++;
	}
	
	cout<<endl<<"calibration_matrix::"<<endl;
	for(int i=0; i<9; i++){
		cout<<res.data[i]<<"  ";
	}
	
	for(int i=0; i<5; i++){
			distor_mat[i]=mat.at<double>(i);	
	}
	
	for(int i=0; i<5; i++){
		res.dist.push_back(distor_mat[i]);
	}
	cout<<endl<<"distortion coeffs:: "<<endl;
	for(int i=0; i<5; i++){
		cout<<res.dist[i]<<"  ";
	}	
	cout<<endl;
	ROS_INFO("sending back response");
	return true;
}



int main (int argc,char* argv[]){
	ros::init(argc, argv, "cal_matrix");
	ros::NodeHandle n;

	ros::ServiceServer service = n.advertiseService("cal_matrix", matrix);
	ROS_INFO("Ready to Give calibration matrix and distortion matrix");
	ros::spin();

  

	//cout<<"camera matrix ="<<endl<<cal.cameraMatrix<<endl<<endl;
	//cout<<"distortion coeffs ="<<endl<<cal.distCoeffs<<endl<<endl;
	return 0;
}

