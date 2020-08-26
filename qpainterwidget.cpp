#include "qpainterwidget.h"

#include <QPainter>
#include <QMouseEvent>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>
#include <QFileDialog>
//#include "C:\Qt\5.12.4\mingw73_64\include\QtWidgets\qfiledialog.h"

QPainterWidget::QPainterWidget(QWidget * parent) : QWidget(parent)
{
        // устанавливаем цвет фона
    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::white);
     setAutoFillBackground(true);
    setPalette(Pal);

}
/*void QPainterWidget::mousePressEvent(QMouseEvent* event)
   {
    pos =event->pos();
       repaint();
       repaint();
       draw=QPixmap(size());
       render(&draw);
       image=draw.toImage();

}*/
double  QPainterWidget::razbienie(double min,double max,double n,int* i)
{
    double c;
    c=((max-min)/n);
    if (c<0)
        c=-c;
    int a;
    int j=0;
        while(c>=1.0)
        {
            c=c/10.0;
            j++;
        }

        while (c<1.0)
        {
            c=c*10.0;
            j--;
        }

    a=c;
    c=a;
    *i=j;
    c=c*pow(10,j);
    return c;
}

void QPainterWidget::paintEvent( QPaintEvent *) {
    QPainter p(this);// Создаём новый объект рисовальщика
    //p.drawImage(0,0,image); //отрисовываем предыдущую картинку

    QString text;
    p.setViewport(0,0,this->width(),this->height());//физические координаты
    //p.setWindow(0,0,this->width(),this->height());
    QFont newFont;
    newFont.setPointSizeF(8);
    //set font of application
    p.setFont(newFont);
    p.scale(1.0,-1.0);
    //p.setWindow(minx,-maxy,-minx+maxx,-miny+maxy);
    p.translate(QPointF(80.0,0));
    xscale=(this->width()-80.0)/(-minx+maxx);
    yscale=(this->height()-50.0)/(-miny+maxy);
    p.scale(xscale,yscale);
    p.translate(QPointF(-minx,-maxy));
    //левый верхний угол (minx,-maxy)
    QPen pen(Qt::black,1,Qt::DotLine);
    pen.setCosmetic(true);//чтоб не масштабировалась кисть
    p.setPen(pen);
    //создание сетки
    double x,y,n=9.0;
    int i,j;
    int c;
    stepx=razbienie(minx,maxx,12.0,&i);
    stepy=razbienie(miny,maxy,8.0,&j);
    //stepx=10.0;
    //stepy=10.0;
    //p.scale(1/xscale,1/yscale);
    //p.drawText(QPointF(minx*xscale,maxy*yscale),QString::number(minx));
    //p.scale(xscale,yscale);
    //p.setPen(pen);

    if ((minx<0.0)&&(maxx>0.0))
    {
        x=0.0;
        while (x>minx)
        {
            p.drawLine(QLineF(x,miny,x,maxy));
            text=QString::number(x);
          p.scale(1/xscale,-1/yscale);
            p.drawText(QPointF(x*xscale,(-miny)*yscale+20.0),text);
            p.scale(xscale,-yscale);
            x=x-stepx;
        }
        x=0.0+stepx;
        while (x<maxx)
        {
            p.drawLine(QLineF(x,miny,x,maxy));
            text=QString::number(x);
            p.scale(1/xscale,-1/yscale);
            p.drawText(QPointF(x*xscale,(-miny)*yscale+20.0),text);
            p.scale(xscale,-yscale);
            x=x+stepx;
        }
    }
        else
    {
       x=minx/pow(10.0,i);
       c=x;
       x=c;
       x=x*pow(10.0,i);
       while (x<maxx)
       {
           {
               p.drawLine(QLineF(x,miny,x,maxy));
               text=QString::number(x);
               p.scale(1/xscale,-1/yscale);
               p.drawText(QPointF(x*xscale,(-miny)*yscale+20.0),text);
               p.scale(xscale,-yscale);
               x=x+stepx;
           }
       }


    }
    if ((miny<0.0)&&(maxy>0.0))
    {
        y=0.0;
        while (y>miny)
        {
            p.drawLine(QLineF(minx,y,maxx,y));
            text=QString::number(y);
            p.scale(1/xscale,-1/yscale);
            p.drawText(QPointF((minx)*xscale-40.0,-y*yscale),text);
           p.scale(xscale,-yscale);
            y=y-stepy;
        }
        y=0.0+stepy;
        while (y<maxy)
        {
            p.drawLine(QLineF(minx,y,maxx,y));
            text=QString::number(y);
            p.scale(1/xscale,-1/yscale);
            p.drawText(QPointF((minx)*xscale-40.0,-y*yscale),text);
            p.scale(xscale,-yscale);
            y=y+stepy;
        }
    }
        else
    {
       y=miny/pow(10.0,j);
       c=y;
       y=c;
       y=y*pow(10.0,j);
       while (y<maxy)
       {
           {
               p.drawLine(QLineF(minx,y,maxx,y));
               text=QString::number(y);
               p.scale(1/xscale,-1/yscale);
               p.drawText(QPointF((minx)*xscale-40.0,-y*yscale),text);
               p.scale(xscale,-yscale);
               y=y+stepy;
           }
       }


    }

    pen.setColor(Qt::black);
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(3);
    p.setPen(pen);
    p.drawLine(QLineF(minx,0.0,maxx,0.0));
    p.drawLine(QLineF(0.0,miny,0.0,maxy));
    p.drawLine(QLineF(minx,miny,maxx,miny));
    p.drawLine(QLineF(minx,miny,minx,maxy));
    p.scale(1/xscale,-1/yscale);
    newFont.setPointSizeF(8);
    p.setFont(newFont);
    p.drawText(QPointF((minx)*xscale-80.0,((-maxy-miny)/2.0)*yscale-18),funcname);
    text="x, km";
    p.drawText(QPointF((maxx+minx)/2.0*xscale,(-miny)*yscale+40.0),text);
    p.scale(xscale,-yscale);
    //ура, нарисовалисетку, подписи и тп
    pen.setColor(Qt::red);
    pen.setWidth(2);
    p.setPen(pen);
    p.drawPolyline(polyPoints1);
    pen.setColor(Qt::green);
    pen.setWidth(2);
    p.setPen(pen);
    p.drawPolyline(polyPoints2);
    // Рисование линии


}
void QPainterWidget::save()
{
    const QString filename= QFileDialog::getSaveFileName(this,tr("Save File"),"","*.jpg,*.bmp");
    //const string name=filename.toStdString();
    draw=QPixmap(size());
    render(&draw);
    image=draw.toImage();

    image.save(filename);
}

/*void QPainterWidget::graph()
{
    std::string str;
    std::fstream fin;
    double n;
    int f=0;
    int t=0;
    fin.open("D:/table.txt", std::ios_base::in);
    polyPoints.clear();
    while(std::getline(fin,str))
    {
        t=0;
        std::istringstream iss(str,std::istringstream::in);
        double m[2];
        while (iss >> n)
           { m[t]=n;
            t++;
        }
        if (f==0)
        {
          minx=m[0];
          maxx=m[0];
          miny=m[1];
          maxy=m[1];
         f=1;
        }
        if (minx>m[0])
                minx=m[0];
        if (maxx<m[0])
                maxx=m[0];
        if (miny>m[1])
                miny=m[1];
        if (maxy<m[1])
                maxy=m[1];

       polyPoints+=QPointF(m[0],m[1]);
    }
    repaint();

}*/
