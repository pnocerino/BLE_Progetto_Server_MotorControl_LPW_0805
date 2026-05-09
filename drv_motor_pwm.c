#include "drv_motor_pwm.h"

void Drv_Motor_Init(MotorPWM_Handle_t *h_motor, TIM_HandleTypeDef *htim, uint32_t channel) {
    h_motor->htim = htim;
    h_motor->channel = channel;
    HAL_TIM_PWM_Start(htim, channel);
}

void Drv_Motor_SetRaw(MotorPWM_Handle_t *h_motor, uint32_t raw_pwm_value) {
    // Utilizziamo la macro per efficienza, ma incapsulata qui
    __HAL_TIM_SET_COMPARE(h_motor->htim, h_motor->channel, raw_pwm_value);
}

void Drv_Motor_Stop(MotorPWM_Handle_t *h_motor) {
    // Implementazione di sicurezza: ferma il PWM o imposta a 0
    __HAL_TIM_SET_COMPARE(h_motor->htim, h_motor->channel, 757);
}
