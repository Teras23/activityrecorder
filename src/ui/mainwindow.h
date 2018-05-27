#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "../tracking/entry.h"
#include "../tracking/filedata.h"

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

	void createTray();
    void updateFileDataInfo(FileData);
	void closeEvent(QCloseEvent *event);

	Entry m_entry;
};

#endif // MAINWINDOW_H
