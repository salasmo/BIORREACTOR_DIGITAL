/*TAREA 3 Diseño de sistemas de bioinstrumentación digital BI2002B
por: Santiago Salas Montiel    Matricula: A01748055*/

int duty = 0;
int i = 0;

// SECTIONS
void sec_a() // DIMMER LED (a)
{
    int i;
    for (i = 0; i <= 255; i++)
    {
        PORTB.F3 = i % 21;
        if (i % 21 == 0)
        {
            PORTB.F3 = 0;
        }
        Delay_ms(100);
    }
}
void sec_b() // RGB COLOR (b)
{
    // RED 1
    PORTB = 0;      // OFF
    PORTB.F0 = 255; // RED
    PORTB.F1 = 0;   // GREEN
    PORTB.F2 = 0;   // BLUE
    Delay_ms(1000);
    // GREEN 2
    PORTB = 0;      // OFF
    PORTB.F0 = 0;   // RED
    PORTB.F1 = 255; // GREEN
    PORTB.F2 = 0;   // BLUE
    Delay_ms(1000);
    // BLUE 3
    PORTB = 0;      // OFF
    PORTB.F0 = 0;   // RED
    PORTB.F1 = 0;   // GREEN
    PORTB.F2 = 255; // BLUE
    Delay_ms(1000);
    // AQUAMARINE 4
    PORTB = 0;      // OFF
    PORTB.F0 = 127; // RED
    PORTB.F1 = 255; // GREEN
    PORTB.F2 = 212; // BLUE
    Delay_ms(1000);
    // BLUEVIOLET 5
    PORTB = 0;      // OFF
    PORTB.F0 = 138; // RED
    PORTB.F1 = 43;  // GREEN
    PORTB.F2 = 226; // BLUE
    Delay_ms(1000);
    // CYAN 6
    PORTB = 0;      // OFF
    PORTB.F0 = 0;   // RED
    PORTB.F1 = 255; // GREEN
    PORTB.F2 = 255; // BLUE
    Delay_ms(1000);
    // DARKORANGE 7
    PORTB = 0;      // OFF
    PORTB.F0 = 233; // RED
    PORTB.F1 = 150; // GREEN
    PORTB.F2 = 122; // BLUE
    Delay_ms(1000);
    // DARKCHID 8
    PORTB = 0;      // OFF
    PORTB.F0 = 153; // RED
    PORTB.F1 = 50;  // GREEN
    PORTB.F2 = 204; // BLUE
    Delay_ms(1000);
    // GREENYELLOW 9
    PORTB = 0;      // OFF
    PORTB.F0 = 173; // RED
    PORTB.F1 = 255; // GREEN
    PORTB.F2 = 47;  // BLUE
    Delay_ms(1000);
    // HOTPINK 10
    PORTB = 0;      // OFF
    PORTB.F0 = 255; // RED
    PORTB.F1 = 105; // GREEN
    PORTB.F2 = 180; // BLUE
    Delay_ms(1000);
    PORTB = 0; // OFF
}
void sec_c() // DC MOTO L TO R AND R TO L (c)
{
    PORTC.F1 = 1;
    PORTC.F2 = 0;
    duty = 25.5;
    PWM1_Set_Duty(duty);
    for (i = 0; i < 9; i++)
    {
        Delay_ms(1000);
        duty = duty + 25.5;
        PWM1_Set_Duty(duty);
    }
    PORTC.F1 = 0;
    PORTC.F2 = 1;
    duty = 25.5;
    PWM1_Set_Duty(duty);
    for (i = 0; i < 9; i++)
    {
        Delay_ms(1000);
        duty = duty + 25.5;
        PWM1_Set_Duty(duty);
    }
    PORTC.F1 = 0;
    PORTC.F2 = 0;
}

// START CODE
void main()
{
    // PORT CONFIGURATION
    ADCON1 = 0b1111;
    PWM1_Init(5000);
    TRISA = 1;
    TRISB = 0;
    TRISC = 0;

    PORTC = 0;
    PORTB = 0;

    // INFINITE LOOP
    while (1)
    {
        if (PORTA.F0 == 1 && /*AND*/ PORTA.F1 == 0 && /*AND*/ PORTA.F2 == 0) // LED DIMMER
        {
            sec_a(); //(a)
        }
        if (PORTA.F0 == 0 && /*AND*/ PORTA.F1 == 0 && /*AND*/ PORTA.F2 == 0) // TURN OFF
        {
            PORTC = 0;
            PORTB = 0;
        }
        if (PORTA.F0 == 0 && /*AND*/ PORTA.F1 == 1 && /*AND*/ PORTA.F2 == 0) // RGB 10 COLORS
        {
            sec_b(); //(b)
        }
        if (PORTA.F0 == 0 && /*AND*/ PORTA.F1 == 0 && /*AND*/ PORTA.F2 == 1) // DC MOTOR
        {
            sec_c(); //(c)
        }
    }
    return;
}