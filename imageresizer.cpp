#include "imageresizer.h"
#include "ui_imageresizer.h"

imageresizer::imageresizer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::imageresizer)
{
    ui->setupUi(this);

    //set up the user interface
    initUI();

    //set up the signal slot connections
    initConnections();
}

imageresizer::~imageresizer()
{
    delete ui;
}


void imageresizer::initConnections()
{
    //actions when the browse and convert are clicked
    connect(ui->browseButton, SIGNAL(clicked()), this, SLOT(browseButtonSLOT()));
    connect(resizeButton, SIGNAL(clicked()), this, SLOT(convertButtonSLOT()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->aboutButton, SIGNAL(released()), this, SLOT(aboutButtonSLOT()));

    //combox change is refelected in the spinboxes
    connect(ui->resolutionComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setSize(int)));
}

void imageresizer::initUI()
{
    //set the title of the window/dialog box
    this->setWindowTitle(tr("Image Resizer"));

    //add minimize/maximize buttons to the dialog box
    this->setWindowFlags(Qt::Window);

    //set fixed dimensions to the window
    this->setFixedSize(400, 400);

    //set the progress bar to zero and range to 100
    ui->statusProgressBar->setValue(0);
    ui->statusProgressBar->setRange(0, 100);
    ui->statusProgressBar->setDisabled(true);

    //re-label Ok Buttton from the Button Box as "Convert"
    resizeButton = ui->buttonBox->button(QDialogButtonBox::Ok);
    resizeButton->setText(tr("&Resize"));
    resizeButton->setDisabled(true);

    //re-label Cancel Button from the Button Box as "Close"
    closeButton = ui->buttonBox->button(QDialogButtonBox::Cancel);
    closeButton->setText(tr("Close"));

    //hide progress bar initially
    ui->statusProgressBar->hide();
}

void imageresizer::setSpinBoxes(const int &width, const int &height)
{
    ui->widthSpinBox->setValue(width);
    ui->heightSpinBox->setValue(height);
}

void imageresizer::setSize(const int &currentResolutionSelection)
{
    //move cursor here to indicate that it is editable
    ui->heightSpinBox->setFocus();

    switch(currentResolutionSelection)
    {
    case VGA:setSpinBoxes(640,480);
           break;

    case WVGA:setSpinBoxes(800,480);
           break;

    case SVGA:setSpinBoxes(800,600);
           break;

    case XGA:setSpinBoxes(1024,768);
           break;

    case HD720P:setSpinBoxes(1280,720);
           break;

    case HD1080P:setSpinBoxes(1920,1080);
           break;
    }

}

void imageresizer::setProgress(const int &progressIndex)
{
    //disable convert button while re-sizing is in progress
    resizeButton->setDisabled(true);

    //disable brows button while re-sixing is in progress
    ui->browseButton->setDisabled(true);

    //udpate the progress bar and the debug text browser
    int progressValue = ((progressIndex+1)*100)/(fileNamesList.count());
    ui->statusProgressBar->setValue(progressValue);
    ui->debugTextBrowser->setText(tr("Resizing Image\t") + QString::number(progressIndex+1)+ tr(" of ") + QString::number(fileNamesList.count()));
    ui->widthSpinBox->setDisabled(true);
    ui->heightSpinBox->setDisabled(true);
}

void imageresizer::resizeDoneSLOT()
{
    //resize thread completed sucessfully
    ui->debugTextBrowser->setText(tr("Resizing Done!"));

    //open the directory path from
    QDesktopServices::openUrl(QUrl(targetPath));

    //clear the url line edit which holds all the file's paths
    ui->urlLineEdit->clear();
    fileNamesList.clear();

    //re-enable browse button
    ui->browseButton->setEnabled(true);

    //re-enable spin boxes
    ui->widthSpinBox->setEnabled(true);
    ui->heightSpinBox->setEnabled(true);
}

void imageresizer::resizeLoadErrorSLOT(const QString &loadErrorImage)
{
    //this is triggered from the thread when we are not able to load an image for resizing
    ui->debugTextBrowser->setText(tr("Error Loading Image\t") + loadErrorImage);
}

void imageresizer::resizeSaveErrorSLOT(const QString &saveErrorImage)
{
    //this is triggered from the thread when we are unable to save the image that we just resized
    ui->debugTextBrowser->setText(tr("Error Saving Image\t") + saveErrorImage +tr("\n Disk Full?"));
}



void imageresizer::browseButtonSLOT()
{
    QString fileNamesString;

    //clear the debug text
    ui->debugTextBrowser->clear();

    //clear these strings to start fresh
    fileNamesList.clear();
    fileNamesString.clear();

    //launch a file dialog with the native home location
    fileNamesList = QFileDialog::getOpenFileNames(this, tr("Choose valid image file(s)"), QDir::homePath(), "Images (*.png *.bmp *.jpg *.jpeg *.BMP *.JPG)");

    //convert the string list to a single string for display purpose
    for(int i=0;i < fileNamesList.count(); i++)
    {
        //get each file
        fileNamesString.append(QDir::toNativeSeparators(fileNamesList.at(i)));

        //seperator added after each file
        fileNamesString.append(";");
    }

    //check if any files were selected from the file dialog
    if(fileNamesList.count() > 0)
    {
        //update the linedit with the filename paths
        ui->urlLineEdit->setText(fileNamesString);

        //enable the convert button and display message on the debug text
        resizeButton->setEnabled(true);
        ui->debugTextBrowser->setText(tr("Ready to Resize..."));

        //enable the spin boxes
        ui->widthSpinBox->setEnabled(true);
        ui->heightSpinBox->setEnabled(true);
    }

}

void imageresizer::convertButtonSLOT()
{
    //display the progressbar, since conversion has started
    ui->statusProgressBar->show();

    //get the source file location stripped without the filename
    QString sourceLocation = QDir::toNativeSeparators(fileNamesList.at(0));
    sourceLocation = QFileInfo(sourceLocation).path();

    //create a dummy target location within the source file location
    QString targetLocation = sourceLocation + QDir::separator() + "resized";
    targetPath = targetLocation;


    //check if a "resized" directory already exists in the source location
    if(QDir(targetLocation).exists())
    {
       //do nothing, use the same directory
    }

    else
    {
        //try creating a directory
        if(QDir(sourceLocation).mkdir("resized"))
        {
            //directory was created succesfully
        }
        else
        {   //directory was not created
            ui->debugTextBrowser->setText(tr("Error: Could not create directory for resizing, disk full?"));
        }
    }

    //save the target path to open the directory
    targetPath = targetLocation;

    //enable the progress bar and set initial value
    ui->statusProgressBar->setEnabled(true);
    ui->statusProgressBar->setValue(0);

    //create a new thread instance, object is parented to delete thread on close
    rescaleThread = new rescalethread(fileNamesList, this);
    rescaleThread->setResizeDimensions(ui->widthSpinBox->value(),ui->heightSpinBox->value());

    //start the thread
    rescaleThread->start(QThread::HighestPriority);

    //setup connections for the thread
    connect(rescaleThread, SIGNAL(imageResized(int)), this, SLOT(setProgress(int)));
    connect(rescaleThread, SIGNAL(finished()), this, SLOT(resizeDoneSLOT()));
    connect(rescaleThread, SIGNAL(imageLoadErrorSIGNAL(QString)), this, SLOT(resizeLoadErrorSLOT(QString)));
    connect(rescaleThread, SIGNAL(imageSaveErrorSIGNAL(QString)), this, SLOT(resizeSaveErrorSLOT(QString)));
}

void imageresizer::aboutButtonSLOT()
{
    //create a new about dialog
    aboutDialog = new aboutdialog(this);
    aboutDialog->setAttribute(Qt::WA_DeleteOnClose);

    //show the about dialog
    aboutDialog->show();
}
