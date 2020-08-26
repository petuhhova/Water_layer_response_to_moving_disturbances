#ifndef MATHS_H
#define MATHS_H

#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

class maths
{
private:
    const double pi= 3.14159265;
    double g=9.81,sigma=0.3;

    void progonka(double *a, double *b, double *c, double *f, int n);

public:

    //double equation_to_math(string equation,double x);

    int Nx=101,Nt=101;
    double fi(int n,int m);
    class point{
    public:
        double x;
        double t;
        double u;
    };
    point p;
    double T =10.0 , L = 10.0;
    double h = L / (Nx-1),  tau = T / (Nt-1), speed=0.15,ampl=1.0,k=0.26,xcenter=300;//speed=0.3
    double gamma1 = sigma / (h * h);
    double tconst=sqrt(g/1000.0);
    double *A=new double[Nx], *B=new double[Nx], *C=new double[Nx], *F=new double[Nx], *U=new double[Nx], *alpha=new double[Nx], *betta=new double[Nx],*uDown=new double[Nx],*uMiddle=new double[Nx], *uUp=new double[Nx];
    double *funcm= new double[Nx*Nt];
    double *Hm=new double[Nx];
    vector<point> func_vector;
    vector<point> depth_vector;
    double Accuracy=(tau*tau+h*h);
    double x1,x2,H1,H2;
    double tstop,alph;
    int model_func,model_H;


    maths(double T,double L,double t,double h);
    maths();
    double H(double x);
    double func(double x,double t);
    int set_func();
   int setH();
    double interpolate(double x, double y);
    int equationSolve(string filename);
    int setAll(double Tt,double Ll, double dt, double dx);
    void layerSolve(int m);
    void initialConditions();
};

#endif // MATHS_H
