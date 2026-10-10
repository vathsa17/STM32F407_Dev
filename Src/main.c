#include "stm32f407xx.h"
#include <string.h>
#define RCC_AHB1ENR   (*(volatile uint32_t *)0x40023830)

GPIO_PinConf_t UserButton_Conf;

GPIO_PinConf_t IRSensor_Conf;

GPIO_PinConf_t CAN_TX;
GPIO_PinConf_t CAN_RX;
TIM_Base_InitTypeDef TIM6_BaseConf;
USART_Conf_t USART2_Conf;
USART_Conf_t USART3_Conf;
volatile bool isTransmitt=FALSE;
#define RX_BUFFER_SIZE 8U
#define TX_BUFFER_SIZE 8U
#define BUTTON_DEBOUCE_TIME 1000U
volatile uint8_t RecievedMessage[RX_BUFFER_SIZE];
volatile uint8_t TransmitMessage[TX_BUFFER_SIZE];
volatile uint8_t motion_detected = 0;
volatile uint8_t IsRxAvailable =FALSE;
volatile uint8_t ButtonEvent = FALSE;
volatile uint8_t TxMessageSize=2U;
volatile uint8_t RxIndex=0U;
volatile uint16_t Timer6Counter=0;
char tx_msg[] = "No GPS Lock\r\n";
volatile uint8_t RingBufferOverflow = 0;
RingBuffer_t RingBuffer;
NMEA_Parser_t nmea_parser;



void USART2_Init(void)
{
	GPIO_PinConf_t USART_Pin;
	USART_Pin.GPIO_PinMode=GPIO_MODE_ALT;
	USART_Pin.GPIO_PUPD=GPIO_NO_PUPD;
	USART_Pin.GPIO_OutType=GPIO_OutType_PP;
	USART_Pin.GPIO_OutSpeed=GPIO_OutSpeed_Fast;
	USART_Pin.GPIO_AltFnc=GPIO_AF7;
	GPIOA_CLK_ENB();

	USART_Pin.GPIO_PinNumber=GPIO_PIN_NUM_2;
	GPIO_Init(GPIOA,USART_Pin);
	USART_Pin.GPIO_PinNumber=GPIO_PIN_NUM_3;
	GPIO_Init(GPIOA,USART_Pin);

	USART2_Conf.Mode=USART_MODE_RX_TX;
	USART2_Conf.Parity=USART_PARITY_NONE;
	USART2_Conf.StopBits=USART_STOPBITS_1;
	USART2_Conf.WordLenght=USART_WORDLENGTH_8B;
	USART2_Conf.OverSampleing=0U; /*OverSampling by 16*/
	USART2_Conf.BaudRate=USART_BAUDRATE_9600;
	USART2_CLK_ENB();
	USART_Init(USART2,USART2_Conf);

	
	NVIC_SetPriority(IRQ_NO_USART2,0U);
	NVIC_EnableIRQ(IRQ_NO_USART2);
	USART2_RXNEIE_ENB();
	




}


void USART3_Init(void)
{
	GPIO_PinConf_t USART_Pin;
	USART_Pin.GPIO_PinMode=GPIO_MODE_ALT;
	USART_Pin.GPIO_PUPD=GPIO_NO_PUPD;
	USART_Pin.GPIO_OutType=GPIO_OutType_PP;
	USART_Pin.GPIO_OutSpeed=GPIO_OutSpeed_Fast;
	USART_Pin.GPIO_AltFnc=GPIO_AF7;
	GPIOD_CLK_ENB();

	USART_Pin.GPIO_PinNumber=GPIO_PIN_NUM_8;
	GPIO_Init(GPIOD,USART_Pin);
	USART_Pin.GPIO_PinNumber=GPIO_PIN_NUM_9;
	GPIO_Init(GPIOD,USART_Pin);

	USART3_Conf.Mode=USART_MODE_RX_TX;
	USART3_Conf.Parity=USART_PARITY_NONE;
	USART3_Conf.StopBits=USART_STOPBITS_1;
	USART3_Conf.WordLenght=USART_WORDLENGTH_8B;
	USART3_Conf.OverSampleing=0U; /*OverSampling by 16*/
	USART3_Conf.BaudRate=USART_BAUDRATE_9600;
	USART3_CLK_ENB();
	USART_Init(USART3,USART3_Conf);
}
void BlueLED_Init()
{
	GPIO_PinConf_t GPIOD_PinConf = {GPIO_PIN_NUM_15,GPIO_MODE_OUTPUT,GPIO_OutType_PP,GPIO_OutSpeed_Low,GPIO_NO_PUPD};

	// Enable clock for GPIOD
    //RCC_AHB1ENR |= (1 << 3);
	GPIOD_CLK_ENB();
	GPIO_Init(GPIOD,GPIOD_PinConf);
}

/**
 * @brief Function Initilizes the User Button
 *
 */

void UserButton_Init()
{

    UserButton_Conf.GPIO_PinNumber=GPIO_PIN_NUM_0;
    UserButton_Conf.GPIO_PinMode=GPIO_MODE_INPUT;
    UserButton_Conf.GPIO_PUPD=GPIO_NO_PUPD;
    UserButton_Conf.GPIO_EdgeTrigger=GPIO_IT_EDGE_RFT;
    GPIOA_CLK_ENB();
    GPIO_Init(GPIOA,UserButton_Conf);
    GPIO_IT_Init(GPIOA,UserButton_Conf,1);

}

void CAN_GPIO_Init()
{
	CAN_TX.GPIO_PinNumber=GPIO_PIN_NUM_12;
	CAN_TX.GPIO_PinMode=GPIO_MODE_ALT;
	CAN_TX.GPIO_OutType=GPIO_OutType_PP;
	CAN_TX.GPIO_OutSpeed=GPIO_OutSpeed_Fast;
	CAN_TX.GPIO_PUPD=GPIO_NO_PUPD;
	CAN_TX.GPIO_AltFnc=GPIO_AF9;


	CAN_RX.GPIO_PinNumber=GPIO_PIN_NUM_11;
	CAN_RX.GPIO_PinMode=GPIO_MODE_ALT;
	CAN_RX.GPIO_OutType=GPIO_OutType_PP;
	CAN_RX.GPIO_OutSpeed=GPIO_OutSpeed_Fast;
	CAN_RX.GPIO_PUPD=GPIO_NO_PUPD;
	CAN_RX.GPIO_AltFnc=GPIO_AF9;

	GPIOA_CLK_ENB();
	GPIO_Init(GPIOA,CAN_TX);
	GPIO_Init(GPIOA,CAN_RX);

}

void IRSensorInit()
{

	IRSensor_Conf.GPIO_PinNumber=GPIO_PIN_NUM_1;
	IRSensor_Conf.GPIO_PinMode=GPIO_MODE_INPUT;
	IRSensor_Conf.GPIO_PUPD=GPIO_NO_PUPD;
	IRSensor_Conf.GPIO_EdgeTrigger=GPIO_IT_EDGE_RT;
    GPIOA_CLK_ENB();
    GPIO_Init(GPIOA,IRSensor_Conf);
    GPIO_IT_Init(GPIOA,IRSensor_Conf,1);

}


void TIM6_Init()
{
	TIM6_BaseConf.Period=999;
	TIM6_BaseConf.Prescaler=15;
	TIM6_BaseConf.AutoReloadPreload=TIM_AUTO_RELOAD_PRELOAD_ENABLE;
	TIM6_CLK_ENB();
	TIM6_Base_Init(TIM6,TIM6_BaseConf);
	//NVIC_SetPriority(IRQ_NO_TIM6,1U);
	//NVIC_EnableIRQ(IRQ_NO_TIM6);
}


void TIM6_Start()
{
	TIM_Base_Start(TIM6);
}

void TIM6_Stop()
{
	TIM_Base_Stop(TIM6);
}

void TIM6_IT_Init()
{
	TIM_Base_IT_Init(TIM6,1U); 
}

/**
 * @brief Function Introduses a Simulated Delay
 *
 */
void simDelay(void)
{
	uint32_t delayCounter;
	for(delayCounter=0;delayCounter<100000;delayCounter++)
	{

	}

}

void delay_ms(uint32_t ms)
{
	SYSTICK_LOAD= 16000 - 1;     // 1 ms @ 16 MHz
    SYSTICK_VAL  = 0;
    SYSTICK_CTRL = 5;             // Enable, processor clock, no interrupt

    for (uint32_t i = 0; i < ms; i++)
    {
        while ((SYSTICK_CTRL & (1U << 16)) == 0);
    }

    SYSTICK_CTRL = 0;
}


static void FPU_Enable(void)
{
    *(volatile uint32_t *)0xE000ED88U |= (0xFU << 20U);
    __asm volatile ("dsb");
    __asm volatile ("isb");
}


void GNSS_PrintPosition(USART_RegDef_t *USARTx, const GNSS_Data_t *data)
{
    char buffer[128];


    int n =snprintf(buffer, sizeof(buffer),
             "Lat: %f\r\n"
             "Lon: %f\r\n"
             "Alt: %f m\r\n"
             "Fix: %u\r\n"
             "Sat: %u\r\n",
             data->latitude,
             data->longitude,
             data->altitude,
             data->fix_quality,
             data->num_satellites);



    USART_Transmit(USARTx, (uint8_t *)buffer, strlen(buffer));
}

/**
 * @brief The Main Function of the Driver
 *
 * @return int
 */

int main(void)
{
	FPU_Enable();
	BlueLED_Init();
	RingBuffer_Init(&RingBuffer);
	USART2_Init();
	//UserButton_Init();
	USART3_Init();
	NMEA_ResetParser(&nmea_parser);
	TIM6_Init();
	TIM6_IT_Init();
	TIM6_Start();

	
	while(1)
	{

		if(ButtonEvent == TRUE)
		{
			ButtonEvent = FALSE;
			USART_Transmit(USART2, (uint8_t *)tx_msg, sizeof(tx_msg) - 1U);
		}

		uint8_t data;
		if(RingBuffer_Get(&RingBuffer, &data))
		{
			
			NMEA_ParseByte(&nmea_parser, data);
			
		}

		

		if(isTransmitt==TRUE)
		{
			isTransmitt=FALSE;
			if(nmea_parser.isValid)
			{

				GNSS_PrintPosition(USART3, &nmea_parser.data);
				delay_ms(1000);
				nmea_parser.isValid = FALSE; // Reset isValid after processing
			}
			else
			{
				//GNSS_PrintPosition(USART3, &nmea_parser.data);
				USART_Transmit(USART3, (uint8_t *)tx_msg, sizeof(tx_msg) - 1U);
			}
		}




	}
}


/*
void EXTI0_IRQHandler(void)
{
	
	TIM6_Start();
	EXTI->PR = (1U << 0U);
	ButtonEvent = TRUE;
}
*/
/**
 * @brief Service Routine for USART2 Interrupts. This function is called when an interrupt occurs on USART2.
 * 
 */

void USART2_IRQHandler(void)
{

	if(USART2->SR & (1U<<5U)) //Check if RXNE Flag is Set
	{
		uint8_t RxData;
		RxData=USART2->DR; //Read the Data from DR Register
		if (!RingBuffer_Put(&RingBuffer, RxData))
        {
            RingBufferOverflow++;
        }
	}
}

void TIM6_DAC_IRQHandler(void)
{
	if(TIM6_UEV_STS())
	{
		TIM6_UEV_CLEAR();
		Timer6Counter++;
		if(Timer6Counter>=BUTTON_DEBOUCE_TIME)
		{
			Timer6Counter=0;
			isTransmitt=TRUE;
			//TIM6_Stop();
		}
	}
} 



