#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
int select = 0;
bool flag = false;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  //display.display();
  //delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
}

void listFace(){
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Player");
  display.setCursor(50, 0);
  display.print("2:15");
  display.setCursor(106, 0);
  display.print("42");
  display.print("%");
  display.setCursor(0, 0);
  display.drawLine(0, 12, display.width(), 12, WHITE);
  display.setCursor(3,18);
  display.print("Song 1");
  display.setCursor(3,32);
  display.print("Song 2");
  display.setCursor(3,45);
  display.print("Song 3");
  switch(select){
    case 0:display.drawRoundRect(0,15, 128, 15, 0, WHITE);
            break;
    case 1:display.drawRoundRect(0,29, 128, 15, 0, WHITE);
            break;
    case 2:display.drawRoundRect(0,42, 128, 15, 0, WHITE);
            break;
  }
  display.display(); // Update screen with each newly-drawn line
  delay(1);
}

void playerFace(){
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Player");
  display.setCursor(50, 0);
  display.print("2:15");
  display.setCursor(106, 0);
  display.print("42");
  display.print("%");
  display.setCursor(0, 0);
  display.drawLine(0, 12, display.width(), 12, WHITE);
  display.setCursor(3,20);
  display.print("Song 1");
  display.drawLine(0, 46, display.width(), 46, WHITE);
  display.setCursor(2,50);
  display.print("Playing  ");
  display.drawLine(48, 46, 48, 64, WHITE);
  display.setCursor(51 ,51);
  display.print("Length: ");
  display.print("4:15");
  display.drawLine(0, 63, display.width(), 63, WHITE);
  display.drawLine(0, 46, 0, 64, WHITE);
  display.drawLine(127, 46, 127, 64, WHITE);
  display.display(); // Update screen with each newly-drawn line
  delay(1);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(flag){
    playerFace();
    delay(2000);
    flag = false;
    select = -1;
  }
  else {
    listFace();
  }
  
  delay(1000);

  select += 1;
  if(select == 3) {
    select = 0;
    flag = true;
  }

}
