#include "rescalethread.h"

rescalethread::rescalethread(QStringList sourceImageFiles, QObject *parent)
    :QThread(parent)
{
    //point the parent's stringlist which holds the list of files to a local variable
    imageFilesList = sourceImageFiles;
}

//method to set the resize dimensions for the thread
void rescalethread::setResizeDimensions(int width, int height)
{
    resizeWidth = width;
    resizeHeight = height;
}

rescalethread::~rescalethread()
{
    //call for delete later, this routine will be triggered by the parent thread
    deleteLater();
}

void rescalethread::run()
{
    //this is the routine that will run when the thread is triggered by start() method in the parent

    for(int index=0;index<imageFilesList.count();index++)
    {
        //call the method to scale image, pass the current filename from the list
        scaleImage(QDir::toNativeSeparators(imageFilesList.at(index)), index);
    }

    //re-implemented finished signal emited when all the image resizing was done succesfully
    emit finished();

}

void rescalethread::scaleImage(const QString &imageFileName, const int &currentIndex)
{

    QImage image, result;

    //construct the sting for the filename of the resized image
    targetFileName = QFileInfo(imageFileName).path() + QDir::separator() +"resized" + QDir::separator() + QFileInfo(imageFileName).baseName()+ "_resized" +".jpg";

    //if image was loaded succesfully, then scale it
    if(image.load(imageFileName))
    {
        //resize to this width/height and save to target file location
        result = image.scaled(resizeWidth, resizeHeight, Qt::KeepAspectRatio, Qt::FastTransformation);

        //try saving the image
        if(result.save(targetFileName, "JPG"))
        {
            //this signal is required to communicate the progress status on the main GUI thread
            emit imageResized(currentIndex);
        }

        else
        {
            //there was a problem with saving the image
            emit imageSaveErrorSIGNAL(imageFileName);
        }
    }

    else
    {
        //there was a problem with loading the image
        emit imageLoadErrorSIGNAL(imageFileName);
    }


}
