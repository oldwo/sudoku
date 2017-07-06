/* mainw2.cpp
2017jul03:(VK) Created
2017jul04:(VK)+editing
2017jul05:(VK)+h/v/q-labels,SStack,SHil
*/
#include "mainw2.h"
#include "ui_mainw2.h"
#include <QPainter>
#include "bigentry.h"
#include <QStack>

#define CVSID static int _cvsiduse=_cvsiduse+(long)

/*
VARS
*****************************************************************************/
CVSID "$Header: /VKCVS/tools/krIda/SudokuS/src/mainw2.cpp,v 1.1 2017/07/05 19:41:16 vk Exp $";
static char SBG[9][9];
char SHil='~';//highlight one of 9 numbers
static QLabel *hlabels[9];
static QLabel *vlabels[9];
static QLabel *qlabels[9];
QStack<QString> SStack;

/*
FUNCTIONS
*****************************************************************************/
static QString bitstring(int mask) {
	//return QString::asprintf("%d",mask);
	QString q;
	q.reserve(11);
	for(int i=10;--i>=0;mask>>=1) if(mask&1) q+=QString('0'+9-i);
	return q;
}//bitstring

/*
CLASSES
*****************************************************************************/
//Tstmodel -------------------------------------------------------------------
QModelIndex Tstmodel::index(int row,int column,const QModelIndex &/*parent*/) const {
	return createIndex(row,column);
}//Tstmodel::index

QModelIndex Tstmodel::parent(const QModelIndex &/*child*/) const {
	return QModelIndex();
}//Tstmodel::parent

QVariant Tstmodel::data(const QModelIndex &AIndex,int role) const {
	if (!AIndex.isValid()) return QVariant();
	if(role==Qt::DisplayRole || role==Qt::EditRole)
		return  Fsudoku.nums[AIndex.row()][AIndex.column()];
	if(role==Qt::ToolTipRole)
		return Fsudoku.cans[AIndex.row()][AIndex.column()];
///		return  QString::asprintf("r=%d c=%d",AIndex.row(),AIndex.column());
	return QVariant();
}//Tstmodel::data

Qt::ItemFlags Tstmodel::flags(const QModelIndex &AIndex) const
{
	if(!AIndex.isValid()) return Qt::ItemIsEnabled;
	if(SBG[AIndex.row()][AIndex.column()]%3) return QAbstractItemModel::flags(AIndex);
	return QAbstractItemModel::flags(AIndex) | Qt::ItemIsEditable;
}//Tstmodel::flags

bool Tstmodel::setData(const QModelIndex&AIndex,const QVariant&AValue,int ARole)
{
	if(Qt::EditRole==ARole) Fsudoku.setValue(AIndex.row(),AIndex.column(),AValue.toInt());
	else return false;
	Fsudoku.calccans();
	layoutChanged();
	for(int i=9;--i>=0;) {
		hlabels[i]->setText(bitstring(Fsudoku.hcan[i]));
		vlabels[i]->setText(bitstring(Fsudoku.vcan[i]));
		qlabels[i]->setText(bitstring(Fsudoku.qcan[i]));
		//flabels[i]->setText(QString::number(f['1'+i]));
	}//for

	return true;
	//qDebug("setData(r%d,c%d)",AIndex.row(),AIndex.column());
}//Tstmodel::setData

//TCellDelegate --------------------------------------------------------------
void TCellDelegate::paint(QPainter *painter,const QStyleOptionViewItem &option,const QModelIndex &AIndex) const {
	int r=AIndex.row(),c=AIndex.column();
	int i=AIndex.model()->data(AIndex,Qt::DisplayRole).toInt();
	if(1==SBG[r][c]) painter->fillRect(option.rect,Qt::gray);
	if(2==SBG[r][c]) painter->fillRect(option.rect,Qt::yellow);
	if(3==SBG[r][c]) painter->fillRect(option.rect,Qt::green);
	if(SHil==i) painter->fillRect(option.rect,Qt::darkRed);
	if(option.state & QStyle::State_Selected) painter->fillRect(option.rect,option.palette.highlight());

	//int size = qMin(option.rect.width(), option.rect.height());
	painter->save();
	painter->setRenderHint(QPainter::Antialiasing, true);
	painter->setPen(Qt::NoPen);

	if (option.state & QStyle::State_Selected)
		painter->setBrush(option.palette.highlightedText());
	else
		painter->setBrush(option.palette.text());
	///painter->setBrush(option.palette.highlightedText());//invisible
	painter->setPen(Qt::black);
	if(i) {
		if(SHil==i) painter->setPen(Qt::darkYellow);
		painter->drawText(QRect(option.rect),Qt::AlignCenter,QString::asprintf("%d",i));
	} else {
		int i=AIndex.model()->data(AIndex,Qt::ToolTipRole).toInt();
		QString s=bitstring(i);
		painter->setPen(Qt::blue);
		painter->setFont(QFont("Helvetica",10-s.length()/1.5));
		QPoint piv=option.rect.center();
		painter->translate(piv);
		painter->rotate(-35);
		painter->translate(-piv);
		if((1<<SHil)&i) painter->setPen(Qt::darkYellow);
		painter->drawText(QRect(option.rect),Qt::AlignCenter,s);
	}//if
	painter->restore();
}//TCellDelegate::paint

QWidget *TCellDelegate::createEditor(QWidget *parent,const QStyleOptionViewItem &/* option */,const QModelIndex &/* index */) const {
	return new BigEntry(parent);
}//TCellDelegate::createEditor

void TCellDelegate::setEditorData(QWidget *editor,const QModelIndex &index) const {
	int value=index.model()->data(index,Qt::EditRole).toInt();
	BigEntry *entry=static_cast<BigEntry*>(editor);
	entry->setValue(value);
}//TCellDelegate::setEditorData

void TCellDelegate::setModelData(QWidget*editor,QAbstractItemModel*model,const QModelIndex&AIndex) const {
	BigEntry *entry=static_cast<BigEntry*>(editor);
	model->setData(AIndex,entry->result()%10);
	qDebug("//TCellDelegate::setEditorData");
}//TCellDelegate::setModelData

//MainW2 ---------------------------------------------------------------------
MainW2::MainW2(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainW2)
{
	ui->setupUi(this);
	ui->tableView->setModel(&FModel);
	ui->tableView->setItemDelegate(&FDelegate);
	hlabels[0]=ui->lblH0; vlabels[0]=ui->lblV0; qlabels[0]=ui->lblQ0;
	hlabels[1]=ui->lblH1; vlabels[1]=ui->lblV1; qlabels[1]=ui->lblQ1;
	hlabels[2]=ui->lblH2; vlabels[2]=ui->lblV2; qlabels[2]=ui->lblQ2;
	hlabels[3]=ui->lblH3; vlabels[3]=ui->lblV3; qlabels[3]=ui->lblQ3;
	hlabels[4]=ui->lblH4; vlabels[4]=ui->lblV4; qlabels[4]=ui->lblQ4;
	hlabels[5]=ui->lblH5; vlabels[5]=ui->lblV5; qlabels[5]=ui->lblQ5;
	hlabels[6]=ui->lblH6; vlabels[6]=ui->lblV6; qlabels[6]=ui->lblQ6;
	hlabels[7]=ui->lblH7; vlabels[7]=ui->lblV7; qlabels[7]=ui->lblQ7;
	hlabels[8]=ui->lblH8; vlabels[8]=ui->lblV8; qlabels[8]=ui->lblQ8;
}

MainW2::~MainW2()
{
	delete ui;
}

void MainW2::highlight(int Anum)
{
	SHil=Anum;
	FModel.layoutChanged();

	int hfreq[9]={0};
	int vfreq[9]={0};
	int qfreq[9]={0};
	int mask=1<<Anum;
	for(int i=9;--i>=0;) {
		for(int j=9;--j>=0;) {
			hfreq[i]+=0<(FModel.Fsudoku.cans[i][j]&mask);
			vfreq[j]+=0<(FModel.Fsudoku.cans[i][j]&mask);
			qfreq[i/3*3+j/3]+=0<(FModel.Fsudoku.cans[i][j]&mask);
		}//for
	}//for
	for(int i=9;--i>=0;) {
		hlabels[i]->setProperty("hi",hfreq[i]);
		vlabels[i]->setProperty("hi",vfreq[i]);
		qlabels[i]->setProperty("hi",qfreq[i]);
		hlabels[i]->style()->polish(hlabels[i]);
		vlabels[i]->style()->polish(vlabels[i]);
		qlabels[i]->style()->polish(qlabels[i]);
	}//for
}//MainW2::highlight

void MainW2::on_tableView_clicked(const QModelIndex &AIndex)
{
	qDebug("click(r%d,c%d)",AIndex.row(),AIndex.column());
	int r=AIndex.row(),c=AIndex.column();
	if(ui->rbOrig->isChecked()) SBG[r][c]=1;
	if(ui->rbFix->isChecked()) SBG[r][c]=2;
	if(ui->rbWork->isChecked()) SBG[r][c]=3;
	if(ui->rbClear->isChecked()) SBG[r][c]=0;
}//MainW2::on_tableView_clicked

void MainW2::on_txtExImp_customContextMenuRequested(const QPoint &)
{
	QString s;
	for (int r=0;r<9;r++) {
		for (int c=0;c<9;c++) {
			int i=FModel.data(FModel.index(r,c),Qt::DisplayRole).toInt();
			s+=i?QChar('0'+i):SBG[r][c]?QChar('*'):QChar('.');
		}//for
		s+=QChar('\n');
	}//for
	s.chop(1);
	ui->txtExImp->setPlainText(s);
}//MainW2::on_txtExImp_customContextMenuRequested

void MainW2::on_txtExImp_selectionChanged()
{
	int r=0,c=0;
	QChar x;
	foreach(x,ui->txtExImp->toPlainText()) {
		switch(x.toLatin1()) {
		case '\n':++r;c=0;continue;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			if(r>8 or c>8) continue;
			FModel.setData(FModel.index(r,c),x.toLatin1()-'0',Qt::EditRole);
			++c;
			continue;
		case ' ':
		case '.':
		case '*':
		default:
			if(r>8 or c>8) continue;
			FModel.setData(FModel.index(r,c),0,Qt::EditRole);
			++c;
		}//switch
	}//foreach
	FModel.layoutChanged();
}//MainW2::on_txtExImp_selectionChanged

void MainW2::on_pbPush_clicked()
{
	SStack.push(ui->txtExImp->toPlainText());
	ui->lblStack->setText(QString::number(SStack.size()));
	if(!SStack.empty()) ui->lblStack->setToolTip(SStack.top());
}//MainW2::on_pbPush_clicked

void MainW2::on_pbPop_clicked()
{
	if(SStack.empty()) return;
	ui->txtExImp->setPlainText(SStack.pop());
	ui->lblStack->setText(QString::number(SStack.size()));
	if(!SStack.empty()) ui->lblStack->setToolTip(SStack.top());
}//MainW2::on_pbPop_clicked
