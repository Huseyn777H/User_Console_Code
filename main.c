#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define FILE_NAME "users.txt"

void showMenu();
void addUser();
void showUsers();
void createFileIfNotExists();

int main() {
    createFileIfNotExists();
    showMenu();
    return 0;
}

void createFileIfNotExists() {
    FILE *file = fopen(FILE_NAME, "r");

    if (file == NULL) {
        file = fopen(FILE_NAME, "w");
        fprintf(file, "Name,Surname,Age\n");
    }

    fclose(file);
}

void showMenu() {
    int choice;

    printf("\n--- USER MANAGEMENT ---\n");
    printf("1. Add User\n");
    printf("2. Show Users\n");
    printf("3. Quit\n");
    printf("Select option: ");

    scanf("%d", &choice);

    switch (choice) {
        case 1:
            addUser();
            break;
        case 2:
            showUsers();
            break;
        case 3:
            printf("Program finished.\n");
            exit(0);
        default:
            printf("Wrong choice!\n");
            showMenu();
    }
}

void addUser() {
    char name[50];
    char surname[50];
    int age;
    char answer;

    FILE *file = fopen(FILE_NAME, "a");

    printf("Name: ");
    scanf("%s", name);

    printf("Surname: ");
    scanf("%s", surname);

    printf("Age: ");
    scanf("%d", &age);

    fprintf(file, "%s,%s,%d\n", name, surname, age);
    fclose(file);

    printf("Continue adding user? (y/n): ");
    scanf(" %c", &answer);

    if (tolower(answer) == 'y') {
        addUser();
    } else {
        showMenu();
    }
}

void showUsers() {
    FILE *file = fopen(FILE_NAME, "r");
    char line[150];

    printf("\n--- USERS LIST ---\n");
    printf("Name       Surname       Age\n");
    printf("-----------------------------\n");

    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        char name[50], surname[50];
        int age;

        sscanf(line, "%[^,],%[^,],%d", name, surname, &age);
        printf("%-10s %-13s %d\n", name, surname, age);
    }

    fclose(file);
    showMenu();
}
