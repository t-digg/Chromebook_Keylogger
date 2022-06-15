#include "arduino_secrets.h"
// includes the built in keyboard library
#include <Keyboard.h>

/* Modify the following definitions to fit your wireless and login credentials. */
#define wifi_name SECRET_SSID // Define SSID for your wireless connection.
#define wifi_pass SECRET_WIFIPW // Define the password for your wireless connection.


// advanced settings for outlier cases
#define addPerson false // Set to true if the chrome book is the type where you need to add person and there are user names on the right side
#define reenterWifi false //Set to true to enter wifi information back in after login in the case of an auto logout. (kipp baltimore)
#define differentAccount false // Set to true if you need to click the sign in with a different account link
#define enroll false // Set to true if enrolling chromebook

// Testing URL's
#define micTest "mictests.com" // Mic test URL
#define webcamTest "webcamtests.com" // Webcam test URL
#define keyboardTest "keyboardchecker.com" //keyboard test URL
#define audioTest "audiocheck.net/audiotests_stereo.php" //audio tester
#define batteryTest "battery_test 5" //crosh tester

// Adjust parameters here if running into issues with timing
#define startDelay 35 // Time to wait until keystroak program begins after it is plugged in
#define wifiDelay 35 // Time to wait after the wifi credentials are entered
#define userDelay 20 // Time to wait after the username is entered
#define passwordDelay 35 // Time to wait after the login password is entered

#define school 0 // Pick the school you need credentials for
/*
 * 0. Uncommon Schools
 * 1. Kipp NYC
 * 2. Kipp Baltimore
 * 3. Great Oaks
 * 4. Forte Prep
 * 5. Northwood Charter
 * 6. GCCS
 * 7. Hope Academy
 * 8. Freire Highschool
 * 9. Freire Middleschool
 * 10. Freire Tech
 * 11. Freire Wilmington
 * 12. Whin Music
 * 13. Sankofa
 * 14. Kipp Philly
 * 15. Kipp Albany 1
 */

// Special characters definition
#define KEY_LEFT_CTRL   0x80
#define KEY_LEFT_SHIFT  0x81
#define KEY_LEFT_ALT    0x82
#define KEY_RIGHT_CTRL  0x84
#define KEY_RIGHT_SHIFT 0x85
#define KEY_RIGHT_ALT   0x86
#define KEY_VOLUMEUP		0x80
#define KEY_UP_ARROW    0xDA
#define KEY_DOWN_ARROW  0xD9
#define KEY_LEFT_ARROW  0xD8
#define KEY_RIGHT_ARROW 0xD7
#define KEY_BACKSPACE   0xB2
#define KEY_TAB         0xB3
#define KEY_ENTER       0xB0
#define KEY_ESC         0xB1
#define KEY_CAPS_LOCK   0xC1

// school credentials
struct USERCREDENTIALS
{
  //increase these if amount of characters exceed 50 for ids or 20 for pwds
  char id[50];
  char pwd[20];
};

//When adding a new set of credentials increase this number by one
USERCREDENTIALS credentials[16] =
{
  {SECRET_UNCOMMONUSR, SECRET_UNCOMMONPW},
  {SECRET_KIPPNYCUSR, SECRET_KIPPNYCPW},
  {SECRET_KIPPBALTUSR, SECRET_KIPPBALTPW},
  {SECRET_GREATOAKSUSR, SECRET_GREATOAKSPW},
  {SECRET_FORTEPREPUSR, SECRET_FORTEPREPPW},
  {SECRET_NORTHWOODUSR, SECRET_NORTHWOODPW},
  {SECRET_GCCSUSR, SECRET_GCCSPW},
  {SECRET_HOPEACADEMYUSR, SECRET_HOPEACADEMYPW},
  {SECRET_FREIREHSUSR, SECRET_FREIREHSPW},
  {SECRET_FREIREMSUSR, SECRET_FREIREMSPW},
  {SECRET_FREIRETECHUSR, SECRET_FREIRETECHPW},
  {SECRET_FREIREWILLUSR, SECRET_FREIREWILLPW},
  {SECRET_WHINUSR, SECRET_WHINPW},
  {SECRET_SANKOFAUSR, SECRET_SANKOFAPW},
  {SECRET_KIPPPHILLYUSR, SECRET_KPPPHILLYPW},
  {SECRET_KIPPALBANYUSR, SECRET_KIPPALBANYPW},
  
};


// for ardunio boards with visable leds
int RXLED = 17;
static uint8_t __clock_prescaler = (CLKPR & (_BV(CLKPS0) | _BV(CLKPS1) | _BV(CLKPS2) | _BV(CLKPS3)));

void setup()
{
  setPrescaler(); // Set prescaler to highest clock speed
  Keyboard.begin(); // Start they keyboard emulator   
  pinMode(RXLED, OUTPUT); // Configure the on-board LED
  digitalWrite(RXLED, LOW);
  TXLED1;
  wait(startDelay);
  maxVolume();
  wifiConfig();
  if (enroll) {
    enrollment();
  }
  enterCreds();
  openTests();
  wait(300);
}

void loop() { // Main Function - workflow is called within loop();
  
}

// allows you to repeat consecutive key strokes of the same key
void repeatKey(byte key, int num) {
  for (int i = 0; i < num; i++) {
    Keyboard.write(key);
    wait(1);
  }
}

// makes the onboard tx led blink on and off for boards with visable lights
void blink() {
  digitalWrite(RXLED, LOW);
  //  TXLED1;
  delay(250);
  digitalWrite(RXLED, HIGH);
  //  TXLED0;
  delay(250);
}

// on board leds blink once per cycle of wait time
void wait(int cycles) {
  for (int i = 0; i < cycles; i++) {
    blink();
  }
}

// sets the clock speed of the board
void setPrescaler() {
  // Disable interrupts.
  uint8_t oldSREG = SREG;
  cli();

  // Enable change.
  CLKPR = _BV(CLKPCE); // write the CLKPCE bit to one and all the other to zero

  // Change clock division.
  CLKPR = 0x0; // write the CLKPS0..3 bits while writing the CLKPE bit to zero

  // Copy for fast access.
  __clock_prescaler = 0x0;

  // Recopy interrupt register.
  SREG = oldSREG;
}

//turns the volume up
void maxVolume() {
  Keyboard.press(KEY_VOLUMEUP);
  wait(5);
  Keyboard.release(KEY_VOLUMEUP);
  wait(1);
}

void wifiConfig() {
  // Access the Network option from the system tray (Status Area).
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.write('s');
  Keyboard.release(KEY_LEFT_ALT);
  Keyboard.release(KEY_LEFT_SHIFT);
  wait(2);
  //to select the Network
  repeatKey(KEY_TAB, 3);
  wait(2);
  repeatKey(KEY_ENTER, 2);
  wait(2);
  //to select the 'add Wifi' icon
  repeatKey(KEY_TAB, 3);
  // Enter
  Keyboard.write(KEY_ENTER);
  wait(4);
  // SSID
  Keyboard.print(wifi_name);
  wait(2);
  // TAB
  Keyboard.write(KEY_TAB);
  wait(2);
  // Arrow Down
  repeatKey(KEY_DOWN_ARROW, 2); //[1]WEP, [2]PSK (WPA or RSN), [3]EAP;
  wait(2);
  Keyboard.write(KEY_TAB);
  wait(2);
  // type wifi password
  Keyboard.print(wifi_pass);
  wait(2);
  // Enter
  Keyboard.write(KEY_ENTER); // Connect
  // Delay
  wait(wifiDelay);
}


void enterCreds() {

  // find username/password from USERCREDENTIALS struct
  #define username (credentials[school].id) // Sets username by school #
  #define password (credentials[school].pwd) // Sets password by school #
 
  if (differentAccount) {
    repeatKey(KEY_TAB, 5);
    wait(2);
    Keyboard.write(KEY_ENTER);
    wait(6);
  }

  // For chromebook versions requiring you to choose add person before entering username 
  if (addPerson) {
    Keyboard.press(KEY_LEFT_SHIFT);
    wait(2);
    repeatKey(KEY_TAB, 2);
    wait(2);
    Keyboard.release(KEY_LEFT_SHIFT);
    wait(1);
    Keyboard.write(KEY_ENTER);
    wait(8);
  }
  // enter username
  Keyboard.print(username);     
  wait(2);
  // Enter
  Keyboard.write(KEY_ENTER);
  // Delay
  wait(userDelay);
  // enter password
  Keyboard.print(password);
  wait(2);
  // Enter
  Keyboard.press(KEY_ENTER);
  // Delay
  wait(passwordDelay);
  }


void openTests() {

  // If after initial sign in chrome logs you out right away change this at the top to true
  if (reenterWifi) {
    // clear any profile warning issues
    Keyboard.press(KEY_ESC);
    wait(2);
    // close all open tabs
    Keyboard.press(KEY_LEFT_CTRL);
    repeatKey('w', 10);
    wait(2);
    Keyboard.release(KEY_LEFT_CTRL);
    wait(2);
    // Access the Network option from the system tray (Status Area).
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.write('s');
    wait(2);
    Keyboard.release(KEY_LEFT_ALT);
    Keyboard.release(KEY_LEFT_SHIFT);
    wait(2);
    //to select the Network
    repeatKey(KEY_TAB, 6);
    wait(2);
    // program has not been registering the repeated enter seperated them into two seperate entities to correct bug
    Keyboard.press(KEY_ENTER);
    wait(2);
    Keyboard.press(KEY_ENTER);
    //to select the 'add Wifi' icon
    repeatKey(KEY_TAB, 3);
    Keyboard.write(KEY_ENTER);
    wait(4);
    // SSID
    Keyboard.print(wifi_name);
    wait(2);
    // TAB
    Keyboard.write(KEY_TAB);
    wait(2);
    repeatKey(KEY_DOWN_ARROW, 2); //[1]WEP, [2]PSK (WPA or RSN), [3]EAP;
    wait(2);
    Keyboard.write(KEY_TAB);
    wait(2);
    // type wifi password
    Keyboard.print(wifi_pass);
    wait(2);
    // Enter
    Keyboard.write(KEY_ENTER); // Connect
    wait(2);
  }

  // clear any profile warning issues
  Keyboard.press(KEY_ESC);
  wait(2);
  // close any open tabs
  Keyboard.press(KEY_LEFT_CTRL);
  repeatKey('w', 10);
  wait(4);
  // open a tab in a new browser
  Keyboard.write('t');
  wait(10);
  // highlight the URL address bar
  Keyboard.write('l');
  wait(2);
  Keyboard.release(KEY_LEFT_CTRL);
  wait(2);
  
  // open mictests.com
  Keyboard.print(micTest);
  wait(3);
  // for some reason one enter doesn't work and you need two here
  Keyboard.write(KEY_ENTER);
  wait(2);
  Keyboard.write(KEY_ENTER);
  wait(2);
  // open a new tab
  Keyboard.press(KEY_LEFT_CTRL);
  wait(2);
  Keyboard.write('t');
  wait(2);
  // highlight the URL address bar
  Keyboard.write('l');
  wait(2);
  Keyboard.release(KEY_LEFT_CTRL);
  wait(2);
  
  // open webcamtests.com
  Keyboard.print(webcamTest);
  wait(2);
  Keyboard.write(KEY_ENTER);
  wait(6);
  Keyboard.press(KEY_LEFT_CTRL);
  wait(2);
  // open a new tab
  Keyboard.write('t');
  wait(2);
  // highlight the URL address bar
  Keyboard.write('l');
  wait(2);
  Keyboard.release(KEY_LEFT_CTRL);
  wait(2);
  
  // open keyboardchecker.com
  Keyboard.print(keyboardTest);
  wait(2);
  Keyboard.write(KEY_ENTER);
  wait(2);
  
  // If the school is one that gives us permissions open audio tester and battery tester right now it is set to uncommon
  
  if (school != 0) {
  // open a new tab
  Keyboard.press(KEY_LEFT_CTRL);
  wait(2);
  Keyboard.write('t');
  wait(2);
  // highlight the URL address bar
  Keyboard.write('l');
  wait(2);
  Keyboard.release(KEY_LEFT_CTRL);
  wait(2);
  // open audiotester
  Keyboard.print(audioTest);
  wait(2);
  Keyboard.write(KEY_ENTER);
  wait(2);
  
  // Open battery tester
  Keyboard.press(KEY_LEFT_CTRL);
  wait(2);
  Keyboard.press(KEY_LEFT_ALT);
  wait(2);
  Keyboard.write('t');
  wait(2);
  Keyboard.release(KEY_LEFT_CTRL);
  wait(2);
  // Start the battery tester
  Keyboard.print(batteryTest);
  wait(2);
  Keyboard.write(KEY_ENTER);
  
  }
  
}

// needs more testing only sort of works
void enrollment() {
  Keyboard.press(KEY_LEFT_CTRL);
  wait(2);
  Keyboard.press(KEY_LEFT_ALT);
  wait(2);
  Keyboard.write('e');
  wait(2);
  Keyboard.release(KEY_LEFT_CTRL);
  wait(2);
  Keyboard.release(KEY_LEFT_ALT);
  wait(2);
  Keyboard.write(KEY_ENTER);
  wait(5);
  Keyboard.write(KEY_ENTER);
  wait(5);
  repeatKey(KEY_TAB, 2);
  wait(2);
  Keyboard.write(KEY_ENTER);
  wait(2);
  repeatKey(KEY_TAB, 7);
  wait(2);
  Keyboard.write(KEY_ENTER);
  wait(35);
  Keyboard.write(KEY_TAB);
  wait(2);
  Keyboard.write(KEY_ENTER);
  wait(5);

}
