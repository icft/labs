#include "tablef.h"
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <iostream>

function::function(double x, double y, int i) {
	for (int l = 0; l < this->SZ; l++) {
		this->P[l].x = x;
		this->P[l].y = y;
	}
	this->i = i;
}

function::function(double* x) {
	this->i = 0;
	for (int l = 0; l < this->SZ; l++) {
		SetPoint(*x, 0);
		*x++;
	}
}

function::function(double* x, double(*f)(double)) {
	this->i = 0;
	for (int l = 0; l < this->SZ; l++) {
		SetPoint(*x, f(*x));
		*x++;
	}
}


void function::add(Point A) {
	SetPoint(A.x, A.y);
}

void function::add(function F) {
	int flag = 0;
	int l = 0;
	for (int i = 0; i < F.i; i++)
	{
		flag = SetPoint(F.P[i].x, F.P[i].y);
		if (flag == 1) l++;
	}

}

int function::SetPoint(double x, double y) {
	int i = this->i - 1;
	if (i == this->SZ - 1)
		return -1;
	else if (this->P[findPos(x)].x == x)
		return -2;
	else {
		while (i >= 0 && this->P[i].x > x) {
			this->P[i + 1].x = this->P[i].x;
			this->P[i + 1].y = this->P[i].y;
			i = i - 1;
		}
		this->P[i + 1].x = x;
		this->P[i + 1].y = y;
		this->i++;
	}
	return 1;
}

int function::findPos(double key) {
	bool flag = false;
	int l = 0;
	int r = this->i;
	int mid;

	if (this->i == 0 && key == 0)
		return -1;
	while ((l <= r) && (flag != true)) {
		mid = (l + r) / 2; 
		if (this->P[mid].x == key) flag = true;
		if (this->P[mid].x > key)
			r = mid - 1; 
		else l = mid + 1;
	}
	return mid;
}

void function::GetTable() {
	std::cout << "fact size = " << this->i << std::endl << "SIZE =  " << this->SZ << std::endl << std::endl;
	for (int i = 0; i < this->i; i++)
		std::cout << " i = " << i << ": (" << this->P[i].x << "; " << this->P[i].y << ") " << std::endl << std::endl;
}

double function::GetMinMax(int(*f)(double, double))
{
	double res = this->P[0].y;
	int i;
	for (i = 0; i < this->i; ++i)
		if (f(this->P[i].y, res) > 0)
			res = this->P[i].y;
	return res;
}

int function::GetType() {
	if (this->i == 0)
		return -3;
	else if (this->i == 1)
		return 0;
	else {
		int flagi = 0; int flagd = 0; int flagc = 0;
		for (int l = 1; l < this->i; l++) {
			if (this->P[l - 1].y < this->P[l].y)
				flagi = 1;
			else if (this->P[l - 1].y > this->P[l].y)
				flagd = 1;
			else flagc = 1;
			if (flagi == 1 && flagd == 1)
				return -2;
		}
		if (flagi == 1)
			return 1;
		else if (flagd == 1)
			return -1;
		else if (flagc == 1)
			return 0;
	}
}

double function::GetInterpolation(double x) {
	int i = findPos(x);
	double a0, a1;
	if (this->i == 0)
		return 0;
	if (i == 0) i++;
	a1 = (this->P[i].y - this->P[i - 1].y) / (this->P[i].x - this->P[i - 1].x);
	a0 = this->P[i - 1].y - a1 * this->P[i - 1].x;


	return a0 + a1 * x;
}
