#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define LEN_G  8
#define LEN_E 38

// Нахождение CRC
void calc_CRC(int* G, int len_data, int* exten_data, int* result) {
    int i, j, len = LEN_G - 1 + len_data;
    int* temp = (int*)malloc(len * sizeof(int));

    for (i = 0; i < len; i++) {
        temp[i] = exten_data[i];
    }

    for (i = 0; i < len_data; i++) {
        if (temp[i] == 1) {
            for (j = 0; j < LEN_G; j++) {
                temp[j + i] = temp[j + i] ^ G[j];
            }
        }
    }

    for (i = 0; i < LEN_G - 1; i++) {
        result[i] = temp[((LEN_G - 1 + len_data) - LEN_G) + (i + 1)];
    }

    free(temp);
}

// Рандомное заполнение данных
void randomizer_data(int* result, int len_data, int* exten_result, int exten_len_data) {
    int i;

    for (i = 0; i < len_data; i++) {
        result[i] = rand() % 2;
        exten_result[i] = result[i];
    }

    for (i = len_data; i < exten_len_data; i++) {
        exten_result[i] = 0;
    }
}

// Вывод содержимого массива в консоль
void print_data(int* data, int len_data) {
    printf("\n[");
    for (int i = 0; i < len_data; i++) {
        printf(" %d", data[i]);
    }
    printf(" ]\n");
}

// Проверка на наличие ошибок
void check_CRC_on_err(int* CRC, int* result) {
    int i, check = 0;
    for (i = 0; i < LEN_G - 1; i++) {
        if (CRC[i] != 0) {
            check = 1;
        }
    }

    *result = check;
}

int main() {
    srand(time(NULL));

    int i, error;
    int N = 20 + 11;
    int exten_N = N + LEN_G - 1;
    int G[] = { 1, 0, 1, 1, 1, 0, 1, 1 };

    int* data = (int*)malloc(N * sizeof(int));
    int* exten_data = (int*)malloc(exten_N * sizeof(int));
    int* CRC = (int*)malloc((LEN_G - 1) * sizeof(int));

    randomizer_data(data, N, exten_data, exten_N);
    printf("\nG");
    print_data(G, LEN_G);
    printf("\nData N=%d", N);
    print_data(data, N);
    printf("\nextenData");
    print_data(exten_data, exten_N);

    calc_CRC(G, N, exten_data, CRC);
    printf("\nCRC");
    print_data(CRC, LEN_G - 1);

    for (i = 0; i < LEN_G - 1; i++) {
        exten_data[N + i] = CRC[i];
    }

    printf("\nData with CRC");
    print_data(exten_data, exten_N);

    calc_CRC(G, N, exten_data, CRC);
    printf("\nnew CRC");
    print_data(CRC, LEN_G - 1);

    check_CRC_on_err(CRC, &error);
    if (error == 1) {
        printf("\nErrors found\n");
    }
    else {
        printf("\nAll good\n");
    }

    free(data);
    free(exten_data);

    // Пункт 4
    N = 250;
    exten_N = N + LEN_G - 1;
    data = (int*)malloc(N * sizeof(int));
    exten_data = (int*)malloc(exten_N * sizeof(int));

    randomizer_data(data, N, exten_data, exten_N);
    printf("\nG");
    print_data(G, LEN_G);
    printf("\nData N=%d", N);
    print_data(data, N);
    printf("\nextenData");
    print_data(exten_data, exten_N);

    calc_CRC(G, N, exten_data, CRC);
    printf("\nCRC");
    print_data(CRC, LEN_G - 1);

    for (i = 0; i < LEN_G - 1; i++) {
        exten_data[N + i] = CRC[i];
    }

    printf("\nData with CRC");
    print_data(exten_data, exten_N);

    calc_CRC(G, N, exten_data, CRC);
    printf("\nnew CRC");
    print_data(CRC, LEN_G - 1);

    check_CRC_on_err(CRC, &error);
    if (error == 1) {
        printf("\nErrors found\n");
    }
    else {
        printf("\nAll good\n");
    }

    int count_fail = 0, count_good = 0;
    for (i = 0; i < N + LEN_G - 1; i++) {
        exten_data[i] = exten_data[i] ^ 1;
        calc_CRC(G, N, exten_data, CRC);
        check_CRC_on_err(CRC, &error);
        if (error == 1) {
            count_fail++;
        }
        else {
            count_good++;
        }
    }

    printf("\nErrors = %d \nGood = %d ", count_fail, count_good);

    free(data);
    free(exten_data);
    free(CRC);

    return 0;
}
