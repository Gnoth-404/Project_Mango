/*
socket.io-arduino-client: a Socket.IO client for the Arduino
Based on the Kevin Rohling WebSocketClient & Bill Roy Socket.io Lbrary
Based on Florent Vidal Socket.io-v1.x-Library
Copyright 2017 Ngô Huỳnh Ngọc Khánh (nhnkhanh@arduino.vn)
Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:
JSON support added using https://github.com/bblanchon/ArduinoJson

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

#define ESP8266

#include <SocketIOClient.h>

String tmpdata = ""; //External variables
String RID = "";
String Rname = "";
String Rcontent = "";
String Rfull = "";

unsigned char on_disconnected_code[] = {129, 2, 52, 49};
unsigned char force_disconnected_code[] = {136, 2, 3, 232};

bool SocketIOClient::connect(char thehostname[], int theport, char thensp[])
{
	timeout_count = 0;
    while (!client.connect(thehostname, theport)) {
#ifdef DEBUG
        Serial.println(F("Khong the ket noi den Socket Server"));
#endif
        delay(200);
    }
#ifdef DEBUG
    Serial.println(F("Ket noi duoc thiet lap"));
#endif
    /*if (!client.connect(thehostname, theport)) return false;*/
    hostname = thehostname;
    port = theport;
    nsp = thensp;
    sendHandshake(hostname);
#ifdef DEBUG
    Serial.println(F("Qua trinh bat tay duoc thiet lap"));
#endif
    bool result = readHandshake();
    return result;
}

bool SocketIOClient::connectHTTP(char thehostname[], int theport)
{
    if (!client.connect(thehostname, theport))
        return false;
    hostname = thehostname;
    port = theport;
    return true;
}

bool SocketIOClient::reconnect(char thehostname[], int theport, char thensp[])
{
    return connect(thehostname, theport, thensp);
}
//fix disconnected event
bool SocketIOClient::connected()
{
	char ok = true;
	for (char i = 0; i < 4; i++) {
		if ((unsigned char)databuffer[i] != on_disconnected_code[i])
			ok = false;
	}
	if (ok) {
		disconnect();
	} else {
		for (char i = 0; i < 4; i++) {
			if ((unsigned char)databuffer[i] != force_disconnected_code[i])
				ok = false;
		}
		if (ok) {
			disconnect();
		}
	}
    return client.connected();
}

void SocketIOClient::disconnect()
{
    client.stop();
}

// find the nth colon starting from dataptr
void SocketIOClient::findColon(char which)
{
    while (*dataptr) {
        if (*dataptr == ':') {
            if (--which <= 0)
                return;
        }
        ++dataptr;
    }
}

// terminate command at dataptr at closing double quote
void SocketIOClient::terminateCommand(void)
{
    dataptr[strlen(dataptr) - 3] = 0;
}

int SocketIOClient::parser(int index)
{
    String rcvdmsg = "";
    int sizemsg = databuffer[index + 1]; // 0-125 byte, index ok        Fix provide by Galilei11. Thanks
    if (databuffer[index + 1] > 125) {
        sizemsg = databuffer[index + 2]; // 126-255 byte
        index += 1; // index correction to start
    }
#ifdef DEBUG
    Serial.print(F("Kich thuoc tin nhan = "));
#endif //Can be used for debugging
#ifdef DEBUG
    Serial.println(sizemsg);
#endif //Can be used for debugging
    for (int i = index + 2; i < index + sizemsg + 2; i++)
        rcvdmsg += (char)databuffer[i];
#ifdef DEBUG
    Serial.print("Tin nhan nhan duoc la = ");
#endif //Can be used for debugging
#ifdef DEBUG
    Serial.println(rcvdmsg);
#endif //Can be used for debugging
	int setKey = 0;
	if (int(rcvdmsg[setKey]) == 129 || int(rcvdmsg[setKey]) == 95)
		setKey++;
	
    switch (rcvdmsg[setKey]) {
    case '2':
#ifdef DEBUG
        Serial.println(F("Nhan duoc goi tin Ping - Tra loi Pong"));
#endif
        heartbeat(1);
        break;

    case '3':
#ifdef DEBUG
        Serial.println(F("Nhan duoc goi tin Pong - Moi thu hoan hao"));
#endif
        break;

    case '4':
        switch (rcvdmsg[setKey + 1]) {
        case '0':
#ifdef DEBUG
            Serial.println(F("Da xac nhan nang cap len Websocket"));
#endif
            break;
        case '2':
			#ifdef DEBUG
				Serial.println("42");
			#endif
            RID = rcvdmsg.substring(rcvdmsg.indexOf("[\"") + 2, rcvdmsg.indexOf("\","));
            Rfull = rcvdmsg.substring(rcvdmsg.indexOf("\",") + 2, rcvdmsg.length() - 1);
            Rname = rcvdmsg.substring(rcvdmsg.indexOf("\",") + 4, rcvdmsg.indexOf("\":"));
            Rcontent = rcvdmsg.substring(rcvdmsg.indexOf("\":") + 3, rcvdmsg.indexOf("\"}"));
			#ifdef DEBUG
				Serial.println(RID);
				Serial.println(Rfull);
			#endif
            break;
        }
    }
	return sizemsg;
}

bool SocketIOClient::monitor()
{
	
    int index = -1;
    int index2 = -1;
    String tmp = "";
    *databuffer = 0;

    if (!client.connected()) {
        if (!client.connect(hostname, port)) 
            return 0;
    }
	
    if (!client.available()) {
        return 0;
    }
    char which;

    while (client.available()) {
        readLine();
        tmp = databuffer;
#ifdef DEBUG
        Serial.println(databuffer);
#endif
        dataptr = databuffer;
        index = tmp.indexOf((char)129); 
		index2 = tmp.indexOf((char)force_disconnected_code[0]);
		#ifdef DEBUG
			Serial.println("Monitor");
		#endif
		int lengthParser = 0;
        if (index != -1) {
            lengthParser = parser(index);
			#ifdef DEBUG
			Serial.println("index1");
			#endif
        } else 
			lengthParser = parser(-1);
		if (index2 != -1) {
			#ifdef DEBUG
			Serial.println("index2");
			#endif
			char ok = true;
			int length = tmp.length();
			for (char i = 0; i < 4; i++) {
				if (index2 + i >= length) {
					ok = false;
					break;
				}
				if ((unsigned char)tmp[index2 + i] != force_disconnected_code[i])
					ok = false;
			}
			if (ok) {
				disconnect();
			}
		}
		return lengthParser > 0;
    }
    return 0;
}

void SocketIOClient::sendHandshake(char hostname[])
{
    String request = "";
    request += "GET /socket.io/1/?EIO=3&transport=polling&b64=true HTTP/1.1\r\n";
    request += "Host: ";
    request += hostname;
    request += "\r\n";
    request += "Origin: Arduino\r\n";
    request += "Connexion: keep-alive\r\n\r\n";

    client.print(request);
#ifdef DEBUG
    Serial.println(request);
#endif
}

bool SocketIOClient::waitForInput(void)
{
    unsigned long now = millis();
    while (!client.available() && ((millis() - now) < 30000UL)) {
        ;
    }
    return client.available();
}

void SocketIOClient::eatHeader(void)
{
    while (client.available()) { // consume the header
        readLine();
        if (strlen(databuffer) == 0)
            break;
    }
}

bool SocketIOClient::readHandshake()
{

    if (!waitForInput())
        return false;

    // check for happy "HTTP/1.1 200" response
    readLine();
    if (atoi(&databuffer[9]) != 200) {
        while (client.available())
            readLine();

        client.stop();
        return false;
    }
    eatHeader();
    readLine();
    String tmp = databuffer;

    int sidindex = tmp.indexOf("sid");
    int sidendindex = tmp.indexOf("\"", sidindex + 6);
    int count = sidendindex - sidindex - 6;

    for (int i = 0; i < count; i++) {
        sid[i] = databuffer[i + sidindex + 6];
    }
#ifdef DEBUG
    Serial.println(F(" "));
    Serial.print(F("Connected. SID="));
    Serial.println(sid);
#endif // sid:transport:timeout

    while (client.available())
        readLine();
    client.stop();
    delay(200);
    if (!client.connect(hostname, port)) {
#ifdef DEBUG
        Serial.print(F("Websocket failed."));
#endif
        return false;
    }

#ifdef DEBUG
    Serial.println(F("Connecting via Websocket"));
#endif

    String request = "";
    request += "GET /socket.io/1/websocket/?EIO=3&transport=websocket&b64=true&sid=";
    request += sid;
    request += " HTTP/1.1\r\n";
    request += "Host: ";
    request += hostname;
    request += "\r\n";
    request += "Origin: Arduino\r\n";
	request += "Sec-WebSocket-Extensions:permessage-deflate; client_max_window_bits\r\n";
    request += "Sec-WebSocket-Key: ";
    request += sid;
    request += "\r\n";
    request += "Sec-WebSocket-Version: 13\r\n";
    request += "Upgrade: websocket\r\n";
    request += "Connection: Upgrade\r\n";
    request += "\r\n";

    client.print(request);

#ifdef DEBUG
    Serial.println(F("Websocket send"));
#endif

    if (!waitForInput())
        return false;
    readLine();
    if (atoi(&databuffer[9]) != 101) { // check for "HTTP/1.1 101 response, means Updrage to Websocket OK
        while (client.available())
            readLine();

        client.stop();
        return false;
    }
    readLine();
    readLine();
    readLine();
    for (int i = 0; i < 28; i++) {
        key[i] = databuffer[i + 22]; //key contains the Sec-WebSocket-Accept, could be used for verification
    }

    eatHeader();

    /*
	Generating a 32 bits mask requiered for newer version
	Client has to send "52" for the upgrade to websocket
	*/
    randomSeed(analogRead(0));
    String mask = "";
    String masked = "52";
    String message = "52";
    for (int i = 0; i < 4; i++) //generate a random mask, 4 bytes, ASCII 0 to 9
    {
        char a = random(48, 57);
        mask += a;
    }

    for (int i = 0; i < message.length(); i++)
        masked[i] = message[i] ^ mask[i % 4]; //apply the "mask" to the message ("52")

    client.print((char)0x81); //has to be sent for proper communication
    client.print((char)130); //size of the message (2) + 128 because message has to be masked
    client.print(mask);
    client.print(masked);

    monitor(); // treat the response as input
    if (nsp != "") {
        sendNSP();
    }

    return true;
}

void SocketIOClient::getREST(String path)
{
    String message = "GET /" + path + "/ HTTP/1.1";
    client.println(message);
    client.print(F("Host: "));
    client.println(hostname);
    client.println(F("Origin: Arduino"));
    client.println(F("Connection: close\r\n"));
}

void SocketIOClient::postREST(String path, String type, String data)
{
    String message = "POST /" + path + "/ HTTP/1.1";
    client.println(message);
    client.print(F("Host: "));
    client.println(hostname);
    client.println(F("Origin: Arduino"));
    client.println(F("Connection: close\r\n"));
    client.print(F("Content-Length: "));
    client.println(data.length());
    client.print(F("Content-Type: "));
    client.println(type);
    client.println(F("\r\n"));
    client.println(data);
}

void SocketIOClient::putREST(String path, String type, String data)
{
    String message = "PUT /" + path + "/ HTTP/1.1";
    client.println(message);
    client.print(F("Host: "));
    client.println(hostname);
    client.println(F("Origin: Arduino"));
    client.println(F("Connection: close\r\n"));
    client.print(F("Content-Length: "));
    client.println(data.length());
    client.print(F("Content-Type: "));
    client.println(type);
    client.println(F("\r\n"));
    client.println(data);
}

void SocketIOClient::deleteREST(String path)
{
    String message = "DELETE /" + path + "/ HTTP/1.1";
    client.println(message);
    client.print(F("Host: "));
    client.println(hostname);
    client.println(F("Origin: Arduino"));
    client.println(F("Connection: close\r\n"));
}

void SocketIOClient::readLine()
{
	char oldC = 0;
    for (int i = 0; i < DATA_BUFFER_LEN; i++)
        databuffer[i] = ' ';
    dataptr = databuffer;
    while (client.available() && (dataptr < &databuffer[DATA_BUFFER_LEN - 2])) {
        char c = client.read();
#ifdef DEBUG
        Serial.print(c);
		//Serial.print(' ');
		//Serial.println((int)c);
#endif //Can be used for debugging
        if (c == 0) {
#ifdef DEBUG
            Serial.print("");
#endif
        }
        else if (c == 255) {
#ifdef DEBUG
            Serial.println(F(""));
#endif
        }
        else if (c == '\r') {
            ;
        }
        else if (c == '\n' || int(c) == 129) 
            break;
        else {
            *dataptr++ = c;
		}
		oldC = c;
    }
    *dataptr = 0;
}

void SocketIOClient::sendMessage(String message)
{
    String dataSend = "";
    int header[10];
    header[0] = 0x81;
    int msglength = message.length();
    randomSeed(analogRead(0));
    String mask = "";
    String masked = message;
    for (int i = 0; i < 4; i++) {
        char a = random(1, 10);
        mask += a;
    }
    for (int i = 0; i < msglength; i++)
        masked[i] = message[i] ^ mask[i % 4];

    dataSend += (char)header[0];

    if (msglength <= 125) {
        header[1] = msglength + 128;
        dataSend += (char)header[1];
    }
    else if (msglength >= 126 && msglength <= 65535) {
        header[1] = 126 + 128;
        dataSend += (char)header[1];
        header[2] = (msglength >> 8) & 255;
        dataSend += (char)header[2];
        header[3] = (msglength)&255;
        dataSend += (char)header[3];
    }
    else {
        int num = 1;
        header[num] = 127 + 128;
        dataSend += (char)header[num];
        for (int i = 56; i > 0; i -= 8) {
            num++;
            header[num] = (msglength >> i) & 255;
            dataSend += (char)header[num];
        }
        header[9] = (msglength)&255;
        dataSend += (char)header[9];
    }
    dataSend += mask;
    dataSend += masked;

    client.print(dataSend);
}

void SocketIOClient::send(String RID, String Rname, String Rcontent)
{

    String message = "42/" + String(nsp) + ",[\"" + RID + "\",{\"" + Rname + "\":\"" + Rcontent + "\"}]";

    sendMessage(message);
}

void SocketIOClient::send(String RID, String json)
{

    String message = "42/" + String(nsp) + ",[\"" + RID + "\"," + json + "]";

    sendMessage(message);
}

void SocketIOClient::sendNSP()
{

    String message = "40/" + String(nsp);

    sendMessage(message);
}

void SocketIOClient::sendJSON(String RID, String JSON)
{
    String message = "42/" + String(nsp) + ",[\"" + RID + "\"," + JSON + "]";

    sendMessage(message);
}

void SocketIOClient::heartbeat(int select)
{
    randomSeed(analogRead(0));
    String mask = "";
    String masked = "";
    String message = "";
    String dataSend = "";
    if (select == 0) {
        masked = "2";
        message = "2";
    }
    else {
        masked = "3";
        message = "3";
    }
    for (int i = 0; i < 4; i++) //generate a random mask, 4 bytes, ASCII 0 to 9
    {
        char a = random(1, 10);
        mask += a;
    }

    for (int i = 0; i < message.length(); i++)
        masked[i] = message[i] ^ mask[i % 4]; //apply the "mask" to the message ("2" : ping or "3" : pong)

    dataSend += (char)0x81;
    dataSend += (char)129;
    dataSend += mask;
    dataSend += masked;
    client.print(dataSend);
}

void SocketIOClient::clear()
{
    RID = "";
    Rname = "";
    Rcontent = "";
    Rfull = "";
}
