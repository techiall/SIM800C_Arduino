#include "SIM800C.h"

bool SIM800C::init()
{
	echo(false);
	return sim_status() && net_registration() && gprs_status();
}

bool SIM800C::restart()
{
	return exec("AT+CFUN=1,1") && find_result("OK");
}

bool SIM800C::sim_status()
{
	return exec("AT+CPIN?") && find_result("+CPIN: READY", "OK");
}

bool SIM800C::net_registration()
{
	return exec("AT+CREG?") && find_result("+CREG: 0,1", "OK");
}

bool SIM800C::gprs_status()
{
	return exec("AT+CGATT?") && find_result("+CGATT: 1", "OK");
}

bool SIM800C::apn(const char * apn)
{
	char at[20];
	sprintf(at, "AT+CSTT=\"%s\"", apn);
	return exec(at) && find_result("OK") &&
		exec("AT+CIICR") && find_result("OK") &&
		exec("AT+CIFSR") && find_result(".");
}

bool SIM800C::create_tcp(const char *ip, unsigned int port)
{
	char at[100];
	sprintf(at, "AT+CIPSTART=\"TCP\",\"%s\",\"%d\"", ip, port);
	return exec(at) && find_result("CONNECT OK", "OK");
}

bool SIM800C::tcp_send(const char * send)
{
	int len = strlen(send);
	char at[20];
	sprintf(at, "AT+CIPSEND=%d", len);
	Serial.println(at);
	exec(at) && find_result(">");
	
	return exec(send, 1000 * 3) && find_result("SEND OK");
}

String SIM800C::send_result()
{
	return buffer;
}

unsigned long SIM800C::get_num_localip()
{
	while (!(exec("AT+CIFSR") && find_result(".")));
	buffer.trim();
	unsigned long num = 0;
	int cur = 0;
	for (int i = 3; i >= 0; i--) {
		while (buffer[cur] == '.')
			cur++;
		char *ip = (char *)&num;
		while ('0' <= buffer[cur] && buffer[cur] <= '9')
			ip[i] = ip[i] * 10 + buffer[cur++] - '0';
	}

	return num;
}

String SIM800C::get_str_localip()
{
	while (!(exec("AT+CIFSR") && find_result(".")));
	buffer.trim();
	return buffer;
}

bool SIM800C::create_tcp_server(unsigned int port)
{
	char at[25];
	sprintf(at, "AT+CIPSERVER=%d", port);
	return exec(at) && find_result("SERVER OK");
}

// false -> 单链路 true -> 多链路
bool SIM800C::multi_link_mode(bool flag)
{
	char at[15];
	sprintf(at, "AT+CIPMUX=%d", flag ? 1 : 0);
	return exec(at) && find_result("OK");
}

unsigned int SIM800C::exec(const char * AT, unsigned int timeout)
{
	clean_buffer();
	buffer = "";
	sim.println(AT);
	unsigned int time_start = millis();
	while (millis() - time_start < timeout)
		if (sim.available())
			buffer += (char)sim.read();
	//Serial.println("");
	//Serial.print(AT);
	//Serial.println(" :");
	//Serial.println("-----------");
	//Serial.println(buffer);
	//Serial.println("-----------");
	return buffer.length();
}

// false -> not show AT, true -> show AT
bool SIM800C::echo(bool flag)
{
	char at[10];
	sprintf(at, "ATE%d", flag ? 1 : 0);
	return exec(at) && find_result("OK");
}

// 基站定位
bool SIM800C::base_station_position(double &longitude, double &latitude, unsigned int &precision)
{
	exec("AT+SAPBR=3,1,\"APN\",\"3gnet\"");
	exec("AT+SAPBR=3,1,\"APN\",\"3gnet\"");
	exec("AT+SAPBR=1,1");
	exec("AT+CLBS=1,1");

	const char *str = buffer.c_str();
	int point[3];
	int cnt = 0;
	for (int i = 0; str[i]; i++)
		if (str[i] == ',')
			point[cnt++] = i + 1;
	if (cnt == 3) {
		longitude = atof(str + point[0]);
		latitude = atof(str + point[1]);
		precision = atoi(str + point[2]);
	}

	
	//Serial.println("----------");
	//Serial.println(longitude, 6);
	//Serial.println(latitude, 6);
	//Serial.println(precision);
	//Serial.println("----------");
	return longitude && latitude && precision;
}

bool SIM800C::find_result(const char *res1, const char *res2)
{
	//Serial.print("res1:");
	//Serial.println(buffer.indexOf(res1));
	//Serial.print("res2:");
	//Serial.println(buffer.indexOf(res2));
	return buffer.indexOf(res1) != -1 && buffer.indexOf(res2) != -1;
}

void SIM800C::clean_buffer()
{
	while (sim.available())
		sim.read();
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