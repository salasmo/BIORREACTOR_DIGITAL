/*CODIGO FINAL - BIORREACTOR EQUIPO 1
Santaigo Salas Montiel A01748055
Daniel Granado Coutiño A01747457
Valria Olvera A01657006 */

// definimos conexiones
sbit LCD_RS at LATB0_bit;
sbit LCD_EN at LATB1_bit;
sbit LCD_D4 at LATB2_bit;
sbit LCD_D5 at LATB3_bit;
sbit LCD_D6 at LATB4_bit;
sbit LCD_D7 at LATB5_bit;

// Pin direction
sbit LCD_D7_Direction at TRISB5_bit;
sbit LCD_D6_Direction at TRISB4_bit;
sbit LCD_D5_Direction at TRISB3_bit;
sbit LCD_D4_Direction at TRISB2_bit;
sbit LCD_EN_Direction at TRISB1_bit;
sbit LCD_RS_Direction at TRISB0_bit;
// end LCD mc

// Define variables
int pot1 = 0;
int pot2 = 0;
float valor1 = 0; // conversion del 0 al 100%
float valor2 = 0; // conversion del 0 al 100%
int pwm = 0;
int pwm2 = 0;

char valor[] = " "; // valor de porcentaje a char
char val2[] = " ";  // valor de porcentaje a char

char t1[] = "Temperatura";
char t2[] = "Agitacion";
char t3[] = "CO2";
int raw_temp;
char *temp = "000.0000 C";

void main()
{
    // Inicialización de puertos
    ADCON1 = 0b00001101; // A1 en analogicoactivado
    TRISC = 0;           // salida
    TRISB = 0;           // salida
    PORTB = 0;
    PORTC = 0;
    TRISA.F0 = 1; // entrada
    TRISA.F1 = 1; // entrada
    TRISD = 0B0000111;
    PORTD = 0;
    TRISE = 1;    // BOTON DE ENCENDIDO
    PORTB.F7 = 1; // LEDS

    // inicializar PWM
    PWM1_Init(5000); // Frecuencia
    PWM1_Start();

    PWM2_Init(5000);
    PWM2_Start();

    // inicializar ADC
    ADC_Init();

    // Inicialización de LCD
    Lcd_Init();               // Inizialize LCD module
    Lcd_Cmd(_LCD_CURSOR_OFF); // cursor off
    Lcd_Cmd(_LCD_CLEAR);      // clear LCD

    while (1)
    { // ciclo infinito
        // sensor de temperatura
        if (PORTD.F0 == 1)
        {
            PWM2_SET_DUTY(0);
            PWM1_SET_DUTY(0);
            Ow_Reset(&PORTD, 3);
            Ow_Write(&PORTD, 3, 0xCC);
            Ow_Write(&PORTD, 3, 0X44);

            while (Ow_Read(&PORTD, 3) == 0)
                ;
            Ow_Reset(&PORTD, 3);       // Onewire reset signal
            Ow_Write(&PORTD, 3, 0xCC); // Issue command SKIP_ROM
            Ow_Write(&PORTD, 3, 0xBE); // Issue command READ_SCRATCHPAD

            raw_temp = Ow_Read(&PORTD, 3);         // Read temperature LSB byte
            raw_temp |= (Ow_Read(&PORTD, 3) << 8); // Read temperature MSB byte

            if (raw_temp & 0x8000)
            {                             // If the temperature is negative
                temp[0] = '-';            // Put minus sign (-)
                raw_temp = ~raw_temp + 1; // Change temperature value to positive form
            }

            else
            {
                if ((raw_temp >> 4) >= 100) // If the temperatue >= 100 C
                    temp[0] = '1';          // Put 1 of hundreds
                else                        // otherwise
                    temp[0] = ' ';
            } // put space ' '

            // Put the first two digits ( for tens and ones)
            temp[1] = ((raw_temp >> 4) / 10) % 10 + 48; // Put tens digit
            temp[2] = (raw_temp >> 4) % 10 + 48;        // Put ones digit

            // Put the 4 fraction digits (digits after the point)
            // Why 625: because we're working with 12-bit resolution
            temp[4] = ((raw_temp & 0x0F) * 625) / 1000 + 48;       // Put thousands digit
            temp[5] = (((raw_temp & 0x0F) * 625) / 100) % 10 + 48; // Put hundreds digit
            temp[6] = (((raw_temp & 0x0F) * 625) / 10) % 10 + 48;  // Put tens digit
            temp[7] = ((raw_temp & 0x0F) * 625) % 10 + 48;         // Put ones digit

            temp[8] = 223; // Put degree symbol ( ° )
            lcd_out(1, 3, t1);
            lcd_out(2, 4, temp); // Display temperature

            delay_ms(500); // Wait 1 second
        }

        // AGITACIÓN
        else if (PORTD.F1 == 1)
        {
            PWM1_SET_DUTY(0);
            PORTC.F2 == 0;
            Lcd_Cmd(_LCD_CLEAR);
            Lcd_Out(1, 2, t2);
            pot2 = ADC_Read(1);
            valor2 = pot2 * 0.58; // Porcentaje del PWM con respecto a los 255
            pwm2 = (valor2 * 255) / 1023;
            FloatToStr(valor2, val2);
            PWM2_SET_DUTY(pwm2);
            // Lcd_Out(1,1,t2);
            Lcd_Out(2, 1, val2);
            Lcd_Chr_CP('R');
            Lcd_Chr_CP('P');
            Lcd_Chr_CP('M');
            delay_ms(500);
        }

        else if (PORTD.F2 == 1)
        {
            // Display "CO2"
            PWM2_SET_DUTY(0);
            PORTC.F1 == 0; // apagamos el otro motor
            Lcd_Cmd(_LCD_CLEAR);
            Lcd_Out(1, 7, t3);
            // leer el pot
            pot1 = ADC_Read(0); // está en el pin 0
            valor1 = (pot1 / 102);
            pwm = (valor1 * 255) / 1023; // 1023 = 10 bits
            FloatToStr(valor1, valor);
            PWM1_SET_DUTY(pwm);
            Lcd_Out(2, 2, valor); // Valor potenciometro
            Lcd_Chr_CP('%');
            delay_ms(500);
        }
        else
        {
            Lcd_Cmd(_LCD_CLEAR);
        }
    }
}