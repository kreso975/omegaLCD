#include "I2CAccessSystem.h"
#include "I2CDevice.h"
#include "LCD_I2C.h"
#include <iostream>
#include <string>

// Set up the following as it applies to your specific LCD device
#define I2C_ADDR 0x27                   // <<----- The I2C address for your LCD.  Find it using "i2cdetect -y 0"
#define En_pin 2                        // <<----- The enable pin for your LCD
#define Rw_pin 1                        // <<----- The read/write pin for your LCD
#define Rs_pin 0                        // <<----- The reset pin for your LCD
#define D4_pin 4                        // <<----- The Data 4 pin for your LCD
#define D5_pin 5                        // <<----- The Data 5 pin for your LCD
#define D6_pin 6                        // <<----- The Data 6 pin for your LCD
#define D7_pin 7                        // <<----- The Data 7 pin for your LCD
#define BACKLIGHT_PIN 3                 // <<----- The Back light pin for your LCD
#define BACKLIGHT_POLARITY POSITIVE     // <<----- The polarity of the Back light pin for your LCD

#define LCD_NUM_COL 16                  // <<----- The number of columns for your LCD
#define LCD_NUM_ROW 2                   // <<----- The number of rows for your LCD


static void show_usage( std::string argv )
{
	std::cerr 	<< "Usage: <option> [<STRING> <STRING>]\n"
				<< "Options:\n"
                << "\t-i, --init\t\tInitialize LCD\n"
				<< "\t-h, --help\t\tShow this help message\n"
				<< "\t-c, --clear\t\tClear LCD screen\n"
				<< "\t-b, --backliteOff\tLCD backlite Off\n"
				<< "\t-B, --backliteOn\t\tLCD backlite On\n"
                << "\t-sl,--scrolleft\t\tScroll the display without changing the RAM\n"
                << "\t-sr,--scrolright\tScroll the display without changing the RAMn\n"
				<< "\t-w, --write\t\tWrite on LCD - [<STRING> and or <STRING>]\n"
				<< std::endl;
}

int main( int argc, char* argv[] )
{
    I2CAccess * i2cAcc;                 // For general I2C Access
    I2CDevice * lcdDevice;              // Specific I2C Device
    LCD_I2C * lcd;                      // The LCD Display
    
    // Create and setup I2C Access on channel 0 - the only channel the Omega has
    i2cAcc = new I2CAccessSystem(0);
    
    // Create and setup I2C LCD device using the I2C Access with the specified I2C Address
    lcdDevice = new I2CDevice(i2cAcc, I2C_ADDR);
    
    // Create the LCD with access via the I2C Device using the specific data for the actual LCD
    lcd = new LCD_I2C(lcdDevice,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin, BACKLIGHT_PIN, BACKLIGHT_POLARITY);

    // Initialise the LCD for columns and rows
    lcd->begin(LCD_NUM_COL, LCD_NUM_ROW);

    // Position LCD cursor to start of first row
	lcd->setCursor( 0,0 );

    if ( argc > 1 )
	{	
		std::string arg = argv[1];
		if ( ( arg == "-h" ) || ( arg == "--help" ) )
			show_usage(argv[0]); // Show help

        else if ( ( arg == "-i" ) || ( arg == "--init" ) )
        {
            lcd->setCursor( 0,0 );
        }
		else if ( ( arg == "-c" ) || ( arg == "--clear" ) )
			lcd->clear(); // Clear LCD - set cursor position to zero
	
		else if ( ( arg == "-b" ) || ( arg == "--backliteOff" ) )
			lcd->noBacklight();
	
		else if ( ( arg == "-B" ) || ( arg == "--backliteOn" ) )
			lcd->backlight();

        else if ( ( arg == "-sl" ) || ( arg == "--scrolleft" ) )
            lcd->scrollDisplayLeft();

        else if ( ( arg == "-sr" ) || ( arg == "--scrolright" ) )
            lcd->scrollDisplayRight();

		else if ( ( arg == "-w" ) || ( arg == "--write" ) )
		{
			if ( argv[2] )
			{
				lcd->print( argv[2] );
			}
			else
			{
				show_usage(argv[0]); // Show help - no String
				return 1;
			}
				
			if ( argv[3] )
			{
				lcd->setCursor( 0,1 ); // Position LCD cursor to start of second row
				lcd->print( argv[3] ); // Output second input parameter to second row of LCD
			}
		}	
	}
	else
	{
		show_usage( argv[0] );
		return 0;
	}

	// Default output to first row of LCD if no input parameters
	//  lcd->print((char *)"Omega says Hi!");

	return 0;
}
 
