#include "SIM800C.h"
#include "Arduino.h"
#include <SoftwareSerial.h>

SIM800C sim(7, 8, 9600);

void setup()
{
	sim.restart();
	sim.create_http_request("TCP", "127.0.0.1", "80");
	sim.create_http_request_message("GET",
	                                "send.php?first=10&last=10",
	                                "HTTP/1.1",
	                                "Connection: close\nAccpect: */*\n\n");

}

void loop()
{

}