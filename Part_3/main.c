
/*
Task:
		Interrupt-Based PWM Control for SG90 Servo Motor on 8051 Microcontroller
*/


#include <reg51.h>

// Defining servo motor control pins
sbit r1 = P2^4;
sbit r2 = P2^3;
sbit r3 = P2^2;

// Defining output pin to the motor
sbit output = P1^0;  		

// Defining control pins for LCD
sbit rs = P2^6;     // Register select
sbit rw = P2^5;     // Read/Write
sbit en = P2^7;     // Enable

// Defining buttons for user input
sbit btn = P3^0;    // Button to increase angle
sbit btn1 = P3^1;   // Button to decrease angle

// Function declarations
void values_on_ssd(void);
void display_num_7seg(unsigned char num);
void lcd_data(unsigned char item);
void lcd_cmd(unsigned char item);
void lcd_data_int(int time_val);
void lcd_DisplayString(char *string_ptr);
void delay(unsigned int msec);
void timer(int msec);
void shape(void);

// Variables for 7-segment display control
unsigned char values[4];
unsigned char index = 0;
unsigned int select[] = {0x0C, 0x08, 0x04, 0x00}; // Digit selection bits
unsigned int message[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67};  // 7-segment display codes for 0-9

// Function to create a custom "degree" shape on the LCD
void shape() {			
	lcd_cmd(0xC4);           // Move cursor to a specific LCD location
	lcd_data('D');
	lcd_data('E');
	lcd_data('G');
	lcd_data('R');
	lcd_data('E');
	lcd_data('E');
}

// Delay function in milliseconds
void delay(unsigned int msec) {
	int i, j;
	for(i = 0; i < msec; i++)
		for(j = 0; j < 1275; j++);  // Inner loop provides delay for 1 ms
}

// Timer function used to delay for "msec" milliseconds
void timer(int msec) {	 
	int i;
	TR1 = 1;              // Start Timer 1
	for(i = 0; i < msec; i++) {
		while(TF1 == 0);   // Wait until Timer 1 overflows
		TF1 = 0;          // Clear the overflow flag
	}
	TR1 = 0;              // Stop Timer 1
}

void main() {
	int i, p;
	char count;

	// Initialize LCD
	lcd_cmd(0x38);        // Set 2-line, 5x7 matrix
	lcd_cmd(0x0E);        // Display on, cursor blinking
	lcd_cmd(0x01);        // Clear display
	
	// Initialize Timer 1 in Mode 2 (8-bit auto-reload)
	TMOD = 0x20;
	TH1 = -23;            // Set baud rate timer value for servo
	
	output = 0;           // Initialize motor output to 0
	count = 21;           // Start with a default servo angle

	lcd_DisplayString(" SERVO ANGLE ");  // Display string on LCD
	shape();                            // Create degree sign

	while(1) {
		// If count exceeds maximum, reset to 21
		if(count >= 104)
			count = 21;
		else {
			// Button press logic to increase or decrease angle
			if(btn == 0) {
				count++;   // Increment angle
			}
			if(btn1 == 0) {
				count--;   // Decrement angle
			}
		}

		// Calculate angle in degrees
		lcd_cmd(0xC0);               // Move to next line on LCD
		p = 2.5 * (count - 21);      // Convert count to degrees
		lcd_data_int(p);             // Display the angle in degrees

		// Loop to generate PWM signal for the servo motor
		for(i = 0; i < 50; i++) {
			output = 1;              // Set output pin high
			timer(count);            // Generate high pulse proportional to angle
			output = 0;              // Set output pin low
			timer(360);              // Generate low pulse (complete 20ms period)

			display_num_7seg(p);     // Display angle on 7-segment display
			if(p > 180)              // Restrict angle to 180 degrees
				p = 0;

			values_on_ssd();          // Update the 7-segment display
		}
	}
}

// Function to display an integer on the LCD
void lcd_data_int(int time_val) {		
	int a;
	
	// Display hundreds place
	a = time_val / 100;
	lcd_data(a + 48);  // Convert integer to character and display
	time_val = time_val % 100;

	// Display tens place
	a = time_val / 10;
	lcd_data(a + 48);

	// Display ones place
	a = time_val % 10;
	lcd_data(a + 48);
}

// Function to send a command to the LCD
void lcd_cmd(unsigned char item) {		
	P0 = item;         // Place command on data bus
	rs = 0;            // Register select = 0 for command
	rw = 0;            // Write mode
	en = 1;            // Enable LCD
	delay(1);          // Short delay for command to execute
	en = 0;
}

// Function to send data to the LCD
void lcd_data(unsigned char item) {		
	P0 = item;         // Place data on data bus
	rs = 1;            // Register select = 1 for data
	rw = 0;            // Write mode
	en = 1;            // Enable LCD
	delay(1);          // Short delay for data to execute
	en = 0;
}

// Function to display a string on the LCD
void lcd_DisplayString(char *string_ptr) {
	while(*string_ptr) {
		lcd_data(*string_ptr++);  // Send characters one by one
	}
}

// Function to display a number on a 4-digit 7-segment display
void display_num_7seg(unsigned char num) {
	int i = 3;
	while(num != 0) {
		values[i] = num % 10;  // Extract digits one by one
		num = num / 10;
		i--;
	}
}

// Function to update the 7-segment display
void values_on_ssd(void) {
	switch(values[index]) {
		case 0: P2 = select[index]; P0 = message[0]; break;
		case 1: P2 = select[index]; P0 = message[1]; break;
		case 2: P2 = select[index]; P0 = message[2]; break;
		case 3: P2 = select[index]; P0 = message[3]; break;
		case 4: P2 = select[index]; P0 = message[4]; break;
		case 5: P2 = select[index]; P0 = message[5]; break;
		case 6: P2 = select[index]; P0 = message[6]; break;
		case 7: P2 = select[index]; P0 = message[7]; break;
		case 8: P2 = select[index]; P0 = message[8]; break;
		case 9: P2 = select[index]; P0 = message[9]; break;
		default: break;
	}
	
	index += 1;          // Move to the next digit
	if(index > 3) {      // Reset index if all digits have been displayed
		index = 0;
	}
}
