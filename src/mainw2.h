/* mainw2.h
2017jul03:(VK) Created
*/
#ifndef MAINW2_H
#define MAINW2_H

#include <QMainWindow>
#include <QAbstractItemModel>
#include <QAbstractItemDelegate>
#include "sudoku.h"

/*
TYPES
*****************************************************************************/
class QLabel;

/*
VARS
*****************************************************************************/
extern QLabel *hlabelsz[9];
extern QLabel *vlabelsz[9];
extern QLabel *qlabelsz[9];
extern QLabel *flabelsz[9];
extern char SHil;

/*
CLASSES
*****************************************************************************/
//Tstmodel -------------------------------------------------------------------
class Tstmodel:public QAbstractItemModel {
public:
	Tsudoku Fsudoku;
	Tstmodel() {}
	Q_INVOKABLE virtual QModelIndex index(int row,int column,const QModelIndex &parent=QModelIndex()) const Q_DECL_OVERRIDE;
	Q_INVOKABLE virtual QModelIndex parent(const QModelIndex &child) const Q_DECL_OVERRIDE;
	Q_INVOKABLE virtual int rowCount(const QModelIndex& =QModelIndex()) const override {return 9;}
	Q_INVOKABLE virtual int columnCount(const QModelIndex& =QModelIndex()) const override {return 9;}
	Q_INVOKABLE virtual QVariant data(const QModelIndex &index,int role=Qt::DisplayRole) const Q_DECL_OVERRIDE;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
	virtual bool setData(const QModelIndex&index,const QVariant&value,int role) override;
};//Tstmodel

//TCellDelegate --------------------------------------------------------------
class TCellDelegate:public QAbstractItemDelegate {
	Q_OBJECT
public:
	TCellDelegate(QObject *parent=0):QAbstractItemDelegate(parent) {}
	void paint(QPainter *painter,const QStyleOptionViewItem &option,const QModelIndex &index) const override;
	QSize sizeHint(const QStyleOptionViewItem &/*option*/,const QModelIndex &/*index*/) const override {return QSize(32,32);}
	QWidget *createEditor(QWidget *parent,const QStyleOptionViewItem &option,const QModelIndex &index) const override;
	void setEditorData(QWidget *editor, const QModelIndex &index) const override;
	void setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const override;

public slots:
	//void setPixelSize(int size);
private:
	//int pixelSize;
};//TCellDelegate


//MainW2 ---------------------------------------------------------------------
namespace Ui {
class MainW2;
}

class MainW2 : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainW2(QWidget *parent = 0);
	~MainW2();
	void highlight(int Anum);

private slots:
	void on_tableView_clicked(const QModelIndex &AIndex);

	void on_txtExImp_customContextMenuRequested(const QPoint &pos);

	void on_txtExImp_selectionChanged();

	void on_pbPush_clicked();

	void on_pbPop_clicked();

	void on_tb1_clicked() {highlight(1);}
	void on_tb2_clicked() {highlight(2);}
	void on_tb3_clicked() {highlight(3);}
	void on_tb4_clicked() {highlight(4);}
	void on_tb5_clicked() {highlight(5);}
	void on_tb6_clicked() {highlight(6);}
	void on_tb7_clicked() {highlight(7);}
	void on_tb8_clicked() {highlight(8);}
	void on_tb9_clicked() {highlight(9);}

private:
	Ui::MainW2 *ui;
	Tstmodel FModel;
	TCellDelegate FDelegate;
};//MainW2

#endif // MAINW2_H
