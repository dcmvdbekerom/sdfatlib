// SPI peripheral to interface with SD card.
#define SDDRV_SPI               SPI2

// SPI related port.
#define SDDRV_SPI_PORT          GPIOB

// SPI chip selected (CS) related port.
#define SDDRV_SPI_CS_PORT       GPIOA

// SPI MOSI port pin.
#define SDDRV_SPI_MOSI          GPIO_SPI2_MOSI

// SPI MISO port pin.
#define SDDRV_SPI_MISO          GPIO_SPI2_MISO

// SPI SCK port pin.
#define SDDRV_SPI_SCK           GPIO_SPI2_SCK

// SPI chip select (CS) port pin.
#define SDDRV_SPI_CS            GPIO8

// SPI related clock configurations.
#define SDDRV_SPI_PORT_CLK_ID       RCC_GPIOB
#define SDDRV_SPI_CS_PORT_CLK_ID    RCC_GPIOA
#define SDDRV_SPI_CLK_ID            RCC_SPI2