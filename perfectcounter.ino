#include <Preferences.h>
#include <WiFi.h>
#include <WebServer.h>

// WiFi Configuration
const char* ssid = "ClickCounter";
const char* password = "12345678";
const unsigned long HOLD_TIME = 5000;  // 5 seconds for WiFi toggle
const unsigned long WIFI_TIMEOUT = 300000; // 5 minutes auto-shutdown

// Pin Definitions
const int buttonPin1 = 0;   // First button (BOOT button)
const int buttonPin2 = 15;  // Second button
const int ledPin = 2;       // Built-in LED
const int debounceTime = 20;  // Milliseconds for debounce
const int fadeSpeed = 15;     // Lower = slower fade

// Global Objects
WebServer server(80);
Preferences preferences;

// State Variables
int counter = 0;
bool lastButtonState1 = HIGH;
bool lastButtonState2 = HIGH;
bool buttonPressed1 = false;
bool buttonPressed2 = false;
bool wifiEnabled = false;

// Timing Variables
unsigned long lastDebounceTime1 = 0;
unsigned long lastDebounceTime2 = 0;
unsigned long buttonHoldStart1 = 0;
unsigned long buttonHoldStart2 = 0;
unsigned long lastActivityTime = 0;
int currentLedValue = 0;

void blinkWiFiStatus(bool enabled) {
    int currentBrightness = currentLedValue;
    analogWrite(ledPin, 0);
    delay(200);
    
    if (enabled) {
        // Two fast blinks for ON at full brightness
        for(int i = 0; i < 2; i++) {
            analogWrite(ledPin, 255);
            delay(100);
            analogWrite(ledPin, 0);
            delay(100);
        }
    } else {
        // Three fast blinks for OFF at full brightness
        for(int i = 0; i < 3; i++) {
            analogWrite(ledPin, 255);
            delay(100);
            analogWrite(ledPin, 0);
            delay(100);
        }
    }
    analogWrite(ledPin, currentBrightness);
}

void toggleWiFi() {
    wifiEnabled = !wifiEnabled;
    if (wifiEnabled) {
        WiFi.softAP(ssid, password);
        server.begin();
        Serial.println("WiFi AP enabled - SSID: " + String(ssid));
        Serial.println("IP: " + WiFi.softAPIP().toString());
        lastActivityTime = millis();
        blinkWiFiStatus(true);
    } else {
        server.close();
        WiFi.softAPdisconnect(true);
        WiFi.mode(WIFI_OFF);
        Serial.println("WiFi AP disabled");
        blinkWiFiStatus(false);
    }
}

void handleRoot() {
    String html = "<DOCTYPE html><html><head>"
        "<meta name='viewport' content='width=device-width, initial-scale=1'>"
        "<style>"
        "body { background: #000; color: #fff; font-family: Arial; text-align: center; margin: 0; "
        "display: flex; justify-content: center; align-items: center; min-height: 100vh; }"
        "#counter { font-size: 25vw; font-weight: bold; opacity: 1; "
        "transition: opacity 0.2s ease-in-out; }"
        ".fade { opacity: 0.4; }"
        "</style>"
        "<script>"
        "let lastValue = '';"
        "setInterval(() => {"
        "fetch('/count')"
        ".then(r => r.text())"
        ".then(v => {"
        "if (v !== lastValue) {"
        "counter.classList.add('fade');"
        "setTimeout(() => {"
        "counter.textContent = Number(v).toLocaleString();"
        "counter.classList.remove('fade');"
        "}, 200);"
        "lastValue = v;"
        "}"
        "});"
        "}, 67);"  // ~15Hz refresh rate
        "</script>"
        "</head>"
        "<body>"
        "<div id='counter'>" + String(counter) + "</div>"
        "</body></html>";
    server.send(200, "text/html", html);
}

void handleCount() {
    server.send(200, "text/plain", String(counter));
}

void updateLED() {
    bool buttonPressed = (digitalRead(buttonPin1) == LOW || digitalRead(buttonPin2) == LOW);
    int targetValue;
    
    if (wifiEnabled) {
        targetValue = buttonPressed ? 127 : 3; // 50% when pressed, 2/255 minimum for WiFi indicator
    } else {
        targetValue = buttonPressed ? 127 : 0;  // 50% when pressed, fully off when idle
    }
    
    if (currentLedValue < targetValue) {
        currentLedValue = min(currentLedValue + fadeSpeed, targetValue);
    } else if (currentLedValue > targetValue) {
        currentLedValue = max(currentLedValue - fadeSpeed, targetValue);
    }
    
    analogWrite(ledPin, currentLedValue);
}

void setup() {
    Serial.begin(115200);
    
    pinMode(buttonPin1, INPUT_PULLUP);
    pinMode(buttonPin2, INPUT_PULLUP);
    pinMode(ledPin, OUTPUT);
    
    preferences.begin("counter", false);
    counter = preferences.getInt("count", 0);
    
    WiFi.mode(WIFI_OFF);
    
    server.on("/", handleRoot);
    server.on("/count", handleCount);
    
    Serial.println("Counter started at: " + String(counter));
}

void loop() {
    unsigned long currentTime = millis();
    bool currentButtonState1 = digitalRead(buttonPin1);
    bool currentButtonState2 = digitalRead(buttonPin2);
    
    // Handle Button 1
    if (currentButtonState1 != lastButtonState1) {
        lastDebounceTime1 = currentTime;
    }
    
    if ((currentTime - lastDebounceTime1) > debounceTime) {
        if (currentButtonState1 == LOW) {
            if (buttonHoldStart1 == 0) buttonHoldStart1 = currentTime;
            if ((currentTime - buttonHoldStart1) >= HOLD_TIME) {
                toggleWiFi();
                buttonHoldStart1 = 0;
                buttonPressed1 = true;
            }
        } else {
            buttonHoldStart1 = 0;
        }
        
        if (!buttonPressed1 && currentButtonState1 == LOW) {
            counter++;
            preferences.putInt("count", counter);
            Serial.print("Counter: ");
            Serial.println(counter);
            buttonPressed1 = true;
        }
        if (currentButtonState1 == HIGH) {
            buttonPressed1 = false;
        }
    }
    
    // Handle Button 2
    if (currentButtonState2 != lastButtonState2) {
        lastDebounceTime2 = currentTime;
    }
    
    if ((currentTime - lastDebounceTime2) > debounceTime) {
        if (currentButtonState2 == LOW) {
            if (buttonHoldStart2 == 0) buttonHoldStart2 = currentTime;
            if ((currentTime - buttonHoldStart2) >= HOLD_TIME) {
                toggleWiFi();
                buttonHoldStart2 = 0;
                buttonPressed2 = true;
            }
        } else {
            buttonHoldStart2 = 0;
        }
        
        if (!buttonPressed2 && currentButtonState2 == LOW) {
            counter++;
            preferences.putInt("count", counter);
            Serial.print("Counter: ");
            Serial.println(counter);
            buttonPressed2 = true;
        }
        if (currentButtonState2 == HIGH) {
            buttonPressed2 = false;
        }
    }
    
    // Update LED and handle web clients
    updateLED();
    if (wifiEnabled) {
        server.handleClient();
        
        if (buttonPressed1 || buttonPressed2) {
            lastActivityTime = currentTime;
        }
        if ((currentTime - lastActivityTime) > WIFI_TIMEOUT) {
            Serial.println("WiFi timeout - disabling");
            toggleWiFi();
        }
    }
    
    lastButtonState1 = currentButtonState1;
    lastButtonState2 = currentButtonState2;
}
