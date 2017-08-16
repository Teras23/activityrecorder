#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "activitychecker.h"

#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	QPushButton* button = ui->pushButton;
	QTimer* pollTimer = new QTimer(this);
	QTimer* saveTimer = new QTimer(this);

	connect(button, SIGNAL(released()), this, SLOT(handleButton()));
	connect(pollTimer, SIGNAL(timeout()), this, SLOT(update()));

	createTray();

	pollTimer->start(1000);
	saveTimer->start(10000);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::handleButton()
{
	std::string windowName = ActivityChecker::getActivity();
	ui->label->setText(QString::fromStdString(windowName));
}

void MainWindow::update() {
	std::string windowName = ActivityChecker::getActivity();
	ui->label->setText(QString::fromStdString(windowName));
}

void MainWindow::createTray() {
	QSystemTrayIcon* trayIcon = new QSystemTrayIcon(QIcon(":/icon.png"), this);

	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(toggleTray()));

	QAction* showAction = new QAction("Show", trayIcon);
	connect(showAction, SIGNAL(triggered()), this, SLOT(show()));

	QAction* quitAction = new QAction("Exit", trayIcon);
	connect(quitAction, SIGNAL(triggered()), this, SLOT(quit()));

	QMenu* trayMenu = new QMenu;
	trayMenu->addAction(showAction);
	trayMenu->addAction(quitAction);

	trayIcon->setContextMenu(trayMenu);
	trayIcon->show();
}

void MainWindow::toggleTray(QSystemTrayIcon::ActivationReason reason) {
	if(reason = QSystemTrayIcon::DoubleClick) {
		this->show();
	}
}

void MainWindow::quit() {
	this->hide();
	this->close();
}

void MainWindow::closeEvent(QCloseEvent* event) {
	if(!this->isHidden()) {
		this->hide();
		event->ignore();
	}
}
