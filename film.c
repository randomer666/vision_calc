#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#define FORMAT_NUMBER 7

typedef struct {
    float length;
    float frames;
    char format[20]; // Using char array instead of CS50 string type
    float frames_per_foot;
    float price;
} film;

film formats[FORMAT_NUMBER];

float calc_length(float run_time, float fps, film arr[], int selectedFormat);

// Function to retrieve URL based on film format and length
const char* getPriceURL(int selectedFormat, float length);

int main(void) {
    // Populate film formats array
    strcpy(formats[0].format, "8mm");
    formats[0].frames_per_foot = 80.0;

    strcpy(formats[1].format, "Super 8");
    formats[1].frames_per_foot = 72.0;

    strcpy(formats[2].format, "16mm");
    formats[2].frames_per_foot = 40.0;

    strcpy(formats[3].format, "35x2 perf");
    formats[3].frames_per_foot = 32.0;

    strcpy(formats[4].format, "35x3 perf");
    formats[4].frames_per_foot = 21.33;

    strcpy(formats[5].format, "35x4 perf");
    formats[5].frames_per_foot = 16.0;

    strcpy(formats[6].format, "65");
    formats[6].frames_per_foot = 12.8;

    float run_time;
    printf("Enter the length of your project (in minutes): ");
    scanf("%f", &run_time);

    printf("Choose a stock:\n");
    printf("1 - B/W\n");
    printf("2 - Vision3\n");
    printf("Your choice: ");

    char c;
    scanf(" %c", &c); // Use space before %c to consume any whitespace characters
    if (c == '1') {
        printf("B/W stock\n");
        // Handle B/W stock selection
    } else if (c == '2') {
        printf("Vision 3 stock\n");
        // Handle Vision 3 stock selection
    }

    printf("Choose a format:\n");
    for (int i = 0; i < FORMAT_NUMBER; i++) {
        printf("%d - %s\n", i + 1, formats[i].format);
    }
    int format_choice;
    printf("Enter the format number: ");
    scanf("%d", &format_choice);

    if (format_choice >= 1 && format_choice <= FORMAT_NUMBER) {
        int selectedFormat = format_choice - 1; // Adjust for array indexing
        float fps;
        printf("Enter the fps of the project: ");
        scanf("%f", &fps);

        if (selectedFormat >= 0 && selectedFormat < FORMAT_NUMBER) {
            float length = calc_length(run_time, fps, formats, selectedFormat);
            printf("Length of film needed: %.2f feet\n", length);
            
        // Calculate short ends length based on film length
if (length <= 100.0) {
    printf("You only need one 100ft roll\n");
    printf("Short ends length = %d feet\n", 100 - (int)length);
} else if (length <= 400.0) {
    printf("You need one 400ft roll\n");
    printf("Short ends length = %d feet\n", 400 - (int)length);
} else if(length < 1800.0 ) {
    int rolls400 = (int)length / 400;
    int remainingLength = (int)length % 400;

    if (remainingLength > 100) {
        printf("You need %d rolls of 400ft film\n", rolls400+1);
        printf("Short ends length = %d feet\n", 400 - remainingLength);
    } else if (remainingLength > 0) {
        printf("You need %d rolls of 400ft film and one 100ft roll\n", rolls400);
        printf("Short ends length = %d feet\n", 100 - remainingLength);
    } else {
        printf("You need %d rolls of 400ft film\n", rolls400);
        printf("No short ends\n");
    }
}

            
    } else {
        printf("Invalid format choice\n");
    }
} else {
    printf("Invalid format choice\n");
}

return 0;
}

float calc_length(float run_time, float fps, film arr[], int selectedFormat) {
    // Fetch frames_per_foot based on the selected format
    if (selectedFormat >= 0 && selectedFormat < FORMAT_NUMBER) {
        float frames_per_foot = arr[selectedFormat].frames_per_foot;
        float length = (run_time * ((fps * 60) / frames_per_foot));
        length = ceil(length); // Round up to the next whole number
        return length;
    } else {
        return -1.0; // Invalid format index
    }
}

//Add a function to specify the shooting ratio e.g 20:1

