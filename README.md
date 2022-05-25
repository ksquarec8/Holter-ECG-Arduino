# Holter-ECG-Arduino
AD8232 three lead ECG sensor have been used with Arduino as a microcontroller, 
An AD8232 sense the heart rhythm and sen the analog signal to Arduino, 
Arduino converts the analog signal to digital data and transfer it to PC through Bluetooth also it stores the digital data in SD card.
In the program section, the digital data obtained from Arduino is being processed. 
First, the data is being filtrated with the help of Bandpass Butter-worth filter of fifth-order with cutoff frequency(0.5-100)Hz 
after that again that data is being filtered with IIR notch filter with 50 Hz cutoff frequency 
to eliminate the AC noise. After that R peak has been detected, After detection of the R peak interval between each Rinterval is being calculated, with help of the RRinterval Heartbeat, Heartbeat variability is being calculated. If the process is in real time the plot is shown in real-time with help of py-serial within the program.
