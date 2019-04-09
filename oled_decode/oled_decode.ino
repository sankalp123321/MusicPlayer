#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <SoftwareSerial.h>
//#define OLED_RESET 4
#define ENABLE_READUART 1
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);//OLED_RESET);
SoftwareSerial mySerial(10, 11);
//int screen = 0;             //0 for list & 1 for currently playing song
String readStringFromUART;
int i = 0, j = 0;

struct dataDecode {
  int _screen;
  String _timee = "";
  String _battery = "";
  String _songs[5] = {};
  String _currentlyPlaying = "S";
  String _duration = "5";
};

void oledDisplay(struct dataDecode dac);
void setup() {

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);// initialize with the I2C addr 0x3C
  Serial.begin(4800);
  while (!Serial) {
    ;
  }
  mySerial.begin(4800);
  delay(5000);
}

void loop() {
  mySerial.flush();
struct dataDecode dac;
#if ENABLE_READUART
  if (mySerial.available()>0) {
    
    readStringFromUART = mySerial.readStringUntil('#');
    Serial.println(readStringFromUART);
    int ind1 = readStringFromUART.indexOf(';');  //finds location of first ,
    String screen = readStringFromUART.substring(0, ind1);   //captures first data String

    int ind2 = readStringFromUART.indexOf(';', ind1 + 1); //finds location of second ,
    String timee = readStringFromUART.substring(ind1 + 1, ind2 + 1); //captures second data String

    int ind3 = readStringFromUART.indexOf(';', ind2 + 1); //finds location of first ,
    String battery = readStringFromUART.substring(ind2 + 1, ind3 + 1); //captures first data String

    int ind4 = readStringFromUART.indexOf(';', ind3 + 1); //finds location of second ,
    String song = readStringFromUART.substring(ind3 + 1, ind4 + 1); //captures second data String

    int ind5 = readStringFromUART.indexOf(';', ind4 + 1); //finds location of first ,
    String currentlyPlaying = readStringFromUART.substring(ind4 + 1, ind5 + 1); //captures first data String

    int ind6 = readStringFromUART.indexOf(';', ind5 + 1); //finds location of second ,
    String duration = readStringFromUART.substring(ind5 + 1, ind6 + 1); //captures second data String

    dac._timee = timee.substring(5, 9);
    dac._battery = battery.substring(8, 10);
    dac._screen = screen.charAt(8) - 48;
    
    currentlyPlaying = currentlyPlaying.substring(17);
    currentlyPlaying.remove(currentlyPlaying.length()-1,1);
    dac._currentlyPlaying = currentlyPlaying;
    
    dac._duration = duration.substring(9, 13);

    ind1 = song.indexOf(',');  //finds location of first ,
    String songOne = song.substring(0, ind1);   //captures first data String

    ind2 = song.indexOf(',', ind1 + 1); //finds location of second ,
    String songTwo = song.substring(ind1 + 1, ind2 + 1); //captures second data String
    int s1 = songTwo.length();
    songTwo.remove(s1 - 1, 1);

    ind3 = song.indexOf(',', ind2 + 1); //finds location of first ,
    String songThree = song.substring(ind2 + 1, ind3 + 1); //captures first data String
    int s2 = songThree.length();
    songThree.remove(s2 - 1, 1);

    ind4 = song.indexOf(',', ind3 + 1); //finds location of second ,
    String songFour = song.substring(ind3 + 1, ind4 + 1); //captures second data String
    int s3 = songFour.length();
    songFour.remove(s3 - 1, 1);

    ind5 = song.indexOf(';', ind4 + 1); //finds location of first ,
    String songFive = song.substring(ind4 + 1, ind5 + 1); //captures first data String
    int s4 = songFive.length();
    songFive.remove(s4 - 1, 1);

    dac._songs[0] = songOne;
    dac._songs[1] = songTwo;
    dac._songs[2] = songThree;
    dac._songs[3] = songFour;
    dac._songs[4] = songFive;
    //Serial.println(dac._timee);
    //Serial.println(dac._screen);
    oledDisplay(dac);
  }
  
#endif


}

void oledDisplay(struct dataDecode dac) {
  //struct dataDecode decStruct;
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Player   ");
  display.print(dac._timee);
  display.print("     ");
  display.print(dac._battery);
  display.print("%");
  display.print("---------------------\n");
  if (dac._screen == 0) {
    display.print(dac._songs[i]);
    display.print("> \n");
    display.print(dac._songs[i + 1]);
    display.print("\n");
    i++;
    delay(2000);
    display.display();
  }
  else if (dac._screen == 1) {
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("Player    ");
    display.print(dac._timee);
    display.print("    ");
    display.print(dac._battery);
    display.print("%");
    display.print("---------------------\n");
    display.print(dac._currentlyPlaying);
    display.print("\n");

    display.print("Playing....    ");
    display.print(dac._duration);
    display.display();
  }
}
