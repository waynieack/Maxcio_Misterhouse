/*
 *   Tested with "Maxcio KS-602S Wifi Smart Switch"
 *
 *   The Relay could be toggeled with the physical pushbutton
*/

#include <Homie.h>

const int PIN_RELAY = 12;
const int PIN_LED = 13;
const int PIN_BUTTON = 0;

unsigned long buttonDownTime = 0;
byte lastButtonState = 1;
byte buttonPressHandled = 0;

#define FW_NAME "MH_Maxcio_KS-602S_Switch"
#define FW_VERSION "1.0.3"


HomieNode switchNode("switch", "switch");

bool switchOnHandler(HomieRange range, String value) {
  if (value != "on" && value != "off") return false;

  bool on = (value == "on");
  digitalWrite(PIN_RELAY, on ? HIGH : LOW);
  switchNode.setProperty("on").send(value);
  Homie.getLogger() << "Switch is " << (on ? "on" : "off") << endl;

  return true;
}

void toggleRelay() {
  bool on = digitalRead(PIN_RELAY) == HIGH;
  digitalWrite(PIN_RELAY, on ? LOW : HIGH);
  switchNode.setProperty("on").send(on ? "off" : "on");
  Homie.getLogger() << "Switch is " << (on ? "off" : "on") << endl;
}


bool RestartNode(HomieRange range, String value) {
  Homie.getLogger() << "Message: " << value << endl;
  if (value != "true") return false;
  if (value == "true") ESP.reset();
  return true;
}


void buttonPressHandler() {
  byte buttonState = digitalRead(PIN_BUTTON);
  if ( buttonState != lastButtonState ) {
    if (buttonState == LOW) {
      buttonDownTime     = millis();
      buttonPressHandled = 0;
    }
    else {
      unsigned long dt = millis() - buttonDownTime;
      if ( dt >= 90 && dt <= 900 && buttonPressHandled == 0 ) {
        toggleRelay();
        buttonPressHandled = 1;
      }
    }
    lastButtonState = buttonState;
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  pinMode(PIN_RELAY, OUTPUT);
  pinMode(PIN_BUTTON, INPUT);
  digitalWrite(PIN_RELAY, LOW);

  Homie_setFirmware(FW_NAME, FW_VERSION);
  Homie.setLedPin(PIN_LED, LOW).setResetTrigger(PIN_BUTTON, LOW, 5000);

  switchNode.advertise("on").settable(switchOnHandler);
  switchNode.advertise("restart").settable(RestartNode);
  
  Homie.setup();
}

void loop() {
  Homie.loop();
  buttonPressHandler();
}
