#include <SoftwareSerial.h>
#include <LCD5110_Basic.h>


// Pin definitions for HC-05 or HC-06 Bluetooth module
SoftwareSerial BT(2, 3); 

// Nokia 5110 LCD pins: (SCK, DIN, DC, CS, RST)
LCD5110 myGLCD(A1, A2, A3, A4, A5);

extern uint8_t SmallFont[];

// Create a string array for each line
// Six lines in total
String lines[6];
int lineIndex = 0; // Tracks the current line

void setup() {
  // Initialize the LCD display
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);

  // Initialize serial communication for debugging
  Serial.begin(9600);
  
  // Initialize Bluetooth serial communication
  BT.begin(9600);

  Serial.println("Arduino and Bluetooth connection ready.");
  
  // The display is empty at startup
  myGLCD.clrScr();
  myGLCD.print("Waiting text...", CENTER, 10); // Display the message near the center

}

// Helper function to draw all lines on the LCD display
void drawLines() {
  myGLCD.clrScr(); // Clear the display first
  for (int i = 0; i < 6; i++) {
    // Convert String to char* and display it on the LCD
    myGLCD.print(lines[i].c_str(), CENTER, i * 8); // Leave 8 pixels between each line
  }
}

void loop() {
  // Check if new data is available from Bluetooth
  if (BT.available()) {
    // Read the incoming Bluetooth data as a line
    String incomingText = BT.readStringUntil('\n'); 
    
    // Remove the line ending characters and unnecessary whitespace
    incomingText.trim(); 

    // Display the received text in the Serial Monitor
    Serial.print("Received text: ");
    Serial.println(incomingText);

    if (lineIndex < 6) {
      // If the display is not full, add the text to a new line
      lines[lineIndex] = incomingText;
      lineIndex++;
    } else {
      // If the display is full, shift all lines upward
      for (int i = 0; i < 5; i++) {
        lines[i] = lines[i+1];
      }
      // Place the new text on the last line
      lines[5] = incomingText;
    }
    
    // Draw the updated lines on the display
    drawLines();
  }
}