#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "process.h"
#include "file.h"
#include "entry.h"
#include "infowindow.h"
#include "processinfo.h"
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

    this->setWindowIcon(getIcon());
#ifdef QT_DEBUG
    this->setWindowTitle("Activity Recorder Debug");
#else
    this->setWindowTitle("Activity Recorder");
#endif
    QAction *loadAction = ui->actionLoad;
    connect(loadAction, SIGNAL(triggered()), this, SLOT(load()));

	QAction *saveAction = ui->actionSave;
	connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

	InfoWindow *infoWindow = new InfoWindow(this);

	QAction *infoAction = ui->actionInfo;
	connect(infoAction, SIGNAL(triggered()), infoWindow, SLOT(show()));

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

    ui->currentActivityName->setText(QString::fromStdWString(process.getProcessName()));
    ui->currentActivityTitle->setText(QString::fromStdWString(process.getProcessTitle()));
    ui->currentActivityPath->setText(QString::fromStdWString(process.getProcessPath()));
}

void MainWindow::load()
{
    FileData fileData = File::load();
    updateFileDataInfo(fileData);
}

void MainWindow::save()
{   
    m_entry.endCurrent();
    FileData fileData = File::update(m_entry);
    QMessageBox* saveInfo = new QMessageBox(this);
    saveInfo->setText("Saved!");
    saveInfo->show();
    updateFileDataInfo(fileData);
}

void MainWindow::updateFileDataInfo(FileData fileData)
{
    QTreeWidget *tree = ui->windowsTreeWidget;

    tree->clear();

    for(auto process : fileData.m_processTitles) {
        QTreeWidgetItem *item = new QTreeWidgetItem(0);
        item->setText(0, QString::number(process.first));
        item->setText(1, QString::fromStdWString(fileData.m_processesReverse.at(process.first)));

        for(auto title : process.second)
        {
            QTreeWidgetItem *subitem = new QTreeWidgetItem(0);
            subitem->setText(0, QString::number(title.second));
            subitem->setText(1, QString::fromStdWString(title.first));
            item->addChild(subitem);
        }
        tree->addTopLevelItem(item);
    }
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
