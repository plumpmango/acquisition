#include "acquisition.h"

char *convertToChar(std::string s){
  //convert string to char*
  char charName[s.length()];
  for(int i = 0; i < sizeof(charName); i++){
    charName[i] = s[i];
    std::cout << charName[i];
  }


  return charName;
}

void Acquisition::startAcquisition(){
  setPathVideo(_pathTosave + "/" + _videoName + ".avi");
  std::cout << getPathVideo() << std::endl;
  frameNumber = 0;

  // char *charName = convertToChar(getCameraName());
std::string s = getCameraName();
  char charName[s.length()];
  for(int i = 0; i <= sizeof(charName); i++){
    charName[i] = s[i];
    std::cout << charName[i];
  }

  m_arv = new ArvDriver(charName,getFrequency(),getDelay());
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
    // if(frameNumber == 5)
    //   std::cerr << "Error : Stop acquisition" << std::endl;

  }
  else{

    // std::cout << m_currentFrame.channels() << std::endl;
    std::cout << m_currentFrame.size()<< std::endl;
    cv::resize(m_currentFrame,m_currentFrame,Size(416,416));
    // cv::cvtColor(m_currentFrame,m_currentFrame,COLOR_BayerBG2BGR);
    video.write(m_currentFrame);
    cv::imshow(getCameraName(),m_currentFrame);
  }

}


bool Acquisition::stopAcquisition(){
  m_arv->stopAcquisition();
  cv::destroyAllWindows();
  if (video.isOpened())
    video.release();
  return true;
}
