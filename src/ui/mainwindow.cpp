#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../tracking/process.h"
#include "../tracking/file.h"
#include "../tracking/entry.h"
#include "../ui/infowindow.h"
#include "../tracking/processinfo.h"
#include "../tracking/filedata.h"

#include <QTimer>
#include <QMessageBox>
#include <QDebug>

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

    QAction *saveOverAction = ui->actionSave_Over;
    connect(saveOverAction, SIGNAL(triggered()), this, SLOT(saveOver()));

	InfoWindow *infoWindow = new InfoWindow(this);

	QAction *infoAction = ui->actionInfo;
	connect(infoAction, SIGNAL(triggered()), infoWindow, SLOT(show()));

	m_pollTimer = new QTimer(this);
	m_saveTimer = new QTimer(this);

	connect(m_pollTimer, SIGNAL(timeout()), this, SLOT(update()));
	connect(m_saveTimer, SIGNAL(timeout()), this, SLOT(save()));

	createTray();

    m_statistics = Statistics();

    m_fileData = new FileData();

    m_pollTime = 2000; //10 seconds
    m_saveTime = 5000; //1 minute

	m_pollTimer->start(m_pollTime);
    m_saveTimer->start(m_saveTime);
    update();
}

MainWindow::~MainWindow()
{
    //File::cleanFileData();

    if(m_fileData != nullptr) {
        delete m_fileData;
    }
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

    m_fileData->update(new ProcessInfo(process));

    ui->currentActivityName->setText(QString::fromStdWString(process.getProcessName()));
    ui->currentActivityTitle->setText(QString::fromStdWString(process.getProcessTitle()));
    ui->currentActivityPath->setText(QString::fromStdWString(process.getProcessPath()));
}

void MainWindow::load()
{
    FileData* m_fileData = File::read();
    updateFileDataInfo(*m_fileData);
}

void MainWindow::save()
{
    File::write(m_fileData);
    //QMessageBox* saveInfo = new QMessageBox(this);
    //saveInfo->setText("Saved!");
    //saveInfo->show();
    updateFileDataInfo(*m_fileData);
}

void MainWindow::saveOver()
{
    File::write(m_fileData);
    auto saveInfo = new QMessageBox(this);
    saveInfo->setText("Saved Over!");
    saveInfo->show();
    updateFileDataInfo(*m_fileData);
}

void MainWindow::updateFileDataInfo(FileData fileData)
{
    QTreeWidget *tree = ui->windowsTreeWidget;

    tree->clear();

    for(auto process : fileData.m_processTitles) {
        auto item = new QTreeWidgetItem(0);
        item->setText(0, QString::number(process.first));
        item->setText(1, QString::fromStdWString(fileData.m_processesReverse.at(process.first)));

        for(auto title : process.second)
        {
            auto subitem = new QTreeWidgetItem(0);
            subitem->setText(0, QString::number(title.second));
            subitem->setText(1, QString::fromStdWString(title.first));
            item->addChild(subitem);
        }
        tree->addTopLevelItem(item);
    }

    QTreeWidget *entryTree = ui->entriesTreeWidget;
    entryTree->clear();
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
