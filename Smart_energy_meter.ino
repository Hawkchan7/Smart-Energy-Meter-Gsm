/*
========================================================
SMART ENERGY METER INTERFACE WITH GSM
========================================================

Project Description:
This project implements a Smart Energy Meter system
using Arduino and GSM module for remote monitoring
of electricity consumption.

Features:
- Energy usage monitoring
- Current sensor interfacing
- GSM-based SMS transmission
- LCD display output
- Automatic billing calculation
- Embedded system implementation

========================================================
HARDWARE COMPONENTS
========================================================

1. Arduino UNO
2. GSM Module (SIM800L / SIM900A)
3. ACS712 Current Sensor
4. 16x2 LCD Display
5. Jumper Wires
6. Power Supply

========================================================
PIN CONNECTIONS
========================================================

ACS712 Current Sensor:
--------------------------------
VCC  -> Arduino 5V
GND  -> Arduino GND
OUT  -> Arduino A0

GSM Module:
--------------------------------
TX   -> Arduino Pin 9
RX   -> Arduino Pin 10
GND  -> Arduino GND
VCC  -> External 5V Supply

16x2 LCD Display:
--------------------------------
RS   -> Arduino Pin 12
EN   -> Arduino Pin 11
D4   -> Arduino Pin 5
D5   -> Arduino Pin 4
D6   -> Arduino Pin 3
D7   -> Arduino Pin 2

========================================================
WORKING PRINCIPLE
========================================================

1. Current sensor measures current consumption.
2. Arduino calculates:
      Power = Voltage × Current

3. Energy consumption is calculated:
      Energy (kWh) = Power × Time

4. Billing amount is estimated.

5. GSM module sends SMS notification.

6. LCD displays energy usage and bill.

========================================================
*/

#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

/* GSM Module Communication Pins */
SoftwareSerial gsm(9, 10);

/* LCD Pin Configuration */
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

/* Current Sensor Analog Pin */
const int sensorPin = A0;

/* System Variables */
float voltage = 230.0;
float current = 0.0;
float power = 0.0;
float energy = 0.0;
float bill = 0.0;

void setup()
{
    /* Initialize Serial Monitor */
    Serial.begin(9600);

    /* Initialize GSM Module */
    gsm.begin(9600);

    /* Initialize LCD */
    lcd.begin(16, 2);

    /* Startup Message */
    lcd.print("SMART ENERGY");
    lcd.setCursor(0, 1);
    lcd.print("METER SYSTEM");

    delay(3000);

    lcd.clear();

    Serial.println("System Initialized");
}

void loop()
{
    /*
    ====================================================
    SENSOR DATA ACQUISITION
    ====================================================
    */

    int sensorValue = analogRead(sensorPin);

    /*
    Convert Analog Reading to Current Value
    */

    current = (sensorValue / 1023.0) * 5.0;

    /*
    ====================================================
    POWER CALCULATION
    ====================================================
    */

    power = voltage * current;

    /*
    ====================================================
    ENERGY CALCULATION
    ====================================================
    */

    energy = power / 1000.0;

    /*
    ====================================================
    BILLING SYSTEM
    ====================================================
    */

    if (energy <= 100)
    {
        bill = energy * 5;
    }
    else
    {
        bill = energy * 8;
    }

    /*
    ====================================================
    LCD DISPLAY OUTPUT
    ====================================================
    */

    lcd.setCursor(0, 0);
    lcd.print("Energy:");
    lcd.print(energy);
    lcd.print("kWh ");

    lcd.setCursor(0, 1);
    lcd.print("Bill:Rs ");
    lcd.print(bill);

    /*
    ====================================================
    SERIAL MONITOR OUTPUT
    ====================================================
    */

    Serial.print("Current: ");
    Serial.println(current);

    Serial.print("Power: ");
    Serial.println(power);

    Serial.print("Energy: ");
    Serial.println(energy);

    Serial.print("Bill: ");
    Serial.println(bill);

    /*
    ====================================================
    GSM SMS TRANSMISSION
    ====================================================
    */

    sendSMS();

    delay(5000);
}

/*
========================================================
FUNCTION: sendSMS()
========================================================
This function sends energy consumption details
to the user through GSM module.
========================================================
*/

void sendSMS()
{
    gsm.println("AT");
    delay(1000);

    gsm.println("AT+CMGF=1");
    delay(1000);

    gsm.println("AT+CMGS=\"+911234567890\"");
    delay(1000);

    gsm.print("SMART ENERGY METER\n");

    gsm.print("Energy Used: ");
    gsm.print(energy);
    gsm.println(" kWh");

    gsm.print("Estimated Bill: Rs ");
    gsm.println(bill);

    delay(1000);

    /* CTRL+Z Command to Send SMS */
    gsm.write(26);

    Serial.println("SMS Sent Successfully");
}