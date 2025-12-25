#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

#define MAX_ELEMENTS 50
#define MAX_SETS 5
#define FILENAME "sets.txt"

typedef struct {
    int elements[MAX_ELEMENTS];
    int size;
} Set;

void clearBuffer() {
    while (getchar() != '\n');
}

int hasElement(Set* set, int element) {
    for (int i = 0; i < set->size; i++) {
        if (set->elements[i] == element) {
            return 1;
        }
    }
    return 0;
}

void addElement(Set* set, int element) {
    if (set->size < MAX_ELEMENTS && !hasElement(set, element)) {
        set->elements[set->size] = element;
        set->size++;
    }
}

void printSet(Set* set) {
    printf("{");
    for (int i = 0; i < set->size; i++) {
        printf("%d", set->elements[i]);
        if (i < set->size - 1) printf(", ");
    }
    printf("}\n");
}

void inputSet(Set* set) {
    set->size = 0;
    int count;

    printf("Сколько элементов добавить? (макс %d): ", MAX_ELEMENTS);
    scanf("%d", &count);
    clearBuffer();

    if (count > MAX_ELEMENTS) count = MAX_ELEMENTS;

    printf("Введите %d элементов:\n", count);
    for (int i = 0; i < count; i++) {
        int element;
        scanf("%d", &element);
        addElement(set, element);
    }
    clearBuffer();
}

void randomSet(Set* set) {
    set->size = 0;
    int count;

    printf("Сколько случайных элементов? (макс %d): ", MAX_ELEMENTS);
    scanf("%d", &count);
    clearBuffer();

    if (count > MAX_ELEMENTS) count = MAX_ELEMENTS;

    printf("Диапазон чисел (от и до): ");
    int min, max;
    scanf("%d %d", &min, &max);
    clearBuffer();

    for (int i = 0; i < count; i++) {
        int num = min + rand() % (max - min + 1);
        addElement(set, num);
    }

    printf("Создано множество: ");
    printSet(set);
}
Set intersectMultipleSets(Set sets[], int indices[], int count) {
    Set result;
    result.size = 0;

    if (count < 2) {
        if (count == 1) {
            return sets[indices[0]];
        }
        return result;
    }

    Set temp;
    temp.size = 0;

    for (int i = 0; i < sets[indices[0]].size; i++) {
        addElement(&temp, sets[indices[0]].elements[i]);
    }

    for (int i = 1; i < count; i++) {
        Set newTemp;
        newTemp.size = 0;

        for (int j = 0; j < temp.size; j++) {
            if (hasElement(&sets[indices[i]], temp.elements[j])) {
                addElement(&newTemp, temp.elements[j]);
            }
        }

        temp = newTemp;
    }

    return temp;
}

void saveToFile(Set sets[], int count, Set result) {
    FILE* file = fopen(FILENAME, "w");
    if (!file) {
        printf("Ошибка сохранения!\n");
        return;
    }

    fprintf(file, "Множеств: %d\n", count);
    for (int i = 0; i < count; i++) {
        fprintf(file, "Множество %d: ", i + 1);
        for (int j = 0; j < sets[i].size; j++) {
            fprintf(file, "%d ", sets[i].elements[j]);
        }
        fprintf(file, "\n");
    }

    fprintf(file, "Пересечение: ");
    for (int i = 0; i < result.size; i++) {
        fprintf(file, "%d ", result.elements[i]);
    }

    fclose(file);
    printf("Сохраненов %s\n", FILENAME);
}

int loadFromFile(Set sets[], int* count, Set* result) {
    FILE* file = fopen(FILENAME, "r");
    if (!file) return 0;

    fscanf(file, "Множеств: %d\n", count);
    for (int i = 0; i < *count; i++) {
        sets[i].size = 0;
        fscanf(file, "Множество %*d: ");

        int element;
        while (fscanf(file, "%d", &element) == 1) {
            char c = fgetc(file);
            sets[i].elements[sets[i].size] = element;
            sets[i].size++;
            if (c == '\n') break;
        }
    }

    result->size = 0;
    fscanf(file, "Пересечение: ");
    int element;
    while (fscanf(file, "%d", &element) == 1) {
        result->elements[result->size] = element;
        result->size++;
    }

    fclose(file);
    return 1;
}

void viewFile() {
    FILE* file = fopen(FILENAME, "r");
    if (!file) {
        printf("Файл %s не найден!\n", FILENAME);
        return;
    }

    printf("\n СОДЕРЖИМОЕ ФАЙЛА %s \n", FILENAME);
    printf("-\n");

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }

    printf("\n");
    fclose(file);
}

void printTitlePage() {
    printf("\n");
    printf("Курсовая работа по дисциплине\n");
    printf("\"Логика и основы алгоритмизации в инженерия задач\"\n");
    printf("на тему \"Реализация нахождения пересечения двух и более множеств\"\n");
    printf("\n");
    printf("Выполнил студент группы 248BB1: Выдрин Дмитрий\n");
    printf("Приняла: Юрова О.В.\n");
    printf("\n");
}

int main() {
    Set sets[MAX_SETS];
    Set result;
    int setCount = 0;

    setlocale(LC_ALL, "RUSSIAN");

    srand(time(NULL));

    printTitlePage();

    printf("Нажмите Enter");
    clearBuffer();

    while (1) {
        printf("\n         МЕНЮ \n");
        printf("1. Создать множества\n");
        printf("2. Найти пересечение\n");
        printf("3. Показать все множества\n");
        printf("4. Сохранить в файл\n");
        printf("5. Загрузить из файла\n");
        printf("6. Просмотреть содержимое файла\n");
        printf("7. Очистить все\n");
        printf("0. Выход\n");
        printf("Выбор: ");

        int choice;
        scanf("%d", &choice);
        clearBuffer();

        switch (choice) {
        case 0:
            printf("Выход\n");
            return 0;

        case 1: {
            printf("\nСколько множеств создать? (макс %d): ", MAX_SETS);
            scanf("%d", &setCount);
            clearBuffer();

            if (setCount > MAX_SETS) setCount = MAX_SETS;

            for (int i = 0; i < setCount; i++) {
                printf("\nМножество %d:\n", i + 1);
                printf("1. Ввести вручную\n");
                printf("2. Случайные числа\n");
                printf("Выбор: ");

                int method;
                scanf("%d", &method);
                clearBuffer();

                sets[i].size = 0;

                if (method == 1) {
                    inputSet(&sets[i]);
                }
                else {
                    randomSet(&sets[i]);
                }
            }
            break;
        }

        case 2:
            if (setCount < 2) {
                printf("Нужно хотя бы 2 множества!\n");
                break;
            }

            printf("\nПересечение скольких множеств найти? (от 2 до %d): ", setCount);
            int intersectCount;
            scanf("%d", &intersectCount);
            clearBuffer();

            if (intersectCount < 2 || intersectCount > setCount) {
                printf("Неверное количество!\n");
                break;
            }

            int indices[MAX_SETS];
            printf("Введите номера %d множеств через пробел: ", intersectCount);
            for (int i = 0; i < intersectCount; i++) {
                scanf("%d", &indices[i]);
                indices[i]--;

                if (indices[i] < 0 || indices[i] >= setCount) {
                    printf("Неверный номер множества!\n");
                    clearBuffer();
                    intersectCount = -1;
                    break;
                }
            }
            clearBuffer();

            if (intersectCount == -1) break;

            result = intersectMultipleSets(sets, indices, intersectCount);

            printf("\nВыбранные множества:\n");
            for (int i = 0; i < intersectCount; i++) {
                printf("Множество %d: ", indices[i] + 1);
                printSet(&sets[indices[i]]);
            }
            printf("Пересечение: ");
            printSet(&result);
            break;

        case 3:
            if (setCount == 0) {
                printf("Множеств нет!\n");
            }
            else {
                printf("\nВсемножества:\n");
                for (int i = 0; i < setCount; i++) {
                    printf("Множество %d: ", i + 1);
                    printSet(&sets[i]);
                }
                if (result.size > 0) {
                    printf("Последнее пересечение: ");
                    printSet(&result);
                }
            }
            break;

        case 4:
            if (setCount > 0) {
                saveToFile(sets, setCount, result);
            }
            else {
                printf("Нет данных для сохранения!\n");
            }
            break;

        case 5:
            if (loadFromFile(sets, &setCount, &result)) {
                printf("Загружено %d множеств\n", setCount);
            }
            else {
                printf("Файл не найден!\n");
            }
            break;

        case 6:
            viewFile();
            break;

        case 7:
            setCount = 0;
            result.size = 0;
            printf("Все очищено!\n");
            break;

        default:
            printf("Неверный выбор!\n");
        }

        printf("\nНажмите Enter");
        clearBuffer();
    }

    return 0;
}

