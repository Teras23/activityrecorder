#ifndef FILEDATAINFOWINDOW_H
#define FILEDATAINFOWINDOW_H

#include "filedata.h"

#include <QDialog>

namespace Ui {
class FileDataInfoWindow;
}

class FileDataInfoWindow : public QDialog
{
	Q_OBJECT

public:
    explicit FileDataInfoWindow(QWidget *parent = 0);
	~FileDataInfoWindow();

    void changeText(FileData);

private:
	Ui::FileDataInfoWindow *ui;
};

#endif // FILEDATAINFOWINDOW_H
