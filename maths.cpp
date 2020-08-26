#include "maths.h"
#include <fstream>

maths::maths(double Tt,double Ll, double dt, double dx)
{
    T=Tt*tconst;
    L=Ll;
    Nx=L/dx+1,Nt=Tt/dt+1;
    h = L / (Nx-1),  tau = T / (Nt-1);
     gamma1 = sigma / (h * h);
    A=new double[Nx];
    B=new double[Nx];
     C=new double[Nx];
     F=new double[Nx];
     U=new double[Nx];
     alpha=new double[Nx];
     betta=new double[Nx];
     uDown=new double[Nx];
     uMiddle=new double[Nx];
     uUp=new double[Nx];
     Hm=new double[Nx];
     funcm =new double[Nx*Nt];
     Accuracy=(tau*tau+h*h);
     //setH();
     //set_func();
}
maths::maths()
{

}

double maths::interpolate(double x , double t)
{
    double r;
vector<point>::iterator it1=(func_vector.end()-1);
vector<point>::iterator it2=func_vector.begin();
vector<point>::iterator it3=func_vector.begin();
vector<point>::iterator it4=func_vector.begin();
double h1,h2,t1,t2,tp,xp;
 if((it1->t<t)||((it4->t)>t))
 {

     return 0;
 }
 if((it1->x<x)||(it4->x)>x)
 {

     return 0;
 }
 if(it1==it4)
 {

     return it1->u;
 }
while((it4->t<t)||(it4->x<x))
{
it4++;
}
if(it4->t==func_vector.begin()->t)//bottom boundary
{
    if(it4->x==func_vector.begin()->x)
        return it4->u;
    it3=it4-1;
     r=(it3->u*(it4->x-x)+it4->u*(-it3->x+x))/(it4->x-it3->x);
     return r;
}
it3=it4-1;
tp= it3->t;
xp=it3->x;
if((xp>=it4->x)||(tp!=it4->t))
    return 0;
if(it4->x==func_vector.begin()->x)//left boundary
{
    while((it3->x>it4->x)&&(tp==it3->t))
        {
          it3--;
        }
    if(tp==it3->t)
        r=(it3->u*(it4->t-t)+it4->u*(-it3->t+t))/(it4->t-it3->t);
    else
         return 0;
    return r;
}
it1=it3;
while(it1->t==tp)
    it1--;
tp=it1->t;
while ((it1->x>=x)&&(tp==it1->t))
    it1--;
if((tp>t)||(tp!=it1->t))
    return 0;
it2=it1+1;
h1=it2->x-it1->x;
h2=it4->x-it3->x;
t1=it3->t-it1->t;
t2=it4->t-it2->t;
if(it2->t>it1->t)
{if(it1->x<it3->x)
    {
        t2=t1;
        h2=h1;
        r=it3->u*(it4->x-x)*(-it1->t+t)/h2/t1+it4->u*(-it3->x+x)*(-it1->t+t)/h2/t2;
        return r;
    }
    else
    {
        t2=t1;
        h2=h1;
        r=it1->u*(it4->x-x)*(it3->t-t)/h1/t1+it3->u*(it4->x-x)*(-it1->t+t)/h2/t1+it4->u*(-it3->x+x)*(-it1->t+t)/h2/t2;
        return r;
    }}
     r=it1->u*(it2->x-x)*(it3->t-t)/h1/t1+it2->u*(-it1->x+x)*(it4->t-t)/h1/t2+
        it3->u*(it4->x-x)*(-it1->t+t)/h2/t1+it4->u*(-it3->x+x)*(-it2->t+t)/h2/t2;

return r;
}
int maths::setH()
{
    vector<point>::iterator it=depth_vector.begin();
    //fstream fout("setH.txt",ios_base::out|ios_base::trunc);

    int i=0;
    if(depth_vector.empty()==true)
    {
        //fout.close();
        return -1;
    }

    while(i*h<it->x)
    {   Hm[i]=1;
        //fout<<i*h<<"\t"<<Hm[i]<<endl;
        i++;
    }
    if(i*h==it->x)
    {
        Hm[i]=it->u;
        //fout<<i*h<<"\t"<<Hm[i]<<endl;
       i++;

    }
    while((i<Nx)&&(it<depth_vector.end()))
    {


        while((it->x<i*h))
        {    it++;
        if (it==depth_vector.end())
        {
            break;
            break;

        }
        }
        Hm[i]=(((it-1)->u)*(it->x-i*h)+it->u*(i*h-(it-1)->x))/(it->x-(it-1)->x);
        //fout<<i*h<<"\t"<<Hm[i]<<endl;

            i++;
    }
          while(i<Nx)
        {   i++;
          Hm[i]=1;
          //fout<<i*h<<"\t"<<Hm[i]<<endl;
  }
          //fout.close();
          return 0;
}
int maths::set_func()
{
    switch(model_func)
       {
    case 1:
    {
    int i=0, j=0;
    double x,t;
    fstream fout("setfunc.txt",ios_base::out|ios_base::trunc);
    if(func_vector.empty()==true)
    {   /*while (j<Nt)
        {
        while(i<Nx)
       {
            funcm[Nx*j+i]=0;
            fout<<i*h<<"\t"<<j*tau<<"\t"<<funcm[Nx*j+i]<<endl;
            i++;
            }
        j++;
        i=0;
     }*/
        fout.close();
        return -1;
    }
double f=-100.0;
    while (j<Nt)
    {
    while(i<Nx)
   {
        funcm[Nx*j+i]=interpolate(i*h,j*tau/tconst);
        if(f!=funcm[Nx*j+i])
{
            f=funcm[Nx*j+i];
            fout<<i*h<<"\t"<<j*tau<<"\t"<<funcm[Nx*j+i]<<endl;

}
        i++;
        }
    j++;
    i=0;
 }

    fout.close();
    return 0;
    }
    case 2:
    {
    return 0;
    }
     default:
    return -1;
    }
}

int maths::setAll(double Tt,double Ll, double dt, double dx)
{
    T=Tt*tconst;
    L=Ll;
    Nx=L/dx+1,Nt=Tt/dt+1;
    h = L/(Nx-1),  tau = T/ (Nt-1);
     gamma1 = sigma / (h * h);
     A=new double[Nx];
     B=new double[Nx];
      C=new double[Nx];
      F=new double[Nx];
      U=new double[Nx];
      alpha=new double[Nx];
      betta=new double[Nx];
      uDown=new double[Nx];
      uMiddle=new double[Nx];
      uUp=new double[Nx];
      Hm=new double[Nx];
      funcm =new double[Nx*Nt];
      Accuracy=(tau*tau+h*h);
      if ((model_H==1)&&(setH()!=0))
          return -1;
      if ((model_func==1)&&(set_func()!=0))
          return -1;
      return 0;
}
double maths::H(double x)
{
    //double alpha=0.05;
    //double dh=4;
    switch(model_H)
    {case 1:
    {
    int i=(x/h+0.01);
    if ((i>=0)&&(i<Nx))
        return Hm[i];
    else
        return 1;
    }
    case 2:
    {
        return H1;
        break;
    }
    case 3://stupen
    {
                if (x<x1)
                    return H1;
                else
                    if (x<x2)
                        return H2;
                else return H1;
                break;
    }
    case 4://ustup
    {
        if (x<x1)
            return H1;
        else
                return H2;
    }
    case 5://naklonnoe dno
    {
        if(x<x1)
            return H1;
        double Hl;
        Hl=H1-tan(alph/180.0*pi)*(x-x1);
        if (((Hl<H2)&&(Hl>H1))||((Hl>H2)&&(Hl<H1)))
            return Hl;
        else return H2;

    }
    case 6://cube_up
    {
        if(x<x1)
                 return 5.0;
                     if(x<=x2)
                             return (1.0+4.0/pow((x1-x2),3.0)*(x-x2)*(x-x2)*(x-x2));
                    return 1;
    }
    case 7:
    {
            if(x<x1)
                     return 5.0;
                         if(x<=x2)
                                 return (5.0+4.0/pow((x1-x2),3.0)*(x-x1)*(x-x1)*(x-x1));
                          return 1;
        }
    case 8:
    {
        if(x<x1)
          return 5.0;
        else
           if (x< x2)
              {
                  double xr=(x2*pow(5.0,(3.0/4.0))-x1)/(pow(5.0,(3.0/4.0))-1);
                  double Aq=5.0/pow((xr-x1),(4.0/3.0));
                  return Aq*pow((xr-x),(4.0/3.0));

              }
              else
               return 1.0;
    }


       default:
    {
        return 1;
    }
    }

}
double maths::func(double x,double t)
{

    switch(model_func)
    {case 1:
   {int i=0, j=0;
   i=(x/h+0.01); j=(t/tau+0.01);
   if((i>=0)&&(i<Nx)&&(j>=0)&&(j<Nt))
    return funcm[Nx*j+i];
   else
               return 0;
    }
    case 2:
    {
    double r;
    if ((k*(x-xcenter-speed*t)>-pi/2)&&((k*(x-xcenter-speed*t))<=pi/2)&&(t<tstop))
      {
        r= -ampl*(sin(k*(x-xcenter-speed*t)-pi/2));
       return r;
    }
    else
    return 0;
    }
    case 3:
    {
        if(t<tstop)
        {
        ampl=1.0;
        double Ll=250.0;
        double alpha=2*4.6/Ll;
        return ampl/cosh(alpha*(x-xcenter-speed*t));
        }else return 0;
    }
    }
    return 0;
}
double maths::fi(int n,int m)//заменить tau на tau tconst
{
    //if ((k*(h*(n)-xcenter-speed*(m)*tau/tconst)>-pi/2)&&((k*(h*(n)-xcenter-speed*(m)*tau/tconst))<=pi/2))
        //if((n>=1)&&(n<Nx-1)&&(m>=2)&&(m<Nt-2))
            //return ampl*(sin(k*(n*h-xcenter-speed*tau*m/tconst)-pi/2));

        //return -1/2.0/tau/h*speed*(sin(k*((n+0.5)*h-speed*(m+1)*tau))-sin(k*((n+0.5)*h-speed*(m-1)*tau))-sin(k*((n-0.5)*h-speed*(m+1)*tau))+sin(k*((n-0.5)*h-speed*(m-1)*tau)));
        //return -1/2.0/tau/h*speed*((k*((n+0.5)*h-speed*(m+1)*tau))-(k*((n+0.5)*h-speed*(m-1)*tau))-(k*((n-0.5)*h-speed*(m+1)*tau))+(k*((n-0.5)*h-speed*(m-1)*tau)));

   //if ((k*(h*(n-1)-xcenter-speed*(m+2)*tau/tconst)>-pi/2)&&((k*(h*(n+1)-xcenter-speed*(m-2)*tau/tconst))<=pi/2))
    if((n>=1)&&(n<Nx-1)&&(m>=2)&&(m<Nt-2))
        //return speed/2/h/tau*(func(h*(n+0.5),tau/tconst*(m+1))-func(h*(n+0.5),tau/tconst*(m-1))-func(h*(n-0.5),tau/tconst*(m+1))+func(h*(n-0.5),tau/tconst*(m-1)));
        return 1.0/(8.0*tau*tau)*(func(h*(n+1),tau/tconst*(m+2))+2.0*func(h*n,tau/tconst*(m+2))+func(h*(n-1),tau/tconst*(m+2))
                                        -2.0*(func(h*(n+1),tau/tconst*(m+1))+2.0*func(h*n,tau/tconst*(m+1))+func(h*(n-1),tau/tconst*(m+1)))
                                        +2.0*func(h*(n+1),tau/tconst*m)+4.0*func(h*n,tau/tconst*m)+2.0*func(h*(n-1),tau/tconst*m)
                                        -2.0*(func(h*(n+1),tau/tconst*(m-1))+2.0*func(h*n,tau/tconst*(m-1))+func(h*(n-1),tau/tconst*(m-1)))
                                        +func(h*(n+1),tau/tconst*(m-2))+2.0*func(h*n,tau/tconst*(m-2))+func(h*(n-1),tau/tconst*(m-2)));
    else return 0;
}

void maths::progonka(double *a, double *b, double *c, double *f, int n)
{
    double kappa1,kappa2,mu1,mu2;
     kappa1 = 0.0, kappa2=0.0,mu1 = uMiddle[0]+(tau/h)*(sqrt(H(h))*uMiddle[1]-sqrt(H(0))*uMiddle[0]) ,mu2 = uMiddle[Nx-1]-(tau/h)*(sqrt(H(h*(Nx-1)))*uMiddle[Nx-1]-sqrt(H(h*(Nx-2)))*uMiddle[Nx-2]); //gu
    /*double k1=sqrt(H(0)+H(h)/2.0)*tau/2.0/h,k2=sqrt(H(L)+H(L-h)/2.0)*tau/2.0/h;
    kappa1=k1/(1+k1);kappa2=k2/(1+k2);
    mu1=k1*(uDown[0]-uDown[1])+2.0*uMiddle[0]-uDown[1];
    mu2=k2*(uDown[Nx-1]-uDown[Nx-2])+2.0*uMiddle[Nx-1]-uDown[Nx-1];
    */
    alpha[1] = kappa1;
    betta[1] = mu1;

    for (int i = 1; i <= n - 1; i++)
        {
        alpha[i + 1] =-1.0*b[i]/(c[i] + alpha[i] * a[i]);//koeff

        betta[i + 1] = (-a[i] * betta[i] + f[i]) / (c[i] +alpha[i] * a[i]);

        }
    // mu2=f[n];    kappa2=-a[n];
    U[n] = (mu2 + betta[n] * kappa2) / (1 - alpha[n] * kappa2);//obratnaya


    for (int i = n - 1; i >= 0; i--)
        U[i] = alpha[i + 1] * U[i + 1] + betta[i + 1];
}
/*void maths::layerSolve(int m)
{
    int i;
   U[0] =uMiddle[0]+tau/h*sqrt((H(h))*uMiddle[1]-sqrt(H(0))*uMiddle[0]) , U[Nx-1] =uMiddle[Nx-1]-tau/h*(sqrt(H(h*(Nx-1)))*uMiddle[Nx-1]-sqrt(H(h*(Nx-1-1)))*uMiddle[Nx-1-1]);
    for(i=1;i<Nx-1;i++)
    {
        U[i]=(H(i*h)*tau/h)*tau/h*(uMiddle[i+1]-2.0*uMiddle[i]+uMiddle[i-1])+func(i*h,m*tau)*tau*tau+(2.0*uMiddle[i]-uDown[i]);

    }
        //cout<<U[1]<<endl;

        for (int k = 0; k < Nx; k++)
            uUp[k] = U[k];
}*/
void maths::initialConditions()
{
    int i;
    for(i=0;i<Nx-1;i++)
    {
        uDown[i]=0;
        uMiddle[i]=0;
    }


}
void maths::layerSolve(int m)
{

        for (int i = 1; i < Nx - 1; i++)
        {
            A[i] = 0.5 * gamma1*(H(h*i)+H(h*(i-1)));
            C[i] = -1.0/tau/tau - gamma1*0.5*(H(h*(i+1))+H(h*(i-1))+2*H(h*i));
            B[i] = 0.5 * gamma1*(H(h*(i+1))+H(h*i));
            F[i] =(-2.0/tau/tau)*uMiddle[i]+1.0/tau/tau*uDown[i]-fi(i,m)-0.5*gamma1*((uDown[i+1]-uDown[i])*(H(h*(i+1))+H(h*i))-(uDown[i]-uDown[i-1])*(H(h*i)+H(h*(i-1))))-(-2.0*sigma+1.0)/2.0/h/h*((uMiddle[i+1]-uMiddle[i])*(H(h*(i+1))+H(h*i))-(uMiddle[i]-uMiddle[i-1])*(H(h*i)+H(h*(i-1)))) ;//1й полуслой

        }


       // cout<<A[i]<<"\t"<<B[i]<<"\t"<<C[i]<<"\t"<<F[i]<<"\t"<<endl;
            progonka(A, B, C, F, Nx-1);
            for (int k = 0; k < Nx; k++)
                uUp[k] = U[k];
}

int maths::equationSolve(string filename)
{
    int i,t,m;
    fstream fout;
    fout.open("D:/table.txt", ios_base::out | ios_base::trunc);

        for (int i = 0; i < Nx; i++)
        {
           uDown[i]=0;//initial conditions
           uMiddle[i]=0;
        }
        fout<<"{";
    for ( t = 2; t <= Nt; t++){
        layerSolve(t);
        for( m = 0; m < Nx; m++)
        fout<<"{"<<h*m<<","<<t<<","<<uUp[m]<<"},";
        for (i = 0; i < Nx; i++)
            {
                uDown[i] = uMiddle[i];
                uMiddle[i]=uUp[i];

            }
    }
    fout<<"};";
    //freopen("data1.txt", "w", stdout);

                //printf("{%f,%f,%f},", n * dx, m * dy, uUp[n][m]);

    return 0;
}
