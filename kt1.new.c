#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <locale.h>

#define X_MIN 0.0
#define X_MAX 30.0
#define X_MIN_VALID -1.0

// Границы нормы для Q (50.00 +- 10.00)
#define Q_NORM_MIN 40.0 
#define Q_NORM_MAX 60.0 
#define Q_BASE 50.0

// Информативные имена коэффициентов формулы
#define COEF_SQRT 6.0
#define OFFSET_SQRT 5.0
#define COEF_LINEAR 2.0
#define SHIFT_LINEAR 4.0
#define OFFSET_LINEAR 50.0
#define COEF_SQR 0.25
#define SHIFT_SQR 8.0
#define OFFSET_SQR 55.0

int read_x(double* x);
double compute_q(double x);
int check_state(double q);
double compute_mean(double* data, int size);
double compute_std(double* data, int size);

int main(void) {
    setlocale(LC_ALL, "Russian");

    printf("Добро пожаловать в систему мониторинга содержания примесей!\n");
    printf("Программа рассчитывает показатель качества Q(X) и контролирует состояние процесса.\n\n");

    double x = 0.0;

    printf("Введите значение X (от -1.0 до 30.0): ");

    if (!read_x(&x)) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        return 1; // Ошибка чтения
    }

    // Проверка на допустимость (корень из отрицательного числа)
    if (x < X_MIN_VALID) {
        printf("\nОшибка: значение X = %.2f выходит за допустимые пределы (X >= %.1f).\n", x, X_MIN_VALID);
        return 1;
    }

    // Расчет Q
    double q = compute_q(x);
    printf("\nВходное значение X: %.2f %%\n", x);
    printf("Рассчитанное значение Q: %.2f\n", q);

    // Контроль состояния
    int state = check_state(q);

    if (state == 0) {
        printf("Состояние: НОРМА (штатный режим).\n");
    }
    else {
        printf("Состояние: ОТКЛОНЕНИЕ (требуется проверка).\n");
    }

    // Заглушки для статистики
    printf("\nБлок статистики (в разработке):\n");
    printf("Среднее значение: %.2f\n", compute_mean(NULL, 0));
    printf("Стандартное отклонение: %.2f\n", compute_std(NULL, 0));

    return 0;
}

// Функция чтения X. Возвращает 1 при успехе, 0 при ошибке.
int read_x(double* x) {
    if (scanf("%lf", x) != 1) {
        printf("Ошибка: некорректный ввод. Ожидалось число.\n");
        return 0; 
    }
    return 1; 
}

// Функция расчета Q
double compute_q(double x) {
    if (x < 0.0) {
        return COEF_SQRT * sqrt(x + 1.0) + OFFSET_SQRT;
    }
    else if (x >= 0.0 && x <= 8.0) {
        return COEF_LINEAR * (x - SHIFT_LINEAR) + OFFSET_LINEAR;
    }
    else {
        return COEF_SQR * pow(x - SHIFT_SQR, 2.0) + OFFSET_SQR;
    }
}

// Функция контроля (0 - норма, 1 - отклонение)
int check_state(double q) {
    if (q >= Q_NORM_MIN && q <= Q_NORM_MAX) {
        return 0;
    }
    return 1;
}

// Заглушки 
double compute_mean(double* data, int size) {
    // Реализовать в КТ2
    return 0.0;
}

double compute_std(double* data, int size) {
    // Реализовать в КТ2
    return 0.0;
}