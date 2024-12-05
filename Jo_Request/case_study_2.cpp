#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024 // Maximum length of a line to read

// Define a struct
typedef struct {
    // all the features from the CSV, 
    // using 144 character, to avoid overflow
    char location[144];
    char city[144];
    long long int price;
    int rooms;
    int bathroom;
    int carpark;
    char type[144];
    char furnish[144];
} Data;

// Prototype of all function that created
void getFileSize(int &size);
void readFile(Data datas[], int size);
void printFile(Data datas[], int size);
int compareData(const Data* a, const Data* b, int column, int ascending);
int partition(Data* arr, int low, int high, int column, int ascending);
void quickSort(Data* arr, int low, int high, int column, int ascending);
int strCaseCmp(const char* str1, const char* str2);
int getColumnIndex(const char* columnName);
void sortBy(Data datas[], int size);
void searchData(Data datas[], int size);
void exportData(Data datas[], int size);

int main() {
    int input; 
    int size = 0;

    getFileSize(size);
    // printf("file size : %d\n", size);

    Data* datas = (Data*)malloc(size * sizeof(Data));

    readFile(datas, size);
    // printFile(datas, size);

    do{
        puts("What do you want to do?");
        
        puts("\tDisplay Data");
        puts("\tSearch Data");
        puts("\tSort Data");
        puts("\tExport Data");
        puts("\tExit");
        
        do{
            puts("");
            printf("Your Choise: ");
            scanf("%d",&input); getchar();
        }while(input < 0 || input > 5);
        
        switch(input){
            case 1: 
                int numOfRows;
                do{
                    printf("Number of rows: ");
                    scanf("%d", &numOfRows); getchar();
                }while(numOfRows < 0);
                if (numOfRows > size){
                    printFile(datas, size);
                } else {
                    printFile(datas, numOfRows);
                }
                
                break;
            case 2: 
                searchData(datas, size);
                break;
            case 3: 
                sortBy(datas, size);
                break;
            case 4: 
                exportData(datas, size);
                break;
        }
    }while(input != 5);
    puts("~~~ Thanks for Using The App ~~~");
}

void getFileSize(int &size) {
    const char* filename = "data.csv";
    FILE* file = fopen(filename, "r");

    if (!file) {
        perror("Failed to open file");
        return;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        // Remove the newline character at the end if present
        size_t length = strlen(line);
        if (line[length - 1] == '\n') {
            line[length - 1] = '\0';
            length--; // Adjust length after removing newline
        }

        size++;
    }
    size = size -1;

    fclose(file);
}

void readFile(Data* datas, int size) {
    const char* filename = "data.csv";
    FILE* file = fopen(filename, "r");

    if (!file) {
        perror("Failed to open file");
        return;
    }

    // Skip the first line (header row)
    char buffer[1024];
    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        perror("Failed to read header line");
        fclose(file);
        return;
    }

    int count = 0; // Track the number of records read

    while (count < size && 
           fscanf(file, "%[^,],%[^,],%lld,%d,%d,%d,%[^,],%[^\n]\n",
                  datas[count].location,
                  datas[count].city,
                  &datas[count].price,
                  &datas[count].rooms,
                  &datas[count].bathroom,
                  &datas[count].carpark,
                  datas[count].type,
                  datas[count].furnish) == 8) {
        count++; // Increment the count for each successfully read record
    }

    fclose(file);
}

void printFile(Data datas[], int size) {
    // Print the header line
    printf("Location\t\tCity\t\t\tPrice\tRooms\tBathroom\tCarpark\tType\t\tFurnish\n");

    // Print each record
    for (int i = 0; i < size; i++) {
        // Print each field, adjusting spacing for alignment
        printf("%-15s\t%-15s\t%-10lld\t%-5d\t%-8d\t%-8d\t%-10s\t%-10s\n",
               datas[i].location, 
               datas[i].city, 
               datas[i].price, 
               datas[i].rooms, 
               datas[i].bathroom, 
               datas[i].carpark, 
               datas[i].type, 
               datas[i].furnish);
    }

    // Print empty lines for spacing
    puts("");
    puts("");
    puts("");

    // Wait for user input before closing
    getchar();
}

// Function to compare two Data entries by a specific column
int compareData(const Data* a, const Data* b, int column, int ascending) {
    int result = 0;
    switch (column) {
        case 1: // Location
            result = strcmp(a->location, b->location);
            break;
        case 2: // City
            result = strcmp(a->city, b->city);
            break;
        case 3: // Price
            result = (a->price > b->price) - (a->price < b->price);
            break;
        case 4: // Rooms
            result = (a->rooms > b->rooms) - (a->rooms < b->rooms);
            break;
        case 5: // Bathroom
            result = (a->bathroom > b->bathroom) - (a->bathroom < b->bathroom);
            break;
        case 6: // Carpark
            result = (a->carpark > b->carpark) - (a->carpark < b->carpark);
            break;
        case 7: // Type
            result = strcmp(a->type, b->type);
            break;
        case 8: // Furnish
            result = strcmp(a->furnish, b->furnish);
            break;
        default:
            break;
    }
    return ascending ? result : -result;
}

// Partition function for quicksort
int partition(Data* arr, int low, int high, int column, int ascending) {
    Data pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (compareData(&arr[j], &pivot, column, ascending) < 0) {
            i++;
            Data temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    Data temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;

    return i + 1;
}

// Quicksort function
void quickSort(Data* arr, int low, int high, int column, int ascending) {
    if (low < high) {
        int pi = partition(arr, low, high, column, ascending);

        quickSort(arr, low, pi - 1, column, ascending);
        quickSort(arr, pi + 1, high, column, ascending);
    }
}

int strCaseCmp(const char* str1, const char* str2) {
    while (*str1 && *str2) {
        if (tolower((unsigned char)*str1) != tolower((unsigned char)*str2)) {
            return tolower((unsigned char)*str1) - tolower((unsigned char)*str2);
        }
        str1++;
        str2++;
    }
    return tolower((unsigned char)*str1) - tolower((unsigned char)*str2);
}

int getColumnIndex(const char* columnName) {
    if (strCaseCmp(columnName, "Location") == 0) return 1;
    if (strCaseCmp(columnName, "City") == 0) return 2;
    if (strCaseCmp(columnName, "Price") == 0) return 3;
    if (strCaseCmp(columnName, "Rooms") == 0) return 4;
    if (strCaseCmp(columnName, "Bathroom") == 0) return 5;
    if (strCaseCmp(columnName, "Carpark") == 0) return 6;
    if (strCaseCmp(columnName, "Type") == 0) return 7;
    if (strCaseCmp(columnName, "Furnish") == 0) return 8;
    return -1; // Invalid column
}

void sortBy(Data datas[], int size) {
    char columnName[50];
    char direction[50];
    int ascending;

    printf("\nEnter column to sort by (e.g., 'Location', 'Price', etc.): ");
    scanf("%s", columnName);

    int column = getColumnIndex(columnName);
    if (column == -1) {
        printf("Invalid column name!\n");
        return;
    }
    printf("Enter sort direction (asc for Ascending, dsc for Descending): ");
    scanf("%s", direction);

    if (strcmp(direction, "asc") == 0) ascending = 1;
    else if (strcmp(direction, "dsc") == 0) ascending = 0;
    else {
        printf("Invalid Input!\n");
        return;
    }
    

    quickSort(datas, 0, size - 1, column, ascending);
    printFile(datas, 10);
}

void searchData(Data datas[], int size) {
    char columnName[50];
    char query[144];

    // Prompt user for column name
    printf("Choose column: (e.g., 'Location', 'Price', etc.): ");
    scanf("%s", columnName);

    // Get column index
    int column = getColumnIndex(columnName);
    if (column == -1) {
        printf("Invalid column name!\n");
        return;
    }

    // Prompt user for query value
    printf("What data do you want to find? ");
    scanf("%s", query);

    int found = 0; // Flag to track if any data is found

    for (int i = 0; i < size; i++) {
        int match = 0;
        switch (column) {
            case 1: match = (strCaseCmp(datas[i].location, query) == 0); break;
            case 2: match = (strCaseCmp(datas[i].city, query) == 0); break;
            case 3: match = (atoll(query) == datas[i].price); break;
            case 4: match = (atoi(query) == datas[i].rooms); break;
            case 5: match = (atoi(query) == datas[i].bathroom); break;
            case 6: match = (atoi(query) == datas[i].carpark); break;
            case 7: match = (strCaseCmp(datas[i].type, query) == 0); break;
            case 8: match = (strCaseCmp(datas[i].furnish, query) == 0); break;
        }

        if (match) {
            if (!found) {
                // Print the header only once
                printf("Location\t\tCity\t\t\tPrice\tRooms\tBathroom\tCarpark\tType\t\tFurnish\n");
            }
            found = 1;
            printf("%-15s %-15s %-10lld %-6d %-9d %-8d %-10s %-10s\n",
                   datas[i].location, datas[i].city, datas[i].price,
                   datas[i].rooms, datas[i].bathroom, datas[i].carpark,
                   datas[i].type, datas[i].furnish);
        }
    }

    if (!found) {
        printf("Not Found\n");
    }
}

void exportData(Data datas[], int size) {
    char filename[100];

    // Meminta nama file dari pengguna
    printf("File name: : ");
    scanf("%s", filename);

    // Membuka file untuk penulisan
    FILE* file = fopen(strcat(filename, ".csv"), "w");
    if (!file) {
        perror("Gagal membuka file");
        return;
    }

    // Menulis header CSV
    fprintf(file, "Location,City,Price,Rooms,Bathroom,Carpark,Type,Furnish\n");

    // Menulis data dari array ke file
    for (int i = 0; i < size; i++) {
        fprintf(file, "%s,%s,%lld,%d,%d,%d,%s,%s\n",
                datas[i].location,
                datas[i].city,
                datas[i].price,
                datas[i].rooms,
                datas[i].bathroom,
                datas[i].carpark,
                datas[i].type,
                datas[i].furnish);
    }

    // Menutup file
    fclose(file);
    printf("Data successfully written to file %s!\n", filename);
}
