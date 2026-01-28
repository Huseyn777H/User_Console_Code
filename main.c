#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define FILE_NAME "users.txt"

typedef struct
{
    int id;
    char name[50];
    char surname[50];
    int age;
} User;

void showMenu();
void addUser();
void showUsers();
void updateUser();
void deleteUser();
void createFileIfNotExists();

void formatName(char *str);
int isOnlyLetters(const char *s);

void readName(const char *msg, char *buf, int size);
void readOptionalString(const char *msg, char *buf, int size);
int readInt(const char *msg);

int getNextID();

int main()
{
    createFileIfNotExists();

    while (1)
        showMenu();

    return 0;
}

void createFileIfNotExists()
{
    FILE *file = fopen(FILE_NAME, "r");
    if (!file)
    {
        file = fopen(FILE_NAME, "w");
        if (!file)
        {
            printf("File creation error!\n");
            exit(1);
        }
        fprintf(file, "ID,Name,Surname,Age\n");
    }
    fclose(file);
}

void showMenu()
{
    printf("\n==============================\n");
    printf("     USER MANAGEMENT SYSTEM\n");
    printf("==============================\n");
    printf("[1] Add user\n");
    printf("[2] Show users\n");
    printf("[3] Update user (by ID)\n");
    printf("[4] Delete user (by ID)\n");
    printf("[0] Exit\n");

    int choice = readInt("Select option: ");

    switch (choice)
    {
    case 1: addUser(); break;
    case 2: showUsers(); break;
    case 3: updateUser(); break;
    case 4: deleteUser(); break;
    case 0:
        printf("Program finished.\n");
        exit(0);
    default:
        printf("Invalid option!\n");
    }
}

void addUser()
{
    FILE *file = fopen(FILE_NAME, "a");
    if (!file)
    {
        printf("File error!\n");
        return;
    }

    User u;
    u.id = getNextID();

    readName("Name: ", u.name, sizeof(u.name));
    formatName(u.name);

    readName("Surname: ", u.surname, sizeof(u.surname));
    formatName(u.surname);

    u.age = readInt("Age: ");

    fprintf(file, "%d,%s,%s,%d\n",
            u.id, u.name, u.surname, u.age);

    fclose(file);

    printf("User added successfully! (ID: %d)\n", u.id);
}

void showUsers()
{
    FILE *file = fopen(FILE_NAME, "r");
    if (!file)
    {
        printf("File error!\n");
        return;
    }

    char line[200];
    User u;

    printf("\nID   Name        Surname        Age\n");
    printf("-----------------------------------\n");

    fgets(line, sizeof(line), file); // skip header

    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%d,%[^,],%[^,],%d",
               &u.id, u.name, u.surname, &u.age);

        printf("%-4d %-11s %-14s %d\n",
               u.id, u.name, u.surname, u.age);
    }

    fclose(file);
}

void updateUser()
{
    int searchID = readInt("Enter ID to update: ");

    FILE *file = fopen(FILE_NAME, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!file || !temp)
    {
        printf("File error!\n");
        return;
    }

    char line[200];
    User u;
    int found = 0;

    fprintf(temp, "ID,Name,Surname,Age\n");
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%d,%[^,],%[^,],%d",
               &u.id, u.name, u.surname, &u.age);

        if (u.id == searchID)
        {
            char newName[50], newSurname[50];

            printf("Updating user: %s %s (Age: %d)\n",
                   u.name, u.surname, u.age);

            readOptionalString("New name (Enter to keep): ", newName, sizeof(newName));
            readOptionalString("New surname (Enter to keep): ", newSurname, sizeof(newSurname));

            if (strlen(newName) > 0 && isOnlyLetters(newName))
            {
                strcpy(u.name, newName);
                formatName(u.name);
            }

            if (strlen(newSurname) > 0 && isOnlyLetters(newSurname))
            {
                strcpy(u.surname, newSurname);
                formatName(u.surname);
            }

            u.age = readInt("New age: ");
            found = 1;
        }

        fprintf(temp, "%d,%s,%s,%d\n",
                u.id, u.name, u.surname, u.age);
    }

    fclose(file);
    fclose(temp);

    remove(FILE_NAME);
    rename("temp.txt", FILE_NAME);

    printf(found ? "User updated successfully!\n"
                 : "User not found!\n");
}

void deleteUser()
{
    int searchID = readInt("Enter ID to delete: ");

    FILE *file = fopen(FILE_NAME, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!file || !temp)
    {
        printf("File error!\n");
        return;
    }

    char line[200];
    User u;
    int found = 0;

    fprintf(temp, "ID,Name,Surname,Age\n");
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%d,%[^,],%[^,],%d",
               &u.id, u.name, u.surname, &u.age);

        if (u.id == searchID)
        {
            printf("Deleting user: %s %s (Age: %d)\n",
                   u.name, u.surname, u.age);
            found = 1;
            continue;
        }

        fprintf(temp, "%d,%s,%s,%d\n",
                u.id, u.name, u.surname, u.age);
    }

    fclose(file);
    fclose(temp);

    remove(FILE_NAME);
    rename("temp.txt", FILE_NAME);

    printf(found ? "User deleted successfully!\n"
                 : "User not found!\n");
}

void formatName(char *str)
{
    if (!str[0]) return;

    str[0] = toupper(str[0]);
    for (int i = 1; str[i]; i++)
        str[i] = tolower(str[i]);
}

int isOnlyLetters(const char *s)
{
    for (int i = 0; s[i]; i++)
        if (!isalpha((unsigned char)s[i]))
            return 0;
    return 1;
}

void readName(const char *msg, char *buf, int size)
{
    while (1)
    {
        printf("%s", msg);
        fgets(buf, size, stdin);
        buf[strcspn(buf, "\n")] = 0;

        if (strlen(buf) == 0)
        {
            printf("Empty input not allowed!\n");
            continue;
        }

        if (!isOnlyLetters(buf))
        {
            printf("Only letters allowed!\n");
            continue;
        }
        return;
    }
}

void readOptionalString(const char *msg, char *buf, int size)
{
    printf("%s", msg);
    fgets(buf, size, stdin);
    buf[strcspn(buf, "\n")] = 0;
}

int readInt(const char *msg)
{
    char buf[20];
    char *end;
    int value;

    while (1)
    {
        printf("%s", msg);
        fgets(buf, sizeof(buf), stdin);
        value = strtol(buf, &end, 10);

        if (end != buf && *end == '\n' && value > 0)
            return value;

        printf("Invalid number!\n");
    }
}

int getNextID()
{
    FILE *file = fopen(FILE_NAME, "r");
    if (!file)
        return 1;

    char line[200];
    int id = 0;

    fgets(line, sizeof(line), file);
    while (fgets(line, sizeof(line), file))
        sscanf(line, "%d,", &id);

    fclose(file);
    return id + 1;
}
