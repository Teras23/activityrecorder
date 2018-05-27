#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "process.h"
#include "../tracking/file.h"
#include "../tracking/entry.h"
#include "../ui/infowindow.h"
#include "../tracking/processinfo.h"
#include "../tracking/filedata.h"

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

    m_entry = new Entry();

    m_pollTime = 3000; //10 seconds
    m_saveTime = 60000; //1 minute

	m_pollTimer->start(m_pollTime);
    m_saveTimer->start(m_saveTime);
}

MainWindow::~MainWindow()
{
    File::cleanFileData();

    if(m_entry != nullptr) {
        delete m_entry;
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

    m_entry->update(ProcessInfo(process));

    ui->currentActivityName->setText(QString::fromStdWString(process.getProcessName()));
    ui->currentActivityTitle->setText(QString::fromStdWString(process.getProcessTitle()));
    ui->currentActivityPath->setText(QString::fromStdWString(process.getProcessPath()));
}

void MainWindow::load()
{
    FileData* fileData = File::load();
    updateFileDataInfo(*fileData);
}

void MainWindow::save()
{   
    m_entry->endCurrent();
    FileData* fileData = File::update(m_entry);
    m_entry = new Entry();
    QMessageBox* saveInfo = new QMessageBox(this);
    //saveInfo->setText("Saved!");
    //saveInfo->show();
    updateFileDataInfo(*fileData);
}

void MainWindow::saveOver()
{
    m_entry->endCurrent();
    FileData* fileData = File::saveOver(m_entry);
    m_entry = new Entry();
    QMessageBox* saveInfo = new QMessageBox(this);
    saveInfo->setText("Saved Over!");
    saveInfo->show();
    updateFileDataInfo(*fileData);
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

    QTreeWidget *entryTree = ui->entriesTreeWidget;
    entryTree->clear();

    for(auto entry : fileData.m_entries) {
        QTreeWidgetItem *item = new QTreeWidgetItem(0);

        item->setText(0, QString(entry->m_startTime.toString() + " -> " + entry->m_endTime.toString()));

        for(auto processInfo : entry->m_processBuffer)
        {
            QTreeWidgetItem *subitem = new QTreeWidgetItem(0);
            subitem->setText(0, processInfo.getRecordedTime().toString());
            subitem->setText(1, QString::number(processInfo.getProcessTitleId()));
            item->addChild(subitem);
        }
        entryTree->addTopLevelItem(item);
    }
    entryTree->resizeColumnToContents(0);

    QTreeWidget *statisticsTree = ui->statisticsTreeWidget;

    File::generateStatistics();
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
