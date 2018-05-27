#include "infowindow.h"
#include "ui_infowindow.h"
#include "../tracking/file.h"

InfoWindow::InfoWindow(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::InfoWindow)
{
	ui->setupUi(this);

	QLabel* saveLocationLabel = ui->saveLocation;
	saveLocationLabel->setText(File::getFilePath());

	QLabel* os = ui->os;
	os->setText(QSysInfo::prettyProductName());
}

InfoWindow::~InfoWindow()
{
	delete ui;
}
