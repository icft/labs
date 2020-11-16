#pragma once
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <sstream>

struct Point {
	double x, y;
	Point(double x0 = 0, double y0 = 0) :x(x0), y(y0) {}
};

class function
{
public:
	function(double x = 0, double y = 0, int i = 0);
	function(double* x);
	function(double* x, double(*f)(double));
	void add(Point A);
	void add(function F);
	int SetPoint(double x, double y);
	int findPos(double key);
	void GetTable();
	int GetType();
	double GetInterpolation(double x);
	double GetMinMax(int(*f)(double, double));
private:
	static const int SZ = 5;
	Point P[SZ];
	int i;
};
