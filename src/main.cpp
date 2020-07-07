#include "acquisition.h"
#include <iostream>
#include <string>

int main(){
  std::string path = "../../Result";
  std::string name = "video";
  int ind = 0;
  char c;

  std::cout << "Lancer un enregistrement ? O/N" << std::endl;
  std::cin >> c;

  while(c != 'N'){
    name = name + std::to_string(ind);
    std::cout << "Création acquisition" <<std::endl;
    Acquisition acq(path,name);
    
    acq.acquireVideo();

    ++ind;
    std::cout << "Lancer un enregistrement ? O/N" << std::endl;

    std::cin >> c;
  }


  return 0;
}
