#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <locale.h>

#define X_MIN 0.0
#define X_MAX 8.0
#define X_MIN_VALID -1.0
#define Q_NORM_MIN 42.0
#define Q_NORM_MAX 58.0

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
    printf("Программа рассчитывает показатель качества Q(X) и контролирует состояние.\n\n");

    double x = read_x();

    // Проверка ограничения (корень из отрицательного числа)
    if (x < X_MIN_VALID) {
        printf("Ошибка: значение X = %.2f выходит за допустимые пределы (минимум %.1f).\n", x, X_MIN_VALID);
        return 1;
    }

    double q = compute_q(x);
    printf("Входное значение X: %.2f %%\n", x);
    printf("Рассчитанное значение Q: %.2f\n", q);

    int current_state = check_state(q);
    print_state(current_state);

    // Заглушки для статистики
    printf("\nБлок статистики (в разработке):\n");
    printf("Среднее: %.2f\n", compute_mean(NULL, 0));
    printf("Отклонение: %.2f\n", compute_std(NULL, 0));

    return 0;
}

// Функция чтения X (заглушка)
double read_x(void) {
    double test_x = 4.00;
    return test_x;
}

// Функция расчета Q 
double compute_q(double x) {
    double result;

    if (x < X_MIN) {
        result = A * sqrt(x + 1.0) + B;
    }
    else if (x >= X_MIN && x <= X_MAX) {
        result = C * (x - D) + E;
    }
    else {
        result = F * pow(x - G, 2.0) + H;
    }

    return result;
}

int check_state(double q) {
    if (q >= Q_NORM_MIN && q <= Q_NORM_MAX) {
        return 0; // Норма
    }
    else {
        return 1; // Отклонение
    }
}

void print_state(int state) {
    if (state == 0) {
        printf("Состояние: НОРМА (штатный режим).\n");
    }
    else if (state == 1) {
        printf("Состояние: ОТКЛОНЕНИЕ (требуется проверка).\n");
    }
    else {
        printf("Состояние: неизвестно.\n");
    }
}

// Заглушки для статистики
double compute_mean(double* data, int size) {
    return 0.0;
}

double compute_std(double* data, int size) {
    return 0.0;
}