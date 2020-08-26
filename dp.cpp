#include "dp.h"
#include "ui_dp.h"
#include <QTimer>
//#include "C:\Qt\5.12.4\mingw73_64\include\QtWidgets\qfiledialog.h"
#include "QErrorMessage"
#include "QInputDialog"

dp::dp(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::dp)
{
    ui->setupUi(this);
    m_time=0;
    T=1000;
    L=500;
    dt=1;
    dx=0.1;
    deltaT=1;
    lfrom=0;
    lto=L;
    xmin=100;
    xmax=L;
    ui->SliderLFrom->setMinimum(0);
    ui->SliderLTo->setMinimum(1);
    ui->SliderT->setMinimum(1);
    ui->SliderL->setMinimum(1);
    ui->SliderT->setMaximum(10000);
    ui->SliderL->setMaximum(10000);
    ui->SliderLFrom->setMaximum(L-1);
    ui->SliderLTo->setMaximum(L);

    ui->SliderL->setValue(L);
    ui->SliderT->setValue(T);
    ui->SliderLFrom->setValue(-L);
    ui->SliderLTo->setValue(L);
    ui->deltaTSlider->setValue(deltaT);

    ui->labelL->setText(QString::number(L));
    ui->labelT->setText(QString::number(T));
    ui->labelfrom->setText(QString::number(lfrom));
    ui->labelto->setText(QString::number(lto));
    ui->labeldeltat->setText(QString::number(deltaT));
    QString s=QChar(0x3BE);
    QString s1=QChar(0x3B7);
    ui->pwidget1->funcname=QString(s+"(x,t)\n"+s1+"(x,t)\n"+"m");
    ui->pwidget2->funcname=QString("H(x),km");
    ui->textlog->setReadOnly(true);
    Error.setWindowIcon(QIcon());
    Error.setWindowTitle("Error");
    Error.hide();
    f = new maths();
    f->model_H=1;
    f->model_func=1;
    //th = new QThread();
    timer= new QTimer();
    is_stopped=true;
    //this->moveToThread(th);
    //s="x''(t)+"+QString::number(9.81/l)+"*x(t)=0";
    //ui->label->setText(s);
    QObject::connect(ui->SliderL, SIGNAL(valueChanged(int)), this, SLOT(setL()));
    QObject::connect(ui->SliderT, SIGNAL(valueChanged(int)), this, SLOT(setT()));
    QObject::connect(ui->SliderLFrom, SIGNAL(valueChanged(int)), this, SLOT(setlFrom()));
    QObject::connect(ui->SliderLTo, SIGNAL(valueChanged(int)), this, SLOT(setlTo()));
    QObject::connect(ui->deltaTSlider, SIGNAL(valueChanged(int)), this, SLOT(setdeltaT()));
    //QObject::connect(ui->tauEdit, SIGNAL(textChanged(QString)), this, SLOT(settau()));
    //QObject::connect(ui->hEdit, SIGNAL(textChanged(QString)), this, SLOT(seth()));
    QObject::connect(ui->pushButtondT, SIGNAL(clicked()), this, SLOT(settau()));

    QObject::connect(ui->pushButtondx, SIGNAL(clicked()), this, SLOT(seth()));

    QObject::connect(ui->toolButtonHx, SIGNAL(clicked()), this, SLOT(setHx()));
    QObject::connect(ui->toolButtonFx, SIGNAL(clicked()), this, SLOT(setfunc()));
    QObject::connect(ui->toolButtonSF, SIGNAL(clicked()), this, SLOT(setSaveFile()));
    QObject::connect(ui->saveImadgeButton, SIGNAL(clicked()), ui->pwidget1, SLOT(save()));
    QObject::connect(ui->SaveHxButton, SIGNAL(clicked()), ui->pwidget2, SLOT(save()));
    QObject::connect(ui->checkBox, SIGNAL(clicked()),this, SLOT(set_time()));
     QObject::connect(ui->checkBox_Fx,SIGNAL(clicked()),this,SLOT(setmodelfunc()));
     QObject::connect(ui->checkBox_Hx,SIGNAL(clicked()),this,SLOT(setmodelH()));


    QObject::connect(ui->startButton, SIGNAL(clicked()), this, SLOT(startcalc()));
        QObject::connect(this, SIGNAL(started()),this,SLOT(docalc()));
        QObject::connect(this, SIGNAL(stepdone()),this,SLOT(isPaused()));
        QObject::connect(timer, SIGNAL(timeout()),this,SLOT(isPaused()));
        QObject::connect(this, SIGNAL(dostep()),this,SLOT(docalc()));
        QObject::connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(stopcalc()));
        QObject::connect(ui->pauseButton, SIGNAL(clicked()), this, SLOT(pause()),Qt::DirectConnection);

        //QObject::connect(ui->calcButton, SIGNAL(clicked()), this, SLOT(calc()));
        //QObject::connect(thr1, SIGNAL(started()), f, SLOT(runge()),Qt::DirectConnection);
        //QObject::connect(f, SIGNAL(finished()), thr1, SLOT(quit()),Qt::DirectConnection);

}

double dp::H(double x)
{
    return 1000.0;
}
double dp::func(double x,double t)
{
    if((x-4.0*t)<0&&((x-4.0*t)>-100))
        return sin(2*3.14/100.0*(x-4.0*t));
                else return 0;
}
double dp::max(double x,double y,double z)
{
    double max=x;
    if (x<y)
        max=y;
    if (max<z)
        max=z;
    return max;
}
double dp::min(double x,double y,double z)
{
    double max=x;
    if (x>y)
        max=y;
    if (max>z)
        max=z;
    return max;
}
void dp::setT()
{
   T=ui->SliderT->value();
   ui->deltaTSlider->setMaximum(T);
   ui->labelT->setText(QString::number(ui->SliderT->value()));
}
void dp::setL()
{
    L=ui->SliderL->value();
    ui->labelL->setText(QString::number(ui->SliderL->value()));
    ui->SliderLFrom->setMaximum(L-1);
    ui->SliderLTo->setMaximum(L);
    lto=L;
}
void dp::setlFrom()
{
    lfrom=ui->SliderLFrom->value();
    ui->labelfrom->setText(QString::number(ui->SliderLFrom->value()));
    ui->SliderLTo->setMinimum(lfrom+1);
}
void dp::setlTo()
{
    lto=ui->SliderLTo->value();
    ui->labelto->setText(QString::number(ui->SliderLTo->value()));
}
void dp::settau()
{
    bool* ok= new bool;
    dt= QInputDialog::getDouble(this,"Шаг по времени","dt(c)=",1.0,1.0,1000,2,ok);

    //QString text;
    //bool* ok= new bool;
    //text=ui->tauEdit->text();
    //dt=text.toDouble(ok);
    if (dt>1.0)
        ui->deltaTSlider->setMinimum(dt/1+1);
    else
        ui->deltaTSlider->setMinimum(1);
}
void dp::seth()
{
    //QString text;
    bool* ok= new bool;

    dx= QInputDialog::getDouble(this,"Шаг по координате","dx (km) =",1.0,0.1,100,2,ok);
    //text=ui->hEdit->text();
    //dx=text.toDouble(ok);

}
int dp::setHx()
{
     //vector<point>::iterator it=f->depth_vector.end();

        QString filename= QFileDialog::getOpenFileName(this,tr("Open File"),"","");
        const string name=filename.toStdString();
        ifstream file(name);
        if(!file)
        {
            //ui->LabelHx->setText("error");
            return -1;
        }
        f->depth_vector.clear();
        ui->LabelHx->setText(tr("загрузка файла..."));
        this->repaint();
        while (file>>f->p.x>>f->p.u)
            f->depth_vector.push_back(f->p);

         ui->LabelHx->setText(filename);
         file.close();
         return 0;
}
int dp::set_time()
{
    if(ui->checkBox->isChecked()==true)
    time=500;
    else
        time=1;
}
int dp::setfunc()
{
    //double *m=new double [3];

        QString filename= QFileDialog::getOpenFileName(this,tr("Open File"),"","");
        const string name=filename.toStdString();
        ifstream file(name);
        if(!file)
        {
            ui->LabelFx->setText(tr("Выберете файл"));
            return -1;
        }
        ui->LabelFx->setText(tr("загрузка файла.."));
        this->repaint();
        f->func_vector.clear();

        while (file>>f->p.x>>f->p.t>>f->p.u)
        {
             f->func_vector.push_back(f->p);
        }
        //for(int i=0;i<10;i++)

                //ui->textlog->append(QString::number((f->func_vector.end()-1)->t)+"yes");
         ui->LabelFx->setText(filename);
         file.close();
         return 0;

}
void dp::setdeltaT()
{
    deltaT=ui->deltaTSlider->value();
    //ui->textlog->append("dooooo");
    ui->labeldeltat->setText(QString::number(ui->deltaTSlider->value()));

}
void dp::setSaveFile()
{
    QString filename= QFileDialog::getSaveFileName(this,tr("Save File"),"","*.txt");
    const string name=filename.toStdString();
    if(fout.is_open()==true)
        fout.close();
    fout.open(name,ios_base::out|ios_base::trunc);
    if(fout.is_open()==true)
        ui->LabelSaveFile->setText(filename);
}
void dp::setmodelfunc()
{


    if(ui->checkBox_Fx->isChecked()==true)
    {
    QString text;
    bool* ok= new bool;
    f->model_func=QInputDialog::getInt(this,"f(x-vt)","f(x-vt)=\n2- Asin(k(x-xc-vt))\n3-1/ch(2*ach(50)/A*(x-xc-vt))\n",2,2,3,1,ok);
   if(!ok)
   {
       f->model_func=2;
   }
   switch(f->model_func)
   {
   case 2:
   {
    f->ampl= QInputDialog::getDouble(this,"f(x-vt)","A(km)=",1,0,10000,2,ok);

     f->k= QInputDialog::getDouble(this,"f(x-vt)","k(1/km)=",0.26,0,500,4,ok);

    f->speed= QInputDialog::getDouble(this,"f(x-vt)","v(km/c)=",0.15,-1000,1000,4,ok);

    f->xcenter= QInputDialog::getDouble(this,"f(x-vt)","xcenter(km)=",300,-10000,10000,2,ok);

    f->tstop= QInputDialog::getDouble(this,"f(x-vt)","stop time(c)=",10000,0,12001,3,ok);
    break;
   }
      case 3:
   {
       f->ampl= QInputDialog::getDouble(this,"f(x-vt)","A(km)=",1,0,10000,2,ok);

        //f->k= QInputDialog::getDouble(this,"f(x-vt)","k(1/km)=",0.26,0,500,4,ok);

       f->speed= QInputDialog::getDouble(this,"f(x-vt)","v(km/c)=",0.15,-1000,1000,4,ok);

       f->xcenter= QInputDialog::getDouble(this,"f(x-vt)","xcenter(km)=",300,-10000,10000,2,ok);

       f->tstop= QInputDialog::getDouble(this,"f(x-vt)","stop time(c)=",10000,0,12000,3,ok);
       break;
   }
   default:
   {
        f->model_func=1;
        break;
   }
   }
    }
    else
        f->model_func=1;
}

void dp::setmodelH()
{
    if(ui->checkBox_Hx->isChecked()==true)
    {
        bool* ok= new bool;
         f->model_H=QInputDialog::getInt(this,"H(x)","2-const\n3-ступенька\n4-уступ\n5-наклонное дно\n6-cube1\n7-cube2\n 8-безотражательный профиль\n",2,2,8,1,ok);
        if(!ok)
        {
            f->model_H=2;
        }
        switch(f->model_H)
        {
        case 2:
        {
           f->H1=QInputDialog::getDouble(this,"H(x)=const","H1=",1,0.001,12,3,ok);
           break;
        }
        case 3:

           {
               f->H1=QInputDialog::getDouble(this,"stupenka","H1[km]=",1,0.001,12,3,ok);
               f->x1=QInputDialog::getDouble(this,"stupenka","x1[km]=",400,-10000,1000,2,ok);
               f->H2=QInputDialog::getDouble(this,"stupenka","H2[km]=",0.5,0.001,12,3,ok);
               f->x2=QInputDialog::getDouble(this,"stupenka","x2[km]=",500,-10000,10000,2,ok);
               break;
           }
       case 4:
           {
               f->H1=QInputDialog::getDouble(this,"ustup","H1[km]=",1,0.001,12,3,ok);
                f->H2=QInputDialog::getDouble(this,"ustup","H2[km]=",0.5,0.001,12,3,ok);
                 f->x1=QInputDialog::getDouble(this,"ustup","x1[km]=",400,-10000,10000,2,ok);
                 break;
           }
case 5:
        {
            f->H1=QInputDialog::getDouble(this,"naklon","H1[km]=",5,0.001,12,3,ok);
            f->x1=QInputDialog::getDouble(this,"naklon","x1[km]=",400,-10000,10000,2,ok);
            f->H2=QInputDialog::getDouble(this,"naklon","H2[km]=",1,0.001,12,3,ok);
            f->alph=QInputDialog::getDouble(this,"naklon","alpha[degrees]=",45,-90,90,2,ok);
            break;
        }
       case 6:
        {
            f->H1=QInputDialog::getDouble(this,"cube1","H1[km]=",5,0.001,12,3,ok);
            f->x1=QInputDialog::getDouble(this,"cube1","x1[km]=",400,-10000,10000,2,ok);
            f->H2=QInputDialog::getDouble(this,"cube1","H2[km]=",1,0.001,12,3,ok);
            f->x2=QInputDialog::getDouble(this,"cube1","x2[km]=",480,-10000,10000,2,ok);
            //do nothing
            break;
        }
        case 7:

         {
            f->H1=QInputDialog::getDouble(this,"cube2","H1[km]=",5,0.001,12,3,ok);
            f->x1=QInputDialog::getDouble(this,"cube2","x1[km]=",400,-10000,10000,2,ok);
            f->H2=QInputDialog::getDouble(this,"cube2","H2[km]=",1,0.001,12,3,ok);
            f->x2=QInputDialog::getDouble(this,"cube2","x2[km]=",480,-10000,10000,3,ok);

             //do nothing
             break;
         }
        case 8:
        {
            f->H1=QInputDialog::getDouble(this,"unreflecting","H1[km]=",5,0.001,12,3,ok);
            f->x1=QInputDialog::getDouble(this,"unreflecting","x1[km]=",400,-10000,10000,2,ok);
            f->H2=QInputDialog::getDouble(this,"unreflecting","H2[km]=",1,0.001,12,3,ok);
            f->x2=QInputDialog::getDouble(this,"unreflecting","x2[km]=",480,-10000,10000,2,ok);
            //do nothing
            break;
        }
        default:
        {
            f->model_H=2;
            f->H1=1;
            break;
        }
        }
    }
    else
        f->model_H=1;
}
/*void pmy::calc()
{
    t=0.0;
    ui->pwidget1->polyPoints1.clear();
    ui->pwidget1->polyPoints2.clear();
    *U=vector(3.14,0.0);
    ui->pwidget1->minx=t;
    ui->textlog->append("Begin with parametres:");
    ui->textlog->append(QString::number(t) + '\t' + QString::number(U->x) + "\t" + QString::number(U->vx) + "\n");
    ui->pwidget1->polyPoints1+=QPointF(t,U->x);
    ui->pwidget1->polyPoints2+=QPointF(t,U->vx);
    ui->pwidget2->polyPoints1+=QPointF(U->x,U->vx);
    ui->pwidget1->miny=fmin(U->x,U->vx);
    ui->pwidget1->maxy=fmax(U->x,U->vx);
        {
    ui->pwidget2->miny=U->vx;
    ui->pwidget2->maxy=U->vx;
    ui->pwidget2->minx=U->x;
    ui->pwidget2->maxx=U->x;
   // создание элемента класса подсчета рунге-кутт
   //thr1->sleep(10);//в отдельный поток
    ui->pwidget1->maxx=100.0;
    ui->pwidget1->minx=t;
    int flag=0;
    //f->ex();
    *(f->t)=t;
    *(f->U)=*U;
    //thr1->start();
    while (t<=ui->pwidget1->maxx)
    {
        thr1->start();
        //thr1->sleep(1);
        //f->runge(&t, U);
        t+=dt;
        //*(f->t)=t;
        *U=*(f->U);
        ui->textlog->append(QString::number(t) + '\t' + QString::number(U->x) + "\t" + QString::number(U->vx) + "\t");
        ui->pwidget1->polyPoints1+=QPointF(t,U->x);
        ui->pwidget1->polyPoints2+=QPointF(t,U->vx);
        ui->pwidget2->polyPoints1+=QPointF(U->x,U->vx);
        if (ui->pwidget1->miny>fmin(U->x,U->vx))
                ui->pwidget1->miny=fmin(U->x,U->vx);
        if (ui->pwidget1->maxy<fmax(U->x,U->vx))
                ui->pwidget1->maxy=fmax(U->x,U->vx);
        if (ui->pwidget2->miny>U->vx)
                ui->pwidget2->miny=U->vx;
        if (ui->pwidget2->maxy<U->vx)
                ui->pwidget2->maxy=U->vx;
        if (ui->pwidget2->minx>U->x)
                ui->pwidget2->minx=U->x;
        if (ui->pwidget2->maxx<U->x)
                ui->pwidget2->maxx=U->x;
        //thr1->terminate();

    }
    ui->textlog->append("end of quick calculation");
    ui->textlog->append(QString::number(ui->pwidget1->miny) + '\t' + QString::number(ui->pwidget1->maxy)+ "\t");
    t=0.0;
    ui->pwidget1->repaint();
     ui->pwidget2->repaint();
}*/
    int dp::startcalc()

{
        if (is_stopped==true)
        {
        if(f->setAll(T,L,dt,dx)==-1)
            {
            Error.showMessage(tr("Не заданы все начальные данные из файлов"));
            //Error.showMessage("Не заданы все начальные данные из файлов");
            return -1;
        }
        tau=f->tau/f->tconst;
        ui->textlog->append(tr("Рассчет для параметров:\nВремя ")+QString::number(T) + tr("\nДлина ") + QString::number(L) + tr("\nШаг по времени ") + QString::number(f->tau) +tr("\nШаг по координате ") + QString::number(f->h)+ "\n");


        switch(f->model_func)
        {
        case 1:
        {
            ui->textlog->append(tr("f(x,t)-из файла")+ui->LabelFx->text());
            break;
        }
        case 2:
        {
            ui->textlog->append("f(x-vt)=Asin(k(x-xc-vt)+pi/2, pi/2<x-xc-vt<pi/2\nA="+QString::number(f->ampl)+"km\nk="+QString::number(f->k)+"1/km\nxc="+QString::number(f->xcenter)+"km\nv="+QString::number(f->speed)+"km/c\n");
            break;
        }
        case 3:
        {
            ui->textlog->append("f(x-vt)=1/ch(2*ach(50)/A*(x-xc-vt))\nA="+QString::number(f->ampl)+"km\nxc="+QString::number(f->xcenter)+"km\nv="+QString::number(f->speed)+"km/c\n");
            break;
        }
        default:
            break;
        }
            switch(f->model_H)
        {
            case 1:
            {
                ui->textlog->append(tr("H(x)-из файла")+ui->LabelHx->text());
                break;
            }
            case 2:
            {
                ui->textlog->append(tr("H(x)=const:\nH= ")+QString::number(f->H1)+" km\t");
                break;
            }
            case 3:
            {
                ui->textlog->append(tr("H(x)-cтупенька с параметрами:\nH1= ")+QString::number(f->H1)+" km\tx1="+QString::number(f->x1)+" km\tH2= "+QString::number(f->H2)+"km\tx2= "+QString::number(f->x2)+"km\n");
            break;
            }
            case 4:
            {
                ui->textlog->append(tr("H(x)-уступ с параметрами:\nH1= ")+QString::number(f->H1)+" km\tx1="+QString::number(f->x1)+" km\tH2= "+QString::number(f->H2)+"km\n");
                break;

            }
            case 5:
            {
                ui->textlog->append(tr("H(x)- наклонное дно:\nH1= ")+QString::number(f->H1)+" km\tx1="+QString::number(f->x1)+" km\tH2= "+QString::number(f->H2)+"km\alpha="+QString::number(f->alph)+"degrees\n");
                    break;
            }
            case 6:
            {
                 ui->textlog->append(tr("H(x)=(1-4/(x1-x2)^3*(x-x2)^3)\t 1<H(x)<5)\n"));
                         break;
            }
            case 7:
            {
                ui->textlog->append(tr("H(x)=(5.0-1.0/(x1-x2)^3*(x-x1)^3)\t,1<H(x)<5\n"));
                        break;

            }
            case 8:
            {
                ui->textlog->append(tr("H(x)=A(x0-x)^(4/3)"));
                break;
            }
            default:
                break;
            }

            this->repaint();
        x=0.0;
        t=0.0;
        m_time=0;
        NT=1;
        //ui->textlog->append(QString::number(dx) + '\t' + QString::number(f->tau) + "\t" + QString::number(f->h) + "\n");
        if(fout.is_open()==false)
            fout.open("results.txt",ios_base::out|ios_base::trunc);
        int i;
        //fout<<"{";
            for ( i = 0; i < f->Nx; i++)
            {
               f->uDown[i]=0;//initial conditions

               f->uMiddle[i]=0;
               fout<<"{"<<f->h*i<<","<<t<<","<<f->uDown[i]<<"},";
            }
            //f->initialConditions();
            t=tau;
            m_time=1;
            for ( i = 0; i < f->Nx; i++)
                fout<<"{"<<f->h*i<<","<<t<<","<<f->uMiddle[i]<<"},";
        ui->pwidget1->polyPoints1.clear();
        ui->pwidget1->polyPoints2.clear();
        ui->pwidget2->polyPoints1.clear();
        //*U=vector(3.14,0.0);
        ui->pwidget1->minx=lfrom;
        ui->pwidget1->maxx=lto;
        ui->pwidget2->minx=lfrom;
        ui->pwidget2->maxx=lto;
        i=0;
        t=0.0;
        while(x<lfrom)
        {
            x+=f->h;
            i++;
        }
        ui->pwidget1->miny=f->func(0,t);
        ui->pwidget1->maxy=f->func(0,t);
        ui->pwidget2->miny=-(f->H(0));
        ui->pwidget2->maxy=-(f->H(0));
        xmin=x;
        //ui->textlog->append(QString::number(t) + '\t' + QString::number(f->tau) + "\t" + QString::number(f->h) + "\n");
        while(x<lto)
        {

            ui->pwidget1->polyPoints2+=QPointF(x,f->uDown[i]);
        ui->pwidget1->polyPoints1+=QPointF(x,f->func(x,t));
        ui->pwidget2->polyPoints1+=QPointF(x,-(f->H(x)));
        ui->pwidget1->miny=min(f->uDown[i],f->func(x,t),ui->pwidget1->miny);
        ui->pwidget1->maxy=max(f->uDown[i],f->func(x,t),ui->pwidget1->maxy);
        ui->pwidget2->miny=fmin(-(f->H(x)),ui->pwidget2->miny);
        ui->pwidget2->maxy=fmax(-(f->H(x)),ui->pwidget2->maxy);
        i++;
        x+=f->h;
        }
        xmax=x-f->h;
        if(ui->pwidget1->miny==ui->pwidget1->maxy)
            ui->pwidget1->maxy=ui->pwidget1->maxy+f->h;

            ui->pwidget2->miny-=0.5;
            ui->pwidget2->maxy+=0.5;
            t+=2.0*(tau);
            m_time=2;

        //f = new func(l,dt);
        //f->moveToThread(thr1);
        //thr1->start();// создание элемента класса подсчета рунге-кутты

        is_paused=false;
        is_stopped=false;
        ui->pwidget1->repaint();
         ui->pwidget2->repaint();
        emit started();

    }
    }

    void dp::docalc()
{

        //thr1->start();
        int i,m;
        while((t<=deltaT*NT)&&(t<=T))
        {//if(t/f->tau<4)
            f->layerSolve(m_time);

            for(i = 0; i <=f->Nx-1; i++)
            {
            //fout<<"{"<<f->h*i<<","<<t<<","<<f->uUp[i]<<"},";

                    f->uDown[i] = f->uMiddle[i];
                    f->uMiddle[i]=f->uUp[i];
            }

            t+=tau;
            m_time++;
            }
        t-=tau;
        x=xmin;
        ui->pwidget1->polyPoints1.clear();
        ui->pwidget1->polyPoints2.clear();
        for(i = 0; i <=f->Nx-1; i++)
        {
        fout<<f->h*i<<"\t"<<t<<"\t"<<f->uUp[i]<<"\n";
        }
        for(i=xmin/f->h;i<=xmax/f->h;i++)
        {
            //if(f->uMiddle[i]>tau*tau*0.01)
            ui->pwidget1->polyPoints2+=QPointF(x,f->uMiddle[i]);
        ui->pwidget1->miny=min(f->uMiddle[i],f->func(x,t),ui->pwidget1->miny);
        ui->pwidget1->maxy=max(f->uMiddle[i],f->func(x,t),ui->pwidget1->maxy);
            ui->pwidget1->polyPoints1+=QPointF(x,f->func(x,t));
        x+=f->h;
        }

        NT++;
        if (t>=T)
        {
            is_stopped=true;
            fout.close();
            ui->textlog->append(tr("Рассчеты завершены.\n"));
        }
        //ui->textlog->append(QString::number(t) + '\t' + QString::number(U->x) + "\t" + QString::number(U->vx) + "\t");
        ui->label_t->setText(QString::number(t));
        this->repaint();
        ui->pwidget1->repaint();
        t+=tau;
        //if (is_paused==false)
            timer->start(time);
        //emit stepdone();

        //is_paused=true;

    }

    void dp::isPaused()

    {
        if (is_stopped==false)
        {
        timer->stop();
        if (is_paused==false)
        {
            emit dostep();

        }
        }

    }
    void dp::pause()
    {
        if (is_stopped==false)
        {
        if (is_paused==true)
          {
            is_paused=false;
        emit dostep();
        }
        else
            is_paused=true;
        }
    }

    void dp::stopcalc()
    {
        if((is_stopped)==false)
        {
        is_paused=true;
        is_stopped=true;
        if(fout.is_open()==true)
           fout.close();
            t=0;
            m_time=0;
        ui->textlog->append(tr("Рассчеты завершены.\n"));
        }
        //*U=vector(3.14,0);

    }
dp::~dp()
{
    delete ui;
}


