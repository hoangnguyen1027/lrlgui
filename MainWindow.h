#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <wiringPi.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

	protected slots :
		void Launch_Button_1_clicked();
		void Launch_Button_2_clicked();
		void Launch_Button_3_clicked();
		void Launch_Button_4_clicked();
		void Launch_Button_5_clicked();
		void Launch_Button_6_clicked();

		void PT_Button_clicked();
		void Start_Button_clicked();
		void Emergency_Button_clicked();
		void Cancel_Button_clicked();
private:
		Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
