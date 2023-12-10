#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FitnessDataStruct.h"
// #include <sys/resource.h>

#define MAX_FILENAME_SIZE 100
#define MAX_LINE_SIZE 200

// Function prototypes
void tokeniseRecord(const char *input, const char *delimiter, char *date, char *time, char *steps);
void importData(char *filename, FITNESS_DATA **data, int *numRecords);
void sortDataDescending(FITNESS_DATA *data, int numRecords);
void writeDataAsTSV(char *filename, FITNESS_DATA *data, int numRecords);

int main() {
    // struct rusage r_usage;

    char filename[MAX_FILENAME_SIZE];
    FITNESS_DATA *data = NULL;
    int numRecords = 0;

    do {
        printf("Enter Filename: ");
        scanf("%99s", filename); // Limit input length to prevent buffer overflow
        importData(filename, &data, &numRecords);

        if (numRecords == 0) {
            printf("No records found in the file. Please try again.\n");
        } else {
            sortDataDescending(data, numRecords);
            writeDataAsTSV(filename, data, numRecords);
        }

    } while (numRecords == 0);

    /*getrusage(RUSAGE_SELF, &r_usage);
    printf("Memory usage: %ld kilobytes\n", r_usage.ru_maxrss);
    */
    // Clean up memory
    free(data);

    return 0;
}

// The importData Function
void importData(char *filename, FITNESS_DATA **data, int *numRecords) {
    // Open the CSV file for reading
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error when opening file");
        return;
    }

    // Determine the number of records in the file
    *numRecords = 0;
    char line[MAX_LINE_SIZE];

    // Record the line of data
    while (fgets(line, sizeof(line), file) != NULL) {
        (*numRecords)++;
    }

    // Allocate memory for the data
    *data = (FITNESS_DATA *)malloc(*numRecords * sizeof(FITNESS_DATA));
    if (*data == NULL) {
        perror("Error allocating memory");
        fclose(file);
        return;
    }

    // Reset the file pointer to the beginning
    fseek(file, 0, SEEK_SET);

    // Read and store the data from the file
    int i = 0;
    for (; i < *numRecords; i++) {
        char record[MAX_LINE_SIZE];
        if (fgets(record, sizeof(record), file) != NULL) {
            char date[11], time[6], steps[10];
            tokeniseRecord(record, ",", date, time, steps);
            //strncpy((*data)[i].date, date, sizeof((*data)[i].date));
            //strncpy((*data)[i].time, time, sizeof((*data)[i].time));
            strcpy((*data)[i].date, date);
            strcpy((*data)[i].time, time);
            (*data)[i].steps = atoi(steps);
        } else {
            printf("Error reading data from file.\n");
            break;
        }
    }

    // Close the file
    fclose(file);
}

//The orignal tokeniseRecord function
void tokeniseRecord(const char *input, const char *delimiter,
                    char *date, char *time, char *steps) {
    // Create a copy of the input string as strtok modifies the string
    char *inputCopy = strdup(input);

    // Tokenize the copied string
    char *token = strtok(inputCopy, delimiter);
    if (token != NULL) {
        strcpy(date, token);
    }

    token = strtok(NULL, delimiter);
    if (token != NULL) {
        strcpy(time, token);
    }

    token = strtok(NULL, delimiter);
    if (token != NULL) {
        strcpy(steps, token);
    }

    // Free the duplicated string
    free(inputCopy);
}

// The sortDataDescending function
void sortDataDescending(FITNESS_DATA *data, int numRecords) {
    // Sort the data in descending order by step count
    int i = 0;
    for (; i < numRecords - 1; i++) {
        int j = i + 1;
        for (; j < numRecords; j++) {
            if (data[i].steps < data[j].steps) {
                // Swap records if they are out of order
                FITNESS_DATA temp = data[i];
                data[i] = data[j];
                data[j] = temp;
            }
        }
    }
}

// The writeDataAsTSV function
void writeDataAsTSV(char *filename, FITNESS_DATA *data, int numRecords) {
    // Create a TSV file with the same filename (excluding .csv) with the file extension .tsv
    char *extension = ".tsv";
    char tsvFilename[MAX_FILENAME_SIZE];

    // Check if the filename ends with ".csv" and remove it
    if (strstr(filename, ".csv") == filename + strlen(filename) - 4) {
        snprintf(tsvFilename, sizeof(tsvFilename), "%.*s%s", (int)(strlen(filename) - 4), filename, extension);
    } else {
        snprintf(tsvFilename, sizeof(tsvFilename), "%s%s", filename, extension);
    }

    // Open the file for writing
    FILE *file = fopen(tsvFilename, "w");
    if (file == NULL) {
        perror("Error when opening file");
        return;
    }

    // Write the sorted data in TSV format
    int i = 0;
    for (; i < numRecords; i++) {
        fprintf(file, "%s\t%s\t%d\n", data[i].date, data[i].time, data[i].steps);
    }

    printf("Data sorted and written to %s\n", tsvFilename);

    // Close the file
    fclose(file);
}
