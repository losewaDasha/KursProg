#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>
#include <string.h>

#define X_MIN 0.0
#define X_MAX 8.0
#define Q_NORM_MIN_DEFAULT 42.0
#define Q_NORM_MAX_DEFAULT 58.0
#define X_MIN_VALID -1.0

#define A 6.0
#define B 5.0
#define C 2.0
#define D 4.0
#define E 50.0
#define F 0.25
#define G 8.0
#define H 55.0

#define MAX_RECORDS 10000
#define MAX_WINDOW 100

double g_q_min = Q_NORM_MIN_DEFAULT;
double g_q_max = Q_NORM_MAX_DEFAULT;
int g_window_size = 3;

double compute_q(double x);
int check_state(double q);
void print_state(int state, double x, double q, double avg_q);
void print_help(const char* prog_name);

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "RUS");

    char filename[256] = { 0 };

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_help(argv[0]);
            return 0;
        }
        else if (strcmp(argv[i], "-min") == 0 && i + 1 < argc) {
            g_q_min = atof(argv[++i]);
        }
        else if (strcmp(argv[i], "-max") == 0 && i + 1 < argc) {
            g_q_max = atof(argv[++i]);
        }
        else if (strcmp(argv[i], "-w") == 0 && i + 1 < argc) {
            g_window_size = atoi(argv[++i]);
            if (g_window_size <= 0 || g_window_size > MAX_WINDOW) {
                printf("Ошибка: Размер окна должен быть от 1 до %d. Установлено значение по умолчанию (3).\n", MAX_WINDOW);
                g_window_size = 3;
            }
        }
        else if (filename[0] == '\0') {
            strncpy(filename, argv[i], sizeof(filename) - 1);
        }
    }

    printf("=== Система мониторинга содержания примесей ===\n");
    printf("Настройки: Норма Q = [%.1f, %.1f], Окно сглаживания = %d\n\n", g_q_min, g_q_max, g_window_size);

    if (filename[0] == '\0') {
        printf("Введите имя файла с данными (например, data.txt): ");
        if (scanf("%255s", filename) != 1) {
            printf("Ошибка чтения имени файла.\n");
            return 1;
        }
    }

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Ошибка: Не удалось открыть файл '%s'. Проверьте, существует ли он.\n", filename);
        return 1;
    }

    char line[256];
    int record_count = 0;
    int valid_count = 0;

    double fifo_q[MAX_WINDOW] = { 0 };
    int fifo_count = 0;
    int fifo_index = 0;
    double sum_q = 0.0;

    printf("--- Начало потоковой обработки файла: %s ---\n", filename);

    while (fgets(line, sizeof(line), file)) {
        record_count++;
        double x;

        if (sscanf(line, "%lf", &x) != 1) {
            printf("[Запись %d] Ошибка формата данных (не число). Пропуск.\n", record_count);
            continue;
        }

        if (x < X_MIN_VALID) {
            printf("[Запись %d] Ошибка данных: X = %.2f (меньше допустимого %.1f). Пропуск.\n",
                record_count, x, X_MIN_VALID);
            continue;
        }

        double q = compute_q(x);
        int state = check_state(q);

        sum_q -= fifo_q[fifo_index];
        fifo_q[fifo_index] = q;
        sum_q += fifo_q[fifo_index];
        fifo_index = (fifo_index + 1) % g_window_size;

        if (fifo_count < g_window_size) {
            fifo_count++;
        }
        double avg_q = sum_q / fifo_count;

        print_state(state, x, q, avg_q);

        valid_count++;
    }

    if (ferror(file)) {
        printf("\nВнимание: Произошла ошибка при чтении файла.\n");
    }

    if (record_count == 0) {
        printf("Файл пуст или не содержит данных.\n");
    }
    else {
        printf("\n--- Обработка завершена ---\n");
        printf("Всего прочитано строк: %d\n", record_count);
        printf("Успешно обработано (валидных): %d\n", valid_count);
    }

    fclose(file);
    return 0;
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
    if (q >= g_q_min && q <= g_q_max) {
        return 0;
    }
    else {
        return 1;
    }
}

void print_state(int state, double x, double q, double avg_q) {
    printf("[X = %6.2f] Q = %6.2f | FIFO-ср. Q = %6.2f | Состояние: ", x, q, avg_q);
    if (state == 0) {
        printf("НОРМА\n");
    }
    else {
        printf("ОТКЛОНЕНИЕ (Авария)\n");
    }
}

void print_help(const char* prog_name) {
    printf("Использование: %s [файл] [опции]\n", prog_name);
    printf("Опции:\n");
    printf("  -min <val>   Минимальная норма Q (по умолчанию 42.0)\n");
    printf("  -max <val>   Максимальная норма Q (по умолчанию 58.0)\n");
    printf("  -w <size>    Размер FIFO-окна для сглаживания (по умолчанию 3)\n");
    printf("  -h           Показать эту справку\n");
}