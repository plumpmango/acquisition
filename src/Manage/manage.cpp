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

    setupManage();

}

Manage::~Manage()
{
    delete ui;
}

void Manage::setupManage(){
  timer= new QTimer(this);


  this->setupDevicesQueueComboBox();
  ui->lineEditFreq->setPlaceholderText("5");
  ui->lineEditDelay->setPlaceholderText("25");
  ui->label_4->setText("Inter Packets Delay (" + QString("\u03BC") + "s) :");
  const QString qs = QString::fromStdString(getPath());
  ui->labelPath->setText(qs);
  connect(timer, SIGNAL(timeout()), this, SLOT(handleAcquire()));
  connect(ui->lineEditFreq, SIGNAL(textChanged(const QString &)), this, SLOT(handleLineEditFreq(const QString &)));
  connect(ui->lineEditDelay, SIGNAL(textChanged(const QString &)), this, SLOT(handleLineEditDelay(const QString &)));
  connect(ui->selectPathpushButton, SIGNAL (released()), this, SLOT (handleButtonSelectPath()));
  connect(ui->startpushButton, SIGNAL (released()), this, SLOT (handleStartAcquisition()));
  connect(ui->stoppushButton, SIGNAL (released()), this, SLOT (handleStopAcquisition()));
}

void Manage::setupDevicesQueueComboBox(){
  ui->devicesQueueComboBox->clear();
  std::vector<std::string> dQ = this->getDevicesQueue();
  if(dQ.size() == 0){
    std::cerr << "No camera found. Please connect devices to run acquisition" << std::endl;
    return;
  }
  while(dQ.size() == 0){
    dQ = this->getDevicesQueue();
  }
  for(int i = 0; i < dQ.size(); i++){
    // std::cout << dQ[i] << std::endl;
    const QString item = QString::fromStdString(dQ[i]);
    ui->devicesQueueComboBox->addItem(item);
  }
}

void Manage::startAcquisition(std::string nameCam){
  time_t now = time(0);
  tm *ltm = localtime(&now);
  std::string y = std::to_string(1900+ltm->tm_year);
  std::string m = std::to_string(1 + ltm->tm_mon);
  std::string d = std::to_string(ltm->tm_mday);
  std::string h = std::to_string(ltm->tm_hour);
  std::string min = std::to_string(ltm->tm_min);
  std::string s = std::to_string(ltm->tm_sec);

  std::string id = y + "_" + m + "_" + d + "_" + h + "_" + min + "_" + s;

  std::string path = getPath();
  std::string name = nameCam + id;
  // std::cout << name << std::endl;

  acq = new Acquisition(path,name,nameCam,getFrequency(),getDelay());
  stopAc = false;
  timer->start(33);
  acq->startAcquisition();

}


void Manage::stopAcquisition(){
  timer->stop();
  stopAc = acq->stopAcquisition();
  this->setupDevicesQueueComboBox();
}


std::vector<std::string> Manage::getDevicesQueue(){
	printf ("Update Device List\n");

	arv_update_device_list();
	guint n_devices = arv_get_n_devices();

	printf ("Number of found Devices: %d\n", n_devices);

	std::vector<std::string> devicesQueue;
	for (int i = 0; i < n_devices; i++){
		devicesQueue.push_back(arv_get_device_id (i));
		printf ("%s\n", arv_get_device_id (i));
  }
	return devicesQueue;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////// Slots methods ///////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


void Manage::handleStartAcquisition(){
    startAcquisition((ui->devicesQueueComboBox->currentText()).toStdString());
}





void Manage::handleStopAcquisition(){
    if(!stopAc)
      stopAcquisition();
}


void Manage::handleAcquire(){
    // if(!stopAc)
      acq->acquireVideo();
}


void Manage::handleUpdateDevicesQueueCombobox(){
  this->setupDevicesQueueComboBox();
}

void Manage::handleLineEditFreq(const QString &frequency){
  std::string f = frequency.toStdString();
  setFrequency(std::stod(f));
}

void Manage::handleLineEditDelay(const QString &delay){
  std::string d = delay.toStdString();
  setDelay(std::stoi(d));
}

void Manage::handleButtonSelectPath(){

  if(stopAc){
    QString currentPath= QFileDialog::getExistingDirectory(this, ("Select folder to save video"), QString::fromStdString(getPath()), QFileDialog::ShowDirsOnly );
    if (currentPath.toStdString()!=getPath()){
      setPath(currentPath.toStdString());
      const QString qs = QString::fromStdString(getPath());
      ui->labelPath->setText(qs);
    }
  }



}
