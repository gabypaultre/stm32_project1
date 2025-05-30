#ifndef L3GD20_H_
#define L3GD20_H_

#include "stm32f4xx_hal.h"

// === Constantes ===
#define L3GD20_WHO_AM_I_ADDR        0x0F
#define L3GD20_WHO_AM_I_EXPECTED    0xD3

#define L3GD20_READ_CMD             0x80  // Bit 7 à 1 = lecture
#define L3GD20_MULTIPLE_READ        0x40  // Bit 6 à 1 = auto-incrément

// === Registres L3GD20 ===
#define L3GD20_CTRL_REG1_ADDR       0x20
#define L3GD20_CTRL_REG4_ADDR       0x23
#define L3GD20_OUT_X_L_ADDR         0x28  // Base pour lecture des 6 axes

// === Sensibilité pour conversion en dps (degrés/seconde)
#define L3GD20_SENSITIVITY_250DPS   8.75f

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} L3GD20_Axes;

typedef struct {
    float x_dps;
    float y_dps;
    float z_dps;
} L3GD20_Angles;

// === Fonctions publiques ===
void L3GD20_Init(SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port, uint16_t cs_pin);
uint8_t L3GD20_ReadWhoAmI(void);
void L3GD20_EnableDefault(void);
L3GD20_Axes L3GD20_ReadAxes(void);
L3GD20_Angles L3GD20_ReadAngularVelocity(void);

#endif // L3GD20_H_
