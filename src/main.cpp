#include <iostream>
#include <string>
#include <ctime>
#include <QApplication>
#include "src/Manage/manage.h"
int main(int argc, char **argv){
  std::string path = "../Result";
  std::string name = "video";

  QApplication app(argc,argv);
  Manage m(path,name);
  m.show();


  return app.exec();
}
