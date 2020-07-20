/*!
 * \file manage.h
 * \brief Mise en place de l'interface de communication
 * \author Clémence
 * \version 0
 */

//recquired to resolve conflict between GLIB et QT
#define QT_NO_KEYWORDS
#undef signals
#undef slots
#undef emit


#ifndef MANAGE_H
#define MANAGE_H

#include <QDialog>
#include <QtWidgets>
#include <iostream>

#include "src/Acquisition/acquisition.h"
#include "arv.h"

namespace Ui {
class Manage;
}


class Manage : public QMainWindow
{
    Q_OBJECT

public:
  explicit Manage(std::string path,std::string name,QWidget *parent = 0);
  ~Manage();
private:
  inline void setName(std::string s){_name = s;};
  inline std::string getName(){return _name;};
  inline std::string getPath(){return _path;};
  void startAcquisition();
  void stopAcquisition();
  std::queue<std::string> getDevicesQueue();

private Q_SLOTS:
  void handleStartAcquisition();
  void handleStopAcquisition();
  void handleAcquire();

protected Q_SLOTS:
  void keyPressEvent(QKeyEvent *event) override;

private:
  std::string _path;
  std::string _name;
  Acquisition *acq;
  bool stopAc;
private:
   QTimer *timer;
   Ui::Manage *ui;
};

#endif //MANAGE_H
