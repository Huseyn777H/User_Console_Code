#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define FILE_NAME "users.txt"
void showMenu();
void addUser();
void showUsers();
void updateUser();
void deleteUser();
void createFileIfNotExists();
void formatName(char *str);
int getValidAge();


int main()
{
    createFileIfNotExists();
    while (1)
    {
        showMenu();
    }
}


void createFileIfNotExists()
{
    FILE *file = fopen(FILE_NAME, "r");

    if (file == NULL)
    {
        file = fopen(FILE_NAME, "w");
        if (!file)
        {
            printf("File creation error!\n");
            return;
        }
        fprintf(file, "Name,Surname,Age\n");
    }

    fclose(file);
}


void showMenu()
{
    int choice;

    printf("\n--- USER MANAGEMENT ---\n");
    printf("1. Add User\n");
    printf("2. Show Users\n");
    printf("3. Update User\n");
    printf("4. Delete User\n");
    printf("5. Quit\n");

    if (scanf("%d", &choice) != 1)
    {
        printf("Invalid input!\n");
        while (getchar() != '\n')
            ;
        return;
    }
    getchar();

    switch (choice)
    {
    case 1:
        addUser();
        break;
    case 2:
        showUsers();
        break;
    case 3:
        updateUser();
        break;
    case 4:
        deleteUser();
        break;
    case 5:
        printf("Program finished.\n");
        exit(0);
    default:
        printf("Wrong choice!\n");
    }
}


void addUser()
{
    char name[50], surname[50], answer;

    FILE *file = fopen(FILE_NAME, "a");
    if (!file)
    {
        printf("File error!\n");
        return;
    }

    printf("Name: ");
    scanf("%s", name);
    formatName(name);

    printf("Surname: ");
    scanf("%s", surname);
    formatName(surname);

    int age = getValidAge();

    fprintf(file, "%s,%s,%d\n", name, surname, age);
    fclose(file);

    printf("Continue adding user? (y/n): ");
    scanf(" %c", &answer);
    getchar();

    if (tolower(answer) == 'y')
        addUser();
}


void showUsers()
{
    FILE *file = fopen(FILE_NAME, "r");
    if (!file)
    {
        printf("File error!\n");
        return;
    }
    char line[150];

    printf("\n--- USERS LIST ---\n");
    printf("Name       Surname       Age\n");
    printf("-----------------------------\n");

    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file))
    {
        char name[50], surname[50];
        int age;

        sscanf(line, "%[^,],%[^,],%d", name, surname, &age);
        printf("%-10s %-13s %d\n", name, surname, age);
    }

    fclose(file);
}


void updateUser()
{
    char name[50], surname[50];
    char line[150];
    int found = 0;

    FILE *file = fopen(FILE_NAME, "r");
    if (!file)
    {
        printf("File error!\n");
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (!temp)
    {
        printf("Temp file error!\n");
        fclose(file);
        return;
    }

    fprintf(temp, "Name,Surname,Age\n");

    printf("Enter name to update: ");
    scanf("%s", name);
    printf("Enter surname to update: ");
    scanf("%s", surname);

    formatName(name);
    formatName(surname);

    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file))
    {
        char n[50], s[50];
        int age;

        sscanf(line, "%[^,],%[^,],%d", n, s, &age);

        if (strcmp(n, name) == 0 && strcmp(s, surname) == 0)
        {
            printf("Enter new age:\n");
            age = getValidAge();
            found = 1;
        }
        fprintf(temp, "%s,%s,%d\n", n, s, age);
    }

    fclose(file);
    fclose(temp);

    remove(FILE_NAME);
    rename("temp.txt", FILE_NAME);

    if (found)
        printf("User updated successfully.\n");
    else
        printf("User not found.\n");
}


void deleteUser()
{
    char name[50], surname[50];
    char line[150];
    int found = 0;

    FILE *file = fopen(FILE_NAME, "r");
    if (!file)
    {
        printf("File error!\n");
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (!temp)
    {
        printf("Temp file error!\n");
        fclose(file);
        return;
    }

    fprintf(temp, "Name,Surname,Age\n");

    printf("Enter name to delete: ");
    scanf("%s", name);
    printf("Enter surname to delete: ");
    scanf("%s", surname);

    formatName(name);
    formatName(surname);

    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file))
    {
        char n[50], s[50];
        int age;

        sscanf(line, "%[^,],%[^,],%d", n, s, &age);

        if (strcmp(n, name) == 0 && strcmp(s, surname) == 0)
        {
            found = 1;
            continue;
        }
        fprintf(temp, "%s,%s,%d\n", n, s, age);
    }

    fclose(file);
    fclose(temp);

    remove(FILE_NAME);
    rename("temp.txt", FILE_NAME);

    if (found)
        printf("User deleted successfully.\n");
    else
        printf("User not found.\n");
}


void formatName(char *str)
{
    if (str[0] == '\0')
        return;

    str[0] = toupper(str[0]);
    for (int i = 1; str[i]; i++)
    {
        str[i] = tolower(str[i]);
    }
}


int getValidAge()
{
    char buffer[20];
    char *endptr;
    int age;

    while (1)
    {
        printf("Age: ");
        fgets(buffer, sizeof(buffer), stdin);

        age = strtol(buffer, &endptr, 10);

        if (endptr == buffer || *endptr != '\n' || age <= 0)
        {
            printf("Invalid age! Only positive numbers allowed.\n");
        }
        else
        {
            return age;
        }
    }
}
