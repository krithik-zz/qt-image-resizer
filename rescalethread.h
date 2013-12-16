#ifndef RESCALETHREAD_H
#define RESCALETHREAD_H

//include all the qt headers
#include <QThread>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QImage>
#include <QDebug>
#include <QDir>



//touch screen dimensions
#define ts_width 800
#define ts_height 480


class rescalethread: public QThread
{
    Q_OBJECT

public:
    rescalethread(QStringList sourceImageFiles, QObject *parent = 0);
    void setResizeDimensions(int width, int height);
    ~rescalethread();


private:
    void run();

    //core method for resizing images
    void scaleImage(const QString &imageFileName, const int &currentIndex);

    //list of images that are going to be resized
    QStringList imageFilesList;

    int imageCount;
    int resizeWidth;
    int resizeHeight;

    QString targetFileName;

signals:
    void finished();
    void imageResized(int currentIndex);
    void imageLoadErrorSIGNAL(QString loadErrorImage);
    void imageSaveErrorSIGNAL(QString saveErrorImage);
};


#endif // RESCALETHREAD_H
