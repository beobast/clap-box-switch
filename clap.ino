int sensorPin = A0;
int relayPin = 12;
bool relayActivated = false;
byte soundThreshold = 6;
int soundValue = 0;
float soundAverage = 0;
unsigned long currentTime = 0;
unsigned long lastClapTime = 0;
unsigned long lastRelayToggleTime = 0;
unsigned long timeSinceLastClap = 0;
byte clapCounter = 0;
int resetClapCounterDelay = 1000;
int toggleRelayDelay = 5000;

void setup() {
    pinMode(relayPin, OUTPUT);
}

void loop() {
    currentTime = millis();
    timeSinceLastClap = currentTime - lastClapTime;
    soundValue = analogRead(sensorPin);
    soundAverage = 0.95 * soundAverage + 0.05 * soundValue;

    // Reset clap counter
    if (clapCounter != 0 && timeSinceLastClap > resetClapCounterDelay) {
        clapCounter = 0;
    }

    // Detect a clap
    if (abs(soundValue - soundAverage) > soundThreshold) {
        if (timeSinceLastClap > 250) {
            clapCounter++;
            lastClapTime = currentTime;
        }
    }

    // Toggle relay when 2 claps detected
    if (clapCounter == 2 && (currentTime - lastRelayToggleTime) > toggleRelayDelay) {
        clapCounter = 0;
        relayActivated = !relayActivated;
        digitalWrite(relayPin, relayActivated ? HIGH : LOW);
        lastRelayToggleTime = currentTime;
    }
}
