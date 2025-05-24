#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

//  импорт функций из файла functions.asm
extern double f1(double) __attribute__((cdecl));
extern double f2(double) __attribute__((cdecl));
extern double f3(double) __attribute__((cdecl));
extern double f1_p(double) __attribute__((cdecl));
extern double f2_p(double) __attribute__((cdecl));
extern double f3_p(double) __attribute__((cdecl));

// функции для тестирования и их производные
double ff1(double x)
{
	return 2.0 * x;
}

double ff2(double x)
{
	return 2.0 - x;
}

double ff3(double x)
{
	return 1.0 / (2.0 - x);
}

double ff1_p(double x)
{
	return 2.0;
}

double ff2_p(double x)
{
	return -1.0;
}

double ff3_p(double x)
{
	return 1.0 / ((2 - x) * (2 - x));
}

// поиск точки пересечения двух графиков
double root(double x, double (*f_fir)(double), double (*f_sec)(double), double (*f_fir_p)(double), double (*f_sec_p)(double), int *i, double epsi)
{
	while (fabs(f_fir(x) - f_sec(x)) > epsi)
	{
		if (fabs(f_fir_p(x) - f_sec_p(x)) < 1e-10)
		{
			printf("Error: trying to divide by zero\n");
			return NAN; // Возвращаем "не число" в случае ошибки
		}
		*i += 1;
		x = x - (f_fir(x) - f_sec(x)) / (f_fir_p(x) - f_sec_p(x));
	}
	return x;
}

// поиск площади под графиком между заданными точками
double integral(double x1, double x2, double (*f)(double), double eps)
{
	double n = 100000.0;
	double h;
	double s1 = 0;
	double s2 = 0;
	do
	{
		s1 = 0;
		s2 = 0;
		h = (x2 - x1) / n;
		for (int i = 0; i < n; ++i)
		{
			s1 += h * (f(x1 + i * h) + f(x1 + h + i * h)) / 2.0;
		}
		double g = n * 2;
		double m = (x2 - x1) / g;
		for (int i = 0; i < g; ++i)
		{
			s2 += m * (f(x1 + i * m) + f(x1 + m + i * m)) / 2.0;
		}
		n = g;
	} while (fabs(s1 - s2) > eps);
	return s2;
};

// для ключа --help
void print_help()
{
	printf("Options:\n");
	printf("  -a    Print the abscissas of the intersection points of the graphics\n");
	printf("  -o    Print the ordinates of the intersection points of the graphics\n");
	printf("  -i    Print the number of iterations required for the approximate solution of the equations when searching for intersection points\n");
	printf(" -help  Display this message\n");
}

int main(int argc, char *argv[])
{

	double eps1 = 0.001 / 2;

	double x13 = 0.0;
	double x12 = 0.0;
	double x23 = 0.0;

	int abscissas = 0;
	int ordinates = 0;
	int iterations = 0;
	int testroot = 0, testint = 0;
	int ii = 1;
	int n1, n2;
	double a, b, e, r;
	int ni;
	double ai, bi, ei, ri;

	while (ii < argc)
	{
		if (strcmp(argv[ii], "-r") == 0 || strcmp(argv[ii], "--root") == 0)
		{
			abscissas = 1;
		}
		else if (strcmp(argv[ii], "-o") == 0)
		{
			ordinates = 1;
		}
		else if (strcmp(argv[ii], "-i") == 0 || strcmp(argv[ii], "--iterations") == 0)
		{
			iterations = 1;
		}
		else if (strcmp(argv[ii], "--test-root") == 0 || strcmp(argv[ii], "-R") == 0)
		{
			testroot = 1;
			ii++;
			sscanf(argv[ii], "%d:%d:%lf:%lf:%lf:%lf", &n1, &n2, &a, &b, &e, &r);
		}
		else if (strcmp(argv[ii], "--test-integral") == 0 || strcmp(argv[ii], "-I") == 0)
		{
			testint = 1;
			ii++;
			sscanf(argv[ii], "%d:%lf:%lf:%lf:%lf", &ni, &ai, &bi, &ei, &ri);
		}
		else if (strcmp(argv[ii], "--help") == 0 || strcmp(argv[ii], "-h") == 0)
		{
			print_help();
		}
		else
		{
			printf("Option %s is not supported\n", argv[ii]);
			print_help();
		}
		ii++;
	};

	int *i12 = malloc(4); // в i хранится кол-во итераций для поиска корней
	int *i23 = malloc(4);
	int *i13 = malloc(4);
	*i12 = 0, *i23 = 0, *i13 = 0;

	double root13 = root(x13, f1, f3, f1_p, f3_p, i12, eps1);
	double root12 = root(x12, f1, f2, f1_p, f2_p, i13, eps1);
	double root23 = root(x23, f2, f3, f2_p, f3_p, i23, eps1);

	double eps2 = 0.001 / (2 * (f1(root13) + 2 * f2(root23) + f2(root12)));

	double s1 = integral(root13, root12, f1, eps2);
	double s2 = integral(root23, root12, f2, eps2);
	double s3 = integral(root13, root23, f3, eps2);

	if (!testint && !testroot)
	{
		printf("f1: 0.35x^2 - 0.95x + 2.7\n");
		printf("f2: 3x + 1\n");
		printf("f3: 1/(x+2)\n");
	}
	if (abscissas)
	{
		printf("f1 and f2 intersect in x = %lf\n", root12);
		printf("f1 and f3 intersect in x = %lf\n", root13);
		printf("f2 and f3 intersect in x = %lf\n", root23);
		printf("\n");
	}
	if (ordinates)
	{
		printf("f1 and f2 intersect in y = %lf\n", f2(root12));
		printf("f1 and f3 intersect in y = %lf\n", f1(root13));
		printf("f2 and f3 intersect in y = %lf\n", f2(root23));
		printf("\n");
	}
	if (iterations)
	{
		printf("Iterations required to find f1 and f2 intersection: %d\n", *i12);
		printf("Iterations required to find f1 and f3 intersection: %d\n", *i13);
		printf("Iterations required to find f2 and f3 intersection: %d\n", *i23);
		printf("\n");
	}
	if (testroot)
	{
		printf("y1: 2x\n");
		printf("y2: 1-x\n");
		printf("y3: 1/(2-x)\n");
		if (n1 + n2 == 3)
		{
			double computed_root = root(a, ff1, ff2, ff1_p, ff2_p, i12, e);
			if (computed_root >= a && computed_root <= b)
				printf("%.3lf %.3lf %.3lf\n", computed_root, fabs(computed_root - r), fabs(computed_root - r) / fabs(r));
			else
				printf("The intersection point does not belong to this section");
		}
		else if (n1 + n2 == 5)
		{
			double computed_root = root(a, ff3, ff2, ff3_p, ff2_p, i23, e);
			if (computed_root >= a && computed_root <= b)
				printf("%.3lf %.3lf %.3lf\n", computed_root, fabs(computed_root - r), fabs(computed_root - r) / fabs(r));
			else
				printf("The intersection point does not belong to this section");
		}
		else if (n1 + n2 == 4)
		{
			double computed_root = root(a, ff1, ff3, ff1_p, ff3_p, i13, e);
			if (computed_root >= a && computed_root <= b)
				printf("%.3lf %.3lf %.3lf\n", computed_root, fabs(computed_root - r), fabs(computed_root - r) / fabs(r));
			else
				printf("The intersection point does not belong to this section");
		}
		printf("\n");
	}

	if (testint)
	{
		printf("y1: 2x\n");
		printf("y2: 2-x\n");
		printf("y3: 1/(2-x)\n");
		if (ni == 3)
		{
			double computed_value = integral(ai, bi, ff3, ei);
			printf("%.3lf %.3lf %.3lf\n", computed_value, fabs(computed_value - ri), fabs(computed_value - ri) / fabs(ri));
		}
		else if (ni == 1)
		{
			double computed_value = integral(ai, bi, ff1, ei);
			printf("%.3lf %.3lf %.3lf\n", computed_value, fabs(computed_value - ri), fabs(computed_value - ri) / fabs(ri));
		}
		else if (ni == 2)
		{
			double computed_value = integral(ai, bi, ff2, ei);
			printf("%.3lf %.3lf %.3lf\n", computed_value, fabs(computed_value - ri), fabs(computed_value - ri) / fabs(ri));
		}
		printf("\n");
	}

	printf("The area is %lf\n", fabs(s1 - s2 - s3));

	return 0;
}