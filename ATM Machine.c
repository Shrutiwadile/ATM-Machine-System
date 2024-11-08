#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_USERS 3
#define MAX_TRANSACTIONS 10

// Structure to store transaction history
struct Transaction {
    float amount;
    char type[10]; // "Deposit" or "Withdraw"
    char date[11];
};

// Structure to store user details
struct User {
    int id;
    int pin;
    float balance;
    struct Transaction transactions[MAX_TRANSACTIONS];
    int transactionCount;
    float dailyWithdrawn;
};

// Initialize users
struct User users[MAX_USERS] = {
    {1, 1234, 1000.0, {{0}}, 0, 0},
    {2, 5678, 2000.0, {{0}}, 0, 0},
    {3, 4321, 3000.0, {{0}}, 0, 0}
};

// Function prototypes
int authenticateUser(int userId, int enteredPin);
void showMenu();
void checkBalance(struct User* user);
void depositMoney(struct User* user);
void withdrawMoney(struct User* user);
void changePin(struct User* user);
void transferMoney(struct User* user);
void viewTransactionHistory(struct User* user);
void recordTransaction(struct User* user, float amount, const char* type);
const char* getCurrentDate();

int main() {
    int userId, enteredPin, authenticated = 0, choice;
    struct User* currentUser = NULL;

    printf("=== Welcome to the ATM System ===\n");

    // User authentication
    while (!authenticated) {
        printf("\nEnter your User ID: ");
        scanf("%d", &userId);
        printf("Enter your PIN: ");
        scanf("%d", &enteredPin);

        int userIndex = authenticateUser(userId, enteredPin);
        if (userIndex != -1) {
            currentUser = &users[userIndex];
            authenticated = 1;
            printf("\nLogin successful! Welcome, User %d.\n", userId);
        }
        else {
            printf("\nInvalid User ID or PIN. Please try again.\n");
        }
    }

    // Main menu loop
    do {
        showMenu();
        printf("\nChoose an option: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            checkBalance(currentUser);
            break;
        case 2:
            depositMoney(currentUser);
            break;
        case 3:
            withdrawMoney(currentUser);
            break;
        case 4:
            changePin(currentUser);
            break;
        case 5:
            transferMoney(currentUser);
            break;
        case 6:
            viewTransactionHistory(currentUser);
            break;
        case 7:
            printf("\nThank you for using the ATM. Have a Good Day!!\n");
            exit(0);
        default:
            printf("\nInvalid choice. Please try again.\n");
        }
    } while (1);

    return 0;
}

// Function to authenticate user by User ID and PIN
int authenticateUser(int userId, int enteredPin) {
    for (int i = 0; i < MAX_USERS; i++) {
        if (users[i].id == userId && users[i].pin == enteredPin) {
            return i;
        }
    }
    return -1;
}

// Function to display the menu
void showMenu() {
    printf("\n--- ATM Menu ---\n");
    printf("1. Check Balance\n");
    printf("2. Deposit Money\n");
    printf("3. Withdraw Money\n");
    printf("4. Change PIN\n");
    printf("5. Transfer Money\n");
    printf("6. View Transaction History\n");
    printf("7. Exit\n");
}

// Function to check balance
void checkBalance(struct User* user) {
    printf("\nYour current balance is: $%.2f\n", user->balance);
}

void transferMoney(struct User* user) {
    int receiverId;
    float amount;
    printf("\nEnter recipient User ID: ");
    scanf("%d", &receiverId);

    struct User* receiver = NULL;
    for (int i = 0; i < MAX_USERS; i++) {
        if (users[i].id == receiverId && users[i].id != user->id) {
            receiver = &users[i];
            break;
        }
    }

    if (receiver) {
        printf("Enter amount to transfer: $");
        scanf("%f", &amount);

        if (amount > 0 && amount <= user->balance) {
            user->balance -= amount;
            receiver->balance += amount;
            recordTransaction(user, -amount, "Transfer Out");
            recordTransaction(receiver, amount, "Transfer In");
            printf("Successfully transferred $%.2f to User %d.\n", amount, receiverId);
        }
        else {
            printf("Insufficient balance or invalid amount.\n");
        }
    }
    else {
        printf("User not found.\n"); 
    }
}

// Function to deposit money
void depositMoney(struct User* user) {
    float amount;
    printf("\nEnter amount to deposit: $");
    scanf("%f", &amount);

    if (amount > 0) {
        user->balance += amount;
        recordTransaction(user, amount, "Deposit");
        printf("Successfully deposited $%.2f. New balance: $%.2f\n", amount, user->balance);
    }
    else {
        printf("Invalid amount. Please enter a positive number.\n");
    }
}

// Function to withdraw money
void withdrawMoney(struct User* user) {
    float amount;
    printf("\nEnter amount to withdraw: $");
    scanf("%f", &amount);

    if (amount > 0 && amount <= user->balance) {
        user->balance -= amount;
        recordTransaction(user, -amount, "Withdraw");
        printf("Successfully withdrew $%.2f. New balance: $%.2f\n", amount, user->balance);
    }
    else {
        printf("Invalid amount or insufficient balance.\n");
    }
}

// Function to change the user's PIN
void changePin(struct User* user) {
    int newPin, confirmPin;
    printf("\nEnter your new PIN: ");
    scanf("%d", &newPin);
    printf("Confirm your new PIN: ");
    scanf("%d", &confirmPin);

    if (newPin == confirmPin) {
        user->pin = newPin;
        printf("PIN successfully changed.\n");
    }
    else {
        printf("PINs do not match. Try again.\n");
    }
}

// Function to view transaction history
void viewTransactionHistory(struct User* user) {
    printf("\n--- Transaction History ---\n");
    if (user->transactionCount == 0) {
        printf("No transactions found.\n");
    }
    else {
        for (int i = 0; i < user->transactionCount; i++) {
            printf("%s: %s $%.2f\n", user->transactions[i].date, user->transactions[i].type, user->transactions[i].amount);
        }
    }
}

// Function to record a transaction
void recordTransaction(struct User* user, float amount, const char* type) {
    struct Transaction* transaction = &user->transactions[user->transactionCount % MAX_TRANSACTIONS];
    transaction->amount = amount;
    strcpy(transaction->type, type);
    strcpy(transaction->date, getCurrentDate());
    user->transactionCount++;
}

// Function to get current date in "YYYY-MM-DD" format
const char* getCurrentDate() {
    static char date[11];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(date, sizeof(date), "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    return date;
}