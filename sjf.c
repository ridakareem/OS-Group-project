#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define N 30

typedef struct {
    int id;
    char type[20];
    int arrival;
    int burst;
    int completed;
} Transaction;

int getBurst(char type[]) {
    if (strcmp(type, "balance") == 0) return 1;
    if (strcmp(type, "statement") == 0) return 2;
    if (strcmp(type, "withdrawal") == 0) return 3;
    if (strcmp(type, "transfer") == 0) return 5;
    return 0;
}

void randomType(char type[]) {
    int r = rand() % 4;
    if (r == 0) strcpy(type, "balance");
    else if (r == 1) strcpy(type, "statement");
    else if (r == 2) strcpy(type, "withdrawal");
    else strcpy(type, "transfer");
}

int main() {
    Transaction t[N];
    int current_time = 0, completed = 0;
    float total_response = 0;

    srand(time(0)); 

    for (int i = 0; i < N; i++) {
        t[i].id = i + 1;
        randomType(t[i].type);
        t[i].arrival = rand() % 20;
        t[i].burst = getBurst(t[i].type);
        t[i].completed = 0;
    }

    printf("\nGenerated Transactions:\n");
    printf("ID\tType\t\tArrival\tBurst\n");

    for (int i = 0; i < N; i++) {
        printf("%d\t%s\t\t%d\t%d\n",
               t[i].id, t[i].type, t[i].arrival, t[i].burst);
    }

    printf("\nExecution Order (SJF):\n");

    while (completed < N) {
        int idx = -1;
        int min_burst = 9999;

        for (int i = 0; i < N; i++) {
            if (!t[i].completed && t[i].arrival <= current_time) {
                if (t[i].burst < min_burst) {
                    min_burst = t[i].burst;
                    idx = i;
                }
            }
        }

        if (idx != -1) {
            int start = current_time;
            int response = start - t[idx].arrival;

            total_response += response;

            current_time += t[idx].burst;
            t[idx].completed = 1;
            completed++;

            printf("T%d (%s) ran from %d to %d\n",
                   t[idx].id, t[idx].type, start, current_time);
        } else {
            current_time++;
        }
    }

    float avg_response = total_response / N;
    float throughput = (float)N / current_time;

    printf("\nAverage Response Time: %.2f\n", avg_response);
    printf("Throughput: %.2f transactions/unit time\n", throughput);

    return 0;
}
