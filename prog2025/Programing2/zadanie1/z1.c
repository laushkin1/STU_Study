#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void auto_mode(double x_start, double x_stop, double step, int precision, int width, double (*funcs[])(double));
void manual_mode(double x_start, double x_stop, double step, int precision, int width, int optional_params[], int opt_p_index, double (*funcs[])(double));

double f1(double x);
double f2(double x);
double f3(double x);
double f4(double x);
double f5(double x);
double f6(double x);

int main()
{
    // Input all parameters
    double x_start, x_stop, step;
    int precision, width; // always positive

    int optional_params[1024];
    int opt_p_index = 0;
    float manual_mode_activate = false;
    char temp[1024];

    // Checking for errors
    if (scanf("%lf %lf %lf %d %d", &x_start,  &x_stop, &step, &precision, &width) != 5)
    {
        printf("E1");
        return 0;
    }

    if (x_start >= x_stop)
    {
        printf("E2");
        return 0;
    }

    // Continue to input
    while (scanf("%s", temp) == 1)
    {
        if (isdigit(temp[0]) || (temp[0] != '-' && isdigit(temp[1])))
        {
            manual_mode_activate = true;
            int num = atoi(temp);
            if (num >= 1 && num <= 6)
                optional_params[opt_p_index++] = num;
        }
        else break;
    }

    // Dict from python
    double (*funcs[])(double) = {0, f1, f2, f3, f4, f5, f6};

    // If manual rezim
    if (manual_mode_activate) manual_mode(x_start, x_stop, step, precision, width, optional_params, opt_p_index, funcs);
    else auto_mode(x_start, x_stop, step, precision, width, funcs);

    return 0;
}


void auto_mode(double x_start, double x_stop, double step, int precision, int width, double (*funcs[])(double))
{
    printf("%*s", width, "x");
    for (int i = 1; i <= 6; i++)
        printf("%*d", width, i);
    printf("\n");

    for (double x = x_start; x <= x_stop; x+=step)
    {
        printf("%*.*f", width, precision, x);
        for (int i = 1; i <= 6; i++)
            printf("%*.*f", width, precision, funcs[i](x));
        printf("\n");
    }
}


void manual_mode(double x_start, double x_stop, double step, int precision, int width, int optional_params[], int opt_p_index, double (*funcs[])(double))
{
    if (opt_p_index)
    {
        printf("%*s", width, "x");
        for (double x = x_start; x <= x_stop; x+=step)
            printf("%*.*f", width, precision, x);
        printf("\n");
        for (int i = 0; i < opt_p_index; i++)
        {
            printf("%*d", width, optional_params[i]);
            for (double x = x_start; x <= x_stop; x+=step)
                printf("%*.*f", width, precision, funcs[optional_params[i]](x));
            printf("\n");
        }
    }
}


double f1(double x){ return x < 0.0 ? 0.0 : 1.0; }

double f2(double x) { return x > 0 ? x : 0; }

double f3(double x) { double b = 2.0; return 0.5*(x+sqrt((x*x)+b)); }

double f4(double x) { return exp(-x * x); }

double f5(double x) { return (exp(x)-exp(-x))/(exp(x)+exp(-x)); }

double f6(double x) { double b = 0.5; return x/(1+exp(-b*x)); }