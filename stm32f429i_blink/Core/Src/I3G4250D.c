#include "I3G4250D.h"

static SPI_HandleTypeDef *I3G4250D_hspi = NULL;
static GPIO_TypeDef *I3G4250D_CS_Port = NULL;
static uint16_t I3G4250D_CS_Pin = 0;

uint8_t I3G4250D_Init(SPI_HandleTypeDef *hspi, GPIO_TypeDef *CS_GPIO_Port, uint16_t CS_Pin)
{
    I3G4250D_hspi = hspi;
    I3G4250D_CS_Port = CS_GPIO_Port;
    I3G4250D_CS_Pin = CS_Pin;
    uint8_t whoami = I3G4250D_ReadWhoAmI();
    return (whoami == I3G4250D_WHO_AM_I_EXPECTED);
}

void I3G4250D_Configure(void)
{
    I3G4250D_WriteRegister(I3G4250D_CTRL_REG1_ADDR, I3G4250D_CTRL_REG1_CFG);
    I3G4250D_WriteRegister(I3G4250D_CTRL_REG4_ADDR, I3G4250D_FS_245DPS);
}

uint8_t I3G4250D_ReadRegister(uint8_t regAddr)
{
    uint8_t tx = I3G4250D_READ_CMD | regAddr;
    uint8_t rx = 0;
    HAL_GPIO_WritePin(I3G4250D_CS_Port, I3G4250D_CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(I3G4250D_hspi, &tx, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(I3G4250D_hspi, &rx, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(I3G4250D_CS_Port, I3G4250D_CS_Pin, GPIO_PIN_SET);
    return rx;
}

void I3G4250D_WriteRegister(uint8_t regAddr, uint8_t value)
{
    uint8_t tx[2] = { I3G4250D_WRITE_CMD | regAddr, value };
    HAL_GPIO_WritePin(I3G4250D_CS_Port, I3G4250D_CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(I3G4250D_hspi, tx, 2, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(I3G4250D_CS_Port, I3G4250D_CS_Pin, GPIO_PIN_SET);
}

uint8_t I3G4250D_ReadWhoAmI(void)
{
    return I3G4250D_ReadRegister(I3G4250D_WHO_AM_I_ADDR);
}

void I3G4250D_ReadXYZ(int16_t *x, int16_t *y, int16_t *z)
{
    uint8_t tx = I3G4250D_READ_CMD | I3G4250D_MULTI_BYTE | I3G4250D_OUT_X_L;
    uint8_t rx[6];
    HAL_GPIO_WritePin(I3G4250D_CS_Port, I3G4250D_CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(I3G4250D_hspi, &tx, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(I3G4250D_hspi, rx, 6, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(I3G4250D_CS_Port, I3G4250D_CS_Pin, GPIO_PIN_SET);
    *x = (int16_t)(rx[1] << 8 | rx[0]);
    *y = (int16_t)(rx[3] << 8 | rx[2]);
    *z = (int16_t)(rx[5] << 8 | rx[4]);
}

void I3G4250D_ReadXYZ_dps(float *x, float *y, float *z)
{
    int16_t rawX, rawY, rawZ;
    I3G4250D_ReadXYZ(&rawX, &rawY, &rawZ);
    float sensitivity = I3G4250D_SENSITIVITY_245DPS;
    *x = rawX * sensitivity / 1000.0f;
    *y = rawY * sensitivity / 1000.0f;
    *z = rawZ * sensitivity / 1000.0f;
}
