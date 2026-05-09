#ifndef APP_SECURITY_H_
#define APP_SECURITY_H_

#include <stdio.h>
#include "simple_nvm_arbiter.h"

#define KEK_MAGIC_WORD       0xA5A5A5A5

#define BKP_REG_MAGIC_WORD   RTC_BKP_DR0
#define BKP_REG_KEK_WORD_1   RTC_BKP_DR1
#define BKP_REG_KEK_WORD_2   RTC_BKP_DR2
#define BKP_REG_KEK_WORD_3   RTC_BKP_DR3
#define BKP_REG_KEK_WORD_4   RTC_BKP_DR4


uint8_t TAMP_KEK_Is_Empty(void);
void Generate_And_Store_KEK_in_TAMP(void);
void Read_KEK_From_TAMP(uint8_t *p_kek_buffer);
void APP_BLE_HostNvmStore(void);
void AES_Encrypt_Buffer(uint8_t *buffer, uint32_t size_in_bytes, uint8_t *kek);
void AES_Decrypt_Buffer(uint8_t *buffer, uint32_t size_in_bytes, uint8_t *kek);

#endif
