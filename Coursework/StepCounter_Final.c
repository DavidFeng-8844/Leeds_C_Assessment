#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FitnessDataStruct.h"

// Function prototypes
void importData(char *filename, FITNESS_DATA **data, int *numRecords);
int totalRecords(FITNESS_DATA *data, int numRecords);
void fewestSteps(FITNESS_DATA *data, int numRecords);
void largestSteps(FITNESS_DATA *data, int numRecords);
double meanStepCount(FITNESS_DATA *data, int numRecords);
void longestPeriodAbove500(FITNESS_DATA *data, int numRecords);
//Original Tokennisation function
void tokeniseRecord(const char *input, const char *delimiter,
                    char *date, char *time, char *steps);

int main() {
    // Declare variables
    char filename[100];
    FITNESS_DATA *data = NULL;
    int numRecords = 0;

    char choice;// Display the menu until the user chooses to quit
    do {
        printf("Select an option:\n");
        printf("A: Specify the filename to be imported\n");
        printf("B: Display the total number of records in the file\n");
        printf("C: Find the date and time of the timeslot with the fewest steps\n");
        printf("D: Find the date and time of the timeslot with the largest number of steps\n");
        printf("E: Find the mean step count of all the records in the file\n");
        printf("F: Find the longest continuous period where the step count is above 500 steps\n");
        printf("Q: Quit\n");
        scanf(" %c", &choice);

        switch (choice) {
            case 'A':
                printf("Input filename: ");
                scanf("%s", filename);
                importData(filename, &data, &numRecords);
                break;
            case 'B':
                printf("Total records: %d\n", totalRecords(data, numRecords));
                break;
            case 'C':
                fewestSteps(data, numRecords);
                break;
            case 'D':
                largestSteps(data, numRecords);
                break;
            case 'E':
                printf("Mean step count: %.2f\n", meanStepCount(data, numRecords));
                break;
            case 'F':
                longestPeriodAbove500(data, numRecords);
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

//The functions mentioned in the program here
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

int totalRecords(FITNESS_DATA *data, int numRecords) {
    return numRecords;
}

void fewestSteps(FITNESS_DATA *data, int numRecords) {
    // Find and print the date and time of the timeslot with the fewest steps
    if (numRecords > 0) {
        int minSteps = data[0].steps;
        int index = 0;
        for (int i = 1; i < numRecords; i++) {
            if (data[i].steps < minSteps) {
                minSteps = data[i].steps;
                index = i;
            }
        }
        printf("Fewest steps: %s %s\n", data[index].date, data[index].time);
    } else {
        printf("No records available.\n");
    }
}

void largestSteps(FITNESS_DATA *data, int numRecords) {
    // Find and print the date and time of the timeslot with the largest number of steps
    if (numRecords > 0) {
        int maxSteps = data[0].steps;
        int index = 0;
        for (int i = 1; i < numRecords; i++) {
            if (data[i].steps > maxSteps) {
                maxSteps = data[i].steps;
                index = i;
            }
        }
        printf("Largest steps: %s %s\n", data[index].date, data[index].time);
    } else {
        printf("No records available.\n");
    }
}

double meanStepCount(FITNESS_DATA *data, int numRecords) {
    if (numRecords == 0) {
        return 0.0;
    }

    int totalSteps = 0;
    for (int i = 0; i < numRecords; i++) {
        totalSteps += data[i].steps;
    }

    return (double)totalSteps / numRecords;
}

void longestPeriodAbove500(FITNESS_DATA *data, int numRecords) {
    int maxPeriodStart = -1;
    int maxPeriodEnd = -1;
    int currentPeriodStart = -1;
    int currentPeriodEnd = -1;

    for (int i = 0; i < numRecords; i++) {
        if (data[i].steps > 500) {
            if (currentPeriodStart == -1) {
                currentPeriodStart = i;
            }
            currentPeriodEnd = i;
        } else {
            if (currentPeriodStart != -1) {
                if (maxPeriodStart == -1 || (currentPeriodEnd - currentPeriodStart) > (maxPeriodEnd - maxPeriodStart)) {
                    maxPeriodStart = currentPeriodStart;
                    maxPeriodEnd = currentPeriodEnd;
                }
                currentPeriodStart = -1;
                currentPeriodEnd = -1;
            }
        }
    }

    if (maxPeriodStart != -1) {
        printf("Longest period start: %s %s\n", data[maxPeriodStart].date, data[maxPeriodStart].time);
        printf("Longest period end: %s %s\n", data[maxPeriodEnd].date, data[maxPeriodEnd].time);
    } else {
        printf("No period with more than 500 steps found.\n");
    }
}

