#include "stm32f407xx.h"

#define RCC_AHB1ENR   (*(volatile uint32_t *)0x40023830)

GPIO_PinConf_t UserButton_Conf;

GPIO_PinConf_t IRSensor_Conf;

GPIO_PinConf_t CAN_TX;
GPIO_PinConf_t CAN_RX;

USART_Conf_t USART2_Conf;

volatile uint8_t motion_detected = 0;
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
    //GPIO_IT_Init(GPIOA,UserButton_Conf,1);

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
	IRSensorInit();
	BlueLED_Init();
	USART2_Init();
	UserButton_Init();
	
	while(1)
	{

/* 		if(motion_detected==1)
		{
			GPIO_TogglePin(GPIOD,GPIO_PIN_NUM_15);
			delay_ms(5000);
			motion_detected=0;
			USART_Transmit(USART2, (uint8_t *)tx_msg, sizeof(tx_msg) - 1);
			GPIO_TogglePin(GPIOD,GPIO_PIN_NUM_15);
		}
 */
		
	


	}
}


void EXTI0_IRQHandler(void)
{


	if (EXTI->PR & (1U << IRSensor_Conf.GPIO_PinNumber))
	{
	    EXTI->PR = (1U << IRSensor_Conf.GPIO_PinNumber);
	}

	//GPIO_TogglePin(GPIOD,GPIO_PIN_NUM_15);

	simDelay();

	if(GPIO_ReadPin(GPIOA,GPIO_PIN_NUM_0)==GPIO_PIN_HIGH)

	{

		USART_Transmit(USART2, (uint8_t *)tx_msg, sizeof(tx_msg) - 1);
		//motion_detected=1; //Set the Global Variable when Motion is Detected

	}


}
