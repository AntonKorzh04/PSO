# define M_PI           3.14159265358979323846  /* pi */

#include "functions.hpp"

// Реализация методов C_Function
double C_Function::calc(double x, double y) {
    return Core(x, y);
}

double C_Function::GetMinArg() { return minArg; }
double C_Function::GetMaxArg() { return maxArg; }
double C_Function::GetMinFun() { return minFun; }
double C_Function::GetMaxFun() { return maxFun; }
double C_Function::GetMaxFunX() { return maxFunCoords[0]; }
double C_Function::GetMaxFunY() { return maxFunCoords[1]; }
string C_Function::GetNamFun() { return fuName; }

void C_Function::SetMinArg(double min) { minArg = min; }
void C_Function::SetMaxArg(double max) { maxArg = max; }
void C_Function::SetMinFun(double min) { minFun = min; }
void C_Function::SetMaxFun(double max) { maxFun = max; }
void C_Function::SetMaxFunX(double x) { maxFunCoords[0] = x; }
void C_Function::SetMaxFunY(double y) { maxFunCoords[1] = y; }
void C_Function::SetNamFun(string nam) { fuName = nam; }

double C_Function::Core(double x, double y) { return 0.0; }

// Реализация методов C_Skin
C_Skin::C_Skin()
{
    SetNamFun("Skin");
    SetMinArg(-5.0);
    SetMaxArg(5.0);
    SetMinFun(-4.3182); //[x=3.07021;y=3.315935] 1 point
    SetMaxFun(14.0606); //[x=-3.315699;y=-3.072485] 1 point
    SetMaxFunX(-3.315699);
    SetMaxFunY(-3.072485);
}

double C_Skin::Core(double x, double y)
{
    double a1 = 2 * x * x;
    double a2 = 2 * y * y;
    double b1 = cos(a1) - 1.1;
    b1 = b1 * b1;
    double c1 = sin(0.5 * x) - 1.2;
    c1 = c1 * c1;
    double d1 = cos(a2) - 1.1;
    d1 = d1 * d1;
    double e1 = sin(0.5 * y) - 1.2;
    e1 = e1 * e1;

    double res = b1 + c1 - d1 + e1;
    return res;
}

// Реализация методов C_Forest
C_Forest::C_Forest()
{
    SetNamFun("Forest");
    SetMinArg(-50.0);
    SetMaxArg(-18.0);
    SetMinFun(0.0); // many points
    SetMaxFun(15.95123239744); //[x=-25.132741228718345;y=-32.55751918948773] 1 point
    SetMaxFunX(-25.132741228718345);
    SetMaxFunY(-32.55751918948773);
}

double C_Forest::Core(double x, double y)
{
    double a = sin(sqrt(fabs(x - 1.13) + fabs(y - 2.0)));
    double b = cos(sqrt(fabs(sin(x))) + sqrt(fabs(sin(y - 2.0))));
    double f = a + b;

    double res = pow(f, 4);
    if (res < 0.0)
        res = 0.0;
    return res;
}

// Реализация методов C_Megacity
C_Megacity::C_Megacity()
{
    SetNamFun("Megacity");
    SetMinArg(-15.0);
    SetMaxArg(15.0);
    SetMinFun(0.0); // many points
    SetMaxFun(15.0); //[x=`3.16;y=1.990] 1 point
    SetMaxFunX(3.16);
    SetMaxFunY(1.990);
}

double C_Megacity::Core(double x, double y)
{
    double a = sin(sqrt(fabs(x - 1.13) + fabs(y - 2.0)));
    double b = cos(sqrt(fabs(sin(x))) + sqrt(fabs(sin(y - 2.0))));
    double f = a + b;

    double res = floor(pow(f, 4));
    return res;
}


C_Ackley::C_Ackley()
{
    SetNamFun("Ackley");
    SetMinArg(-5);
    SetMaxArg(5);
    SetMinFun(0);
    SetMaxFun(14);
    SetMaxFunX(0);
    SetMaxFunY(0);
}

double C_Ackley::Core(double x, double y)
{
    return 20 * exp(-0.2 * sqrt(0.5 * (x*x + y*y))) + exp(0.5 * (cos(2 * M_PI * x) + cos(2 * M_PI * y))) - exp(1) - 6;
}

C_Function* SelectFunction(EFunc f)
{
    C_Function* func;
    switch (f)
    {
    case Skin:
        func = new C_Skin();
        break;
    case Forest:
        func = new C_Forest();
        break;
    case Megacity:
        func = new C_Megacity();
        break;
    case Ackley:
        func = new C_Ackley();
        break;    
    default:
        func = new C_Skin();
        break;
    }
    return func;
}
