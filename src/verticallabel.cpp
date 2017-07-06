/* verticallabel.cpp
2017jul04:(VK) Created from https://stackoverflow.com/questions/9183050/vertical-qlabel-or-the-equivalent
*/
#include "verticallabel.h"

#include <QPainter>

VerticalLabel::VerticalLabel(QWidget *parent):QLabel(parent) {}
VerticalLabel::VerticalLabel(const QString &text,QWidget *parent):QLabel(text, parent) {}

void VerticalLabel::paintEvent(QPaintEvent*) {
	QPainter painter(this);
	painter.setPen(Qt::black);
	painter.setBrush(Qt::Dense1Pattern);

	//painter.rotate(90);
	//painter.drawText(0,0,text());
	painter.translate(0,sizeHint().height());
	painter.rotate(270);
	painter.drawText(QRect(QPoint(0,0),QLabel::sizeHint()),Qt::AlignCenter,text());
}//VerticalLabel::paintEvent

QSize VerticalLabel::minimumSizeHint() const
{
	QSize s = QLabel::minimumSizeHint();
	return QSize(s.height(), s.width());
}//VerticalLabel::minimumSizeHint

QSize VerticalLabel::sizeHint() const
{
	QSize s = QLabel::sizeHint();
	return QSize(s.height(), s.width());
}//VerticalLabel::sizeHint
