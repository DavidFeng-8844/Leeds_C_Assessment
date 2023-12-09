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