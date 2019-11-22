#include "MainWindow.h"
#include <QApplication>
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>
#include <iostream>
// State Machine
#include "state_machine.h"
#include "input.h"
#include "logger.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
    
}
