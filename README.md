# M5StickC-Plus : Voltmeter
The simplest voltmeter program for this voltmeter:

https://shop.m5stack.com/products/voltmeter-unit-ads1115
![image](https://user-images.githubusercontent.com/1586332/196499942-1855ed89-2424-4f96-842b-9a18566a773c.png)

Did you know the example program that comes with the voltmeter is unneceserilly complex, and as you've noticed - very cluttered on screen?

So I've written a bare bones, no frills, full accuracy across the full 32v range.  (not 36v as printed on the label!)

**The details:**            
To be short - the getValue() function works just fine for reading all the range the device is designed for....

**Hear is why:**

The demo code shows how to grab a sequence of raw ADC values, and average them out, and apply the calibration to it. But I've found the library function "getValue" works just fine. The demo's not working around anything, just there for information for the coder. We don't NEED to do it like that.

Also - it measures 0 to 32 volts.... not 36 as shown on the label.         

The adc_raw value is a signed integer: int16_t adc_raw;               
I figure that's to read -36 volts to +36 volts.                

But the device doesn't do that! adc_raw values goes from 65535 for 0 volts, to 0 for 32 volts.. it doesn't read negative voltages at all! (in my version just bought in August 2022)             
So just convert the int16_t : uint16_t unsignedADC_Raw = adc_raw;              

I discovered some facts:          
1: Set the sample time to RATE_8 - the SLOWEST and most stable readings.                
2: PAG_256 needs to be set for the gain, NOT PAG_512.            

This then gives the full range of 0 to 32v to THREE decimal places on the whole range.               

My device does NOT read negative voltages. It reads 0 to 32 volts to the millivolt.                   
I figured that out from some real world measurements, I read UP TO 8 volts can be sensed with PAG_256, but that is NOT the case with the module as it is built! It's specifically designed to measure voltage to 3 decimal places, with an accuracy of a millivolt. 0.000??.              

At 10.009836 volts the raw ADC was 20395             
At 10.009345 volts the raw ADC was 20394                  
It's easy to see that 10v = 20,000 so 30v = 60,000 well within the 65535 value of a 2 byte word.                    
And therefore, each single bit rise = 0.000491 volts (that half a millivolt the tec-specs mention - CONFIRMED)                   
So the meaningful digits for the entire 32v range are xx.xxx            

Running the maths for that "resolution per ADC bit", I get:              
65535 (2 bytes) max * 0.000491 = 32.177685 volts maximum.              
Highest accuracy reading of 3 decimal places, and up to 30 volts = PAG_256                
(The circuit's not built to read higher volts? Setting it less than this maximum gain only reduces accuracy!)                


![image](https://user-images.githubusercontent.com/1586332/196498682-bbaa7d30-88db-4d70-86c1-06e6b1755f60.png)
