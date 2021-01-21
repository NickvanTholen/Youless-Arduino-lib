// MY OWN LICENSE //

// INCLUDED LICENSES //
/*
The MIT License (MIT)
---------------------

Copyright © 2014-2020 Benoit BLANCHON

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#define ARDUINOJSON_USE_LONG_LONG 1
#include "Arduino.h"
#include "WiFiClient.h"
#include "Ethernet.h"
#include "ArduinoJson.h"
#include "YoulessMonitor.h"

/* CONSTRUCTOR */
YoulessMonitor::YoulessMonitor(IPAddress host, uint8_t conType, uint8_t port)
{
    _host = host;
    _port = port;
    switch (conType)
    {
    case 0:
        _client = new WiFiClient;
        break;

    case 1:
        _client = new EthernetClient;
        break;
    }
    this->_Connect();
}

/* DESTRUCTOR */
YoulessMonitor::~YoulessMonitor()
{
    /* Not Implemented */
}

/* Public Functions*/
// Get basic status from youless
DynamicJsonDocument YoulessMonitor::getBasicStatus()
{
    unsigned long now = millis();
    String Jsonbuffer;
    DynamicJsonDocument doc(256);
    if (now - _lastBasicStatus >= _updateTimeOut)
    {
        Jsonbuffer = this->_GetResponse("/a?f=j");
    }
    else
    {
        Jsonbuffer = _basicStatusString;
    }
    deserializeJson(doc, Jsonbuffer);
    if (doc["Status"] == false)
    {
        deserializeJson(doc, _basicStatusString);
    }
    else
    {
        _basicStatusString = Jsonbuffer;
    }
    return doc;
}

// Get energy count from youless
float YoulessMonitor::readCounter()
{
    DynamicJsonDocument doc(256);
    doc = this->getBasicStatus();
    String tempCount = doc["cnt"];
    tempCount.trim();
    tempCount.replace(",", ".");
    return tempCount.toFloat();
}

// Get power from youless
long YoulessMonitor::readPower()
{
    DynamicJsonDocument doc(256);
    doc = this->getBasicStatus();
    return doc["pwr"];
}

// Check if connection between youless en p1-port is OK
bool YoulessMonitor::readConnectionStatus()
{
    DynamicJsonDocument doc(256);
    doc = this->getBasicStatus();
    if (doc["con"] == "OK")
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Get uploaded values
DynamicJsonDocument YoulessMonitor::getUploadedValues()
{
    unsigned long now = millis();
    String Jsonbuffer;
    DynamicJsonDocument doc(256);
    if (now - _lastUploadedValues >= _uploadTimeOut)
    {
        Jsonbuffer = this->_GetResponse("/e");
    }
    else
    {
        Jsonbuffer = _uploadedValuesString;
    }
    deserializeJson(doc, Jsonbuffer);
    if (doc["Status"] == false)
    {
        deserializeJson(doc, _uploadedValuesString);
    }
    else
    {
        _uploadedValuesString = Jsonbuffer;
    }
    return doc[0];
}

// Read the consumption counter
float YoulessMonitor::readConsumptionCounter(int tarrif)
{
    return this->_readTarrifCounter(tarrif, false);
}

// Read the production counter
float YoulessMonitor::readProductionCounter(int tarrif)
{
    return this->_readTarrifCounter(tarrif, true);
}

// Read the gas counter
float YoulessMonitor::readGasCounter()
{
    DynamicJsonDocument doc(256);
    doc = this->getUploadedValues();
    return doc["gas"];
}

/* Private Functions */
// Check for connection, if not connected try to connect
bool YoulessMonitor::_Connect()
{
    if (_client->connected())
    {
        return true;
    }
    else
    {
        _client->connect(_host, _port);
        if (_client->connected())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

// Try to get a response from the server
String YoulessMonitor::_GetResponse(String input)
{
    DynamicJsonDocument doc(128);
    if (this->_Connect())
    {
        String line;
        _client->print(String("GET ") + input + " HTTP/1.1\r\n" + "Host: " + _host.toString() + "\r\n" + "Connection: close\r\n\r\n");
        delay(10);
        while (_client->available())
        {
            line = _client->readStringUntil('\r');
        }
        line.trim();
        if (line != "")
        {
            return line;
        }
    }
    //Serial.println("No connection available!");
    doc["Status"] = false;
    String retData;
    serializeJson(doc, retData);
    return retData;
}

// Read the tarrif counter false = consumption, true = production
float YoulessMonitor::_readTarrifCounter(int tarrif, bool select)
{
    float retValue = 0;
    DynamicJsonDocument doc(256);
    doc = this->getUploadedValues();
    switch (tarrif)
    {
    case -1:
        float temp;
        if (select)
        {
            temp = doc["n1"];
            retValue = doc["n2"];
            retValue += temp;
        }
        else
        {
            temp = doc["p1"];
            retValue = doc["p2"];
            retValue += temp;
        }
        break;

    case 0:
        if (select)
        {
            retValue = doc["n1"];
        }
        else
        {
            retValue = doc["p1"];
        }
        break;

    case 1:
        if (select)
        {
            retValue = doc["n2"];
        }
        else
        {
            retValue = doc["p2"];
        }
        break;

    default:
        retValue = -1;
        break;
    }
    return retValue;
}