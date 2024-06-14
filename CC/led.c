#include <bcm2835.h>

unsigned char down[8] = {0x00, 0x00, 0x00, 0x00, 0x18, 0x3C, 0x7E, 0xFF};

int main()
{
        if (!bcm2835_init())
        d("bcm2835_init() failed\n", NULL);
    bcm2835_spi_begin();
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);    // The default
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                    // The default
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_256); // The default

    bcm2835_gpio_fsel(RPI_GPIO_P1_26, BCM2835_GPIO_FSEL_OUTP);

    bcm2835_gpio_write(0x84, HIGH);

    bcm2835_delay(50);

    Write_Max7219(0x09, 0x00, 0x09, 0x00);
    Write_Max7219(0x0a, 0x03, 0x0a, 0x03);
    Write_Max7219(0x0b, 0x07, 0x0b, 0x07);
    Write_Max7219(0x0c, 0x01, 0x0c, 0x01);
    Write_Max7219(0x0f, 0x00, 0x0f, 0x00);

        for (int i = 1; i < 9; i++)
        {
            bcm2835_gpio_write(RPI_GPIO_P1_26, LOW);
            bcm2835_spi_transfer(i);
            bcm2835_spi_transfer(down[i - 1]);
            bcm2835_gpio_write(RPI_GPIO_P1_26, HIGH);
        }

        bcm2835_spi_end();
    bcm2835_close();

}
