#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "process.h"

#include <QTimer>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <sstream>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	QIcon icon(":/res/icon.png");
	this->setWindowIcon(icon);

	QPushButton* button = ui->pushButton;
	QTimer *pollTimer = new QTimer(this);
	QTimer *saveTimer = new QTimer(this);

	connect(button, SIGNAL(released()), this, SLOT(save()));
	connect(pollTimer, SIGNAL(timeout()), this, SLOT(update()));
	connect(saveTimer, SIGNAL(timeout()), this, SLOT(save()));

	createTray();

	pollTimer->start(10);
	saveTimer->start(10000);
}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::update() {
	Process process = Process::getActiveProcess();

	Process::_processHistory.push_back(process);

	std::wstringstream text;

	text << "Name: " << process.getProcessName() << std::endl;
	text << "Title: " << process.getProcessTitle() << std::endl;
	text << "Path: " << process.getProcessPath() << std::endl;

	ui->textBrowser->setText(QString::fromStdWString(text.str()));
}

void MainWindow::save() {
	QString dataLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

	QFile data(dataLocation + "/data.dat");

	std::clog << "Save " << Process::_processHistory.size() << std::endl;

	if(!QDir(dataLocation).exists()) {
		QDir().mkpath(dataLocation);
		std::clog << "Created directory " << dataLocation.toStdString() << std::endl;
	}

	if(data.open(QIODevice::WriteOnly | QIODevice::Text)) {
		data.write("test");
		data.close();
	}
	else {
		QFileInfo info(data);
		std::cerr << "Could not open file " << info.absoluteFilePath().toStdString() << std::endl;
	}

	Process::_processHistory.clear();
}

void MainWindow::createTray() {
	QSystemTrayIcon* trayIcon = new QSystemTrayIcon(QIcon(":/res/icon.png"), this);

	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(toggleTray(QSystemTrayIcon::ActivationReason)));

	QAction *showAction = new QAction("Show", trayIcon);
	connect(showAction, SIGNAL(triggered()), this, SLOT(show()));

	QAction *quitAction = new QAction("Exit", trayIcon);
	connect(quitAction, SIGNAL(triggered()), this, SLOT(quit()));

	QMenu *trayMenu = new QMenu;
	trayMenu->addAction(showAction);
	trayMenu->addAction(quitAction);

	trayIcon->setContextMenu(trayMenu);
	trayIcon->show();
}

void MainWindow::toggleTray(QSystemTrayIcon::ActivationReason reason) {
	if(reason == QSystemTrayIcon::DoubleClick) {
		this->show();
	}
}

void MainWindow::quit() {
	this->hide();
	this->close();
}

void MainWindow::closeEvent(QCloseEvent *event) {
	if(!this->isHidden()) {
		this->hide();
		event->ignore();
	}
}
