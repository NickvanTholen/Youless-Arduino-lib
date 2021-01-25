# YOULESS ARDUINO LIBRARY

This is the readme for the Youless Arduino library, it contains some usefull information on how to use the library.

## DESCRIPTION

This is a library for use with the Youless energy monitor, this library uses a TCP connection on port 80 to get data from the web interface. The library also does the formating of the data. For more information about the Youless energy monitor visit their [website](https://www.youless.nl/watis.html).

## FUNCTIONS

### Setup

To use this library it's mandatory to include some other library to connect to the internet. Below you will find the apropiate instructions for each of the commonly used boards.

#### **Arduino Ethernet**

```CPP
#include <SPI.h>
#include <Ethernet.h>

// The media acces control (MAC) adress for the ethernet shield:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
// The IP adress for the shield:
byte ip[] = {10, 0, 0, 177};

void setup() {
    Ethernet.begin(mac, ip);
}

void loop() {
    // Your main code here
}
```

#### **ESP8266** or **ESP32**

```CPP
// #include <ESP8266WiFi.h> // Uncomment for use with the ESP8266
// #include <WiFi.h> // Uncomment for use with the ESP32

// Your access point SSID
const char* ssid = "Your SSID";
// Your access point password
const char* password = "Your Password";

void setup() {
    Serial.print("Trying to connect to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password); // Begin the WiFi connection
    Serial.println("Connecting");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Adress: ");
    Serial.println(WiFi.localIP());
}

void loop() {
    // Your main code here
}
```

### Available Functions

- **Constructor** - YoulessMonitor _your_name_(host, conType), host is the IP address of the Youless monitor as a variable of the type IPAddress. For conType you can select `WIRED` for a wired connection with the ethernet library or `WIRELESS` for a wireless connection with a WiFi library.
- **Destructor** - This function has no implementation yet.
- **getBasicStatus** - _your_name_.getBasicStatus(), this function returns a formatted JSON document with basic gatherd data from the Youless Monitor, the type of the returned variable is a `DynamicJsonDocument`. Below is an example of the returned data, for more information on the data visit the [Youless-Wiki](https://wiki.td-er.nl/index.php?title=YouLess).

```json
{
  "cnt": " 3876,188",
  "pwr": 35,
  "lvl": 0,
  "dev": "",
  "det": "&#x25A0;",
  "con": "OK",
  "sts": "(02)",
  "cs0": " 0,000",
  "ps0": 0,
  "raw": 0
}
```

- **readCounter** - _your_name_.readCounter(), this function returns the count of the energy meter as floating point number.
- **readPower** - _your_name_.readPower(), this function returns the actual power usage as long variable. _**Note** This function can also return a negative value wich means energy consumption is lower then the energy production._
- **readConnectionStatus** - _your_name_.readConnectionStatus(), this function returns `false` if the Youless monitor is not connected to the energy meter and `true` if the Youless monitor is connected to the energy meter.
- **getUploadedValues** - _your_name_.getUploadedValues(), this function returns a formatted JSON document with meter data from the Youless Monitor, the type of the returned variable is a `DynamicJsonDocument`. Below is an example of the returned data, for more information on the data visit the [Youless-Wiki](https://wiki.td-er.nl/index.php?title=YouLess).

```json
{
  "tm": 1611571509,
  "net": 3876.281,
  "pwr": -35,
  "ts0": 1605604200,
  "cs0": 0.0,
  "ps0": 0,
  "p1": 5631.23,
  "p2": 4916.995,
  "n1": 2053.916,
  "n2": 4618.028,
  "gas": 4547.905,
  "gts": 2101251140
}
```

- **readConsumptionCounter** - _your_name_.readConsumptionCounter(tarrif), tarrif can be left empty to return the total consumption counter or tarrif can be `LOW` to return the low tarrif consumption counter or tarrif can be `HIGH` to return the high tarrif consumption counter. This function returns a value with the type of float.
- **readProductionCounter** - _your_name_.readProductionCounter(tarrif), tarrif can be left empty to return the total production counter or tarrif can be `LOW` to return the low tarrif production counter or tarrif can be `HIGH` to return the high tarrif production counter. This function returns a value with the type of float.
- **readGasCounter** - _your_name_.readGasCounter(), this function returns the count of the gas meter as floating point number.

## ROADMAP

- [x] Add basic functionality
- [ ] Add functions for the rest of the json data.
- [ ] Add functions to retrieve historical data.
- [ ] Add functions to get s0 data.

## Contributing

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## LICENSE

[MIT](../blob/master/LICENSE)
