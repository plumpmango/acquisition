#include <iostream>
#include <string>
#include "src/Acquisition/acquisitionIP.h"

//Acquisition IP : https://putuyuwono.wordpress.com/2015/05/29/multi-thread-multi-camera-capture-using-opencv/


//Acquisition
int main(int argc, char **argv){
  std::string pathToSave = "../Result/ipTests/";
  // std::string name = "video";
  //TODO
  std::string camerasId = {"rtsp://admin:oBRDycz3TQ@192.168.2.207/live"};

  AcquisitionIP acq(pathToSave,camerasId);
  bool acStop = false;
  cv::VideoWriter video(pathToSave + camerasId,cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),30,cv::Size(416,416),true);
  acq.startAcquisition();
  std::cout << "acStop = " << acStop << std::endl;
  while(!acStop){
    cv::Mat frame = acq.acquire();
    if(!frame.empty()){
      cv::imshow(camerasId,frame);
      if(video.isOpened()){
        video.write(frame);
      }
    }

    char c=(char)cv::waitKey(25);
	  if(c==27){
      acStop = acq.stopAcquisition();
    }
  }
  return 0;

}
