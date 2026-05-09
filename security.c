#include "security.h"
#include "log_module.h"
#include "rtc.h"
#include "rng.h"
#include "aes.h"

static uint32_t aes_iv[4] = { 0x01234567, 0x89ABCDEF, 0x11223344, 0x55667788 };

uint8_t TAMP_KEK_Is_Empty(void) {
	uint32_t magic_word = HAL_RTCEx_BKUPRead(&hrtc, BKP_REG_MAGIC_WORD);

	if (magic_word == KEK_MAGIC_WORD) {
		return 0;
	}
	return 1;
}

void Generate_And_Store_KEK_in_TAMP(void) {
	uint32_t random_kek[4];
	LOG_INFO_APP("==>> SICUREZZA: Risveglio forzato dell'oscillatore HSI...\n");

	__HAL_RCC_HSI_ENABLE();

	while (__HAL_RCC_GET_FLAG(RCC_FLAG_HSIRDY) == RESET) {
	}

	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RNG;
	PeriphClkInit.RngClockSelection = RCC_RNGCLKSOURCE_HSI;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		LOG_INFO_APP("ERRORE: Fallito il routing del clock all'RNG!\n");
	}

	HAL_RNG_DeInit(&hrng);
	hrng.Init.ClockErrorDetection = RNG_CED_ENABLE;
	if (HAL_RNG_Init(&hrng) != HAL_OK) {
		LOG_INFO_APP("ERRORE: Inizializzazione RNG flit!\n");
	}

	LOG_INFO_APP("==>> SICUREZZA: Clock stabile. Generazione in corso...\n");
	for (int i = 0; i < 4; i++) {
		if (HAL_RNG_GenerateRandomNumber(&hrng, &random_kek[i]) != HAL_OK) {
			LOG_INFO_APP(
					"ERRORE CRITICO: RNG Fallito! Codice Errore RNG: 0x%08lX\n",
					hrng.ErrorCode);

			LOG_INFO_APP("Stato RNG: 0x%02X\n", HAL_RNG_GetState(&hrng));
			Error_Handler();
		}
	}

	HAL_PWR_EnableBkUpAccess();

	HAL_RTCEx_BKUPWrite(&hrtc, BKP_REG_KEK_WORD_1, random_kek[0]);
	HAL_RTCEx_BKUPWrite(&hrtc, BKP_REG_KEK_WORD_2, random_kek[1]);
	HAL_RTCEx_BKUPWrite(&hrtc, BKP_REG_KEK_WORD_3, random_kek[2]);
	HAL_RTCEx_BKUPWrite(&hrtc, BKP_REG_KEK_WORD_4, random_kek[3]);

	HAL_RTCEx_BKUPWrite(&hrtc, BKP_REG_MAGIC_WORD, KEK_MAGIC_WORD);

	//HAL_PWR_DisableBkUpAccess();

}

void Read_KEK_From_TAMP(uint8_t *p_kek_buffer) {
	uint32_t kek_word;

	kek_word = HAL_RTCEx_BKUPRead(&hrtc, BKP_REG_KEK_WORD_1);
	memcpy(&p_kek_buffer[0], &kek_word, 4);

	kek_word = HAL_RTCEx_BKUPRead(&hrtc, BKP_REG_KEK_WORD_2);
	memcpy(&p_kek_buffer[4], &kek_word, 4);

	kek_word = HAL_RTCEx_BKUPRead(&hrtc, BKP_REG_KEK_WORD_3);
	memcpy(&p_kek_buffer[8], &kek_word, 4);

	kek_word = HAL_RTCEx_BKUPRead(&hrtc, BKP_REG_KEK_WORD_4);
	memcpy(&p_kek_buffer[12], &kek_word, 4);
}

void AES_Encrypt_Buffer(uint8_t *buffer, uint32_t size_in_bytes, uint8_t *kek) {
	hcryp.Init.DataType = CRYP_DATATYPE_8B;
	hcryp.Init.KeySize = CRYP_KEYSIZE_128B;
	hcryp.Init.pKey = (uint32_t*) kek;
	hcryp.Init.Algorithm = CRYP_AES_CBC;
	hcryp.Init.pInitVect = aes_iv;
	hcryp.Init.DataWidthUnit = CRYP_DATAWIDTHUNIT_BYTE;

	if (HAL_CRYP_Init(&hcryp) != HAL_OK) {
		Error_Handler();
	}

	if (HAL_CRYP_Encrypt(&hcryp, (uint32_t*) buffer, size_in_bytes,
			(uint32_t*) buffer, HAL_MAX_DELAY) != HAL_OK) {
		Error_Handler();
	}
}

void AES_Decrypt_Buffer(uint8_t *buffer, uint32_t size_in_bytes, uint8_t *kek) {
	hcryp.Init.DataType = CRYP_DATATYPE_8B;
	hcryp.Init.KeySize = CRYP_KEYSIZE_128B;
	hcryp.Init.pKey = (uint32_t*) kek;
	hcryp.Init.Algorithm = CRYP_AES_CBC;
	hcryp.Init.pInitVect = aes_iv;
	hcryp.Init.DataWidthUnit = CRYP_DATAWIDTHUNIT_BYTE;

	if (HAL_CRYP_Init(&hcryp) != HAL_OK) {
		Error_Handler();
	}

	if (HAL_CRYP_Decrypt(&hcryp, (uint32_t*) buffer, size_in_bytes,
			(uint32_t*) buffer, HAL_MAX_DELAY) != HAL_OK) {
		Error_Handler();
	}
}

