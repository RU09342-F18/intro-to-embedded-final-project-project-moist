# The Problem
People who love plants know that in order to be succesfful plants require a consistent amount of water each day.
This can be hard for some poeple that are very busy with work or travel alot.

# Our Solution
We created an automated self watering system that will read the moisture value of a plant. The value can be read
from your mobile device and also when the plant gets to dry that system will water the plant to keep it at a healthy level of moisture.

# How it works
A moisture sensor is inserted into the plant, and the outputted value of that will go to the ADC of the MSP430.
Once that happens the value will be sent to the ESP8266 to be wirelessly transmitted to an MQTT server to allow the user to
see the values from their mobile device. At the same time that is happening our system is constantly monitoring the moisture
values of the system. When the value dips below a certain threshold it will activate a relay which is used to turn on the water pump.
That water pump will then continue pumping water until it reaches the desired moisture value.

# How to use it yourself
All the components you will need for this design is a moisture sensor, MSP430F5529, 5V relay, ESP8266,
water pump, and a breadboard. The moisture sensor will be powered with 3.3V, grounded, and the signal pin is connected to the
ADC pin on the MPS430F5529. The data is put onto a Tx buffer and sent to the Rx pin of the ESP8266. When using the ESP8266 it
requires 3.3V as well has having it grounded and the chip enable set to 3.3V also. Then in order for the moisture value to be
used to turn the relay on and off we choose a pin to turn to high when the moisture value gets below the threshold. So we take
that pin from the MSP430 and connect it to 5V which then connects to the VCC of the relay. The normally open pin on the relay
contains the positive lead of the pump, and the common pin contains the positive lead from a 12V power supply. The ground lead
of the 12V power supply will be supplied to the negative lead of the pump.
