#include "acquisitionIP.h"

//
// void AcquisitionIP::startAcquisition(){
//   cv::VideoCapture *cap;
//
//   std::thread *curThread;
//
//   for(int i = 0; i<getNbCameras(); i++){
//     std::vector<cv::VideoWriter*> vw = getVideos();
//     vw[i] = new cv::VideoWriter(getPathToSave()+getCamerasId()[i],cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),30,cv::Size(416,416),true);
//     setVideos(vw);
//   }
//
//   for(int i =0; i < getNbCameras(); i++){
//     std::string url = getCamerasId()[i];
//     cap = new cv::VideoCapture(url);
//     std::cout << "Camera setup : " << url << std::endl;
//
//     caps.push_back(cap);
//
//     curThread = new std::thread(&AcquisitionIP::acquireVideo,this,i);
//
//     std::vector<std::thread*> vThreadsCur = getVThreads();
//     vThreadsCur.push_back(curThread);
//     setVThreads(vThreadsCur);
//   }
// }
//
// void AcquisitionIP::acquireVideo(int ind){
//   cv::VideoCapture *cap = caps[ind];
//   cv::VideoWriter *video = videos[ind];
//   while(1){
//     cv::Mat frame;
//     (*cap) >> frame;
//     if(video->isOpened()){
//       video->write(frame);
//       cv::imshow(getCamerasId()[ind],frame);
//     }
//     frame.release();
//   }
// }
//
// bool AcquisitionIP::stopAcquisition(){
//   cv::VideoCapture *cap;
//   for(int i = 0; i < getNbCameras(); i++){
//     cap = getCaps()[i];
//     if(cap->isOpened())
//       cap->release();
//   }
//   return true;
// }

void AcquisitionIP::startAcquisition(){
  cap.open(getCamerasId());
  if(!cap.isOpened()){
    std::cerr << "Error opening video stream or file" << std::endl;
  }
}

cv::Mat AcquisitionIP::acquire(){

    cv::Mat frame;
    cap.read(frame);
    if(!frame.empty()){
      cv::resize(frame,frame,cv::Size(416,416));
    }
    return frame;
}

bool AcquisitionIP::stopAcquisition(){
  cap.release();
  return true;
}
