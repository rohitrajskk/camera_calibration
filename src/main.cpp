#include "calibrate.hpp"
#include <iostream>
using namespace cv;
using namespace std;

int main (int argc,char* argv[]){
int width,height,squareSize,frames;
/*cout<<"enter width of board" <<endl;
cin>>width;
cout<<"enter height of board"<<endl;
cin>>height;
cout<<"enter size of boxes"<<endl;
cin>>squareSize;
cout<<"enter frames"<<endl;
cin>>frames;*/
Calibrate cal ;
width = 8; height = 5; squareSize = 28; frames = 20;
//cal.width1 = 8;cal.height1 = 5; cal.squareSize1 = 28;
cal.run(argv,width,height,squareSize,frames);//argv[1] is the absolute directory path
cout<<"camera matrix ="<<endl<<cal.cameraMatrix<<endl<<endl;
cout<<"distortion coeffs ="<<endl<<cal.distCoeffs<<endl<<endl;
return 0;
}




