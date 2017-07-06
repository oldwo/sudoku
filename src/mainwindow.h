/* mainwindow.h	- SudokuStudio
2017apr24:(VK) Created
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
	void on_pushButton_clicked();

	void on_checkBox1_toggled(bool checked);

private:
	Ui::MainWindow *ui;
	//void shownums();
};//MainWindow

#endif // MAINWINDOW_H
