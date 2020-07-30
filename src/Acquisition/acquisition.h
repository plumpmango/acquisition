
#ifndef ACQUISITION_H
#define ACQUISITION_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include "src/Acquisition/arvdriver.h"


class Acquisition {
public:
  Acquisition(std::string pathToSave,std::string videoName = NULL,std::string cameraName = NULL,double freq = 10, gint64 delay= 250): _pathTosave(pathToSave), _videoName(videoName), _cameraName(cameraName), _frequency(freq), _delay(delay){};
  inline void setPathVideo(std::string path){_pathVideo=path;};
  inline std::string getPathVideo(){return _pathVideo;};
  inline void setCameraName(std::string cN){_cameraName=cN;}
  inline std::string getCameraName(){return _cameraName;};
  inline void setVideoName(std::string vN){_videoName=vN;};
  inline std::string getVideoName(){return _videoName;};
  inline double getFrequency(){return _frequency;};
  inline gint64 getDelay(){return _delay;};
  void startAcquisition();
  void acquireVideo();
  bool stopAcquisition();

private:
  std::string _pathTosave;
  std::string _videoName;
  std::string _cameraName;
  double _frequency;
  gint64 _delay;
  std::string _pathVideo;
  ArvDriver *m_arv;
  cv::Mat m_currentFrame;
  cv::VideoWriter video;
  int frameNumber;

};
#endif
