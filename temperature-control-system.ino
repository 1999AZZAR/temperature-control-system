// Thermistor connections
const int thermistorPin1 = A0;
const int thermistorPin2 = A1;

// PWM output pin
const int pwmPin = 9;

// Constants for temperature calculation
const int thermistorNominal = 10000;  // Nominal resistance at 25°C
const int temperatureNominal = 25;    // Nominal temperature for the thermistor
const int betaValue = 3950;           // Beta value of the thermistor

// Desired temperature in Celsius
const int desiredTemperature = 28;

void setup() {
  // Initialize PWM pin
  pinMode(pwmPin, OUTPUT);

  // Initialize Serial Monitor
  Serial.begin(9600);
}

void loop() {
  // Read temperature from both thermistors
  float temperature1 = readTemperature(thermistorPin1);
  float temperature2 = readTemperature(thermistorPin2);

  // Calculate average temperature
  float averageTemperature = (temperature1 + temperature2) / 2.0;

  // Calculate PWM ratio
  float temperatureRatio = max(0.0, (averageTemperature - temperatureNominal) / (desiredTemperature - temperatureNominal));
  int pwmValue = int(255 * temperatureRatio);
  pwmValue = constrain(pwmValue, 0, 255); // Ensure PWM value is within range

  // Set PWM output
  analogWrite(pwmPin, pwmValue);

  // Print temperature and PWM values to Serial Monitor
  Serial.print("Temperature 1: ");
  Serial.print(temperature1);
  Serial.print(" °C   Temperature 2: ");
  Serial.print(temperature2);
  Serial.print(" °C   Average Temperature: ");
  Serial.print(averageTemperature);
  Serial.print(" °C   PWM Value: ");
  Serial.println(pwmValue);

  delay(100);  // Delay for one second before taking the next reading
}

float readTemperature(int pin) {
  // Read the analog value from the thermistor pin
  int sensorValue = analogRead(pin);

  // Convert the analog value to resistance
  float resistance = 1023.0 / sensorValue - 1.0;
  resistance = thermistorNominal / resistance;

  // Calculate temperature in Celsius using the Steinhart-Hart equation
  float temperature = resistance / thermistorNominal;
  temperature = log(temperature);
  temperature /= betaValue;
  temperature += 1.0 / (temperatureNominal + 273.15);
  temperature = 1.0 / temperature;
  temperature -= 273.15;

  return temperature;
}
