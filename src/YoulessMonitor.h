// MY OWN LICENSE //
/*
MIT License

Copyright (c) 2021 Nick van Tholen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

// INCLUDED LICENSES //
/*
The MIT License (MIT)
---------------------

Copyright © 2014-2020 Benoit BLANCHON

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef YOULESSMONITOR_H
#define YOULESSMONITOR_H

class Client;

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Client.h>

#define WIRELESS 0
#define WIRED 1

class YoulessMonitor
{
public:
    // Constructor and Destructor
    YoulessMonitor(IPAddress host, uint8_t conType, uint8_t port = 80);
    ~YoulessMonitor();
    // Public variables
    // Public functions
    DynamicJsonDocument getBasicStatus();
    float readCounter();
    long readPower();
    bool readConnectionStatus();
    DynamicJsonDocument getUploadedValues();
    float readConsumptionCounter(int tarrif = -1);
    float readProductionCounter(int tarrif = -1);
    float readGasCounter();

private:
    // Private variables
    IPAddress _host;
    uint8_t _port;
    String _basicStatusString;
    String _uploadedValuesString;
    const unsigned long _updateTimeOut = 5000;
    const unsigned long _uploadTimeOut = 30 * 1000;
    unsigned long _lastBasicStatus = 0;
    unsigned long _lastUploadedValues = 0;
    // Private functions
    bool _Connect();
    String _GetResponse(String input);
    float _readTarrifCounter(int tarrif, bool select);
    // Private classes
    Client *_client;
};

#endif