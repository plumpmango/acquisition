#ifndef ACQUISITION_H
#define ACQUISITION_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include "Acquisition/arvdriver.h"

class Acquisition {
public:
  Acquisition(std::string pathToSave,std::string videoName): _pathTosave(pathToSave), _videoName(videoName){};
  void acquireVideo();

private:
  std::string _pathTosave;
  std::string _videoName;
  ArvDriver *m_arv;
  cv::Mat m_currentFrame;

};
#endif
