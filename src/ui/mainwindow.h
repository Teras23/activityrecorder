#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "../tracking/filedata.h"
#include "statistics.h"

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QCloseEvent>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow 
{
	Q_OBJECT
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void update();
	void toggleTray(QSystemTrayIcon::ActivationReason);
	void quit();
    void load();
    void save();
    void saveOver();

private:
	QIcon getIcon();

	Ui::MainWindow *ui;

	QTimer *m_pollTimer;
	QTimer *m_saveTimer;

	int m_pollTime;
	int m_saveTime;

    Statistics m_statistics;

	void createTray();
    void updateFileDataInfo(FileData);
	void closeEvent(QCloseEvent *event);

    FileData* m_fileData;
};

#endif // MAINWINDOW_H
