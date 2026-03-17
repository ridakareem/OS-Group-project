#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Shared data
int balance;
int read_count = 0;

// Locks
pthread_mutex_t mutex;
pthread_mutex_t write_lock;

// Structure for thread data
typedef struct {
    int id;
    int type;       // 1 = Balance enquiry, 2 = Transaction
    int operation;  // 1 = Deposit, 2 = Withdraw
    int amount;
} ThreadData;

// Reader Function (Balance Enquiry)
void* reader(void* arg) {
    ThreadData *data = (ThreadData*)arg;

    pthread_mutex_lock(&mutex);
    read_count++;
    if (read_count == 1)
        pthread_mutex_lock(&write_lock);
    pthread_mutex_unlock(&mutex);

    // Immediate display of balance
    printf("Reader-%d: Current Balance = %d\n", data->id, balance);
    fflush(stdout);

    pthread_mutex_lock(&mutex);
    read_count--;
    if (read_count == 0)
        pthread_mutex_unlock(&write_lock);
    pthread_mutex_unlock(&mutex);

    return NULL;
}

// Writer Function (Deposit / Withdraw)
void* writer(void* arg) {
    ThreadData *data = (ThreadData*)arg;

    pthread_mutex_lock(&write_lock);

    if (data->operation == 1) {
        balance += data->amount;
        printf("Writer-%d: Deposited %d, New Balance = %d\n",
               data->id, data->amount, balance);
    } 
    else if (data->operation == 2) {
        if (balance >= data->amount) {
            balance -= data->amount;
            printf("Writer-%d: Withdrew %d, New Balance = %d\n",
                   data->id, data->amount, balance);
        } else {
            printf("Writer-%d: Withdrawal failed (Insufficient balance)\n",
                   data->id);
        }
    }

    pthread_mutex_unlock(&write_lock);
    return NULL;
}

int main() {
    int n = 6;
    pthread_t threads[n];
    ThreadData data[n];

    printf("Enter initial balance: ");
    scanf("%d", &balance);

    // Initialize locks
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&write_lock, NULL);

    // Input for each thread
    for (int i = 0; i < n; i++) {
        data[i].id = i + 1;

        printf("\nThread %d:\n", i + 1);
        printf("Enter type (1=Balance enquiry, 2=Transaction): ");
        scanf("%d", &data[i].type);

        if (data[i].type == 2) {
            printf("1. Deposit  2. Withdraw\nEnter operation: ");
            scanf("%d", &data[i].operation);

            printf("Enter amount: ");
            scanf("%d", &data[i].amount);
        }
    }

    // Create threads
    for (int i = 0; i < n; i++) {
        if (data[i].type == 1)
            pthread_create(&threads[i], NULL, reader, &data[i]);
        else
            pthread_create(&threads[i], NULL, writer, &data[i]);
    }

    // Wait for threads to finish
    for (int i = 0; i < n; i++)
        pthread_join(threads[i], NULL);

    printf("\nFinal Balance: %d\n", balance);

    // Destroy locks
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&write_lock);

    return 0;
}