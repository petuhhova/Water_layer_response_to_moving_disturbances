#ifndef DP_H
#define DP_H

#include <QWidget>
#include <QFileSystemModel>
#include <QDir>
#include <QFileDialog>
#include "qpainterwidget.h"
#include "maths.h"
#include "QErrorMessage"

QT_BEGIN_NAMESPACE
namespace Ui { class dp; }
QT_END_NAMESPACE

class dp : public QWidget
{
    Q_OBJECT
public:

    maths *f;// элемент класса подсчета
    int T,L,deltaT,NT,time,m_time;
    double dt,dx,lfrom,lto,xmin,xmax,x,xfrom,xto,t,tau;
    QVector<QPointF> *polypoints;
    double *U;
    double (*funcp) (double x,double t);
    double (*Hxp) (double x);
    QTimer* timer;
    QString s;
    QErrorMessage Error;
    bool is_paused;
    bool is_stopped;
    fstream fout;
    string filename;

private:
signals:
   void started();
   void stepdone();
   void paused();
   void stopped();
   void dostep();
   void error();

public slots:
  double H(double x);
    double func(double x,double t);
    double max(double x,double y,double z);
    double min(double x,double y,double z);
    //void calc();
    int startcalc();
    void stopcalc();
    void docalc();
    void pause();
    void isPaused();
    //void setParaml();
    void setlFrom(); void setlTo();
    void setT();
    void setL();
    void settau();
    void seth();
    int setHx();
    int setfunc();
    void setdeltaT();
    void setSaveFile();
    int set_time();
    void setmodelfunc();
    void setmodelH();

public:
    dp(QWidget *parent = nullptr);
    ~dp();

private:
    Ui::dp *ui;
};
#endif // DP_H
