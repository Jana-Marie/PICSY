#include "main.h"
#include "i2c_client.h"
#include "i2c_registermap.h"

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

I2C_HandleTypeDef hi2c1;
DMA_HandleTypeDef hdma_i2c1_rx;
DMA_HandleTypeDef hdma_i2c1_tx;

TIM_HandleTypeDef htim1;

enum dither_states {DITHER_0 = 0, DITHER_1, DITHER_2};
uint16_t dither_bins[3] = {4095, 2112, 1450};

enum palette_states {PALETTE_0 = 0, PALETTE_1, PALETTE_2, PALETTE_3, PALETTE_4, PALETTE_5};
uint16_t palette_bins[6] = {4095, 2112, 1450, 465, 233, 794};

enum exposure_states {EXPOSURE_0 = 0, EXPOSURE_1, EXPOSURE_2, EXPOSURE_3, EXPOSURE_4};
uint16_t exposure_bins[4] = {4095, 2112, 1450, 605}; // wip, needs to be 5, ordered wrong switch

struct analog_t {
  uint16_t adc[4];
  uint16_t vbat;
  uint8_t binsize;
  uint8_t dither;
  uint8_t palette;
  uint8_t exposure;
} analog = {.binsize = 50};

struct trigger_t {
  uint8_t flag;
  uint16_t r_count;
  uint16_t l_count;
} trigger = {.flag = 0, .r_count = 0, .l_count = 0};

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2C1_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM1_Init(void);
void Error_Handler(void);

i2c_client_init_t i2c_init = {.hi2c = &hi2c1, .error_handler = &Error_Handler};

int main(void)
{
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_ADC1_Init();
  MX_TIM1_Init();

  HAL_TIM_Base_Start_IT(&htim1);
  HAL_ADC_Start_DMA(&hadc1, (uint32_t *) &analog.adc[0], 4);

  I2C_Client_Init(&i2c_init);

  I2C_Set_Register(CHIPID01, 13);
  I2C_Set_Register(CHIPID02, 12);

  I2C_Set_Register(FWVERSMA, 0);
  I2C_Set_Register(FWVERSMI, 1);
  I2C_Set_Register(FWVERSPA, 0);

  I2C_Set_Register(DISPLRST, 0);
  I2C_Set_Register(DISPLCSA, 0);
  I2C_Set_Register(DISPLCSB, 0);

  while (1)
  {
    if(trigger.flag == 1){
      trigger.flag = 0;
      HAL_GPIO_WritePin(GPIOB, INT_OUT_Pin, GPIO_PIN_RESET);
      HAL_Delay(1);
      HAL_GPIO_WritePin(GPIOB, INT_OUT_Pin, GPIO_PIN_SET);
    }

    for(uint8_t i = 0; i < sizeof(dither_bins)/2; i++){
      if(analog.adc[1] >= dither_bins[i]-analog.binsize && analog.adc[1] <= dither_bins[i]+analog.binsize){
        // WIP INT flags and sticky int pin
        //if(i != analog.dither){
        //  // set flag and IO
        //}
        analog.dither = i;
      }
    }
    for(uint8_t i = 0; i < sizeof(palette_bins)/2; i++){
      if(analog.adc[2] >= palette_bins[i]-analog.binsize && analog.adc[2] <= palette_bins[i]+analog.binsize){
        // WIP INT flags and sticky int pin
        //if(i != analog.palette){
        //  // set flag and IO
        //}
        analog.palette = i;
      }
    }
    for(uint8_t i = 0; i < sizeof(exposure_bins)/2; i++){
      if(analog.adc[3] >= exposure_bins[i]-analog.binsize && analog.adc[3] <= exposure_bins[i]+analog.binsize){
        // WIP INT flags and sticky int pin
        //if(i != analog.exposure){
        //  // set flag and IO
        //}
        analog.exposure = i;
      }
    }

    analog.vbat = (analog.adc[0]*9900/8192);

    // Return dial positions
    I2C_Set_Register(RSDITHER, analog.dither);
    I2C_Set_Register(RSPALETT, analog.palette);
    I2C_Set_Register(RSEXPOSU, analog.exposure);

    // Return battery level
    I2C_Set_Register(VBATTERY, analog.vbat);

    // Read & return trigger IO, mostly used for debug purporses right now
    I2C_Set_Register(TRIGGERA, HAL_GPIO_ReadPin(GPIOA, TRIGGER_A_Pin));
    I2C_Set_Register(TRIGGERB, HAL_GPIO_ReadPin(GPIOA, TRIGGER_B_Pin));

    // Return trigger counts
    I2C_Set_Register(TRIACNTA, (uint8_t)(trigger.l_count >> 8) & 0xFF);
    I2C_Set_Register(TRIACNTB, (uint8_t)(trigger.l_count & 0xFF));

    I2C_Set_Register(TRIBCNTA, (uint8_t)(trigger.r_count >> 8) & 0xFF);
    I2C_Set_Register(TRIBCNTB, (uint8_t)(trigger.r_count & 0xFF));

    // Display related IO
    HAL_GPIO_WritePin(GPIOB, DISPLAY_RST_Pin, (!I2C_Get_Register(DISPLRST)) & 0x01);
    HAL_GPIO_WritePin(GPIOA, DISPLAY_1_CS_Pin, (!I2C_Get_Register(DISPLCSA)) & 0x01);
    HAL_GPIO_WritePin(GPIOA, DISPLAY_2_CS_Pin, (!I2C_Get_Register(DISPLCSB)) & 0x01);
  }
}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == TRIGGER_A_Pin) {
    trigger.flag = 1;
    trigger.l_count++;
  } else if(GPIO_Pin == TRIGGER_B_Pin) {
    trigger.flag = 1;
    trigger.r_count++;
  } else {
    __NOP();
  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);
}

static void MX_ADC1_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.LowPowerAutoPowerOff = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 4;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIG_T1_TRGO2;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  hadc1.Init.SamplingTimeCommon1 = ADC_SAMPLETIME_19CYCLES_5;
  hadc1.Init.SamplingTimeCommon2 = ADC_SAMPLETIME_1CYCLE_5;
  hadc1.Init.OversamplingMode = DISABLE;
  hadc1.Init.TriggerFrequencyMode = ADC_TRIGGER_FREQ_HIGH;
  HAL_ADC_Init(&hadc1);

  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = ADC_REGULAR_RANK_3;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_REGULAR_RANK_4;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
}

static void MX_I2C1_Init(void)
{
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00300617;
  hi2c1.Init.OwnAddress1 = 84;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  HAL_I2C_Init(&hi2c1);
  HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE);
  HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0);
}

static void MX_TIM1_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 10;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 15999;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  HAL_TIM_Base_Init(&htim1);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig);
}

static void MX_DMA_Init(void)
{
  __HAL_RCC_DMA1_CLK_ENABLE();

  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOA, DISPLAY_1_CS_Pin|DISPLAY_2_CS_Pin, GPIO_PIN_SET);

  HAL_GPIO_WritePin(GPIOB, DISPLAY_RST_Pin|INT_OUT_Pin, GPIO_PIN_SET);

  GPIO_InitStruct.Pin = TRIGGER_A_Pin|TRIGGER_B_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = DISPLAY_1_CS_Pin|DISPLAY_2_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = DISPLAY_RST_Pin|INT_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}