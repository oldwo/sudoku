/* bigentry.h
2017jul04:(VK) Created
*/
#ifndef BIGENTRY_H
#define BIGENTRY_H

#include <QDialog>

namespace Ui {
class BigEntry;
}

class BigEntry : public QDialog
{
	Q_OBJECT

public:
	explicit BigEntry(QWidget *parent = 0);
	~BigEntry();
	void setValue(int);

private slots:
	void on_pb0_clicked() {done(10);}//distinguish from 0=abort
	void on_pb1_clicked() {done(1);}
	void on_pb2_clicked() {done(2);}
	void on_pb3_clicked() {done(3);}
	void on_pb4_clicked() {done(4);}
	void on_pb5_clicked() {done(5);}
	void on_pb6_clicked() {done(6);}
	void on_pb7_clicked() {done(7);}
	void on_pb8_clicked() {done(8);}
	void on_pb9_clicked() {done(9);}

	void on_BigEntry_rejected();

private:
	Ui::BigEntry *ui;
	int r;
};

#endif // BIGENTRY_H
