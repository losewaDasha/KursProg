#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <locale.h>

#define X_MIN 0.0
#define X_MAX 8.0
#define Q_NORM_MIN 42.0
#define Q_NORM_MAX 58.0
#define X_MIN_VALID -1.0

#define A 6.0
#define B 5.0
#define C 2.0
#define D 4.0
#define E 50.0
#define F 0.25
#define G 8.0
#define H 55.0

double read_x(void);
double compute_q(double x);
int check_state(double q);
void print_state(int state);
double compute_mean(double* data, int size);
double compute_std(double* data, int size);

int main(void) {
    setlocale(LC_ALL, "RUS");

    printf("Добро пожаловать в систему мониторинга содержания примесей!\n");
    printf("Программа рассчитывает показатель качества Q(X) и контролирует состояние процесса.\n\n");

    double x = read_x();

    if (x < X_MIN_VALID) {
        printf("Ошибка: Значение X не может быть меньше %.1f (извлечение корня из отрицательного числа).\n", X_MIN_VALID);
        return 1;
    }

    double q = compute_q(x);
    int state = check_state(q);

    printf("\n--- Результаты ---\n");
    printf("Значение X: %.2f %%\n", x);
    printf("Показатель Q(X): %.2f\n", q);

    print_state(state);

    double test_data[1] = { q };
    printf("\n[Статистика] Среднее (заглушка): %.2f\n", compute_mean(test_data, 1));
    printf("[Статистика] Отклонение (заглушка): %.2f\n", compute_std(test_data, 1));

    return 0;
}

double read_x(void) {
    double x;
    printf("Введите значение содержания примеси X (в %%): ");
    if (scanf("%lf", &x) != 1) {
        printf("Ошибка ввода! Введите число.\n");
        return -999.0;
    }
    return x;
}

double compute_q(double x) {
    if (x < 0.0) {
        return A * sqrt(x + 1.0) + B;
    }
    else if (x >= X_MIN && x <= X_MAX) {
        return C * (x - D) + E;
    }
    else {
        return F * pow(x - G, 2.0) + H;
    }
}

int check_state(double q) {
    if (q >= Q_NORM_MIN && q <= Q_NORM_MAX) {
        return 0;
    }
    else {
        return 1;
    }
}

void print_state(int state) {
    if (state == 0) {
        printf("Состояние: Норма\n");
    }
    else {
        printf("Состояние: Отклонение\n");
    }
}

double compute_mean(double* data, int size) {
    return 0.0;
}

double compute_std(double* data, int size) {
    return 0.0;
}
