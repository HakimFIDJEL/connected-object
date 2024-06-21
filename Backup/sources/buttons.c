#include "../heads/buttons.h"
#include <bcm2835.h>

#define ROWS 4
#define COLS 4

int rowPins[ROWS] = {27,22, 5, 6};
int colPins[COLS] = {25, 26, 19, 13};



void setupButtons() {

    for (int i = 0; i < ROWS; i++) {
        bcm2835_gpio_fsel(rowPins[i], BCM2835_GPIO_FSEL_INPT);
        bcm2835_gpio_set_pud(rowPins[i], BCM2835_GPIO_PUD_UP);
        printf("Row pin %d set as INPUT with pull-up\n", rowPins[i]);
    }

    for (int j = 0; j < COLS; j++) {
        bcm2835_gpio_fsel(colPins[j], BCM2835_GPIO_FSEL_OUTP);
        bcm2835_gpio_write(colPins[j], HIGH);
        printf("Column pin %d set as OUTPUT and set HIGH\n", colPins[j]);
    }
}

Position readButtons() {

    Position position = {-1, -1};

    for (int j = 0; j < COLS; j++) {
        // Set all columns to HIGH
        for (int k = 0; k < COLS; k++) {
            bcm2835_gpio_write(colPins[k], HIGH);
        }
        
        // Set current column to LOW
        bcm2835_gpio_write(colPins[j], LOW);
        //printf("Column pin %d set to LOW\n", colPins[j]);

        for (int i = 0; i < ROWS; i++) {
            uint8_t state = bcm2835_gpio_lev(rowPins[i]);
            //printf("Reading row pin %d: %d\n", rowPins[i], state);

            if (state == LOW) {
                bcm2835_delay(50);  // Debouncing delay
                if (bcm2835_gpio_lev(rowPins[i]) == LOW) {  // Confirm button press
                    while (bcm2835_gpio_lev(rowPins[i]) == LOW);
                    bcm2835_gpio_write(colPins[j], HIGH);
                    printf("Button at row %d, column %d pressed\n", i, j);
                    position.row = i;
                    position.col = j;
                    return position;
                }
            }
        }

        bcm2835_gpio_write(colPins[j], HIGH);
    }

    return position;
}
