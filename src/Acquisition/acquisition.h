
#ifndef ACQUISITION_H
#define ACQUISITION_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include "src/Acquisition/arvdriver.h"


class Acquisition {
public:
  Acquisition(std::string pathToSave,std::string videoName): _pathTosave(pathToSave), _videoName(videoName){};
  inline void setPathVideo(std::string path){_pathVideo=path;};
  inline std::string getPathVideo(){return _pathVideo;};
  void startAcquisition();
  void acquireVideo();
  bool stopAcquisition();

private:
  std::string _pathTosave;
  std::string _videoName;
  std::string _pathVideo;
  ArvDriver *m_arv;
  cv::Mat m_currentFrame;
  cv::VideoWriter video;
  int frameNumber;

};
#endif
