#ifndef IMAGERESIZER_H
#define IMAGERESIZER_H


//qt libraries
#include <QDialog>
#include <QDir>
#include <QFileDialog>
#include <QDebug>
#include <QDesktopWidget>
#include <QImage>
#include <QDesktopServices>



//thread object
#include "rescalethread.h"

//about dialog
#include "aboutdialog.h"

namespace Ui {
class imageresizer;
}

class imageresizer : public QDialog
{
    Q_OBJECT
    
public:
    explicit imageresizer(QWidget *parent = 0);
    ~imageresizer();
    
private:
    Ui::imageresizer *ui;

    //button pointers for the button box buttons
    QPushButton *resizeButton;
    QPushButton *closeButton;

    /*holds the list of files that were selected, both variables serve
    the same purpose, but are formatted differently for usage*/
    QStringList fileNamesList;

    //target file name and target file path
    QString targetFile;
    QString targetPath;

    //create a thread object
    rescalethread *rescaleThread;

    //create an about dialog object
    aboutdialog *aboutDialog;

    //init methods to set signal/slot connections and the gui
    void initConnections();
    void initUI();

    //method to set the values of the spinboxes
    void setSpinBoxes(const int &width, const int &height);

    //enumerated types for resolution
    enum {VGA, WVGA, SVGA, XGA, HD720P, HD1080P};


private slots:

    //slots triggered when buttons are pressed
    void browseButtonSLOT();
    void convertButtonSLOT();
    void aboutButtonSLOT();

    //slots triggered by signals from the thread object
    void setProgress(const int &progressIndex);
    void resizeDoneSLOT();
    void resizeLoadErrorSLOT(const QString &loadErrorImage);
    void resizeSaveErrorSLOT(const QString &saveErrorImage);

    //slot to set the state of spinboxes
    void setSize(const int &currentResolutionSelection);
};

#endif // IMAGERESIZER_H
