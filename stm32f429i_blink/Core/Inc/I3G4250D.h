#ifndef I3G4250D_H
#define I3G4250D_H

#include "stm32f4xx_hal.h"

#define I3G4250D_READ_CMD            0x80
#define I3G4250D_WRITE_CMD           0x00
#define I3G4250D_MULTI_BYTE          0x40

#define I3G4250D_WHO_AM_I_ADDR       0x0F
#define I3G4250D_CTRL_REG1_ADDR      0x20
#define I3G4250D_CTRL_REG4_ADDR      0x23
#define I3G4250D_OUT_X_L             0x28

#define I3G4250D_WHO_AM_I_EXPECTED   0xD3

#define I3G4250D_DR_100HZ            0x00
#define I3G4250D_BW_25HZ             0x01
#define I3G4250D_POWER_ON_XYZ        0x0F
#define I3G4250D_CTRL_REG1_CFG      (I3G4250D_DR_100HZ | I3G4250D_BW_25HZ | I3G4250D_POWER_ON_XYZ)

#define I3G4250D_FS_245DPS           0x00
#define I3G4250D_FS_500DPS           0x10
#define I3G4250D_FS_2000DPS          0x20

#define I3G4250D_SENSITIVITY_245DPS  8.75f
#define I3G4250D_SENSITIVITY_500DPS  17.5f
#define I3G4250D_SENSITIVITY_2000DPS 70.0f

uint8_t I3G4250D_Init(SPI_HandleTypeDef *hspi, GPIO_TypeDef *CS_GPIO_Port, uint16_t CS_Pin);
void    I3G4250D_Configure(void);
uint8_t I3G4250D_ReadRegister(uint8_t regAddr);
void    I3G4250D_WriteRegister(uint8_t regAddr, uint8_t value);
uint8_t I3G4250D_ReadWhoAmI(void);
void    I3G4250D_ReadXYZ(int16_t *x, int16_t *y, int16_t *z);
void    I3G4250D_ReadXYZ_dps(float *x, float *y, float *z);

#endif /* I3G4250D_H */
