#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DATA_INTERVAL_MS 10 // Data collection interval in milliseconds
#define SIMULATION_DURATION 300 // Simulation duration in seconds (5 minutes)
#define NUM_SAMPLES (SIMULATION_DURATION * 1000 / DATA_INTERVAL_MS)

void generateAccelerometerData(int *x, int *y, int *z, int step) {
    // Simulate accelerometer data generation 
    *x = rand() % 12000 - 1000;
    *y = rand() % 5000 - 1000;
    *z = rand() % 2000 - 1000;

    // Introduce steps at specific intervals
    if (step % 500 == 0) {
        *x += 3000; // Simulate a step in the x-axis
    }
    if (step % 700 == 0) {
        *y += 3000; // Simulate a step in the y-axis
    }
}

int main() {
    srand(time(NULL));

    // Open the CSV file for writing
    FILE *csvFile = fopen("accelerometer_data.csv", "w");

    if (csvFile == NULL) {
        perror("Error opening file");
        return -1;
    }

    // Print header in the CSV file
    fprintf(csvFile, "Timestamp,X-Axis,Y-Axis,Z-Axis\n");

    for (int i = 0; i < NUM_SAMPLES; i++) {
        int x, y, z;
        generateAccelerometerData(&x, &y, &z, i);

        // Simulate timestamp based on data interval
        int timestamp = i * DATA_INTERVAL_MS;

        // Print simulated data to the CSV file
        fprintf(csvFile, "%d,%d,%d,%d\n", timestamp, x, y, z);

        // Add a delay to simulate real-time data collection
        // (This is optional and can be adjusted based on your requirements)
        // Sleep function might not be available on all platforms. If not, you can use an alternative.
        // For example: #include <unistd.h> and use usleep(1000 * DATA_INTERVAL_MS);
        // Note: usleep is deprecated, and nanosleep is recommended, but it's more complex to use.
        // Please refer to the documentation for your platform.
        // sleep(DATA_INTERVAL_MS / 1000);
    }

    // Close the CSV file
    fclose(csvFile);

    return 0;
}
