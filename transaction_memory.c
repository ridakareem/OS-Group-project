#include <stdio.h>
#include <stdlib.h>

#define TOTAL_TRANSACTIONS 50
#define BUFFER_SIZE 8

// Function for FIFO Page Replacement
int fifo(int pages[], int n, int capacity) {
    int frames[BUFFER_SIZE];
    int front = 0, faults = 0;

    // Initialize frames
    for(int i = 0; i < capacity; i++)
        frames[i] = -1;

    for(int i = 0; i < n; i++) {
        int found = 0;

        // Check if page exists
        for(int j = 0; j < capacity; j++) {
            if(frames[j] == pages[i]) {
                found = 1;
                break;
            }
        }

        if(!found) {
            frames[front] = pages[i];
            front = (front + 1) % capacity;
            faults++;
        }
    }

    return faults;
}


// Function for Optimal Page Replacement
int optimal(int pages[], int n, int capacity) {
    int frames[BUFFER_SIZE];
    int faults = 0;

    // Initialize frames
    for(int i = 0; i < capacity; i++)
        frames[i] = -1;

    for(int i = 0; i < n; i++) {
        int found = 0;

        // Check if page exists
        for(int j = 0; j < capacity; j++) {
            if(frames[j] == pages[i]) {
                found = 1;
                break;
            }
        }

        if(!found) {
            faults++;

            // Find empty frame
            int empty = -1;
            for(int j = 0; j < capacity; j++) {
                if(frames[j] == -1) {
                    empty = j;
                    break;
                }
            }

            if(empty != -1) {
                frames[empty] = pages[i];
            } else {
                int farthest = -1, index = -1;

                for(int j = 0; j < capacity; j++) {
                    int k;
                    for(k = i + 1; k < n; k++) {
                        if(frames[j] == pages[k])
                            break;
                    }

                    if(k == n) {
                        index = j;
                        break;
                    }

                    if(k > farthest) {
                        farthest = k;
                        index = j;
                    }
                }

                frames[index] = pages[i];
            }
        }
    }

    return faults;
}


// Main Function
int main() {
    int pages[TOTAL_TRANSACTIONS];

    // Generate random transaction logs (1–15)
    printf("Transaction Access Sequence:\n");
    for(int i = 0; i < TOTAL_TRANSACTIONS; i++) {
        pages[i] = rand() % 15 + 1;
        printf("%d ", pages[i]);
    }

    int fifo_faults = fifo(pages, TOTAL_TRANSACTIONS, BUFFER_SIZE);
    int optimal_faults = optimal(pages, TOTAL_TRANSACTIONS, BUFFER_SIZE);

    printf("\n\nBuffer Size: %d", BUFFER_SIZE);
    printf("\nTotal Transactions: %d", TOTAL_TRANSACTIONS);

    printf("\n\nFIFO Page Faults: %d", fifo_faults);
    printf("\nOptimal Page Faults: %d", optimal_faults);

    float fifo_rate = (float)fifo_faults / TOTAL_TRANSACTIONS;
    float optimal_rate = (float)optimal_faults / TOTAL_TRANSACTIONS;

    printf("\n\nFIFO Fault Rate: %.2f", fifo_rate);
    printf("\nOptimal Fault Rate: %.2f\n", optimal_rate);

    return 0;
}
