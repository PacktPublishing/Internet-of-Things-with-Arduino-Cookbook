


# Internet of Things with Arduino Cookbook
This is the code repository for [Internet of Things with Arduino Cookbook](https://www.packtpub.com/hardware-and-creative/internet-things-arduino-cookbook?utm_source=github&utm_medium=repository&utm_campaign=9781785286582) , published by Packt. It contains all the supporting project files necessary to work through the book from start to finish.


##Instructions and Navigation

The code included with this book is meant for use as an aid in performing the exercises and should not be used as a replacement for the book itself.
Used out of context, the code may result in an unusable configuration and no warranty is given.

The code will look like the following:
```
/ Pins
int sensorPin = A0;
void setup() {
 // Serial
 Serial.begin(115200);
}
void loop() {
 // Reading
 int sensorValue = analogRead(sensorPin);
 // Display
 Serial.print("Sensor reading: ");
 Serial.println(sensorValue);
 // Wait
 delay(500);
}
```

##References

[Internet of Things with Arduino Blueprints] (https://www.packtpub.com/hardware-and-creative/internet-things-arduino-blueprints?utm_source=github&utm_medium=repository&utm_campaign=9781785285486)

[Arduino Robotic Projects] (https://www.packtpub.com/hardware-and-creative/arduino-robotic-projects?utm_source=github&utm_medium=repository&utm_campaign=9781783989829)
### Download a free PDF

 <i>If you have already purchased a print or Kindle version of this book, you can get a DRM-free PDF version at no cost.<br>Simply click on the link to claim your free PDF.</i>
<p align="center"> <a href="https://packt.link/free-ebook/9781785286582">https://packt.link/free-ebook/9781785286582 </a> </p>