#ifndef FITNESS_DATA_STRUCT_H
#define FITNESS_DATA_STRUCT_H

typedef struct {
    char date[11];
    char time[6];
    int steps;
} FITNESS_DATA;

// Declare my helper functions here
void tokeniseRecord(const char *input, const char *delimiter, char *date, char *time, char *steps);

#endif
