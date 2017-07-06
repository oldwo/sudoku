/* mainwindow.cpp	- SudokuStudio
2017apr24:(VK) Created
2017apr26:(VK)+show,calculate
2017apr27:(VK)+ponderg
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"

/*
VARS
*****************************************************************************/
//CVSID "$Header: /VKCVS/tools/krIda/SudokuS/src/mainwindow.cpp,v 1.2 2017/07/05 19:41:16 vk Exp $";
char nums[9][9];
int cans[9][9];
QLabel *labls[81];
QLabel *hlabels[9];
QLabel *vlabels[9];
QLabel *qlabels[9];
QLabel *flabels[9];
char nbits[512];
int *group[270];

/*
FUNCTIONS
*****************************************************************************/
void suload(QString s) {
	char *pc=nums[0];
	foreach(const QString &li,s.split('\n',QString::SplitBehavior::SkipEmptyParts)) {
		//if (str.isEmpty())			  break;
		foreach(QChar qc,li) *pc++=qc.toLatin1();
		qDebug(li.toUtf8().data());
	}//foreach
}//suload
QString bitstring(int mask) {
	QString q("");
	q.reserve(10);
	for(int i=9;--i>=0;mask>>=1) if(mask&1) q+=QString('0'+9-i);
	return q;
}//bitstring
void shownums() {
	for(int i=81;--i>=0;) {
		bool f=nums[0][i]>'0' && nums[0][i]<':';
		labls[i]->setProperty("fixed",f);
		labls[i]->setProperty("red",!f && 1==nbits[cans[0][i]]);
		labls[i]->style()->polish(labls[i]);
		if(f)
			labls[i]->setText(QString(nums[0][i])+".");
		else
			labls[i]->setText(bitstring(cans[0][i]));
	}//for
}//shownums
void showcans() {
	/*
	for(int i=9;--i>=0;) {
		hlabels[i]->setText(bitstring(hcan[i]));
		vlabels[i]->setText(bitstring(vcan[i]));
		qlabels[i]->setText(bitstring(qcan[i]));
		flabels[i]->setText(QString::number(f['1'+i]));
	}//for
	*/
}//showcans
void calccans() {
	int hcan[9],vcan[9],qcan[9];
	char f[256];
	memset(f,0,sizeof(f));
	for(int i=9;--i>=0;) hcan[i]=vcan[i]=qcan[i]=511;
	for(int i=9;--i>=0;) {
		for(int j=9;--j>=0;) {
			int mask=1<<nums[i][j]-'1'; mask&=511; mask=511-mask;
			hcan[i]&=mask;
			vcan[j]&=mask;
			qcan[i/3*3+j/3]&=mask;
			++f[nums[i][j]];
		}//for
	}//for
	for(int i=9;--i>=0;) {
		hlabels[i]->setText(bitstring(hcan[i]));
		vlabels[i]->setText(bitstring(vcan[i]));
		qlabels[i]->setText(bitstring(qcan[i]));
		flabels[i]->setText(QString::number(f['1'+i]));
	}//for
	showcans();
	for(int i=9;--i>=0;) {
		for(int j=9;--j>=0;) {
			cans[i][j]=(nums[i][j]>'0' && nums[i][j]<':')?1<<nums[i][j]-'1':
					hcan[i]&vcan[j]&qcan[i/3*3+j/3];
			labls[i*9+j]->setProperty("nine",f[nums[i][j]]>8);
			//qDebug("%d,%d=%c->%d",i,j,nums[i][j],f[nums[i][j]]);
		}//for
	}//for
}//calccans
bool ponderg(int**group) {
	bool r=false;
	char f[512];
	memset(f,0,sizeof(f));
	qDebug("\tpondering %p=`%s`",group,qPrintable(bitstring(**group)));
	//a couple is tied to those places
	for(int i=9;--i>=0;) ++f[*group[i]];
	for(int i=9;--i>=0;) if(2==f[*group[i]] && 2==nbits[*group[i]]) for(int j=9;--j>=0;) {
		if(*group[j]!=*group[i]) {
			r |= *group[j]!=(*group[j]&~*group[i]);
			*group[j]&=~*group[i];}
	}//for-if-for
	//one number available
	for(int i=9;--i>=0;) if(1==nbits[*group[i]]) for(int j=9;--j>=0;) {
		if(j!=i) {
			r |= *group[j]!=(*group[j]&~*group[i]);
			*group[j]&=~*group[i];}
	}//for-if-for
	//one location possible
	int cum=0,mul=0;
	for(int i=9;--i>=0;) {
		int x=cum& *group[i];
		mul|=x;
		cum^=x;
	}//for
	qDebug("single=$%x",511-mul);
	/*
		if(1==nbits[*group[i]]) for(int j=9;--j>=0;) {
		if(j!=i) {
			r |= *group[j]!=(*group[j]&~*group[i]);
			*group[j]&=~*group[i];}
	}//for-if-for
*/
	return r;
}//ponderg
bool ponder() {
	bool r=false;
	for(int i=27;--i>=0;) r|=ponderg(group+i*10);
	return r;
}//ponder

/*
CLASSES
*****************************************************************************/
MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	for(int i=0,lag=1;++i<512;) {
		if(i>=lag+lag) lag+=lag;
		nbits[i]=nbits[i-lag]+1;
	}//for
	for(int i=9;--i>=0;) {
		for(int j=9;--j>=0;) {
			group[i*10+j]=cans[i]+j;
			group[90+i*10+j]=cans[j]+i;
			group[180+i*10+j]=cans[i/3*3+j/3]+i%3*3+j%3;
		}//for
	}//for
	for(int i=81;--i>=0;) {
		ui->gridLayout->addWidget(labls[i]=new QLabel(),i/9,i%9,1,1);
		labls[i]->show();
	}//for
	for(int i=9;--i>=0;) {
		ui->gridLayout->addWidget(hlabels[i]=new QLabel("Hh"),i,9);
		ui->gridLayout->addWidget(vlabels[i]=new QLabel("vV"),9,i,1,1);
		ui->qgrid->addWidget(qlabels[i]=new QLabel("Q"),i/3,i%3);
		ui->fgrid->addWidget(flabels[i]=new QLabel("f"),i,0);
		//hlabels[i]->show();
		//vlabels[i]->show();
		//qlabels[i]->show();
		hlabels[i]->setProperty("sum",true);
		vlabels[i]->setProperty("sum",true);
		qlabels[i]->setProperty("sum",true);
	}//for
	suload("\
====79=53\n\
=======9=\n\
==6=3512=\n\
=45====39\n\
3=2====1=\n\
=79====46\n\
==7=24961\n\
=======82\n\
====61=75");/*
	suload(R"(
====79=53
=======9=
==6=3512=
=45====39
3=2====1=
=79==3=46
==7=24961
======382
====61475
)");
	suload(R"(
418279653
253===794
796435128
=45=8==39
3=2946517
=79=5==46
==7=24961
==1=9=482
924861375
)");//*/
	calccans();
	shownums();
}//MainWindow::MainWindow

MainWindow::~MainWindow()
{
	delete ui;
}//MainWindow::~MainWindow
/*
void MainWindow::shownums() {
	///k
}//MainWindow::shownums
*/
void MainWindow::on_pushButton_clicked()
{
	qDebug("pushing");
	printf("terminator ponder=%d\n",ponder());
	shownums();
	showcans();
	qDebug("unpushing");
	return;
	//ui->label->setText("PaTch");
	/*suload(R"(
0==456===
===123===
===123===
123===789
123===789
123===789
===123===
===123===
===123==9
)");*///
	calccans();
	shownums();
}//MainWindow::on_pushButton_clicked

void MainWindow::on_checkBox1_toggled(bool checked)
{
	int mask=0;
	if(ui->checkBox1->checkState())mask|=1;
	if(ui->checkBox2->checkState())mask|=2;
	if(ui->checkBox3->checkState())mask|=4;
	if(ui->checkBox4->checkState())mask|=8;
	if(ui->checkBox5->checkState())mask|=16;
	if(ui->checkBox6->checkState())mask|=32;
	if(ui->checkBox7->checkState())mask|=64;
	if(ui->checkBox8->checkState())mask|=128;
	if(ui->checkBox9->checkState())mask|=256;
	qDebug("toggled mask=%d c00=%d",mask,cans[0][0]);
	for(int i=81;--i>=0;) {
		labls[i]->setProperty("hi",mask==(cans[0][i]&mask));
		labls[i]->style()->polish(labls[i]);
	}//for
}//MainWindow::on_checkBox_toggled
