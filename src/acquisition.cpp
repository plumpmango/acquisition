#include "acquisition.h"

void Acquisition::acquireVideo(){
  m_arv = new ArvDriver();
  cv::VideoCapture cap(0);
  cap >> m_currentFrame;

  // m_arv->startAcquisition();

  std::cout << "Caméra activé" << std::endl;

  std::string pathVideo = _pathTosave + "/" + _videoName;
  // m_currentFrame = m_arv->getImageFromBuffer();
  // cv::VideoWriter video(pathVideo,VideoWriter::fourcc('X', '2', '6', '4'),30,m_currentFrame.size());



  std::cout << "Entrée boucle" << std::endl;
  while(cap.isOpened()){
    cap >> m_currentFrame;
    std::cout << "Test frame" << std::endl;
    if(m_currentFrame.empty())
      break;

    std::cout << "Ecrit video" << std::endl;
    // video.write(m_currentFrame);

    cv::imshow("Display",m_currentFrame);

    char c = (char)cv::waitKey(25);
    if(c == 27)
        break;

      // m_currentFrame = m_arv->getImageFromBuffer();
  }
  cap.release();
  cv::destroyAllWindows();
  // m_arv->stopAcquisition();
  // video.release();

}
