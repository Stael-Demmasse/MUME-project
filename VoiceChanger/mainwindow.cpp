#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

#include "Data.h"
#include "PortaudioSetup.h"

PaError err;
static callbackData* userData;
PaStream *stream;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // initializing own data
    err = paNoError;

    initializePa(&err);
    initializeUserData(&userData);
    createStream(&stream, userData, &err);

    qDebug() << "MainWindow created successfully";
}

MainWindow::~MainWindow()
{
    closeStream(stream, &err);
    freeUserData(userData);
    terminatePa(&err);
    delete ui;
}


void MainWindow::on_startButton_clicked(bool checked) {
    if (checked) {
        startStream(stream, &err);
        ui->startButton->setText("Stop");
        ui->startButton->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackStop));
    } else {
        stopStream(stream, &err);
        ui->startButton->setText("Start");
        ui->startButton->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::MediaRecord));
    }
}

void MainWindow::on_pitchShiftButton_clicked(bool checked)
{
    if (checked) {
        userData->aFilter->pitchShift = true;
    }
    else {
        userData->aFilter->pitchShift = false;
    }
}

void MainWindow::on_echoButton_clicked(bool checked)
{
    if (checked) {
        userData->aFilter->echo = true;
    }
    else {
        userData->aFilter->echo = false;
    }

}

void MainWindow::on_robotButton_clicked(bool checked)
{
    if (checked) {
        userData->aFilter->robot = true;
    }
    else {
        userData->aFilter->robot = false;
    }
}


void MainWindow::on_shiftValue_sliderMoved(int position)
{
    userData->aFilter->shiftFactor = (double)position/10;
}
