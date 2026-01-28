#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define FILE_NAME "users.txt"

typedef struct {
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

void formatName(char *s);
int isOnlyLetters(const char *s);

void readName(const char *msg, char *buf, int size);
void readOptionalName(const char *msg, char *buf, int size);
int readInt(const char *msg);
int readOptionalAge(const char *msg, int oldAge);

int getNextID();

int main() {
    createFileIfNotExists();
    while (1) showMenu();
}

void createFileIfNotExists() {
    FILE *f = fopen(FILE_NAME, "r");
    if (!f) {
        f = fopen(FILE_NAME, "w");
        fprintf(f, "ID,Name,Surname,Age\n");
    }
    fclose(f);
}

void showMenu() {
    printf("\n1. Add User\n2. Show Users\n3. Update User\n4. Delete User\n0. Exit\n");
    int c = readInt("Choice: ");

    if (c == 1) addUser();
    else if (c == 2) showUsers();
    else if (c == 3) updateUser();
    else if (c == 4) deleteUser();
    else if (c == 0) exit(0);
}

void addUser() {
    FILE *f = fopen(FILE_NAME, "a");
    User u;

    u.id = getNextID();
    readName("Name: ", u.name, 50);
    readName("Surname: ", u.surname, 50);
    u.age = readInt("Age: ");

    fprintf(f, "%d,%s,%s,%d\n", u.id, u.name, u.surname, u.age);
    fclose(f);

    printf("User added (ID=%d)\n", u.id);
}

void showUsers() {
    FILE *f = fopen(FILE_NAME, "r");
    char line[200];
    User u;

    fgets(line, sizeof(line), f);
    while (fgets(line, sizeof(line), f)) {
        sscanf(line, "%d,%[^,],%[^,],%d",
               &u.id, u.name, u.surname, &u.age);
        printf("%d %s %s %d\n", u.id, u.name, u.surname, u.age);
    }
    fclose(f);
}

void updateUser() {
    int id = readInt("Enter ID: ");
    FILE *f = fopen(FILE_NAME, "r");
    FILE *t = fopen("temp.txt", "w");

    char line[200];
    User u;
    int found = 0;

    fprintf(t, "ID,Name,Surname,Age\n");
    fgets(line, sizeof(line), f);

    while (fgets(line, sizeof(line), f)) {
        sscanf(line, "%d,%[^,],%[^,],%d",
               &u.id, u.name, u.surname, &u.age);

        if (u.id == id) {
            printf("Updating: %s %s (%d)\n", u.name, u.surname, u.age);

            readOptionalName("New name (Enter=keep): ", u.name, 50);
            readOptionalName("New surname (Enter=keep): ", u.surname, 50);
            u.age = readOptionalAge("New age (Enter=keep): ", u.age);

            found = 1;
        }
        fprintf(t, "%d,%s,%s,%d\n", u.id, u.name, u.surname, u.age);
    }

    fclose(f);
    fclose(t);
    remove(FILE_NAME);
    rename("temp.txt", FILE_NAME);

    printf(found ? "Updated!\n" : "User not found!\n");
}

void deleteUser() {
    int id = readInt("Enter ID: ");
    FILE *f = fopen(FILE_NAME, "r");
    FILE *t = fopen("temp.txt", "w");

    char line[200];
    User u;
    int found = 0;

    fprintf(t, "ID,Name,Surname,Age\n");
    fgets(line, sizeof(line), f);

    while (fgets(line, sizeof(line), f)) {
        sscanf(line, "%d,%[^,],%[^,],%d",
               &u.id, u.name, u.surname, &u.age);

        if (u.id == id) {
            found = 1;
            continue;
        }
        fprintf(t, "%d,%s,%s,%d\n", u.id, u.name, u.surname, u.age);
    }

    fclose(f);
    fclose(t);
    remove(FILE_NAME);
    rename("temp.txt", FILE_NAME);

    printf(found ? "Deleted!\n" : "User not found!\n");
}

void formatName(char *s) {
    s[0] = toupper(s[0]);
    for (int i = 1; s[i]; i++) s[i] = tolower(s[i]);
}

int isOnlyLetters(const char *s) {
    for (int i = 0; s[i]; i++)
        if (!isalpha((unsigned char)s[i])) return 0;
    return 1;
}

void readName(const char *msg, char *buf, int size) {
    while (1) {
        printf("%s", msg);
        fgets(buf, size, stdin);
        buf[strcspn(buf, "\n")] = 0;
        if (strlen(buf) && isOnlyLetters(buf)) {
            formatName(buf);
            return;
        }
        printf("Only letters allowed!\n");
    }
}

void readOptionalName(const char *msg, char *buf, int size) {
    char temp[50];
    while (1) {
        printf("%s", msg);
        fgets(temp, size, stdin);
        temp[strcspn(temp, "\n")] = 0;

        if (strlen(temp) == 0) return;
        if (isOnlyLetters(temp)) {
            strcpy(buf, temp);
            formatName(buf);
            return;
        }
        printf("Only letters allowed!\n");
    }
}

int readOptionalAge(const char *msg, int oldAge) {
    char buf[20];
    printf("%s", msg);
    fgets(buf, sizeof(buf), stdin);
    if (buf[0] == '\n') return oldAge;
    int age = atoi(buf);
    return age > 0 ? age : oldAge;
}

int readInt(const char *msg) {
    char buf[20];
    int n;
    while (1) {
        printf("%s", msg);
        fgets(buf, sizeof(buf), stdin);
        if (sscanf(buf, "%d", &n) == 1 && n > 0) return n;
        printf("Invalid number!\n");
    }
}

int getNextID() {
    FILE *f = fopen(FILE_NAME, "r");
    char line[200];
    int id = 0;

    fgets(line, sizeof(line), f);
    while (fgets(line, sizeof(line), f))
        sscanf(line, "%d,", &id);

    fclose(f);
    return id + 1;
}
