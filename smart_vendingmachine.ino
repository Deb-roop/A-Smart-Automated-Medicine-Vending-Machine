v#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Keypad setup
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {7, 6, 8, 9};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Servo array and pins
Servo servos[6];
int servoPins[6] = {10, 11, 12, 46, 44, 45};

// Medicine names
const char* medicines[6] = {
  "Napa", "Orsaline", "Tufnil", "Deslor", "Kaldin", "Famomax"
};

// Buzzer pin
#define BUZZER_PIN 49

// Track whether standby message is shown
bool standbyDisplayed = false;
char lastKey = '\0'; // To track previous key press

// Buzzer function
void beep(int duration = 200) {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(duration);
  digitalWrite(BUZZER_PIN, LOW);
}

// External function to handle key '4'
void handleDoctorCall() {
  beep(500); // longer beep
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Doctor Call");

  // Move servo 4 (index 3)
  servos[3].write(280);
  delay(3000);
  servos[3].write(90);

  delay(500);
  lcd.clear();
}

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  // Attach all servos
  for (int i = 0; i < 6; i++) {
    servos[i].attach(servoPins[i]);
    servos[i].write(90); // Initial position
  }
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    standbyDisplayed = false; // Reset standby state on any key

    // Keys '1' to '3' and '5' to '6'
    if ((key >= '1' && key <= '3') || (key >= '5' && key <= '6')) {
      int index = key - '1'; // Convert char to index

      beep(100);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Medicine:");
      lcd.setCursor(0, 1);
      lcd.print(medicines[index]);

      servos[index].write(110);  // Move servo
      delay(3000);
      servos[index].write(90);   // Reset position

      delay(500);
      lcd.clear();
      lastKey = '\0';
    }

    // Handle key '4' separately
    else if (key == '4') {
      handleDoctorCall(); // Call external function
      lastKey = '\0';
    }

    // * then #
    else if (lastKey == '*' && key == '#') {
      beep(300);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Dream Light");
      delay(2000);
      lcd.clear();
      lastKey = '\0';
    }

    // * then 0
    else if (lastKey == '*' && key == '0') {
      beep(300);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Fif Tech");
      delay(2000);
      lcd.clear();
      lastKey = '\0';
    }

    // Store * as last key only
    else if (key == '*') {
      lastKey = '*';
    } else {
      lastKey = '\0'; // clear sequence
    }
  }

  // Show standby message if no key is pressed
  if (!key && !standbyDisplayed) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Faridpur Zilla");
    lcd.setCursor(0, 1);
    lcd.print("Proshashon School");
    standbyDisplayed = true;
  }
}