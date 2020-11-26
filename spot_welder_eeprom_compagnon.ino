#include <EEPROM.h>
int a,b,c;

void setup() {
  // put your setup code here, to run once:
write_eeprom(14,0);
write_eeprom(15,10);
write_eeprom(55,20);

Serial.begin(115200);



}

void loop() {
  // put your main code here, to run repeatedly:
delay(3000);
a = read_eeprom(0);
Serial.println(a);
delay(3000);
b = read_eeprom(10);
Serial.println(b);
delay(3000);
c = read_eeprom(20);
Serial.println(c);



}


void write_eeprom(int a, int addr) {
  EEPROM.begin(512);
  EEPROM.put(addr, a);
  addr += sizeof(int);
  EEPROM.put(addr, a);
  EEPROM.commit();
}

int read_eeprom(int addr) {
  int a;
  EEPROM.begin(512);
  EEPROM.get(addr, a);
  return a;
}
