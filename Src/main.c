#include "stm32f407xx.h"
#include <string.h>

/*
What do we do in the Code?

A GNSS-UART Device is integrated with the STM32F407G-DISCOVERY Board. 
The GNSS-UART Device sends NMEA sentences over UART. The STM32F407G-DISCOVERY 
Board receives these sentences, parses them, and extracts relevant information
such as latitude, longitude, and altitude. This information is then transmitted 
over to PC or another device using another UART interface. Additionally, 
BlueLED on the STM32F407G-DISCOVERY Board is used to indicate the status of the GNSS-UART Device. 
When a valid NMEA sentence is received and parsed successfully, the BlueLED is turned ON for a brief period to indicate successful reception and parsing.

The Baud rate for both UART interfaces is set to 9600 bps. The STM32F407G-DISCOVERY
Board uses a ring buffer to store incoming data from the GNSS-UART Device, ensuring that no data is lost during processing. 
The NMEA parser processes the data byte by byte, validating the sentences and extracting the required information.


What did  I learn from this code?
1. UART Communication: The code demonstrates how to set up UART communication on the STM32F407G-DISCOVERY Board, including configuring GPIO pins for UART functionality, setting baud rates, and handling UART interrupts for data reception.
2. Ring Buffer Implementation: The code implements a ring buffer to efficiently manage incoming data from the GNSS-UART Device. This allows for continuous data reception without losing any bytes, even if the main processing loop is busy.
3. NMEA Sentence Parsing: The code includes an NMEA parser that processes incoming data byte by byte, validating the sentences and extracting relevant information such as latitude, longitude, and altitude. This demonstrates how to handle structured data formats in embedded systems.
4. Timer Usage: The code utilizes timers (TIM6 and TIM7) to manage periodic tasks, such as triggering UART transmissions and CAN message transmissions at specified intervals. This shows how to use timers for scheduling tasks in embedded applications.

We are still in SuperLoop Mode, which means we are not using an RTOS. A FreeRTOS Switch is planned for the future.

*/

/*GPIO Configuration */
GPIO_PinConf_t UserButton_Conf;
GPIO_PinConf_t IRSensor_Conf;
GPIO_PinConf_t CAN_TX_RX;

/*Timer Initialization */
TIM_Base_InitTypeDef TIM7_BaseConf;
TIM_Base_InitTypeDef TIM6_BaseConf;

/*UART Configuration*/
USART_Conf_t USART2_Conf;
USART_Conf_t USART3_Conf;


volatile bool isUARTTransmitt=FALSE;
volatile bool isLEDToggle=FALSE;

/** @brief Variable to store the count for TIM7 */
volatile uint16_t Timer7Counter=0;

/** @brief Variable to store the count for TIM6 */
volatile uint16_t Timer6Counter=0;




char tx_msg[] = "No GPS Lock\r\n";
volatile uint8_t RingBufferOverflow = 0;
RingBuffer_t RingBuffer;
NMEA_Parser_t nmea_parser;


/**
 * @brief Function to Initialize USART2 for communication with the GNSS-UART Device. 
 * This function configures the GPIO pins, sets up the USART parameters, and enables the necessary interrupts.
 * 
 */
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

/**
 * @brief Function to Initialize USART3 for communication with the PC or another device. 
 * This function configures the GPIO pins, sets up the USART parameters, and enables the necessary interrupts.
 * 
 */
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

/**
 * @brief Function to Initialize the Blue LED on the STM32F407G-DISCOVERY Board. 
 * This function configures the GPIO pin connected to the Blue LED as an output pin.
 * 
 */
void BlueLED_Init()
{
	GPIO_PinConf_t GPIOD_PinConf = {GPIO_PIN_NUM_15,GPIO_MODE_OUTPUT,GPIO_OutType_PP,GPIO_OutSpeed_Low,GPIO_NO_PUPD};

	// Enable clock for GPIOD
    //RCC_AHB1ENR |= (1 << 3);
	GPIOD_CLK_ENB();

	GPIO_Init(GPIOD,GPIOD_PinConf);


	GPIOD_PinConf.GPIO_PinNumber=GPIO_PIN_NUM_14;
	GPIO_Init(GPIOD,GPIOD_PinConf);
}

/**
 * @brief Function to Initialize TIM6 for generating time delays or PWM signals.
 * This function configures the TIM6 parameters and enables the clock.
 */
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

/**
 * @brief Function to Initialize TIM7 for generating time delays or PWM signals.
 * This function configures the TIM7 parameters and enables the clock.
 */
void TIM7_Init()
{
	TIM_Base_InitTypeDef TIM7_BaseConf;
	TIM7_BaseConf.Period=999;
	TIM7_BaseConf.Prescaler=15;
	TIM7_BaseConf.AutoReloadPreload=TIM_AUTO_RELOAD_PRELOAD_ENABLE;
	TIM7_CLK_ENB();
	TIM_Base_Init(TIM7,TIM7_BaseConf);
	//NVIC_SetPriority(IRQ_NO_TIM6,1U);
	//NVIC_EnableIRQ(IRQ_NO_TIM6);
}

/**
 * @brief Function to initialize the interrupt for TIM7.
 * This function initializes the interrupt for TIM7.
 */
void TIM7_IT_Init()
{
	TIM_Base_IT_Init(TIM7,1U); 
}


/**
 * @brief Function to start TIM7.
 * This function starts TIM7, allowing it to begin counting and generating interrupts.
 */
void TIM7_Start()
{
	TIM_Base_Start(TIM7);
}


/**
 * @brief Function to stop TIM7.
 * This function stops TIM7, halting its counting and interrupt generation.
 */
void TIM7_Stop()
{
	TIM_Base_Stop(TIM7);
}

/**
 * @brief Function to start TIM6.
 * This function starts TIM6, allowing it to begin counting and generating interrupts.
 */
void TIM6_Start()
{
	TIM_Base_Start(TIM6);
}


/**
 * @brief Function to stop TIM6.
 * This function stops TIM6, halting its counting and interrupt generation.
 */
void TIM6_Stop()
{
	TIM_Base_Stop(TIM6);
}


/**
 * @brief Function to initialize the interrupt for TIM6.
 * This function initializes the interrupt for TIM6.
 */
void TIM6_IT_Init()
{
	TIM_Base_IT_Init(TIM6,1U); 
}



/**
 * @brief Function to enable the FPU.
 * This function enables the Floating Point Unit (FPU) for floating-point operations.
 */
static void FPU_Enable(void)
{
    *(volatile uint32_t *)0xE000ED88U |= (0xFU << 20U);
    __asm volatile ("dsb");
    __asm volatile ("isb");
}



/**
 * @brief The Main Function of the Driver
 *
 * @return int
 */

int main(void)
{
	FPU_Enable();
	RingBuffer_Init(&RingBuffer);
	USART2_Init();
	USART3_Init();
	BlueLED_Init();
	NMEA_ResetParser(&nmea_parser);

	TIM6_Init();
	TIM6_IT_Init();
	TIM6_Start();

	TIM7_Init();
	TIM7_IT_Init();	
	


	
	
	
	while(1)
	{


		uint8_t data;
		if(RingBuffer_Get(&RingBuffer, &data))
		{
			
			NMEA_ParseByte(&nmea_parser, data);
			
		}

		

		if(isUARTTransmitt==TRUE)
		{
			isUARTTransmitt=FALSE;
			if(nmea_parser.isValid)
			{

				GNSS_PrintPosition(USART3, &nmea_parser.data);
				TIM7_Start();
				/*Turn on Blue LED*/
				GPIO_TogglePin(GPIOD,GPIO_PIN_NUM_15);
				nmea_parser.isValid = FALSE; // Reset isValid after processing


			}
			else
			{
				//GNSS_PrintPosition(USART3, &nmea_parser.data);
				USART_Transmit(USART3, (uint8_t *)tx_msg, sizeof(tx_msg) - 1U);

				GPIO_TogglePin(GPIOD,GPIO_PIN_NUM_14);
				delay_ms(500);
				GPIO_TogglePin(GPIOD,GPIO_PIN_NUM_14);
			}
		}







	}
}


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
		if(Timer6Counter>=UART_TRANSMIT_TIME)
		{
			Timer6Counter=0;
			isUARTTransmitt=TRUE;
			//TIM6_Stop();
		}
	}
} 

void TIM7_IRQHandler(void)
{
	if(TIM7_UEV_STS())
	{
		TIM7_UEV_CLEAR();
		Timer7Counter++;
		if(Timer7Counter>=LED_TOGGLE_TIME)
		{
			Timer7Counter=0;
			GPIO_TogglePin(GPIOD,GPIO_PIN_NUM_15);
			TIM7_Stop();
			
		}
		
	}
}



