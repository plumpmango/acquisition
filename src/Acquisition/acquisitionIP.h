#ifndef ACQUISITIONIP_H
#define ACQUISITIONIP_H

#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <vector>


class AcquisitionIP {
public:
  AcquisitionIP(std::string pathToSave,std::string camerasId): _pathTosave(pathToSave), _camerasId(camerasId){};
  inline std::string getPathToSave(){return _pathTosave;};
  inline void setCamerasId(std::string cId){_camerasId = cId;};
  inline std::string getCamerasId(){return _camerasId;};
  inline cv::VideoCapture  getCap(){return cap;};


  void startAcquisition();
  cv::Mat acquire();
  bool stopAcquisition();

private:
  std::string _pathTosave;
  std::string _camerasId;
  cv::VideoCapture cap;
};
#endif
