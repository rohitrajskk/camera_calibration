#include "ros/ros.h"
#include <sensor_msgs/Image.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <std_srvs/Empty.h>
#include <string>
#include <ros/package.h>

class camSave {
public:
	ros::NodeHandle n;
	ros::ServiceServer service;
	ros::Subscriber sub;
	sensor_msgs::ImageConstPtr currentImage;
	char *filename;
	int filecounter;
	camSave() {
		filecounter=65;
		service = n.advertiseService("save_image", &camSave::save_image,this);
		sub= n.subscribe("image_raw",1,&camSave::message,this);
	}
	
	std::string retrieveString(const char* buf, int max ) {

		size_t len = 0;
		while( (len < max) && (buf[ len ] != '\0') ) {
			len++;
		}

		return std::string( buf, len );

	}


	
	
	void SaveImageAsPPM( const sensor_msgs::ImageConstPtr& msg, const char* filename ){
		if ( msg->encoding != "rgb8" ){
			return;  // Can only handle the rgb8 encoding
		}

		std::string basePath = ros::package::getPath("camera_calibration");
		std::string filename_string = camSave::retrieveString(filename,15);
		filename_string = basePath +"/uvc_image/"+ filename;
		char * filename_char = new char[filename_string.length() + 1];
		std::strcpy(filename_char,filename_string.c_str());
		FILE* file = fopen( filename_char, "w" );
  
		fprintf( file, "P3\n" );
		fprintf( file, "%i %i\n", msg->width, msg->height );
		fprintf( file, "255\n" );

		for ( uint32_t y = 0; y < msg->height; y++ ){
			for ( uint32_t x = 0; x < msg->width; x++ ){
				// Get indices for the pixel components
				uint32_t redByteIdx = y*msg->step + 3*x;
				uint32_t greenByteIdx = redByteIdx + 1;
				uint32_t blueByteIdx = redByteIdx + 2;

				fprintf( file, "%i %i %i ", 
				msg->data[ redByteIdx ], 
				msg->data[ greenByteIdx ], 
				msg->data[ blueByteIdx ] );
			}
			fprintf( file, "\n" );
		}

		fclose( file );
	}

	bool save_image(std_srvs::Empty::Request  &req, std_srvs::Empty::Response &res){
		char z[10]="image";
		z[5]=char(filecounter);
		filename=z;
		SaveImageAsPPM(currentImage,filename);
		ROS_INFO("Hi It works");
		filecounter++;
		return true;
	}

	void message(const sensor_msgs::ImageConstPtr& msg){
    
    currentImage = msg;
    
	}

	void spin(){
		ros::spin();
	}

};

int main(int argc, char **argv){

	ros::init(argc, argv, "save_image");
	ROS_INFO("Ready to save image");
    camSave v;
	v.spin();

	return 0;

}
