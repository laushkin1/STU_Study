#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "functions.h" // Swish funkcia a pomocne funkcie -- NEMENIT !!!
#include "data.h" // Makra, vahy a bias hodnoty -- NEMENIT !!!

void mode_1(void);
void mode_2(void);
void mode_3(void);
void mode_4(void);
void mode_5(void);
void mode_6(void);
void mode_7(void);

double get_weight(int neuron_index, int row, int col);
void load_neurons(const double *img, double *neurons);
void softmax(double *neurons);
int findmax(const double *neurons);
int get_digit(const double *img, double *neurons);
void print_success_rate(int K, const double *img, const int *numbers, double *neurons);


int main(void) {
    int user_input;
    scanf("%d", &user_input);

    switch (user_input)
    {
        case 1: mode_1(); break;
        case 2: mode_2(); break;
        case 3: mode_3(); break;
        case 4: mode_4(); break;
        case 5: mode_5(); break;
        case 6: mode_6(); break;
        case 7: mode_7(); break;
        default: break;
    }
    return 0;
}

void mode_1(void)
{
    double img[IMAGE_SIZE];
    load_image(img, IMAGE_SIZE);
    print_image(img, IMAGE_WIDTH, IMAGE_HEIGHT);
}

void mode_2(void)
{
    int neuron_index, row, col;
    scanf("%d %d %d", &neuron_index, &row, &col);
    printf("%.02lf\n", get_weight(neuron_index, row, col));
}

void mode_3(void)
{
    double img[IMAGE_SIZE];
    load_image(img, IMAGE_SIZE);
    double neurons[NUMBER_OF_NEURONS];
    load_neurons(img, neurons);
    for (int i = 0; i < NUMBER_OF_NEURONS; i++)
        printf("%8d%8.2lf\n", i, neurons[i]);
}

void mode_4(void)
{
    double neurons[NUMBER_OF_NEURONS];
    for (int i = 0; i < NUMBER_OF_NEURONS; i++)
        scanf("%lf", &neurons[i]);
    softmax(neurons);
    for (int i = 0; i < NUMBER_OF_NEURONS; i++)
        if (i!=NUMBER_OF_NEURONS-1) printf("%.2lf ", neurons[i]);
        else printf("%.2lf", neurons[i]);
    printf("\n");
}

void mode_5(void)
{
    double neurons[NUMBER_OF_NEURONS];
    for (int i = 0; i < NUMBER_OF_NEURONS; i++)
        scanf("%lf", &neurons[i]);
    printf("%d\n", findmax(neurons));
}

void mode_6(void)
{
    double neurons[NUMBER_OF_NEURONS];
    double img[IMAGE_SIZE];
    load_image(img, IMAGE_SIZE);
    printf("%d\n", get_digit(img, neurons));
}

void mode_7(void)
{
    int K;
    scanf("%d", &K);

    int numbers[K];
    for (int i = 0; i < K; i++)
        scanf("%d", &numbers[i]);

    double img[IMAGE_SIZE*K];
    load_image(img, IMAGE_SIZE*K);

    double neurons[NUMBER_OF_NEURONS];
    print_success_rate(K, img, numbers, neurons);
}


double get_weight(const int neuron_index, const int row, const int col)
{
    return weights[neuron_index*IMAGE_SIZE+row*IMAGE_WIDTH+col];
}

void load_neurons(const double *img, double *neurons)
{
    double sum;
    for (int k = 0; k < NUMBER_OF_NEURONS; k++)
    {
        sum = 0;
        for (int i = 0; i < IMAGE_SIZE; i++)
            sum += get_weight(k, i/IMAGE_WIDTH, i%IMAGE_WIDTH)*img[i];
        sum += bias[k];
        neurons[k] = sum;
    }
}

void softmax(double *neurons)
{
    double sum = 0.0;
    double max_neuron = neurons[0];

    for (int i = 1; i < NUMBER_OF_NEURONS; i++)
        if (neurons[i] > max_neuron) max_neuron = neurons[i];

    for (int i = 0; i < NUMBER_OF_NEURONS; i++)
    {
        neurons[i] = exp(neurons[i] - max_neuron);
        sum += neurons[i];
    }

    for (int i = 0; i < NUMBER_OF_NEURONS; i++)
        neurons[i]  = neurons[i]/sum;
}

int findmax(const double *neurons)
{
    int max_neuron_indx = 0;

    for (int i = 0; i < NUMBER_OF_NEURONS; i++)
        if (neurons[i] > neurons[max_neuron_indx]) max_neuron_indx = i;

    return max_neuron_indx;
}

int get_digit(const double *img, double *neurons)
{
    // Data processing vrstva
    load_neurons(img, neurons);

    // Soft max
    softmax(neurons);

    // Findmax
    return findmax(neurons);
}

void print_success_rate(const int K, const double *img, const int *numbers, double *neurons)
{
    int numbers_of_guesses = 0;
    int guesses_numbers[K];

    for (int i = 0; i < K; i++)
    {
        double sub_img[IMAGE_SIZE];
        for (int j = 0; j < IMAGE_SIZE; j++)
            sub_img[j] = img[i * IMAGE_SIZE + j];

        const int num = get_digit(sub_img, neurons);
        if (num == numbers[i])
            numbers_of_guesses++;
        guesses_numbers[i] = num;
    }

    printf("%.2lf ", (double)numbers_of_guesses/K*100.0);
    for (int i = 0; i < K; i++)
        if (i!=K-1) printf("%d-%d-%d ", i, numbers[i], guesses_numbers[i]);
        else printf("%d-%d-%d", i, numbers[i], guesses_numbers[i]);
    printf("\n");
}
