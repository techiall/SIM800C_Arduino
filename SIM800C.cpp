#include "SIM800C.h"

String SIM800C::get_at_result(const char *at, const bool newline,
                              const unsigned long delay_time)
{
	send_at_command(at, newline, delay_time);
	return serial_event();
}

String SIM800C::create_http_request_message(const char *method,
        const char *url, const char *version,
        const char *head, const char *body)
{
	char request[300] = {0};
	sprintf("%s %s %s\n%s\n%s\n", method, url, version, head, body);
	return String(request);
}

bool SIM800C::tcp_link_init()
{
	restart();
	delay(5000);

	const char *at[] = {
		"AT", "AT+IPR=9600", "AT&W", "AT+CPIN?", "AT+CSQ", "AT+CREG?",
		"AT+CGREG?", "AT+CAGTT=1", "AT+CIPMODE=1", "AT+CSTT=\"CMNET\"",
		"AT+CIICR", "AT+CIFSR"
	};

	String str;
	const unsigned int size = sizeof(at) / sizeof(*at);
	for (int i = 0; i < size; i++)
		str += get_at_result(at[i]);

	Serial.println("--------------------->");
	Serial.println(str);
	Serial.println("---------------------<");

	return find_result(str, "SIM", "Call");
}

bool SIM800C::create_http_request(const char *mothod, const char *ip,
                                  unsigned int port)
{
	char at[40] = {0};
	sprintf(at, "AT+CIPSTART=\"%s\",\"%s\",\"%d\"", mothod, ip, port);


	return find_result(get_at_result(at),
	                   "CONNECT OK", "OK");
}

bool SIM800C::http_send(const char *send)
{
	int len = strlen(send);
	char at[20] = {0};
	sprintf(at, "AT+CIPSEND=%d", len);
	find_result(get_at_result(at), ">", "");

	return find_result(get_at_result(send), "OK");
}

void SIM800C::debug()
{
	while (true) {
		if (sim.available()) {
			Serial.write(sim.read());
		}
		if (Serial.available()) {
			sim.write(Serial.read());
		}
	}
}

void SIM800C::clean_buffer()
{
	while (sim.available())
		sim.read();
}

String SIM800C::serial_event()
{
	String input;
	while (sim.available())
		input += (char)sim.read();
	return input;
}

void SIM800C::send_at_command(const char *at, const bool newline = true,
                              const unsigned long delay_time = 3000)
{
	delay(delay_time);
	clean_buffer();
	if (newline) {
		sim.println(at);
	} else {
		sim.print(at);
	}
}

bool SIM800C::find_result(const String buffer,
                          const char *res1, const char *res2)
{
	return (buffer.indexOf(res1) == -1) &&
	       (buffer.indexOf(res2) == -1);
}

bool SIM800C::restart()
{
	return find_result(get_at_result("AT+CFUN=1,1"),
	                   "OK");
}