#include "stdafx.h"

#include <iostream>
#include <cstdlib>  
#include <cmath>
#include <time.h>

using namespace std;

double F1(double *x)
{
	return x[0] * x[0] + 2 * x[0]*x[1] + 3 * x[1] * x[1] - 2 * x[0] - 3 * x[1];
}
// Производная по первой переменной
double GradF1(double *x)
{
	return	2 * x[0] + 2 * x[1] - 2;
}
// Производная по второй переменной
double GradF2(double *x)
{
	return 2 * x[0] + 6 * x[1] - 3;
}

// Вычисление скалярного произведения
double inner_prod(double *x, double *y) { return x[1] * y[1] + x[0] * y[0]; }


//Одномерная оптимизация - применим метод золотого сечения
double FindMin(double *s, double *p)
{
	const double eps = 1e-8;
	const double tay = 1.618;
	double a = 0;
	double b = 1e5;
	double x0, x1, xf1, xf2;
	x0 = b - (b - a) / tay; // Расчитываем точки деления
	x1 = a + (b - a) / tay;         //
P:
	double t1[2], t2[2];
	t1[0] = s[0] + x0 * p[0];
	t1[1] = s[1] + x0 * p[1];
	t2[0] = s[0] + x1 * p[0];
	t2[1] = s[1] + x1 * p[1];
	xf1 = F1(t1); // Расчитываем в точках деления значение целевой функции
	xf2 = F1(t2); //
	if (xf1 >= xf2)
	{
		a = x0;
		x0 = x1;
		t2[0] = s[0] + x1 * p[0];
		t2[1] = s[1] + x1 * p[1];
		xf1 = F1(t2);
		t2[0] = s[0] + x1 * p[0];
		t2[1] = s[1] + x1 * p[1];
		x1 = a + (b - a) / tay;
		xf2 = F1(t2);
	}
	else
	{
		b = x1;
		x1 = x0;
		xf2 = xf1;
		x0 = b - (b - a) / tay;
		t1[0] = s[0] + x0 * p[0];
		t1[1] = s[1] + x0 * p[1];
		xf1 = F1(t1);
	}
	if (fabs(b - a) < eps)
	{
		return (a + b) / 2;
	}
	else
		goto P;
}

void PrintSolution(double *x, double val, int numIter)
{
	cout << "Количество итераций: " << numIter << endl;
	cout << "Точки: " << endl << "[" << x[0] << "," << x[1] << "]" << endl;
	cout << "Значение: " << val << endl << endl;
}

double FletcherRievesMethod()
{
	double eps = 1e-5;
	const double EPS = 1e-5;
	//Начальное приближение
	double x[2] = { 10,10 };
	double curVal = F1(x);
	double prevVal = curVal;
	double p[2];
	p[0] = -GradF1(x);
	p[1] = -GradF2(x);

	double gradSquare = inner_prod(p, p);

	int numIter = 0;
	do
	{
		numIter++;
		double alpha, beta, newGradSquare;
		double newGrad[2];


		//Ищем минимум F1(x + alpha * p) с помощью метода одномерной оптимизации
		alpha = FindMin(x, p);
		x[0] = x[0] + alpha * p[0];
		x[1] = x[1] + alpha * p[1];
		newGrad[0] = -GradF1(x);
		newGrad[1] = -GradF2(x);

		newGradSquare = inner_prod(newGrad, newGrad);

		if (numIter % (10) == 0) beta = 0; //Обновление
		else
			beta = newGradSquare / gradSquare; //Используем метод Флетчера - Ривса
		p[0] = newGrad[0] + beta * p[0];
		p[1] = newGrad[1] + beta * p[1];

		prevVal = curVal;
		curVal = F1(x);

		gradSquare = newGradSquare;
	} while (gradSquare > eps);
	PrintSolution(x, F1(x), numIter);
	return 0;

}

// Метод сопряженных градиентов формирует направления поиска, в большей мере соответствующие геометрии минимизируемой функции.
// Стратегия метода Флетчера-Ривса состоит в построении последовательности точек {xk}, k=0, 1, 2, ... таких, что f(xk+1) < f(xk), k=0, 1, 2, ... 

int main()
{
	setlocale(LC_ALL, "Russian");
	cout.precision(9);
	cout << "Решение задачи поиска минимума функции методом Сопряженных градиентов Флетчера-Ривса\n";
	cout << "Функция x^2 + 2xy + 3y^2 - 2x - 3y\n";
	cout << "Точность 1e-5, ограничение по итерациям отсутствует\n";
	cout << "Начальные точки: 10, 10\n";
	cout << "В качестве метода одномерной отпимизации - метод золотого сечения\n";
	cout << "*****************************\n";
	float fTimeStart = clock() / (float)CLOCKS_PER_SEC;	
	FletcherRievesMethod();
	float fTimeStop = clock() / (float)CLOCKS_PER_SEC;
	cout << "Длительность процесса " << fTimeStop - fTimeStart << " секунд\n";

	getchar();
	return 0;
}