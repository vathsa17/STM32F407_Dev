#include "stm32f407xx.h"
#include <string.h>
#define RCC_AHB1ENR   (*(volatile uint32_t *)0x40023830)

GPIO_PinConf_t UserButton_Conf;

GPIO_PinConf_t IRSensor_Conf;

GPIO_PinConf_t CAN_TX;
GPIO_PinConf_t CAN_RX;

USART_Conf_t USART2_Conf;
#define RX_BUFFER_SIZE 8U
#define TX_BUFFER_SIZE 8U

volatile uint8_t RecievedMessage[RX_BUFFER_SIZE];
volatile uint8_t TransmitMessage[TX_BUFFER_SIZE];
volatile uint8_t motion_detected = 0;
volatile uint8_t IsRxAvailable =FALSE;
volatile uint8_t ButtonEvent = FALSE;
volatile uint8_t TxMessageSize=2U;
volatile uint8_t RxIndex=0U;
volatile uint8_t RxData=0U;

char tx_msg[] = "J\r\n";


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


/**
 * @brief The Main Function of the Driver
 *
 * @return int
 */

int main(void)
{
	BlueLED_Init();
	USART2_Init();
	UserButton_Init();
	
	while(1)
	{
		if(ButtonEvent == TRUE)
		{
			ButtonEvent = FALSE;
			USART_Transmit(USART2, (uint8_t *)tx_msg, sizeof(tx_msg) - 1U);
		}

		if(IsRxAvailable==TRUE)
		{
			if(RxIndex< RX_BUFFER_SIZE)
			{
				RecievedMessage[RxIndex]=RxData;
				RxIndex++;
			}
			else
			{
				// Buffer overflow, handle error
				RxIndex=0;
			}

			IsRxAvailable=FALSE;


			if(RxData=='\n')
			{
				RecievedMessage[RxIndex-1]='\0'; // Null-terminate the string

				if(strcmp((const char *)RecievedMessage, "LED ON") == 0)
				{
					GPIO_WritePin(GPIOD, GPIO_PIN_NUM_15, GPIO_PIN_HIGH);
				}
				else if (strcmp((const char *)RecievedMessage, "LED OFF") == 0)
				{
					GPIO_WritePin(GPIOD, GPIO_PIN_NUM_15, GPIO_PIN_LOW);
				}

				RxData=0U;
				RxIndex=0;
				strcpy(RecievedMessage, "");
			}
		
		}

	


	}
}


void EXTI0_IRQHandler(void)
{
	EXTI->PR = (1U << 0U);
	ButtonEvent = TRUE;
}

/**
 * @brief Service Routine for USART2 Interrupts. This function is called when an interrupt occurs on USART2.
 * 
 */

void USART2_IRQHandler(void)
{
	motion_detected=1;
	if(USART2->SR & (1U<<5U)) //Check if RXNE Flag is Set
	{
		RxData=USART2->DR; //Read the Data from DR Register
		IsRxAvailable=TRUE; //Set the Flag to Indicate Data is Available
	}
}
