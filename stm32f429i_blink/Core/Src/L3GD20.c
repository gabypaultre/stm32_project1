#include "L3GD20.h"

// Variables internes (static = privées à ce fichier)
static SPI_HandleTypeDef *L3GD20_hspi = NULL;
static GPIO_TypeDef *L3GD20_CS_Port = NULL;
static uint16_t L3GD20_CS_Pin = 0;

// === Fonction de base ===
void L3GD20_Init(SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port, uint16_t cs_pin)
{
    L3GD20_hspi = hspi;
    L3GD20_CS_Port = cs_port;
    L3GD20_CS_Pin = cs_pin;
}

// Lecture du registre WHO_AM_I
uint8_t L3GD20_ReadWhoAmI(void)
{
    uint8_t tx = L3GD20_WHO_AM_I_ADDR | L3GD20_READ_CMD;
    uint8_t rx = 0;

    HAL_GPIO_WritePin(L3GD20_CS_Port, L3GD20_CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(L3GD20_hspi, &tx, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(L3GD20_hspi, &rx, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(L3GD20_CS_Port, L3GD20_CS_Pin, GPIO_PIN_SET);

    return rx;
}

// Configuration basique du gyroscope
void L3GD20_EnableDefault(void)
{
    uint8_t config[] = {
        L3GD20_CTRL_REG1_ADDR, 0x0F,  // Normal mode, X/Y/Z activés, ODR = 95 Hz
    };

    HAL_GPIO_WritePin(L3GD20_CS_Port, L3GD20_CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(L3GD20_hspi, config, 2, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(L3GD20_CS_Port, L3GD20_CS_Pin, GPIO_PIN_SET);
}

// Lecture brute des 3 axes
L3GD20_Axes L3GD20_ReadAxes(void)
{
    L3GD20_Axes axes = {0};
    uint8_t tx = L3GD20_OUT_X_L_ADDR | L3GD20_READ_CMD | L3GD20_MULTIPLE_READ;
    uint8_t rx[6] = {0};

    HAL_GPIO_WritePin(L3GD20_CS_Port, L3GD20_CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(L3GD20_hspi, &tx, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(L3GD20_hspi, rx, 6, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(L3GD20_CS_Port, L3GD20_CS_Pin, GPIO_PIN_SET);

    axes.x = (int16_t)(rx[1] << 8 | rx[0]);
    axes.y = (int16_t)(rx[3] << 8 | rx[2]);
    axes.z = (int16_t)(rx[5] << 8 | rx[4]);

    return axes;
}

// Conversion brute -> dps
L3GD20_Angles L3GD20_ReadAngularVelocity(void)
{
    L3GD20_Axes raw = L3GD20_ReadAxes();
    L3GD20_Angles angles;

    angles.x_dps = raw.x * L3GD20_SENSITIVITY_250DPS / 1000.0f;
    angles.y_dps = raw.y * L3GD20_SENSITIVITY_250DPS / 1000.0f;
    angles.z_dps = raw.z * L3GD20_SENSITIVITY_250DPS / 1000.0f;

    return angles;
}
