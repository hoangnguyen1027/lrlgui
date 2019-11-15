#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>
// State Machine
#include "state_machine.h"
#include "input.h"
#include "logger.h"

// Hardwares
#include <iostream>
#include <cstdio>
#include <thread>
#include <chrono>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	wiringPiSetup();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ButtonClicked()
{
    QMessageBox msgBox;
    msgBox.setText("Hello, World!");
    msgBox.setWindowTitle("VisualGDB Qt Demo");
    msgBox.exec();
}
