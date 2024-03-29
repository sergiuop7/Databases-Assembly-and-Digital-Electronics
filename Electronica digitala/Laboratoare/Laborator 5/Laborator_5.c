int n=0, value = 0, adv=0;
float tmp, vin;
int ms=0, digit=0;
int adc_l=0, adc_h=0;

void display(char p, char c)
{
 PORTA &= 0b11110000;
 PORTC &= 0b00000000;

  switch(c){
     case 0: PORTC|=0b00111111; break;
     case 1: PORTC|=0b00000110; break;
     case 2: PORTC|=0b01011011; break;
     case 3: PORTC|=0b01001111; break;
     case 4: PORTC|=0b01100110; break;
     case 5: PORTC|=0b01101101; break;
     case 6: PORTC|=0b01111101; break;
     case 7: PORTC|=0b00000111; break;
     case 8: PORTC|=0b01111111; break;
     case 9: PORTC|=0b01101111; break;
  }
  switch(p){
      case 1: PORTA|=0b00000001; break;
      case 2: PORTA|=0b00000010; break;
      case 3: PORTA|=0b00000100; break;
      case 4: PORTA|=0b00001000; break;
  }
  // Delay_ms(1); // #include <util/delay.h>
}

void init_timer(){
        TCCR0 = 0b00001011; //CTC-3,6, Prescaler-0,1,2
        TCNT0 = 0;
        OCR0 = 124;
        TIMSK |= 0b00000010;//set interrupt OCM
}

void init_adc(){
  ADMUX = 0b00000000;            //channel ADC7
  ADCSRA= 0b10000111;// IE - ADC bit 3  , AutoTrigger bit 5
}

int readADC (char ch){
ADMUX &= 0b11100000; //Reseteaza canalul de conversie
ADMUX |= ch; //Seteaza canalul conversiei
ADCSRA |= (1<<6); //�ncepe conversia
        while(ADCSRA & (1<<6)); //Asteapta finalizarea conversiei
adc_l = ADCL;
adc_h = ADCH;
return ((adc_h << 8) | adc_l);
}

void Timer0_ISR() iv IVT_ADDR_TIMER0_COMP{
digit++;
        switch(digit)
                {
                case 1: display(4,value%10);break;
                case 2: display(3,(value/10)%10);break;
                case 3: display(2,(value/100)%10);break;
                case 4: display(1,(value/1000)%10); digit=0;break;
                }

        if (ms==100){
                adv = readADC(7);
                vin = ((float)adv)*5/1024;
                tmp = vin*1000/10;
                value = tmp*10;
                ms=0;
        }else ms++;
}

void main(){
  DDRA = 0b00001111;  //seteaza pinii de iesire
  DDRC = 0b11111111;
  init_timer();
  init_adc();
  SREG |= 1<<7;        //Set Enable Interrupt
        for(;;);
}