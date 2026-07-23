#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#define LEFT_SENSOR_PIN 18
#define RIGHT_SENSOR_PIN 19

#define IN1 32
#define IN2 33
#define IN3 25
#define IN4 26

// Wi-Fi AP credentials
const char* AP_SSID = "RobotAP";
const char* AP_PASS = "12345678";

WebServer server(80);

// Remote control state
bool remoteMode = false;
unsigned long lastCmdMillis = 0;
const unsigned long REMOTE_TIMEOUT = 700; // ms

// Motor control functions (your existing implementations)
void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopMoving() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// Helper to mark a remote command and perform it
void doRemoteCommand(const String &cmd) {
  lastCmdMillis = millis();
  if (cmd == "F") {
    moveForward();
    remoteMode = true;
  } else if (cmd == "L") {
    turnLeft();
    remoteMode = true;
  } else if (cmd == "R") {
    turnRight();
    remoteMode = true;
  } else if (cmd == "S") {
    stopMoving();
    remoteMode = false;
  }
}

// HTTP handlers
void handleRoot() {
  const char* page = R"rawliteral(
<!doctype html><meta name="viewport" content="width=device-width,initial-scale=1">
<style>
  body{font-family:Arial,Helvetica,sans-serif;margin:0;background:#111;color:#fff}
  .wrap{display:flex;flex-direction:column;align-items:center;padding:12px}
  h3{margin:8px 0 14px}
  .controller{position:relative;width:320px;height:320px;max-width:92vw;max-height:92vw}
  .pad{position:absolute;left:50%;top:50%;transform:translate(-50%,-50%);width:220px;height:220px;border-radius:50%;
       background:radial-gradient(circle at 30% 30%, #222, #000);display:flex;align-items:center;justify-content:center;
       box-shadow:0 6px 20px rgba(0,0,0,0.6);border:4px solid #2a2a2a}
  .pad svg{width:70%;height:70%;fill:none;stroke:#fff;stroke-width:8;opacity:0.9}
  .btn{position:absolute;width:88px;height:88px;border-radius:12px;background:#1b1b1b;border:2px solid #2f2f2f;color:#fff;font-size:20px;
       display:flex;align-items:center;justify-content:center;box-shadow:0 4px 12px rgba(0,0,0,0.6);
       -webkit-user-select:none;-webkit-touch-callout:none;user-select:none;touch-action:none}
  .btn:active{transform:translateY(2px);box-shadow:0 2px 6px rgba(0,0,0,0.6)}
  .up{left:calc(50% - 44px);top:10px}
  .down{left:calc(50% - 44px);bottom:10px}
  .left{left:10px;top:calc(50% - 44px)}
  .right{right:10px;top:calc(50% - 44px)}
  .center{position:absolute;left:50%;top:50%;transform:translate(-50%,-50%);width:88px;height:88px;border-radius:50%;
          background:#2b2b2b;border:2px solid #3a3a3a;display:flex;align-items:center;justify-content:center;font-weight:700}
  small.hint{display:block;margin-top:10px;color:#bbb;font-size:13px}
</style>

<div class="wrap">
  <h3>Aarav RC Controller</h3>
  <div class="controller">
    <div class="pad" id="pad">
      <svg viewBox="0 0 100 100" xmlns="http://www.w3.org/2000/svg" aria-hidden="true">
        <line x1="20" y1="20" x2="80" y2="80" stroke-linecap="round"/>
        <line x1="80" y1="20" x2="20" y2="80" stroke-linecap="round"/>
      </svg>
    </div>

    <div class="btn up" id="btn-up">▲</div>
    <div class="btn down" id="btn-down">▼</div>
    <div class="btn left" id="btn-left">◀</div>
    <div class="btn right" id="btn-right">▶</div>

    <div class="center" id="btn-stop">X</div>
  </div>
  <small class="hint">Hold an arrow to move; release to stop. Connect to RobotAP → http://192.168.4.1</small>
</div>

<script>
  document.addEventListener('contextmenu', e => e.preventDefault()); // block long-press menu

  function sendCmd(c){
    fetch('/cmd?c='+c).catch(()=>{});
  }

  // on hold: send immediately and keep sending at interval until released
  function addHold(el, cmd){
    let repeater = null;
    function start(e){
      e.preventDefault();
      sendCmd(cmd);
      if (repeater) clearInterval(repeater);
      repeater = setInterval(()=>sendCmd(cmd), 200); // adjust interval as needed
    }
    function stop(e){
      if (e) e.preventDefault();
      if (repeater) { clearInterval(repeater); repeater = null; }
      sendCmd('S');
    }
    el.addEventListener('pointerdown', start, {passive:false});
    el.addEventListener('pointerup', stop, {passive:false});
    el.addEventListener('pointerleave', stop, {passive:false});
    el.addEventListener('pointercancel', stop, {passive:false});
    el.addEventListener('touchcancel', stop, {passive:false});
  }

  addHold(document.getElementById('btn-up'), 'F');    // Forward
  addHold(document.getElementById('btn-left'), 'L');  // Left
  addHold(document.getElementById('btn-right'), 'R'); // Right
  addHold(document.getElementById('btn-down'), 'S');  // currently Stop (map to reverse if you add it)

  document.getElementById('btn-stop').addEventListener('click', () => sendCmd('S'));

  // keyboard support
  window.addEventListener('keydown', e => { if(e.repeat) return;
    if(e.key === 'ArrowUp') sendCmd('F');
    if(e.key === 'ArrowLeft') sendCmd('L');
    if(e.key === 'ArrowRight') sendCmd('R');
    if(e.key === 'ArrowDown') sendCmd('S');
  });
  window.addEventListener('keyup', e => { if(['ArrowUp','ArrowLeft','ArrowRight','ArrowDown'].includes(e.key)) sendCmd('S'); });
</script>
)rawliteral";
  server.send(200, "text/html", page);
}

void handleCmd() {
  if (!server.hasArg("c")) { server.send(400, "text/plain", "missing"); return; }
  String c = server.arg("c");
  doRemoteCommand(c);
  server.send(200, "text/plain", "OK");
}

void setup() {
  Serial.begin(115200);

  pinMode(LEFT_SENSOR_PIN, INPUT);
  pinMode(RIGHT_SENSOR_PIN, INPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopMoving();

  // Start Wi-Fi AP
  WiFi.softAP(AP_SSID, AP_PASS);
  IPAddress ip = WiFi.softAPIP();
  Serial.print("AP started, connect to ");
  Serial.print(AP_SSID);
  Serial.print(" -> http://");
  Serial.println(ip);

  // Routes
  server.on("/", handleRoot);
  server.on("/cmd", handleCmd);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  // If remote command timed out, ensure motors are stopped
  if (remoteMode && (millis() - lastCmdMillis > REMOTE_TIMEOUT)) {
    remoteMode = false;
    stopMoving();
    Serial.println("Remote timeout - stopped");
  }

  // Only act when remoteMode is true (i.e., a movement command was received).
  // Otherwise remain idle (motors stopped).
  // Remote button releases should send 'S' (Stop) which sets remoteMode=false.
  // No autonomous line-following code runs while in this mode.
  delay(10);
}