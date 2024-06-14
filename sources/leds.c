#include "../heads/leds.h"



// Affiche la figure
void displayFigure(int fig[8][8])
{
    for (int i = 0; i < 8; i++)
    {
        bcm2835_gpio_write(RPI_GPIO_P1_26, LOW);
        bcm2835_spi_transfer(i + 1); // Les lignes sur la matrice de LEDs sont indexées de 1 à 8
        bcm2835_spi_transfer(rowToBinary(i, fig)); // Convertit la ligne i en une valeur binaire
        bcm2835_gpio_write(RPI_GPIO_P1_26, HIGH);
    }
}


void emptyMatrice()
{
    for (int i = 0; i < 8; i++)
    {
        bcm2835_gpio_write(RPI_GPIO_P1_26, LOW);
        bcm2835_spi_transfer(i + 1);
        bcm2835_spi_transfer(0x00);
        bcm2835_gpio_write(RPI_GPIO_P1_26, HIGH);
    }
}

void Write_Max7219(unsigned char reg, unsigned char col)
{
    bcm2835_gpio_write(RPI_GPIO_P1_26, LOW);
    bcm2835_spi_transfer(reg);
    bcm2835_spi_transfer(col);
    bcm2835_gpio_write(RPI_GPIO_P1_26, HIGH);
}

void ledSuccess()
{
    int smiley[8][8] = {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0, 1, 1, 0},
        {0, 1, 1, 0, 0, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 1, 0},
        {0, 0, 1, 1, 1, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}
    };
    displayFigure(smiley);
    bcm2835_delay(2000); 
}

void ledError()
{
    int sad[8][8] = {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0, 1, 1, 0},
        {0, 1, 1, 0, 0, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 1, 1, 1, 0, 0},
        {0, 1, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}
    };
    displayFigure(sad);
    bcm2835_delay(2000); 

}

void ledWait()
{
    int wait[8][8] = {
        {1, 1, 1, 1, 1, 1, 1, 1},
        {0, 1, 0, 0, 0, 0, 1, 0},
        {0, 0, 1, 1, 1, 1, 0, 0},
        {0, 0, 0, 1, 1, 0, 0, 0},
        {0, 0, 0, 1, 1, 0, 0, 0},
        {0, 0, 1, 0, 0, 1, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 0},
        {1, 1, 1, 1, 1, 1, 1, 1}
    };

    displayFigure(wait);

}

void ledCountDown(int x)
{
    int digit5[8][8] = {
        {1, 1, 1, 1, 1, 1, 1, 0},
        {1, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 1, 0},
        {1, 0, 0, 0, 0, 1, 1, 0},
        {0, 1, 1, 1, 1, 1, 0, 0}
    };
    int digit4[8][8] = {
        {0, 0, 0, 1, 1, 0, 0, 0},
        {0, 0, 1, 1, 1, 0, 0, 0},
        {0, 1, 0, 1, 1, 0, 0, 0},
        {1, 0, 0, 1, 1, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 1, 1, 0, 0, 0},
        {0, 0, 0, 1, 1, 0, 0, 0},
        {0, 0, 0, 1, 1, 0, 0, 0}
    };
    int digit3[8][8] = {
        {0, 1, 1, 1, 1, 1, 0, 0},
        {1, 0, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0},
        {0, 0, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0},
        {1, 0, 0, 0, 0, 1, 0, 0},
        {0, 1, 1, 1, 1, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}
    };
    int digit2[8][8] = {
        {0, 1, 1, 1, 1, 1, 0, 0},
        {1, 0, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}
    };
    int digit1[8][8] = {
        {0, 0, 0, 1, 1, 0, 0, 0},
        {0, 1, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 1, 1, 0, 0, 0},
        {0, 0, 0, 1, 1, 0, 0, 0},
        {0, 0, 0, 1, 1, 0, 0, 0},
        {0, 0, 0, 1, 1, 0, 0, 0},
        {0, 0, 0, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}
    };

    emptyMatrice();
    switch(x)
    {
        case 5:
            displayFigure(digit5);
            break;
        case 4:
            displayFigure(digit4);
            break;
        case 3:
            displayFigure(digit3);
            break;
        case 2:
            displayFigure(digit2);
            break;
        case 1:
            displayFigure(digit1);
            break;
    }
    



}





// Convertit une ligne de la figure en une valeur binaire
unsigned char rowToBinary(int rowIndex, int fig[8][8])
{
    unsigned char result = 0;
    for (int i = 0; i < 8; i++)
    {
        result <<= 1;
        result |= fig[rowIndex][i];
    }
    return result;
}

void setupLeds()
{

    // Initialisation de la librairie bcm2835
    if (!bcm2835_init())
    {
        printf("bcm2835_init() failed\n");
        return;
    }
    else 
    {
        printf("bcm2835_init() success\n");
    }

    // Initialisation de la communication SPI
    if (!bcm2835_spi_begin())
    {
        printf("bcm2835_spi_begin() failed\n");
        bcm2835_close();
        return;
    }
    else 
    {
        printf("bcm2835_spi_begin() success\n");
    }


    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST); // MSB Premier
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0); // Mode 0
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_256); // 256
    bcm2835_gpio_fsel(RPI_GPIO_P1_26, BCM2835_GPIO_FSEL_OUTP); // GPIO 8
    bcm2835_gpio_write(0x84, HIGH); // GPIO 8


    Write_Max7219(0x09, 0x00); // Disable decoding
    Write_Max7219(0x0a, 0x03); // Set brightness to low
    Write_Max7219(0x0b, 0x07); // Scan limit = 8 LEDs
    Write_Max7219(0x0c, 0x01); // Turn on the display
    Write_Max7219(0x0f, 0x00); // Disable test display

    

    
}

void closeLeds()
{
    bcm2835_spi_end();
    bcm2835_close();
}
