#include "ros/ros.h"
#include <sensor_msgs/Image.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>

void SaveImageAsPPM( const sensor_msgs::ImageConstPtr& msg, const char* filename )
{
  if ( msg->encoding != "rgb8" )
  {
    return;  // Can only handle the rgb8 encoding
  }

  FILE* file = fopen( filename, "w" );

  fprintf( file, "P3\n" );
  fprintf( file, "%i %i\n", msg->width, msg->height );
  fprintf( file, "255\n" );

  for ( uint32_t y = 0; y < msg->height; y++ )
  {
    for ( uint32_t x = 0; x < msg->width; x++ )
    {
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


void message(const sensor_msgs::ImageConstPtr& msg)
{
SaveImageAsPPM(msg, "image");
}

int main(int argc, char **argv)
{

ros::init(argc, argv, "save_image");


ros::NodeHandle n;
ros::Subscriber sub = n.subscribe("image_raw", 1000, message);

  
  ros::spin();

  return 0;
}
