#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define an appropriate struct
typedef struct {
    char date[11];
    char time[6];
    int steps;
} FITNESS_DATA;


// Define any additional variables here

// This is your helper function. Do not change it in any way.
// Inputs: character array representing a row; the delimiter character
// Ouputs: date character array; time character array; steps character array
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

// Complete the main function
int main() {
    // Open the CSV file for reading
    FILE *file = fopen("FitnessData_2023.csv", "r");
    if (file == NULL) {
        perror("Error when opening file");
        return 1;
    }

    // Define an array to store the first 3 rows of data
    FITNESS_DATA data[3]; 

    // Read and store the data from the file
    for (int i = 0; i < 3; i++) {
        char record[44]; // Define a maximum length for a line
        if (fgets(record, sizeof(record), file) != NULL) { //Use fgets to read until a new line is found or the maximum length is reached
            char date[11], time[6], steps[10];
            tokeniseRecord(record, ",", date, time, steps);
            strncpy(data[i].date, date, sizeof(data[i].date));
            strncpy(data[i].time, time, sizeof(data[i].time));
            data[i].steps = atoi(steps);

            // Print the data in the specified format
            printf("%s/%s/%d\n", data[i].date, data[i].time, data[i].steps);
        } else {
            printf("Error reading data from file.\n");
            break;
        }
    }

    // Close the file
    fclose(file);

    return 0;
}
