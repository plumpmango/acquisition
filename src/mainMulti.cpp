#include <iostream>
#include <string>
#include <ctime>
#include <QApplication>
#include "src/Manage/manage.h"


//Acquisition probleme : https://github.com/AravisProject/aravis/issues/311
//camera user manual https://www.smartek.vision/media/downloads/SMARTEKvision_twentynine_UserManual_v1_0_2.pdf

int main(int argc, char **argv){
  std::string path = "../Result";
  // std::string name = "video";

  QApplication app(argc,argv);

  Manage m(path);
  Manage m2(path);
  m.show();
  m2.show();



  return app.exec();
}
