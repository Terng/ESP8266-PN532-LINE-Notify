#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>
 
PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);
 
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti wifiMulti;
const char* ssid = "HUAWEIP10";
#define LINE_TOKEN "pTPaHTPOx4dbfezQLXhMZbU6j3NdDxxnBn60x61nYAw"
String MSG = "TAG-LINEnotify";
void LINE_NOTI1(String MSG);

void setup() {
 Serial.begin(115200);
 Serial.println("Hello!");

 nfc.begin();
 Serial.println("Waiting for NDEF Reader ...");

 wifiMulti.addAP("HUAWEIP10", "1212312121");
 wifiMulti.addAP("HUAWEIP10", "1212312121");

 if (wifiMulti.run() != WL_CONNECTED) {
 Serial.println("WiFi not connected!");
 delay(1000);
 } else {
 Serial.println("WiFi connected!");
 }
 delay(500);
}

void loop() {
 if (wifiMulti.run() != WL_CONNECTED) {
 Serial.println("WiFi not connected!");
 delay(1000);
 } else {}

 Serial.println("\nScan an NFC tag\n");
 if (nfc.tagPresent()) // Do an NFC scan to see if an NFC tag is present
 {
 NfcTag tag = nfc.read(); // read the NFC tag
 if (tag.hasNdefMessage())
 {
 NdefMessage message = tag.getNdefMessage();
 for (int i = 0; i < message.getRecordCount(); i++)
 {
 NdefRecord record = message.getRecord(i);
 int payloadLength = record.getPayloadLength();
 byte payload[payloadLength];
 record.getPayload(payload);
// Serial.write(payload, payloadLength);
 
 /*--------Char2String ---------*/
 int startChar = 0; 
 if (record.getTnf() == TNF_WELL_KNOWN && record.getType() == "T") { // text message
 // skip the language code
 startChar = payload[0] + 1;
 } else if (record.getTnf() == TNF_WELL_KNOWN && record.getType() == "U") { // URI
 // skip the url prefix (future versions should decode)
 startChar = 1;
 }
 
 String payloadAsString = "";
 for (int c = startChar; c < payloadLength; c++) {
 payloadAsString += (char)payload[c];
 }

 Serial.print("\n MSG = ");
 Serial.print(payloadAsString);
 LINE_NOTI1(payloadAsString);
 delay(500);
 }
 }
 }
 delay(1000);
}

void LINE_NOTI1(String MSG) {
 WiFiClientSecure client;

 if (!client.connect("notify-api.line.me", 443)) {
 Serial.println("connection failed");
 return;
 }

 String req = "";
 req += "POST /api/notify HTTP/1.1\r\n";
 req += "Host: notify-api.line.me\r\n";
 req += "Authorization: Bearer " + String(LINE_TOKEN) + "\r\n";
 req += "Cache-Control: no-cache\r\n";
 req += "User-Agent: ESP8266\r\n";
 req += "Content-Type: application/x-www-form-urlencoded\r\n";
 req += "Content-Length: " + String(String("message=" + MSG).length()) + "\r\n";
 req += "\r\n";
 req += "message=" + MSG;
 // Serial.println(req);
 client.print(req);

 delay(20);

 // Serial.println("-------------");
 while (client.connected()) {
 String line = client.readStringUntil('\n');
 if (line == "\r") {
 break;
 }
 //Serial.println(line);
 }
 Serial.println("\n------ Send Successfully ------");
}
