#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define structure for account
struct account {
    int acc_no;
    char name[50];
    float balance;
};

void create_account();
void display_accounts();
void deposit_money();
void withdraw_money();
void search_account();

int main() {
    int choice;

    while (1) {
        printf("\n\n===== BANK MANAGEMENT SYSTEM =====\n");
        printf("1. Create New Account\n");
        printf("2. Display All Accounts\n");
        printf("3. Deposit Money\n");
        printf("4. Withdraw Money\n");
        printf("5. Search Account\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                create_account();
                break;
            case 2:
                display_accounts();
                break;
            case 3:
                deposit_money();
                break;
            case 4:
                withdraw_money();
                break;
            case 5:
                search_account();
                break;
            case 6:
                printf("\nThank you for using the system!\n");
                exit(0);
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    }
    return 0;
}

// Function to create account
void create_account() {
    struct account acc;
    FILE *fp;

    fp = fopen("bankdata.txt", "a");
    if (fp == NULL) {
        printf("Error opening file!");
        return;
    }

    printf("\nEnter Account Number: ");
    scanf("%d", &acc.acc_no);
    printf("Enter Name: ");
    fflush(stdin);
    fgets(acc.name, sizeof(acc.name), stdin);
    acc.name[strcspn(acc.name, "\n")] = '\0'; // remove newline
    printf("Enter Initial Balance: ");
    scanf("%f", &acc.balance);

    fwrite(&acc, sizeof(struct account), 1, fp);
    fclose(fp);

    printf("\nAccount Created Successfully!\n");
}

// Function to display all accounts
void display_accounts() {
    struct account acc;
    FILE *fp;

    fp = fopen("bankdata.txt", "r");
    if (fp == NULL) {
        printf("\nNo data found!\n");
        return;
    }

    printf("\n%-10s %-20s %-10s\n", "Acc No", "Name", "Balance");
    printf("--------------------------------------------\n");

    while (fread(&acc, sizeof(struct account), 1, fp))
        printf("%-10d %-20s %-10.2f\n", acc.acc_no, acc.name, acc.balance);

    fclose(fp);
}

// Function to deposit money
void deposit_money() {
    struct account acc;
    FILE *fp, *temp;
    int acc_no, found = 0;
    float amt;

    printf("\nEnter Account Number to deposit: ");
    scanf("%d", &acc_no);
    printf("Enter Amount to deposit: ");
    scanf("%f", &amt);

    fp = fopen("bankdata.txt", "r");
    temp = fopen("temp.txt", "w");

    if (fp == NULL) {
        printf("No data found!\n");
        return;
    }

    while (fread(&acc, sizeof(struct account), 1, fp)) {
        if (acc.acc_no == acc_no) {
            acc.balance += amt;
            found = 1;
        }
        fwrite(&acc, sizeof(struct account), 1, temp);
    }

    fclose(fp);
    fclose(temp);
    remove("bankdata.txt");
    rename("temp.txt", "bankdata.txt");

    if (found)
        printf("\nAmount Deposited Successfully!\n");
    else
        printf("\nAccount Not Found!\n");
}

// Function to withdraw money
void withdraw_money() {
    struct account acc;
    FILE *fp, *temp;
    int acc_no, found = 0;
    float amt;

    printf("\nEnter Account Number to withdraw: ");
    scanf("%d", &acc_no);
    printf("Enter Amount to withdraw: ");
    scanf("%f", &amt);

    fp = fopen("bankdata.txt", "r");
    temp = fopen("temp.txt", "w");

    if (fp == NULL) {
        printf("No data found!\n");
        return;
    }

    while (fread(&acc, sizeof(struct account), 1, fp)) {
        if (acc.acc_no == acc_no) {
            if (acc.balance >= amt) {
                acc.balance -= amt;
                found = 1;
            } else {
                printf("\nInsufficient balance!\n");
                found = -1;
            }
        }
        fwrite(&acc, sizeof(struct account), 1, temp);
    }

    fclose(fp);
    fclose(temp);
    remove("bankdata.txt");
    rename("temp.txt", "bankdata.txt");

    if (found == 1)
        printf("\nWithdrawal Successful!\n");
    else if (found == 0)
        printf("\nAccount Not Found!\n");
}

// Function to search account by account number
void search_account() {
    struct account acc;
    FILE *fp;
    int acc_no, found = 0;

    printf("\nEnter Account Number to search: ");
    scanf("%d", &acc_no);

    fp = fopen("bankdata.txt", "r");
    if (fp == NULL) {
        printf("No data found!\n");
        return;
    }

    while (fread(&acc, sizeof(struct account), 1, fp)) {
        if (acc.acc_no == acc_no) {
            printf("\nAccount Found!\n");
            printf("Account No: %d\nName: %s\nBalance: %.2f\n",
                   acc.acc_no, acc.name, acc.balance);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("\nAccount Not Found!\n");

    fclose(fp);
}
