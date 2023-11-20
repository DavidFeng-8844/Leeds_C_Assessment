#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FitnessDataStruct.h"

// Function prototypes
void tokeniseRecord(const char *input, const char *delimiter, char *date, char *time, char *steps);
void importData(char *filename, FITNESS_DATA **data, int *numRecords);
void sortDataDescending(FITNESS_DATA *data, int numRecords);
void writeDataAsTSV(char *filename, FITNESS_DATA *data, int numRecords);

int main() {
    char filename[100];
    FITNESS_DATA *data = NULL;
    int numRecords = 0;

    char choice;
    do {
        printf("Select an option:\n");
        printf("A: Specify the filename to be imported\n");
        printf("S: Sort data by step count in descending order\n");
        printf("W: Write data to a TSV file\n");
        printf("Q: Quit\n");
        scanf(" %c", &choice);

        switch (choice) {
            case 'A':
                printf("Enter Filename: ");
                scanf("%s", filename);
                importData(filename, &data, &numRecords);
                break;
            case 'S':
                sortDataDescending(data, numRecords);
                break;
            case 'W':
                writeDataAsTSV(filename, data, numRecords);
                break;
            case 'Q':
                printf("Quitting the program.\n");
                break;
            default:
                printf("Invalid option. Please try again.\n");
                break;
        }
    } while (choice != 'Q');

    // Clean up memory
    if (data) {
        free(data);
    }

    return 0;
}

//The importData Function 
void importData(char *filename, FITNESS_DATA **data, int *numRecords) {
    // Open the CSV file for reading
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error when opening file");
        return;
    }

    // Determine the number of records in the file
    *numRecords = 0;
    char line[100]; // Adjust the buffer size as needed

    //Record the line of data 
    while (fgets(line, sizeof(line), file) != NULL) {
        (*numRecords)++;
    }

    // Allocate memory for the data
    *data = (FITNESS_DATA *)malloc(*numRecords * sizeof(FITNESS_DATA));

    // Reset the file pointer to the beginning
    fseek(file, 0, SEEK_SET);

    // Read and store the data from the file
    for (int i = 0; i < *numRecords; i++) {
        char record[44]; // Define a maximum length for a line
        if (fgets(record, sizeof(record), file) != NULL) {
            char date[11], time[6], steps[10];
            tokeniseRecord(record, ",", date, time, steps);
            strncpy((*data)[i].date, date, sizeof((*data)[i].date));
            strncpy((*data)[i].time, time, sizeof((*data)[i].time));
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

//The sortDataDescending function sorts the data in descending order by step count.
void sortDataDescending(FITNESS_DATA *data, int numRecords) {
    // Sort the data in descending order by step count
    for (int i = 0; i < numRecords - 1; i++) {
        for (int j = i + 1; j < numRecords; j++) {
            if (data[i].steps < data[j].steps) {
                // Swap records if they are out of order
                FITNESS_DATA temp = data[i];
                data[i] = data[j];
                data[j] = temp;
            }
        }
    }
}

//The WriteDataAsTSV function writes the data to a TSV file.
void writeDataAsTSV(char *filename, FITNESS_DATA *data, int numRecords) {
    // Create a TSV file with the same filename (excluding .csv) with the file extension .tsv
    char *extension = ".tsv";
    char tsvFilename[100];

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
    for (int i = 0; i < numRecords; i++) {
        fprintf(file, "%s\t%s\t%d\n", data[i].date, data[i].time, data[i].steps);
    }

    // Close the file
    fclose(file);
}

