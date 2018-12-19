#include <msp430.h> 


/**
 * main.c
 */

//char message[100] = "#MOISt 20\n"; // Initial Greeting you should see upon properly connecting your Launchpad
unsigned int i = 0;

float tempVal = 0.0;
unsigned int moistVal = 0;

// Int to Char variables
unsigned char a;
unsigned char b;
unsigned char c;

unsigned int firstDigit;
unsigned int secondDigit;
unsigned int thirdDigit;

void publishData(int moistVal);

int main(void)
{

     WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    // Relay Pin
    P1DIR |= BIT2;
    P1OUT |= BIT2;

    // UART Initialization at 115200 Baud
    P4SEL |= BIT5+BIT4;                       // P4.4,5 = USCI_A1 TXD/RXD

    //P3SEL |= BIT3 + BIT4;                     // P3.3,4 = USCI_A0 TXD/RXD

    UCA1CTL1 |= UCSWRST;                      // **Put state machine in reset**
    UCA1CTL1 |= UCSSEL_2;                     // SMCLK
    UCA1BR0 = 9;                              // 1MHz 115200 (see User's Guide)
    UCA1BR1 = 0;                              // 1MHz 115200
    UCA1MCTL |= UCBRS_1 + UCBRF_0;            // Modulation UCBRSx=6, UCBRFx=0
    UCA1CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    UCA1IE |= UCRXIE;                         // Enable USCI_A1 RX interrupt
    UCA1IFG &= ~UCRXIFG;                                // Clear Interrupt Flag

    //ADC Initializtion
    ADC12CTL0 = ADC12SHT02 + ADC12ON;         // Sampling time, ADC12 on
    ADC12CTL1 = ADC12SHP + ADC12DIV_7+ ADC12SSEL_1;                     // Use sampling timer
    ADC12IE = 0x01;                           // Enable interrupt
    ADC12CTL0 |= ADC12ENC;
    P6SEL |= 0x01;                            // P6.0 ADC option select
    P1DIR |= 0x01;                            // P1.0 output*/



    while (1)
    {
        ADC12CTL0 |= ADC12SC;                   // Start sampling/conversion


        __bis_SR_register(LPM0_bits + GIE);     // LPM0, ADC12_ISR will force exit
        __no_operation();                       // For debugger
    }


}


#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
    {
      switch(__even_in_range(ADC12IV,34))
      {
      case  0: break;                           // Vector  0:  No interrupt
      case  2: break;                           // Vector  2:  ADC overflow
      case  4: break;                           // Vector  4:  ADC timing overflow
      case  6:                                  // Vector  6:  ADC12IFG0
              tempVal = (ADC12MEM0 / 4095.0) * 100;

              moistVal = (int)tempVal;

              publishData(moistVal);

              if(moistVal < 30)
                  P1OUT &= ~BIT2;
              else if(moistVal > 50)
                  P1OUT |= BIT2;

              __bic_SR_register_on_exit(LPM0_bits);   // Exit active CPU
      case  8: break;                           // Vector  8:  ADC12IFG1
      case 10: break;                           // Vector 10:  ADC12IFG2
      case 12: break;                           // Vector 12:  ADC12IFG3
      case 14: break;                           // Vector 14:  ADC12IFG4
      case 16: break;                           // Vector 16:  ADC12IFG5
      case 18: break;                           // Vector 18:  ADC12IFG6
      case 20: break;                           // Vector 20:  ADC12IFG7
      case 22: break;                           // Vector 22:  ADC12IFG8
      case 24: break;                           // Vector 24:  ADC12IFG9
      case 26: break;                           // Vector 26:  ADC12IFG10
      case 28: break;                           // Vector 28:  ADC12IFG11
      case 30: break;                           // Vector 30:  ADC12IFG12
      case 32: break;                           // Vector 32:  ADC12IFG13
      case 34: break;                           // Vector 34:  ADC12IFG14
      default: break;
      }
}

void publishData(int moistVal)
{
    char message[100] = "#MOISt ";

    firstDigit = moistVal % 100;

    a = ((moistVal - firstDigit) / 100) + '0';

    secondDigit = firstDigit % 10;

    b = ((firstDigit - secondDigit) / 10) + '0';

    c = secondDigit + '0';

    if(a != '0')
    {
        message[7] = a;
        message[8] = b;
        message[9] = c;
        message[10] = '\n';
    }
    else if((a == '0') && (b == '0'))
    {
        message[7] = c;
        message[8] = '\n';
    }
    else
    {
        message[7] = b;
        message[8] = c;
        message[9] = '\n';
    }

    int newLineFound = 0;

    while(newLineFound == 0)
    {
        UCA1TXBUF = message[i];                  // TX -> RXed character

        while (!(UCA1IFG&UCTXIFG));                 // USCI_A1 TX buffer ready?

        if(message[i] == '\n')
            newLineFound = 1;

        i++;
    }

    i= 0;

    while (!(UCA1IFG&UCTXIFG));                 // USCI_A1 TX buffer ready?
}





