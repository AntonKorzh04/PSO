#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <iostream>
#include <cmath>
#include <string>

using namespace std;

class C_Function
{
public:
    double calc(double x, double y);
    double GetMinArg();
    double GetMaxArg();
    double GetMinFun();
    double GetMaxFun();
    double GetMaxFunX();
    double GetMaxFunY();
    string GetNamFun();

protected:
    void SetMinArg(double min);
    void SetMaxArg(double max);
    void SetMinFun(double min);
    void SetMaxFun(double max);
    void SetNamFun(string nam);
    void SetMaxFunX(double x);
    void SetMaxFunY(double y);

private:
    virtual double Core(double x, double y);

    double minArg;
    double maxArg;
    double minFun;
    double maxFun;
    double maxFunCoords[2];
    string fuName;
};

class C_Skin : public C_Function
{
public:
    C_Skin();

private:
    double Core(double x, double y) override;
};

class C_Forest : public C_Function
{
public:
    C_Forest();

private:
    double Core(double x, double y) override;
};

class C_Megacity : public C_Function
{
public:
    C_Megacity();

private:
    double Core(double x, double y) override;
};

class C_Ackley : public C_Function
{
public:
    C_Ackley();

private:
    double Core(double x, double y) override;
};

enum EFunc
{
    Skin,
    Forest,
    Megacity,
    Ackley
};

C_Function* SelectFunction(EFunc f);

#endif // FUNCTIONS_HPP
