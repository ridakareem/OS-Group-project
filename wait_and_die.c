#include <stdio.h> 
  
#define PROCESSES 4 
#define RESOURCES 4 
  
int timestamp[PROCESSES] = {1,2,3,4}; 
int resource_holder[RESOURCES] = {0}; 

// Money mapping
int money[RESOURCES] = {100, 200, 500, 2000};

// User input requests
int request[PROCESSES];
  
void showResources() { 
    printf("\nCurrent Resource Allocation:\n"); 
    for(int i = 0; i < RESOURCES; i++) { 
        if(resource_holder[i] == 0)
            printf("₹%d is FREE\n", money[i]); 
        else
            printf("₹%d held by P%d\n", money[i], resource_holder[i]); 
    } 
} 
  
void abortProcess(int p) { 
    printf("P%d aborted. Releasing its resources.\n", p); 
  
    for(int i = 0; i < RESOURCES; i++) { 
        if(resource_holder[i] == p) { 
            resource_holder[i] = 0; 
        } 
    } 
} 
  
int requestResource(int process) { 
  
    int resource = request[process-1];
    int holder = resource_holder[resource]; 
  
    if(holder == 0) { 
        resource_holder[resource] = process; 
        printf("P%d acquired ₹%d\n", process, money[resource]); 
        return 1; 
    } 
  
    if(timestamp[process-1] < timestamp[holder-1]) { 
        printf("P%d requests ₹%d but P%d holds it -> WAIT\n", 
               process, money[resource], holder); 
        return 0; 
    } 
    else { 
        printf("P%d requests ₹%d but P%d holds it -> ABORT\n", 
               process, money[resource], holder); 
        abortProcess(process); 
        return -1; 
    } 
} 
  
int main() { 
  
    // Initial allocation
    resource_holder[0] = 1; // ₹100 → P1
    resource_holder[2] = 2; // ₹500 → P2
    resource_holder[3] = 3; // ₹2000 → P3
    resource_holder[1] = 4; // ₹200 → P4
  

    printf("Enter resource request for each process:\n");
    printf("0 -> ₹100, 1 -> ₹200, 2 -> ₹500, 3 -> ₹2000\n\n");
  
    for(int i = 0; i < PROCESSES; i++) {
        printf("Enter request for P%d: ", i+1);
        scanf("%d", &request[i]);
    }
  
    int finished[PROCESSES] = {0};
    int progress;
  
    printf("\nInitial Deadlock State\n"); 
    showResources(); 
  
    printf("\n--- Execution Starts ---\n");
  
    do {
        progress = 0;
  
        for(int i = 1; i <= PROCESSES; i++) {
            if(finished[i-1]) continue;
  
            int result = requestResource(i);
  
            if(result == 1) {
                printf("P%d finished execution.\n", i);
                abortProcess(i); // release after completion
                finished[i-1] = 1;
                progress = 1;
            }
            else if(result == -1) {
                finished[i-1] = 1;
                progress = 1;
            }
        }
  
    } while(progress);
  
    printf("\nFinal State:\n");
    showResources(); 
  
    return 0; 
}
