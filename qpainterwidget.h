
#ifndef QPAINTERWIDGET_H
#define QPAINTERWIDGET_H

#include <QWidget>
#include <QVector>
#include <cmath>

class QPaintEvent;

class QPainterWidget : public QWidget
{
    Q_OBJECT
public:
    QPainterWidget(QWidget * parent = 0);
    QPixmap draw;
    QImage image;
    //int i;
    //int j;
    double minx=-100.0,miny=-100.0,maxx=100.0,maxy=100.0;
    double xscale,yscale;
    double stepx, stepy;
    QVector<QPointF> polyPoints1;
    QVector<QPointF> polyPoints2;
    QVector<QPointF> polyPoints3;
    //void mousePressEvent(QMouseEvent *event);
    double  razbienie(double min,double max,double n, int* i);

    QString funcname;
private:
    std::string name;
public slots:
    void save();
    //void graph();
protected:
    void paintEvent(QPaintEvent*);
};

#endif // QPAINTWIDGET_H
