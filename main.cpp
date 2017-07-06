/* main.cpp	- SudokuStudio
2017apr24:(VK) Created
2017jul03:(VK)*mainw2
*/
#include "src/mainw2.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainW2 w;
	w.show();

	return a.exec();
}
