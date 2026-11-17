#include <stdio.h>  
#include <stdlib.h> 
#include <math.h>   

#define MAX_INTERVALS 1048576U 

double f(double x);
double left_rectangles(double a, double b, unsigned int n);
double right_rectangles(double a, double b, unsigned int n);
double trapezoidal(double a, double b, unsigned int n);
double simpson(double a, double b, unsigned int n);
double integrate_with_precision(int method, double a, double b, double eps, unsigned int *final_n);
void clear_input_buffer(void);

int main(void)
{
    int method = 0;          
    double a = 0.0;          
    double b = 0.0;          
    unsigned int n = 0;      
    double eps = 0.0;        
    double result = 0.0;     

    printf("Laboratory work #2.2 | Numerical Integration | Variant 5\n");
    printf("Student: Timanovskyi Mark Olehovych | Group: RS-zp61\n\n");

    do
    {
        printf("Select numerical integration method:\n");
        printf("  1 - Left rectangles method\n");
        printf("  2 - Right rectangles method\n");
        printf("  3 - Trapezoidal method\n");
        printf("  4 - Simpson's (parabolic) method\n");
        printf("  5 - Compare all 4 methods (fixed N and precision eps)\n");
        printf("Enter method (1-5): ");

        if (scanf("%d", &method) != 1)
        {
            printf("Input error! Please enter an integer from 1 to 5.\n\n");
            clear_input_buffer();
            method = 0;
            continue;
        }
        clear_input_buffer();

        if (method < 1 || method > 5)
        {
            printf("Invalid choice! Only values 1..5 are allowed.\n\n");
        }
    } while (method < 1 || method > 5);

    while (1)
    {
        printf("\nEnter lower integration limit a: ");
        if (scanf("%lf", &a) != 1)
        {
            printf("Input error! Enter a valid floating-point number.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        printf("Enter upper integration limit b (b > a): ");
        if (scanf("%lf", &b) != 1)
        {
            printf("Input error! Enter a valid floating-point number.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        if (b <= a)
        {
            printf("Validation error! Upper limit b must be greater than a.\n");
        }
        else
        {
            break;
        }
    }

    while (1)
    {
        printf("Enter number of subintervals N (N >= 2): ");
        if (scanf("%u", &n) != 1)
        {
            printf("Input error! Enter a positive integer.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        if (n < 2)
        {
            printf("Validation error! N must be at least 2.\n");
        }
        else
        {
            break;
        }
    }

    if ((method == 4 || method == 5) && (n % 2 != 0))
    {
        n++;
        printf("Note: Simpson's method requires even N. Adjusted N = %u.\n", n);
    }

    while (1)
    {
        printf("Enter required precision eps (e.g. 0.0001): ");
        if (scanf("%lf", &eps) != 1)
        {
            printf("Input error! Enter a valid positive number.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        if (eps <= 0.0)
        {
            printf("Validation error! Precision eps must be strictly positive.\n");
        }
        else
        {
            break;
        }
    }

    printf("\nIntegration interval: [%.4lf; %.4lf], N = %u, h = %.6lf, eps = %.6lf\n\n",
           a, b, n, (b - a) / (double)n, eps);

    if (method >= 1 && method <= 4)
    {
        unsigned int n_eps = 0;
        double result_eps = 0.0;

        if (method == 1)
        {
            result = left_rectangles(a, b, n);
            printf("Method: Left rectangles\n");
        }
        else if (method == 2)
        {
            result = right_rectangles(a, b, n);
            printf("Method: Right rectangles\n");
        }
        else if (method == 3)
        {
            result = trapezoidal(a, b, n);
            printf("Method: Trapezoidal\n");
        }
        else
        {
            result = simpson(a, b, n);
            printf("Method: Simpson's (parabolic)\n");
        }

        result_eps = integrate_with_precision(method, a, b, eps, &n_eps);

        printf("Integral value (for fixed N = %u) : %.8lf\n", n, result);
        printf("Integral value (with eps = %.6lf): %.8lf (reached at N = %u)\n",
               eps, result_eps, n_eps);
    }
    else
    {
        const char *names[4] = {
            "Left rectangles",
            "Right rectangles",
            "Trapezoidal",
            "Simpson's"
        };
        double fixed_vals[4] = {
            left_rectangles(a, b, n),
            right_rectangles(a, b, n),
            trapezoidal(a, b, n),
            simpson(a, b, n)
        };

        printf("+---------------------+--------------------+--------------------+------------+\n");
        printf("| Method              | I (fixed N=%-5u) | I (precision eps)  | Final N    |\n", n);
        printf("+---------------------+--------------------+--------------------+------------+\n");

        for (int m = 1; m <= 4; m++)
        {
            unsigned int n_eps = 0;
            double val_eps = integrate_with_precision(m, a, b, eps, &n_eps);
            printf("| %-19s | %18.8lf | %18.8lf | %10u |\n",
                   names[m - 1], fixed_vals[m - 1], val_eps, n_eps);
            printf("+---------------------+--------------------+--------------------+------------+\n");
        }
    }

    return 0; 
}

double f(double x)
{
    return x / sqrt(x * x + 16.0);
}

double left_rectangles(double a, double b, unsigned int n)
{
    double h = (b - a) / (double)n;
    double sum = 0.0;

    for (unsigned int i = 0; i < n; i++)
    {
        sum += f(a + (double)i * h);
    }

    return sum * h;
}

double right_rectangles(double a, double b, unsigned int n)
{
    double h = (b - a) / (double)n;
    double sum = 0.0;

    for (unsigned int i = 1; i <= n; i++)
    {
        sum += f(a + (double)i * h);
    }

    return sum * h;
}

double trapezoidal(double a, double b, unsigned int n)
{
    double h = (b - a) / (double)n;
    double sum = 0.5 * (f(a) + f(b));

    for (unsigned int i = 1; i < n; i++)
    {
        sum += f(a + (double)i * h);
    }

    return sum * h;
}

double simpson(double a, double b, unsigned int n)
{
    if (n % 2 != 0)
    {
        n++;
    }

    double h = (b - a) / (double)n;
    double sum = f(a) + f(b);

    for (unsigned int i = 1; i < n; i++)
    {
        double x = a + (double)i * h;
        if (i % 2 == 0)
        {
            sum += 2.0 * f(x);
        }
        else
        {
            sum += 4.0 * f(x);
        }
    }

    return sum * (h / 3.0);
}

double integrate_with_precision(int method, double a, double b, double eps, unsigned int *final_n)
{
    unsigned int curr_n = 2;
    double i_prev = 0.0;
    double i_curr = 0.0;
    double runge_coeff = 1.0; 

    if (method == 1)
    {
        i_prev = left_rectangles(a, b, curr_n);
        runge_coeff = 1.0; 
    }
    else if (method == 2)
    {
        i_prev = right_rectangles(a, b, curr_n);
        runge_coeff = 1.0; 
    }
    else if (method == 3)
    {
        i_prev = trapezoidal(a, b, curr_n);
        runge_coeff = 3.0;
    }
    else
    {
        i_prev = simpson(a, b, curr_n);
        runge_coeff = 15.0; 
    }

    while (curr_n < MAX_INTERVALS)
    {
        curr_n *= 2;

        if (method == 1)
        {
            i_curr = left_rectangles(a, b, curr_n);
        }
        else if (method == 2)
        {
            i_curr = right_rectangles(a, b, curr_n);
        }
        else if (method == 3)
        {
            i_curr = trapezoidal(a, b, curr_n);
        }
        else
        {
            i_curr = simpson(a, b, curr_n);
        }

        if (fabs(i_curr - i_prev) / runge_coeff <= eps)
        {
            break;
        }

        i_prev = i_curr;
    }

    *final_n = curr_n;
    return i_curr;
}

void clear_input_buffer(void)
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
    {
    }
}