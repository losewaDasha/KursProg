#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <locale.h>


double Q1(double x) {
    if (x < 0) {
        return 6.0 * sqrt(x + 1.0) + 5.0;
    }
    else if (x >= 0.0 && x <= 8.0) {
        return 2.0 * (x - 4.0) + 50.0;
    }
    else {
        return 0.25 * pow((x - 8.0), 2.0) + 55.0;
    }
}

int main() {

    setlocale(LC_ALL, "RUS");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    double test_x = 4.00;
    double q1 = 50.00;
    double q2 = Q1(test_x);

    printf("===     Мониторинг реактора     ===\n\n");
    printf("---      Контрольный пример     ---\n");
    printf("Входное значение X:      %.2f %%\n", test_x);
    printf("Ожидаемое значение Q:    %.2f\n", q1);
    printf("Фактически получено Q:   %.2f\n",q2);

    if (fabs(q2 - q1) < 0.0001) {
        printf("Статус проверки:         ТЕСТ ПРОЙДЕН УСПЕШНО\n");
    }
    else {
        printf("Статус проверки:         ОШИБКА! Значения не совпадают.\n");
    }

    return 0;
}