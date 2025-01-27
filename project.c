#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_ACCOUNTS 100
#define CASESAR_KEY 3
#define IFSC_CODE "BANK1234567"

typedef struct {
    int accountNumber;
    char name[100];
    char aadhar[13];
    char password[50];
    char ifsc[12];
    float balance;
} Account;

Account accounts[MAX_ACCOUNTS];
int accountCount = 0;

void encryptPassword(char *password);
void decryptPassword(char *password);
void saveData();
void loadData();
void createAccount();
void userLogin();
void adminMenu();
void deposit(int index);
void withdraw(int index);
void checkBalance(int index);
void viewAllAccounts();
void searchAccount();
void exportData();

int main() {
    int choice;
    loadData();
    while (1) {
        printf("\n=== Bank Account Management System ===\n");
        printf("1. User Login\n");
        printf("2. Create New Account\n");
        printf("3. Admin Login\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                userLogin();
                break;
            case 2:
                createAccount();
                break;
            case 3:
                adminMenu();
                break;
            case 4:
                saveData();
                printf("\nThank you for using the Bank Account Management System!\n");
                printf("Have a great day!\n");
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
    return 0;
}

void encryptPassword(char *password) {
    for (int i = 0; password[i] != '\0'; i++) {
        if (isalpha(password[i])) {
            password[i] = (password[i] - 'a' + CASESAR_KEY) % 26 + 'a';
        }
    }
}

void decryptPassword(char *password) {
    for (int i = 0; password[i] != '\0'; i++) {
        if (isalpha(password[i])) {
            password[i] = (password[i] - 'a' - CASESAR_KEY + 26) % 26 + 'a';
        }
    }
}

void saveData() {
    FILE *file = fopen("bank_data.bin", "wb");
    if (file == NULL) {
        printf("Error saving data.\n");
        return;
    }
    fwrite(&accountCount, sizeof(int), 1, file);
    fwrite(accounts, sizeof(Account), accountCount, file);
    fclose(file);
}

void loadData() {
    FILE *file = fopen("bank_data.bin", "rb");
    if (file == NULL) {
        return;
    }
    fread(&accountCount, sizeof(int), 1, file);
    fread(accounts, sizeof(Account), accountCount, file);
    fclose(file);
}

void createAccount() {
    if (accountCount >= MAX_ACCOUNTS) {
        printf("Account limit reached. Cannot create more accounts.\n");
        return;
    }
    Account newAccount;
    newAccount.accountNumber = 1000 + accountCount;
    strcpy(newAccount.ifsc, IFSC_CODE);

    printf("Enter name: ");
    scanf("%s", newAccount.name);

    printf("Enter Aadhar number: ");
    scanf("%s", newAccount.aadhar);
    if (strlen(newAccount.aadhar) != 12) {
        printf("Invalid Aadhar number. Must be 12 digits.\n");
        return;
    }

    int isValid = 0;
    while (!isValid) {
        printf("Set a password (only letters, no numbers or special characters): ");
        scanf("%s", newAccount.password);

        isValid = 1; 
        for (int i = 0; newAccount.password[i] != '\0'; i++) {
            if (!isalpha(newAccount.password[i])) {
                printf("Error: Password must contain only letters. Please try again.\n");
                isValid = 0; 
                break;
            }
        }
    }

    encryptPassword(newAccount.password);
    newAccount.balance = 0.0f;
    accounts[accountCount++] = newAccount;
    saveData();
    printf("Account created successfully!\n");
    printf("Your account number is %d\n", newAccount.accountNumber);
    printf("Your IFSC code is %s\n", newAccount.ifsc);
}


void userLogin() {
    int accountNumber;
    char password[50];
    printf("Enter account number: ");
    scanf("%d", &accountNumber);
    printf("Enter password: ");
    scanf("%s", password);
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            char decryptedPassword[50];
            strcpy(decryptedPassword, accounts[i].password);
            decryptPassword(decryptedPassword);
            if (strcmp(decryptedPassword, password) == 0) {
                printf("Login successfully\n");
                int choice;
                do {
                    printf("\n1. Deposit\n2. Withdraw\n3. Check Balance\n4. Logout\n");
                    printf("Enter your choice: ");
                    scanf("%d", &choice);
                    switch (choice) {
                        case 1:
                            deposit(i);
                            break;
                        case 2:
                            withdraw(i);
                            break;
                        case 3:
                            checkBalance(i);
                            break;
                        case 4:
                            printf("Logged out successfully.\n");
                            return;
                        default:
                            printf("Invalid choice.\n");
                    }
                } while (choice != 4);
                return;
            }
        }
    }
    printf("Invalid account number or password.\n");
}

void deposit(int index) {
    float amount;
    printf("Enter amount to deposit: ");
    scanf("%f", &amount);
    if (amount <= 0) {
        printf("Invalid amount.\n");
        return;
    }
    accounts[index].balance += amount;
    saveData(); 
    printf("Amount deposited successfully. Current balance: %.2f\n", accounts[index].balance);
}

void withdraw(int index) {
    float amount;
    printf("Enter amount to withdraw: ");
    scanf("%f", &amount);
    if (amount <= 0) {
        printf("Invalid amount.\n");
        return;
    }
    if (amount > accounts[index].balance) {
        printf("Insufficient balance! Your current balance is %.2f.\n", accounts[index].balance);
        printf("Please enter an amount less than or equal to your balance.\n");
        return;
    }
    accounts[index].balance -= amount;
    saveData(); 
    printf("Amount withdrawn successfully. Current balance: %.2f\n", accounts[index].balance);
}

void checkBalance(int index) {
    printf("Current balance: %.2f\n", accounts[index].balance);
}

void adminMenu() {
    char adminUsername[20] = "admin";
    char adminPassword[20] = "admin123";
    char username[20], password[20];

    printf("Enter admin username: ");
    scanf("%s", username);
    printf("Enter admin password: ");
    scanf("%s", password);

    if (strcmp(username, adminUsername) == 0 && strcmp(password, adminPassword) == 0) {
        printf("Admin login successfully!\n");
        int choice;
        do {
            printf("\n1. View All Accounts\n2. Search Account\n3. Export Data\n4. Logout\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);
            switch (choice) {
                case 1:
                    viewAllAccounts();
                    break;
                case 2:
                    searchAccount();
                    break;
                case 3:
                    exportData();
                    break;
                case 4:
                    printf("Admin logged out successfully.\n");
                    return;
                default:
                    printf("Invalid choice.\n");
            }
        } while (choice != 4);
    } else {
        printf("Invalid admin credentials.\n");
    }
}

void viewAllAccounts() {
    printf("\nAccount Details:\n");
    for (int i = 0; i < accountCount; i++) {
        printf("Account Number: %d, Name: %s, Balance: %.2f, Aadhar: %s, IFSC: %s\n",
            accounts[i].accountNumber, accounts[i].name, accounts[i].balance,
            accounts[i].aadhar, accounts[i].ifsc);
    }
}

void searchAccount() {
    int accountNumber;
    printf("Enter account number to search: ");
    scanf("%d", &accountNumber);
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            printf("Account found: Name: %s, Balance: %.2f, Aadhar: %s, IFSC: %s\n",
                accounts[i].name, accounts[i].balance, accounts[i].aadhar, accounts[i].ifsc);
            return;
        }
    }
    printf("Account not found.\n");
}

void exportData() {
    FILE *file = fopen("bank_data.csv", "w");
    if (file == NULL) {
        printf("Error exporting data.\n");
        return;
    }
    fprintf(file, "Account Number,Name,Encrypted Password,Aadhar,IFSC\n");
    for (int i = 0; i < accountCount; i++) {
        fprintf(file, "%d,%s,%s,%s,%s\n",
                accounts[i].accountNumber, accounts[i].name, accounts[i].password,
                accounts[i].aadhar, accounts[i].ifsc);
    }
    fclose(file);
    printf("Data exported successfully to bank_data.csv.\n");
}
