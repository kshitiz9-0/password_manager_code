#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LIMIT 50

typedef struct {
    char website[LIMIT];
    char username[LIMIT];
    char password[LIMIT];
} Manager;

typedef struct {
    Manager account;
} Account;

void openFile(Account *users, int *currentSize);
void updateFile(Account *users, int currentSize);
void addAccount(Account *users, int *currentSize);
void updatePassword(Account *users, int currentSize);
int getUserInput(char *prompt, char *input, int limit);
void printAll(Account *users, int currentSize);
void deleteUser(Account *users, int *currentSize);
void freeAll(Account *users);

int main() {
    Account *users = malloc(sizeof(Account) * LIMIT);
    int currentSize = 0;

    openFile(users, &currentSize);

    int choice;

    printf("Welcome to my password manager.\n");

    do {
        printf("\n\t<<<Main Menu>>>\n");
        printf("(1) Add Account\n");
        printf("(2) Print all accounts\n");
        printf("(3) Update Password\n");
        printf("(4) Delete User\n");
        printf("(5) Exit\n");

        printf("Decision: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addAccount(users, &currentSize);
                break;
            case 2:
                printAll(users, currentSize);
                break;
            case 3:
                updatePassword(users, currentSize);
                break;
            case 4:
                deleteUser(users, &currentSize);
                break;
            case 5:
                updateFile(users, currentSize);
                freeAll(users);
                printf("Exiting Menu...\n");
                sleep(1);
                printf("All usernames & passwords will be uploaded to a file 'passwordmanager.txt'.\n");
                break;
            default:
                printf("Incorrect Input\n");
        }
    } while (choice != 5);

    return 0;
}

void openFile(Account *users, int *currentSize) {
    FILE *managerFile = fopen("passwordmanager.txt", "a+");

    while (fscanf(managerFile, "Website: %s Username: %s Password: %s\n",
                  users[*currentSize].account.website,
                  users[*currentSize].account.username,
                  users[*currentSize].account.password) == 3) {
        (*currentSize)++;
    }

    fclose(managerFile);
}

void updateFile(Account *users, int currentSize) {
    FILE *managerFile = fopen("passwordmanager.txt", "w");

    for (int i = 0; i < currentSize; i++) {
        fprintf(managerFile, "Website: %s Username: %s Password: %s\n",
                users[i].account.website, users[i].account.username, users[i].account.password);
    }

    fclose(managerFile);
}

void addAccount(Account *users, int *currentSize) {
    printf("Enter the website name: ");
    getUserInput("", users[*currentSize].account.website, LIMIT);

    printf("Enter the username for %s: ", users[*currentSize].account.website);
    getUserInput("", users[*currentSize].account.username, LIMIT);

    printf("Enter the password for %s: ", users[*currentSize].account.website);
    getUserInput("", users[*currentSize].account.password, LIMIT);

    (*currentSize)++;
}

void updatePassword(Account *users, int currentSize) {
    if (currentSize == 0) {
        printf("No accounts to update...\n");
        return;
    }

    char website[LIMIT];
    printf("Enter the website name for which you want to update the password: ");
    getUserInput("", website, LIMIT);

    int found = 0;
    for (int i = 0; i < currentSize; i++) {
        if (strcmp(users[i].account.website, website) == 0) {
            printf("Enter the new password for %s: ", website);
            getUserInput("", users[i].account.password, LIMIT);
            found = 1;
            printf("Password updated successfully for %s.\n", website);
            break;
        }
    }

    if (!found) {
        printf("Website not found in the list...\n");
    }
}

int getUserInput(char *prompt, char *input, int limit) {
    printf("%s", prompt);
    scanf("%s", input);
    return 1;
}

void printAll(Account *users, int currentSize) {
    if (currentSize == 0) {
        printf("No accounts to display...\n");
        return;
    }

    for (int i = 0; i < currentSize; i++) {
        printf("Account %d\n", i + 1);
        printf("Website: %s\n", users[i].account.website);
        printf("Username: %s\n", users[i].account.username);
        printf("Password: %s\n", users[i].account.password);

        if (i != currentSize - 1) {
            printf("-----------------------------\n");
        }
    }
}

void deleteUser(Account *users, int *currentSize) {
    if (*currentSize == 0) {
        printf("No users to delete...\n");
        return;
    }

    int userDel;
    printf("Enter account number to delete: ");
    scanf("%d", &userDel);

    if (userDel >= 1 && userDel <= *currentSize) {
        printf("Deleting user #%d\n", userDel);

        for (int i = userDel - 1; i < *currentSize - 1; i++) {
            users[i] = users[i + 1];
        }

        (*currentSize)--;
    } else {
        printf("Invalid account number...\n");
    }
}

void freeAll(Account *users) {
    free(users);
}
