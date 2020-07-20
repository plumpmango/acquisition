//to resolve conflict between GLIB et QT
#define QT_NO_KEYWORDS
#undef signals
#undef slots
#undef emit

#include "manage.h"
#include "ui_manage.h"

Manage::Manage(std::string path,std::string name,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Manage)
{
    ui->setupUi(this);
    _path = path;
    _name = name;
    stopAc = true;
    timer= new QTimer(this);
    // if(!stopAc)
    std::queue<std::string> dQ = this->getDevicesQueue();
    for(int i = 0; i <= dQ.size(); i++){
      std::cout << dQ.front() << std::endl;
      dQ.pop();
    }
    connect(ui->startpushButton, SIGNAL (released()), this, SLOT (handleStartAcquisition()));
    connect(ui->stoppushButton, SIGNAL (released()), this, SLOT (handleStopAcquisition()));
    connect(timer, SIGNAL(timeout()), this, SLOT(handleAcquire()));
}

Manage::~Manage()
{
    delete ui;
}


void Manage::startAcquisition(){
  time_t now = time(0);
  tm *ltm = localtime(&now);
  std::string y = std::to_string(1900+ltm->tm_year);
  std::string m = std::to_string(1 + ltm->tm_mon);
  std::string d = std::to_string(ltm->tm_mday);
  std::string h = std::to_string(ltm->tm_hour);
  std::string min = std::to_string(ltm->tm_min);
  std::string s = std::to_string(ltm->tm_sec);
  std::cout << h << "h" << min << "min" << s << "s" << std::endl;

  std::string id = y + "_" + m + "_" + d + "_" + h + "_" + min + "_" + s;
  // setName(_name + id);
  std::string path = getPath();
  std::string name = getName() + id;
  acq = new Acquisition(path,name);
  stopAc = false;
  timer->start(33);
  acq->startAcquisition();

}

void Manage::stopAcquisition(){
  timer->stop();
  stopAc = acq->stopAcquisition();

}

std::queue<std::string> Manage::getDevicesQueue(){
	printf ("Update Device List\n");

	arv_update_device_list();
	guint n_devices = arv_get_n_devices();

	printf ("Number of found Devices: %d\n", n_devices);

	std::queue<std::string> devicesQueue;
	for (int i = 0; i < n_devices; i++)
		devicesQueue.push(arv_get_device_id (i));
		// printf ("%s\n", arv_get_device_id (i));
	return devicesQueue;
}

void Manage::handleStartAcquisition(){
    startAcquisition();
}

void Manage::handleStopAcquisition(){
    stopAcquisition();
}

void Manage::handleAcquire(){
    // if(!stopAc)
      acq->acquireVideo();
}

void Manage::keyPressEvent(QKeyEvent *event) {
    switch(event->key()) {
        // lance l'acquisition
        case Qt::Key_A:
          std::cout << "Video path : " << getPath() << std::endl;
          if(stopAc)
            startAcquisition();
        case Qt::Key_S:
          if(!stopAc)
            stopAcquisition();
    }
}
