#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>
#include <iostream>
// State Machine
#include "state_machine.h"
#include "input.h"
#include "logger.h"
#include "b1_states.h"

// Hardwares
#include <cstdio>
#include <thread>
#include <chrono>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Start_Button_clicked()
{
	state_machine& sm = state_machine::getInstance();
	initializePins();
	sm.cv_isRunning.notify_all();

	QMessageBox msgBox;
	msgBox.setText("State Machine Started");
	msgBox.setWindowTitle("Flight Computer");
	msgBox.exec();
	
}

void MainWindow::Launch_Button_1_clicked()
{
	state_machine& sm = state_machine::getInstance();
	b1_states::fn_init_idle(sm.getCurrentState());
	QMessageBox msgBox;
	msgBox.setText("Pressure Valves closed. Vent Valves closed.");
	msgBox.setWindowTitle("Flight Computer");
	msgBox.exec();

	
}	

void MainWindow::Launch_Button_2_clicked()
{
	state_machine& sm = state_machine::getInstance();
	b1_states::fn_idle_fill(sm.getCurrentState());
	QMessageBox msgBox;
	msgBox.setText("Pressure Valves closed. Vent Valves opened.");
	msgBox.setWindowTitle("Flight Computer");
	msgBox.exec();
}

void MainWindow::Launch_Button_3_clicked()
{
	state_machine& sm = state_machine::getInstance();
	b1_states::fn_fill_pressurize(sm.getCurrentState());
	QMessageBox msgBox;
	msgBox.setText("Bronco One is being pressurized. Continue when pressurization is being finished");
	msgBox.setWindowTitle("Flight Computer");
	msgBox.exec();
}

void MainWindow::Launch_Button_4_clicked()
{
	state_machine& sm = state_machine::getInstance();
	b1_states::fn_connect_battery(sm.getCurrentState());
	QMessageBox msgBox;
	msgBox.setText("Engaging battery power. Start discussing external power.");
	msgBox.setWindowTitle("Flight Computer");
	msgBox.exec();
}

void MainWindow::Launch_Button_5_clicked()
{
	state_machine& sm = state_machine::getInstance();
	b1_states::fn_pressurize_pressurized(sm.getCurrentState());
	QMessageBox msgBox;
	msgBox.setText("Bronco One is pressurized and ready to launch. Waiting on your go...");
	msgBox.setWindowTitle("Flight Computer");
	msgBox.exec();
}

void MainWindow::Launch_Button_6_clicked()
{
	QMessageBox msgBox;
	msgBox.setText("Launching the Bronco One");
	msgBox.setWindowTitle("Flight Computer");
	msgBox.exec();
	state_machine& sm = state_machine::getInstance();
	b1_states::fn_pressurized_launch(sm.getCurrentState());
	
}

void MainWindow::PT_Button_clicked()
{
	state_machine& sm = state_machine::getInstance();
	b1_states::fn_DATA(sm.getCurrentState());
}

void MainWindow::Emergency_Button_clicked()
{
	state_machine& sm = state_machine::getInstance();
	b1_states::fn_EMERGENCY(sm.getCurrentState());
}

void MainWindow::Cancel_Button_clicked()
{
	state_machine& sm = state_machine::getInstance();
	b1_states::fn_CANCEL(sm.getCurrentState());
}