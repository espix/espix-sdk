#include <Arduino.h>

#include <EEPROM.h>
#include <ESP8266HTTPClient.h>

#include <EasyBuzzer.h>
#include <SH1106Wire.h>

#include <espix-core.h>
#include <espix-design.h>

#include "examples/config.h"
#include "examples/settings/Settings.h"

#include "examples/alarm/Alarm.h"
#include "examples/root/views/RootView.h"
#include "examples/services/ServiceClient.h"

bool connecting = false;
bool isActive = true;
unsigned long lastActiveTime = 0;

SH1106Wire display(0x3c, OLED_SDA_PIN, OLED_CLK_PIN);

StatusBar statusBar;
RootView rootView;
ProgressView connectionView("Connecting to WiFi...", ProgressMode::INDETERMINATE);

void onConnected() {
  connecting = false;
  Application.enableOTA();
  Application.setStatusBar(&statusBar);
  Application.setRootView(&rootView, TRANSITION_OPTIONS_DOWN);
  ServiceClient.begin();
}

void connect() {
  Application.setRootView(&connectionView);
  WiFiNetwork.addToPreferredList(WIFI_DEFAULT_SSID, WIFI_DEFAULT_PWD);
  WiFiNetwork.addToPreferredList(WIFI_2_SSID, WIFI_2_PWD);
  WiFiNetwork.connect(onConnected);
  connecting = true;
}

void setupDevices() {
  pinMode(LED_PIN, OUTPUT);

  Screen.begin(&display);
  Screen.setOrientation(true);
  Screen.setBrightness(100);

  Keyboard.registerJogDial(KY04_CLK_PIN, KY04_DT_PIN, KY04_SW_PIN);
  Keyboard.registerKey(KEY_ESC, ESC_BUTTON_PIN);
  Keyboard.begin();

  EasyBuzzer.setPin(BUZZER_PIN);
}

void activateScreen() {
  isActive = true;
  lastActiveTime = millis();
  Screen.setBrightness(100);
}

void dimScreen() {
  isActive = false;
  auto brightnessSettings = Settings.getBrightnessSettings();
  uint8_t dimmerBrightness = Settings.getBrightnessSettings()->dayTimeBrightness;
  if (brightnessSettings->isNightMode) {
    dimmerBrightness = brightnessSettings->nightTimeBrightness;
    Serial.print("NIGHT MODE: ");
    Serial.println(dimmerBrightness);
  } else {
    auto hours = TimeClient.now().getHours();
    if (hours <= 6) {
      dimmerBrightness = brightnessSettings->nightTimeBrightness;
    } else if (hours == 7 || hours >= 23) {
      dimmerBrightness = round(brightnessSettings->dayTimeBrightness * 0.62);
    }
  }
  Screen.setBrightness(dimmerBrightness);
}

void checkScreenBrightness() {
  if (Alarm.isBeeping()) {
    isActive = true;
    Screen.setBrightness(100);
  } else {
    if (isActive) {
      if (millis() > lastActiveTime + 5 * 1000) {
        dimScreen();
      }
    }
  }
}

void handleKeyPress(KeyEventArgs *e) {
  activateScreen();

  if (e->keyCode == KEY_ESC) {
    if (Alarm.isBeeping()) {
      e->preventDefault();
      Alarm.stopBeep();
    }
  }
}

void handleScroll(ScrollEventArgs *e) {
  activateScreen();
}

void setupApp() {
  Alarm.begin();
  Settings.begin();
  Application.begin();
  Application.onKeyPress(handleKeyPress);
  Application.onScroll(handleScroll);
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  setupDevices();
  setupApp();
#ifndef DEBUG_LOCAL
  connect();
#else
  Application.setStatusBar(&statusBar);
  Application.setRootView(&rootView);
#endif
}

void loop() {
  Alarm.update();
  EasyBuzzer.update();
  ServiceClient.update();
  checkScreenBrightness();

  int timeBudget = Application.update();
  if (timeBudget > 0) {
    delay(timeBudget);
  }
}
