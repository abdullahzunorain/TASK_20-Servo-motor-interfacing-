
/*

Task Title: 
						{Controlling SG90 Servo Motor with 8051 Microcontroller Using PWM}
Explanation:
					The code generates a PWM signal using Timer 1 in Mode 2 of the 8051 microcontroller to control an SG90 servo motor,
					where button inputs adjust the angle, and the pulse width is varied accordingly to set the servo's position between 0° and 180°.



To control an SG90 servo motor with an 8051 microcontroller (like the STC89C52RC) using PWM signals, you need to generate a signal with a 50 Hz frequency. 
The SG90 requires a PWM signal where the pulse width ranges from approximately 1 ms (for 0 degrees) to 2 ms (for 180 degrees), with a total period of 20 ms.

Requirements:
Pulse width for 0°: 1 ms (high time)
Pulse width for 90°: 1.5 ms (high time)
Pulse width for 180°: 2 ms (high time)
Total period: 20 ms (50 Hz)
Approach:
Using Timer 1 in Mode 2 (8-bit auto-reload mode) to generate the required delays.
Manually adjusting the duty cycle for 0°, 90°, and 180° within a while loop first, and then using interrupts.

Code Implementation (Manual Method with Timer and Delay):
You can start with a simple method using a while loop to manually control the pulse widths for the servo.


Explanation:
PWM Signal: We generate a PWM signal where the high pulse width corresponds to the servo angle, and the total period is 20 ms.
0° corresponds to a 1 ms pulse width (1000 µs).
90° corresponds to a 1.5 ms pulse width (1500 µs).
180° corresponds to a 2 ms pulse width (2000 µs).
Timer 1 is used to create small delays for the high time of the PWM signal.
Buttons are used to adjust the angle. The angle is incremented or decremented, and the corresponding pulse width is calculated and sent to the servo.

*/


#include <reg51.h>

sbit output = P1^0;  // Output to the servo motor control pin
sbit btn = P3^0;     // Button to increase the angle
sbit btn1 = P3^1;    // Button to decrease the angle

void delay_ms(unsigned int msec); // Delay function
void timer_us(unsigned int usec); // Microsecond-level delay using Timer1

int angle_to_pulse_width(int angle);

void main()
{
    int angle = 90;  // Start at 90 degrees
    int pulse_width;
    
    // Initial setup
    TMOD = 0x20;   // Timer1 Mode2 (8-bit auto-reload)
    TH1 = -23;     // Timer for 50 us delay (adjust for your clock)
    
    while(1)
    {
        // Adjust angle with buttons
        if (btn == 0) {
            if (angle < 180) angle += 1;
            delay_ms(200);  // Debouncing delay
        }
        if (btn1 == 0) {
            if (angle > 0) angle -= 1;
            delay_ms(200);  // Debouncing delay
        }

        // Convert angle to pulse width
        pulse_width = angle_to_pulse_width(angle);

        // Send the PWM signal to the servo
        output = 1;               // Start the pulse
        timer_us(pulse_width);     // High for pulse width
        output = 0;               // End the pulse
        delay_ms(20 - pulse_width / 1000); // Low for the rest of 20 ms
    }
}

void delay_ms(unsigned int msec)  // Function for millisecond delay
{
    unsigned int i, j;
    for(i = 0; i < msec; i++)
        for(j = 0; j < 1275; j++); // 1 ms delay
}

void timer_us(unsigned int usec)  // Function for microsecond delay using Timer1
{
    int i;
    for(i = 0; i < usec / 50; i++)  // Assuming Timer1 gives a 50 us delay
    {
        TR1 = 1;        // Start Timer1
        while (TF1 == 0); // Wait for Timer1 overflow
        TF1 = 0;        // Clear overflow flag
        TR1 = 0;        // Stop Timer1
    }
}

// Function to convert angle to pulse width (in microseconds)
int angle_to_pulse_width(int angle)
{
    // Linear conversion of angle (0-180) to pulse width (1000-2000 microseconds)
    return (1000 + ((angle * 1000) / 180));  // Map angle to 1000-2000 us
}
