#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "process.h"
#include "file.h"
#include "entry.h"
#include "infowindow.h"
#include "processinfo.h"
#include "filedatainfowindow.h"
#include "filedata.h"

#include <QTimer>
#include <QMessageBox>

#include <sstream>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	QIcon icon = getIcon();

	this->setWindowIcon(icon);

	QAction *saveAction = ui->actionSave;
	connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

	InfoWindow *infoWindow = new InfoWindow(this);

	QAction *infoAction = ui->actionInfo;
	connect(infoAction, SIGNAL(triggered()), infoWindow, SLOT(show()));

    QAction *fileDataInfoAction = ui->actionFileData_Info;
    connect(fileDataInfoAction, SIGNAL(triggered()), this, SLOT(showFileDataInfoWindow()));

	m_pollTimer = new QTimer(this);
	m_saveTimer = new QTimer(this);

	connect(m_pollTimer, SIGNAL(timeout()), this, SLOT(update()));
	connect(m_saveTimer, SIGNAL(timeout()), this, SLOT(save()));

	createTray();

    m_entry = Entry();

	m_pollTime = 1000;
	m_saveTime = 10000;

	m_pollTimer->start(m_pollTime);
	//m_saveTimer->start(10000);
}

MainWindow::~MainWindow()
{
	delete ui;
}

QIcon MainWindow::getIcon()
{
#ifdef QT_DEBUG
	return QIcon(":/res/icon_debug.png");
#else
	return QIcon(":/res/icon.png");
#endif
}


void MainWindow::update()
{
	Process process = Process::getActiveProcess();

    m_entry.update(ProcessInfo(process));

	std::wstringstream text;

	text << "Name: " << process.getProcessName() << std::endl;
	text << "Title: " << process.getProcessTitle() << std::endl;
	text << "Path: " << process.getProcessPath() << std::endl;

	ui->textBrowser->setText(QString::fromStdWString(text.str()));
}

void MainWindow::save()
{   
    m_entry.endCurrent();
    File::update(m_entry);
    QMessageBox* saveInfo = new QMessageBox(this);
    saveInfo->setText("Saved!");
    saveInfo->show();
}

void MainWindow::showFileDataInfoWindow()
{
    m_entry.endCurrent();
    auto fileData = File::update(m_entry);

    FileDataInfoWindow *infoWindow = new FileDataInfoWindow(this);
    infoWindow->changeText(fileData);
    infoWindow->show();
}

void MainWindow::createTray()
{
	QSystemTrayIcon *trayIcon = new QSystemTrayIcon(getIcon(), this);

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

void MainWindow::toggleTray(QSystemTrayIcon::ActivationReason reason)
{
	if (reason == QSystemTrayIcon::DoubleClick) {
		this->show();
	}
}

void MainWindow::quit()
{
	this->hide();
	this->close();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
#ifndef QT_DEBUG
	if (!this->isHidden()) {
		this->hide();
		event->ignore();
	}
#endif
    event->accept();
}
