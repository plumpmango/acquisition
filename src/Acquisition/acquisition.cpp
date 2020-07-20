#include "acquisition.h"


void Acquisition::startAcquisition(){
  setPathVideo(_pathTosave + "/" + _videoName + ".avi");
  std::cout << getPathVideo() << std::endl;
  frameNumber = 0;
  m_arv = new ArvDriver();
  m_arv->startAcquisition();

}

void Acquisition::acquireVideo(){

  if(!video.isOpened()){
    std::cout << m_currentFrame.size()<< std::endl;
    video.open(getPathVideo(),VideoWriter::fourcc('M', 'J', 'P', 'G'),30,Size(416,416),true);
  }

  m_currentFrame = m_arv->getImageFromBuffer();
  ++frameNumber;

  if(m_currentFrame.empty() && video.isOpened()){
    std::cerr << "frame " << frameNumber << " empty" << std::endl;
    if(frameNumber == 5)
      std::cerr << "Error : Stop acquisition" << std::endl;

  }
  else{

    std::cout << m_currentFrame.channels() << std::endl;
    std::cout << m_currentFrame.size()<< std::endl;
    cv::resize(m_currentFrame,m_currentFrame,Size(416,416));
    video.write(m_currentFrame);

    // cv::imshow("Display",m_currentFrame);
  }

}


bool Acquisition::stopAcquisition(){
  m_arv->stopAcquisition();
  if (video.isOpened())
    video.release();
  return true;
}
