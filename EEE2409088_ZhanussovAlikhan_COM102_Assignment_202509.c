#include <stdio.h>
#include <string.h>

// Request structure - stores complete maintenance request information
struct Request {
    int reqId;             // Unique request ID (1-150)
    char location[50];     // Location: LY5, LY6, LY7
    char category[20];     // Type: Electricity, Plumbing, AC
    int priority;          // 1 (high) to 5 (low)
    char status[12];       // NEW, ASSIGNED, DONE
    int techId;            // Technician ID or -1 if none
}; 

// Technician structure - stores maintenance staff information
struct Technician {
    int techId;            // Unique technician ID
    char name[60];         // Technician name
    char specialty[20];    // Specialization area
};

// Global arrays-index corresponds to ID
int requestIds[150] = {0};       // 1 = ID in use, 0 = available
struct Request requests[150];    // All requests records

int techIds[150] = {0};          // 1 = ID in use, 0 = available
struct Technician techs[150];    // All technician records

// Get integer input with validation (1 to max)
int getIntSafe(int max) {
    int value;
    while (1) {
        if (scanf("%d", &value) == 1) {
            if (value > 0 && value <= max){
                return value;
            }
            else printf("Input out of range (%d-%d). Try again: ", 1, max);
        } 
        else {
            printf("Invalid input (not a number). Try again: ");
            while (getchar() != '\n'); // Clear invalid input  
        }
    }
}

// Functions for displaying requests in table format
void printHeader() {
    printf("-----------------------------------------------------------------\n");
    printf("| ID  | Location | Category   | Priority | Status     | Tech ID |\n");
    printf("-----------------------------------------------------------------\n");
}

// Display single request row
void printRow(struct Request r) {
    printf("| %-5d | %-5s | %-11s | %-8d | %-10s | %-7d |\n", 
           r.reqId, r.location, r.category, r.priority, r.status, r.techId);
}

// Display table footer
void printFooter() {
    printf("-----------------------------------------------------------------\n");
}

// Create new maintenance request
struct Request newReq (int technicianId) {
    int reqId1;
    char location1[50];
    char category1[20];
    int priority1;
    char status1[12] = "NEW"; // Default status
    int techId1;
    
    printf("\n=== CREATE NEW REQUEST ===\n\n");
    printf("Enter the Request ID(1-150): ");

    // Get unique request ID
    while (1) {
        reqId1 = getIntSafe(150);
        if (requestIds[reqId1]) {
            printf("ID already exists. Try another: ");
        }
        else break;
    }
    
    // Select location
    printf ("\nSelect your dormitory:\n");
    printf("1. LY5\n");
    printf("2. LY6\n");
    printf("3. LY7\n");
    printf("Enter your choice(1-3): ");
    
    while (1){
        int a = getIntSafe(3);
        if (a == 1) {
            strcpy(location1, "LY5");
            break;
        }
        else if (a == 2) {
            strcpy(location1, "LY6");
            break;
        }
        else if (a == 3) {
            strcpy(location1, "LY7");
            break;
        }
        else printf("Invalid choice. Please select 1 - 3: ");
    }

    // Select category 
    printf ("\nSelect request category: \n1. Electricity \n2. Plumbing \n3. AC\nEnter your choice(1-3): ");
    while (1) {
        int a1 = getIntSafe(3);
        if (a1 == 1) {
            strcpy(category1, "Electricity");
            break;
        }
        else if (a1 == 2) {
            strcpy(category1, "Plumbing");
            break;
        }
        else if (a1 == 3) {
            strcpy(category1, "AC");
            break;
        }
        printf("Invalid choice. Please select 1 - 3: ");
    }

    // Get priority (1 = high, 5 = low)
    printf("\nHow urgent is your request? (1 - Low ...5 - High): ");
    while (1) {
        priority1 = getIntSafe(5);
        if (priority1 > 5 || priority1 < 1) {
            printf("Error, type a digit in range 1 - 5: ");
        }
        else break;
    }   

    printf("\n[Success] Request created!\n\n");
    techId1 = technicianId; // Default -1 (no technician) 

    // Create and store request
    struct Request req;
    req.reqId = reqId1;
    strcpy(req.location, location1);
    strcpy(req.category, category1);
    req.priority = priority1;
    strcpy(req.status, status1);
    req.techId = techId1;

    requestIds[reqId1] = 1; // Mark ID as used
    requests[reqId1] = req; // Store in array
    return req;
}

// Display requests with different filter options
void listRequests(int option){
    if (option == 1){ // All requests
        printHeader();
        for (int i = 0; i < 150; ++ i) {
            if (requestIds[i]){
                printRow(requests[i]);
            }
        }
        printFooter();
        return;
    }
    else if (option == 2){ // NEW requests only
        printHeader();
        for (int i = 0; i < 150; ++ i) {
            if (requestIds[i]) {
                if (!strcmp(requests[i].status, "NEW")){
                    printRow(requests[i]);
                    printFooter();
                }
            }
        }
        printFooter();
    }
    else if (option == 3){ // By category
        char cat[20];
        printf("Which category?:\n 1. Electricity \n 2. Plumbing \n 3. AC \nEnter your choice: ");
        while (1){
            int a = getIntSafe(3);
            if (a == 1){
                strcpy(cat, "Electricity");
                break;
            }
            else if (a == 2) {
                strcpy(cat, "Plumbing");
                break;
            }
            else if (a == 3) {
                strcpy(cat, "AC");
                break;
            }
            else{
                printf("Invalid choice. 1 - 3 only: ");
            }
        }
        int cnt = 0;
        printHeader();
        for (int i = 0; i < 150; ++ i) {
            if (requestIds[i]) {
                if (!strcmp(requests[i].category, cat)){
                    printRow(requests[i]);
                    cnt ++;
                }
            }
        }
        printFooter();
        if (cnt == 0) printf ("No match is found\n");
        return;
    }
    printf("No match is found\n");
}

// Find reuqest by ID
int findRequestById(int reqId) {
    if (reqId > 0 && reqId <= 150 && requestIds[reqId]) {
        return reqId; // Found
    }
    return -1; // Not found
}

// Update request status
void updateStatus(int reqId) {
    if (findRequestById(reqId) == -1){ // Check if exists
        printf("The request doesn't exist\n");
        return;
    }
    printf ("Input new status: \n 1. ASSIGNED \n 2. DONE \n 3. NEW\nEnter your choice: ");
    char newStatus[12];
    while(1){
        int a = getIntSafe(3);

        if (a == 1) {
            strcpy(newStatus, "ASSIGNED");
            break;
        }
        else if (a == 2) {
            strcpy(newStatus, "DONE");
            break;
        }
        else if (a == 3) {
            strcpy(newStatus, "NEW");
            break;
        }
        else printf("1 - 3 ONLY: ");
    }

    // Allow only valid status changes: NEW-> ASSIGNED ->DONE
    if (!strcmp(newStatus, "ASSIGNED") && !strcmp(requests[reqId].status, "NEW")){
        strcpy(requests[reqId].status, newStatus);
        printf("[Updated] Status changed to ASSIGNED\n");
        return;
    }
    if (!strcmp(newStatus, "DONE") && !strcmp(requests[reqId].status, "ASSIGNED")){
        strcpy(requests[reqId].status, newStatus);
        printf("[Updated] Status changed to DONE\n");
        return;
    }
    printf("Invalid status transition. Logic: NEW -> ASSIGNED -> DONE\n");
}

// Find tecnician by ID
int findTechnicianById(int techId){
    if (techId > 0 && techId <= 150 && techIds[techId]){
        return techId; // Found
    }
    return -1; // Not found
}

// Assign technician to request 
void assignTechnician(int reqId, int techId1) {
    if (findRequestById(reqId) == -1){
        printf("The request doesn't exist\n");
        return;
    }
    if (findTechnicianById(techId1) == -1){
        printf("The technician doesn't exist\n");
        return;
    }

    // Check if technician speciality matches request category
    if (!strcmp(techs[techId1].specialty, requests[reqId].category)) {
        requests[reqId].techId = techId1;
        // FIX: Change status to ASSIGNED
        strcpy(requests[reqId].status, "ASSIGNED");
        printf("[Success] Technician %s is assigned to Request %d.\n", techs[techId1].name, requests[reqId].reqId);
        printf("Request status updated to 'ASSIGNED'.\n");
    }
    else printf("The technician doesn't specialize in this\n");
}

// Export NEW requests to text file
void saveNewRequestsToFile() {
    FILE *fp = fopen("new_requests.txt", "w");

    if (fp == NULL) {
        printf("Error: Could not create file.\n");
        return;
    }

    int cnt = 0;
    fprintf(fp, "--- List of NEW Requests ---\n");

    // Write each NEW request to file 
    for (int i = 0; i < 150; i++) {
        if (requestIds[i] && strcmp(requests[i].status, "NEW") == 0) {
            fprintf(fp, "ID: %d | Loc: %s | Cat: %s | Priority: %d\n", 
                    requests[i].reqId, requests[i].location, 
                    requests[i].category, requests[i].priority);
            cnt++;
        }
    }
    fclose(fp);
    printf("Exported %d 'NEW' requests to 'new_requests.txt'.\n", cnt);
}

int main() {

    // Initialize sample technicians 
    struct Technician t = {1, "Ali", "Electricity"};
    techIds[1] = 1;
    techs[1] = t;

    struct Technician t1 = {2, "Omar", "Plumbing"};
    techIds[2] = 1;
    techs[2] = t1;
    
    struct Technician t2 = {3, "Adam", "AC"};
    techIds[3] = 1;
    techs[3] = t2;

    // Main menu loop
    while (1) {
        printf("\n========== MAIN MENU ==========\n");
        printf("1. Log new request\n");
        printf("2. List requests\n");
        printf("3. Assign technician\n");
        printf("4. Update status\n");
        printf("5. Search technician\n");
        printf("6. Save new requests in file\n");
        printf("7. Exit\n");
        printf("===============================\n");
        printf("Enter your choice: ");
        int c = getIntSafe(7);
        printf ("\n");
        if (c == 1){
            struct Request r = newReq((-1)); // Create with no technician
        }
        else if (c == 2) {

            printf("List ALL (1) / NEW only (2) / by category (3): ");

            int c1 = getIntSafe(3);
            if (c1 == 1) listRequests(1);
            else if (c1 == 2) listRequests(2);
            else listRequests(3);
        }
        else if (c == 3) {
            printf("--- Current Requests ---\n");
            listRequests(1); 
            
            printf ("\nEnter the Request ID from the table above: ");
            int rid = getIntSafe(150);
            printf("\nAvailable Technicians: \n1. Ali - Electric \n2. Omar - Plumbing \n3. Adam - AC \n");
            printf("Enter Techician ID to assign: ");
            int tid = getIntSafe(3);
            printf("\n");
            assignTechnician(rid, tid);
        }
        else if (c == 4){
            printf("--- Current Requests ---\n");
            listRequests(1);

            printf("\nSelect Request ID to update: ");
            int rid = getIntSafe(150);
            printf("\n");
            updateStatus(rid);
        }
        else if (c == 5){
            int tid;
            printf("Enter Technician ID(1 - 3): ");
            while (1) {
                tid = getIntSafe(3);
                break;
            }
            int foundIndex = findTechnicianById(tid);
            if (foundIndex != -1) {
                printf("\n==== TECHNICIAN DETAILS ====\n");
                printf("ID: %d\nName: %s\nSpecialization: %s\n", 
                       techs[foundIndex].techId,
                       techs[foundIndex].name, 
                       techs[foundIndex].specialty);
                printf("============================\n\n");
            } else {
                printf("Not found\n");
            }
        }
        else if (c == 6) {
            saveNewRequestsToFile();
        }
        else {
            printf("Exiting...\n");
            return 0;
        }
    }

    return 0;
}