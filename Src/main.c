#include "stm32f407xx.h"
#include <string.h>


/*
What is the purpose of this code?

The Purpose of this code is to demonstrate the usage of UART, Push button and OnBoard LED
on STM32F407G-DISCOVERY Board. The code initializes the UART2 peripheral for serial 
communication,configures a user button for input, and sets up a timer (TIM6) for 
timing operations. It also includes interrupt handling for the USART2 peripheral to 
receive data asynchronously.
Push OnBoard Button: UART Communication to PC with Message "Button Pressed"

Send LED_ON or LED_OFF commands from PC to turn on/off the OnBoard Blue LED.

UART2 BaudRate is 9600, 8 data bits, no parity, 1 stop bit.

*/
/*GPIO Configuration */
GPIO_PinConf_t UserButton_Conf;

/*Timer Initialization */
TIM_Base_InitTypeDef TIM6_BaseConf;

/*UART Configuration*/
USART_Conf_t USART2_Conf;






volatile uint8_t RecievedMessage[RX_BUFFER_SIZE];
volatile uint8_t IsRxAvailable =FALSE;
volatile uint8_t ButtonEvent = FALSE;
volatile uint8_t RxIndex=0U;
volatile uint16_t Timer6Counter=0;
volatile uint8_t RxData;



char tx_msg[] = "Button Pressed\r\n";




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





void TIM6_Init()
{
	TIM6_BaseConf.Period=999;
	TIM6_BaseConf.Prescaler=15;
	TIM6_BaseConf.AutoReloadPreload=TIM_AUTO_RELOAD_PRELOAD_ENABLE;
	TIM6_CLK_ENB();
	TIM6_Base_Init(TIM6,TIM6_BaseConf);
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
	USART2_Init();
	BlueLED_Init();
	UserButton_Init();
	TIM6_Init();
	TIM6_IT_Init();
	TIM6_Start();

	while(1)
	{
		if(ButtonEvent==TRUE)
		{
			ButtonEvent=FALSE;
			USART_Transmit(USART2,(uint8_t*)tx_msg,strlen(tx_msg));
		}
		if(IsRxAvailable==TRUE)
		{
			IsRxAvailable=FALSE;
			/*Compare the Recieved String to LED_ON/LED_OFF*/
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
		RxData=USART2->DR; //Read the Data from DR Register
		IsRxAvailable=TRUE; //Set the Flag to indicate that data is available
	}
}

