#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Wi-Fi credentials
const char* ssid = "Xiomi";
const char* password = "ddddddddd";

// GPIO Pins for motor control
const int motor1Pin1 = D1; // IN1 on L298D
const int motor1Pin2 = D2; // IN2 on L298D
const int motor2Pin1 = D3; // IN3 on L298D
const int motor2Pin2 = D4; // IN4 on L298D

// Create an instance of the web server on port 80
ESP8266WebServer server(80);

// HTML content to host
const char webpage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>Button Controller</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            text-align: center;
            margin-top: 50px;
        }
        .button-container {
            margin-top: 20px;
        }
        button {
            padding: 20px 50px;
            margin: 5px;
            font-size: 36px;
            cursor: pointer;
        }
    </style>
</head>
<body>
    <h1>Button Controller</h1>
    <div class="button-container">
        <button onclick="sendAction('up')">Up</button><br>
        <button onclick="sendAction('left')">Left</button>
        <button onclick="sendAction('right')">Right</button><br>
        <button onclick="sendAction('down')">Down</button>
    </div>
    <script>
        function sendAction(action) {
            console.log(`Sending action: ${action}`); // Debug log
            fetch(`/${action}`)
                .then(response => response.text())
                .then(data => console.log('Response:', data))  // Log response from server
                .catch(error => console.log('Error:', error));  // Log errors
        }
    </script>
</body>
</html>
)rawliteral";

// Function to move the car forward
void moveForward() {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
}

// Function to move the car backward
void moveBackward() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
}

// Function to turn the car left
void turnLeft() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
}

// Function to turn the car right
void turnRight() {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
}

// Function to stop the car
void stopCar() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
}

// Handlers for each button action
void handleUp() {
  Serial.println("Button pressed: Up");
  moveForward();
  delay(1000); // Keep the motor running for 0.5 seconds
  stopCar();
  server.send(200, "text/plain", "Moving Forward");
}

void handleDown() {
  Serial.println("Button pressed: Down");
  moveBackward();
  delay(1000); // Keep the motor running for 0.5 seconds
  stopCar();
  server.send(200, "text/plain", "Moving Backward");
}

void handleLeft() {
  Serial.println("Button pressed: Left");
  turnLeft();
  delay(200); // Keep the motor running for 0.5 seconds
  stopCar();
  server.send(200, "text/plain", "Turning Left");
}

void handleRight() {
  Serial.println("Button pressed: Right");
  turnRight();
  delay(200); // Keep the motor running for 0.5 seconds
  stopCar();
  server.send(200, "text/plain", "Turning Right");
}

void handleRoot() {
  server.send(200, "text/html", webpage); // Serve the HTML content
}

void setup() {
  Serial.begin(115200);

  // Set up motor control pins as outputs
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);

  // Ensure motors are stopped initially
  stopCar();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to Wi-Fi");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to Wi-Fi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Set up routes
  server.on("/", handleRoot);
  server.on("/up", handleUp);
  server.on("/down", handleDown);
  server.on("/left", handleLeft);
  server.on("/right", handleRight);

  server.begin(); // Start the web server
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient(); // Handle client requests
}
