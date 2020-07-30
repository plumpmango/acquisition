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
  explicit Manage(std::string path,std::string name = "",QWidget *parent = 0);
  ~Manage();
private:
  inline void setName(std::string s){_name = s;};
  inline std::string getName(){return _name;};
  inline void setPath(std::string p){_path = p;};
  inline std::string getPath(){return _path;};
  inline void setCameraName(std::string name){cameraName = name;};
  inline std::string getCameraName(){return cameraName;};
  inline double getFrequency(){return frequency;};
  inline void setFrequency(double f){frequency=f;};
  inline gint64 getDelay(){return delay;};
  inline void setDelay(gint64 d){delay=d;};
  void setupManage();
  void setupDevicesQueueComboBox();
  void startAcquisition(std::string nameCam);
  void stopAcquisition();
  std::vector<std::string> getDevicesQueue();


private Q_SLOTS:
  void handleStartAcquisition();
  void handleStopAcquisition();
  void handleAcquire();
  void handleUpdateDevicesQueueCombobox();
  void handleLineEditFreq(const QString &frequency);
  void handleLineEditDelay(const QString &delay);
  void handleButtonSelectPath();

private:
  std::string _path;
  std::string _name;
  Acquisition *acq;
  bool stopAc;
  std::vector<std::string> devicesQueue;
  std::string cameraName;
  double frequency;
  gint64 delay;

private:
   QTimer *timer;
   Ui::Manage *ui;
};

#endif //MANAGE_H
