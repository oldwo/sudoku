/* bigentry.cpp
2017jul04:(VK) Created
*/
#include "bigentry.h"
#include "ui_bigentry.h"

BigEntry::BigEntry(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::BigEntry)
{
	ui->setupUi(this);
}

BigEntry::~BigEntry()
{
	delete ui;
}

void BigEntry::setValue(int AValue)
{
	if(AValue<0 || AValue>9) return;
	(&ui->pb0)[AValue]->setStyleSheet("background-color:red");
	setResult(AValue);
	r=AValue;
}//BigEntry::setValue


void BigEntry::on_BigEntry_rejected()
{
	//int rr=result();
	setResult(r);
	//done(result()); CRASHES
}
