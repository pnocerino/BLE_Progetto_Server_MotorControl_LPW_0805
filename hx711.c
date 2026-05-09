#include "hx711.h"

// Funzione interna per il ritardo in microsecondi tramite Timer Hardware
static void delay_us(TIM_HandleTypeDef *htim, uint16_t us) {
    __HAL_TIM_SET_COUNTER(htim, 0);
    while (__HAL_TIM_GET_COUNTER(htim) < us);
}

// Inizializza la struttura dati e avvia il timer
void HX711_Init(HX711_HandleTypeDef *hx711, TIM_HandleTypeDef *htim, GPIO_TypeDef* DT_Port, uint16_t DT_Pin, GPIO_TypeDef* SCK_Port, uint16_t SCK_Pin) {
    hx711->htim = htim;
    hx711->DT_GPIO_Port = DT_Port;
    hx711->DT_Pin = DT_Pin;
    hx711->SCK_GPIO_Port = SCK_Port;
    hx711->SCK_Pin = SCK_Pin;
    hx711->gain = HX711_GAIN_128; // Default
    hx711->offset = 0;
    hx711->scale = 1.0f;

    HAL_TIM_Base_Start(htim); // Avvia il timer per i microsecondi

    HAL_GPIO_WritePin(hx711->SCK_GPIO_Port, hx711->SCK_Pin, GPIO_PIN_RESET);
    HX711_ReadRaw(hx711); // Lettura a vuoto per impostare il guadagno iniziale
}

// Imposta il guadagno e il canale (verrà applicato alla lettura successiva)
void HX711_SetGain(HX711_HandleTypeDef *hx711, HX711_Gain gain) {
    hx711->gain = gain;
    HX711_ReadRaw(hx711);
}

// Lettura a basso livello dal chip
int32_t HX711_ReadRaw(HX711_HandleTypeDef *hx711) {
    int32_t value = 0;
    uint8_t data[3] = {0};
    uint8_t filler = 0x00;

    // Attesa che il dato sia pronto (DOUT basso)
    while (HAL_GPIO_ReadPin(hx711->DT_GPIO_Port, hx711->DT_Pin) == GPIO_PIN_SET) {
        // Opzionale: inserire un timeout hardware per evitare loop infiniti se il chip si scollega
    }

    // Sezione critica: disabilitazione interrupt per garantire i timing (evitare >60us su SCK)
    __disable_irq();

    // Lettura di 24 bit
    for (int8_t i = 2; i >= 0; i--) {
        for (int8_t j = 7; j >= 0; j--) {
            HAL_GPIO_WritePin(hx711->SCK_GPIO_Port, hx711->SCK_Pin, GPIO_PIN_SET);
            delay_us(hx711->htim, 1); // T3 min > 0.2us

            if (HAL_GPIO_ReadPin(hx711->DT_GPIO_Port, hx711->DT_Pin) == GPIO_PIN_SET) {
                data[i] |= (1 << j);
            }

            HAL_GPIO_WritePin(hx711->SCK_GPIO_Port, hx711->SCK_Pin, GPIO_PIN_RESET);
            delay_us(hx711->htim, 1); // T4 min > 0.2us
        }
    }

    // Impulsi di clock addizionali per il guadagno (1, 2 o 3)
    for (uint8_t i = 0; i < hx711->gain; i++) {
        HAL_GPIO_WritePin(hx711->SCK_GPIO_Port, hx711->SCK_Pin, GPIO_PIN_SET);
        delay_us(hx711->htim, 1);
        HAL_GPIO_WritePin(hx711->SCK_GPIO_Port, hx711->SCK_Pin, GPIO_PIN_RESET);
        delay_us(hx711->htim, 1);
    }

    // Riabilitazione interrupt
    __enable_irq();

    // Costruzione del valore a 32 bit e gestione del segno (complemento a 2)
    data[2] ^= 0x80;
    value = ((uint32_t)data[2] << 16) | ((uint32_t)data[1] << 8) | (uint32_t)data[0];

    // Rimuove l'offset introdotto dallo XOR precedente per ottenere il valore raw effettivo
    value = value - 0x800000;

    return value;
}

// Calcola la media su un numero specificato di letture RAW
int32_t HX711_ReadAverage(HX711_HandleTypeDef *hx711, uint8_t times) {
    int64_t sum = 0;
    for (uint8_t i = 0; i < times; i++) {
        sum += HX711_ReadRaw(hx711);
    }
    return (int32_t)(sum / times);
}

// Esegue l'azzeramento della bilancia
void HX711_Tare(HX711_HandleTypeDef *hx711, uint8_t times) {
    int32_t average = HX711_ReadAverage(hx711, times);
    hx711->offset = average;
}

// Restituisce il peso calibrato
float HX711_GetWeight(HX711_HandleTypeDef *hx711, uint8_t times) {
    int32_t raw_val = HX711_ReadAverage(hx711, times);
    return (float)(raw_val - hx711->offset) / hx711->scale;
}

// Manda il chip in sleep mode
void HX711_PowerDown(HX711_HandleTypeDef *hx711) {
    HAL_GPIO_WritePin(hx711->SCK_GPIO_Port, hx711->SCK_Pin, GPIO_PIN_RESET);
    delay_us(hx711->htim, 1);
    HAL_GPIO_WritePin(hx711->SCK_GPIO_Port, hx711->SCK_Pin, GPIO_PIN_SET);
    delay_us(hx711->htim, 65); // T_powerdown > 60us
}

// Risveglia il chip dallo sleep mode
void HX711_PowerUp(HX711_HandleTypeDef *hx711) {
    HAL_GPIO_WritePin(hx711->SCK_GPIO_Port, hx711->SCK_Pin, GPIO_PIN_RESET);
}
