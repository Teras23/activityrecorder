#include "filedatainfowindow.h"
#include "ui_filedatainfowindow.h"

#include <QDebug>

FileDataInfoWindow::FileDataInfoWindow(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::FileDataInfoWindow)
{
	ui->setupUi(this);
}

FileDataInfoWindow::~FileDataInfoWindow()
{
	delete ui;
}

void FileDataInfoWindow::changeText(FileData fileData)
{
    ui->textBrowser->append("Process Names:");

    for(auto process : fileData.m_processes)
    {
        ui->textBrowser->append(QString::fromStdWString(process.first) + " " + QString::number(process.second));
    }

    ui->textBrowser->append("Process Titles:");

    for(auto process : fileData.m_processTitles)
    {
        qDebug() << process.first;

        for(auto title : process.second)
        {
            qDebug() << QString::fromStdWString(title.first) << " " << QString::number(title.second);
        }
    }

    for(auto process : fileData.m_processTitles)
    {
        ui->textBrowser->append(QString::number(process.first));

        for(auto title : process.second)
        {
            ui->textBrowser->append(QString::fromStdWString(title.first) + " " + QString::number(title.second));
        }
    }
}
