import processing.serial.*;
import ddf.minim.*;
import ddf.minim.signals.*;
import ddf.minim.analysis.*;
import ddf.minim.effects.*;
import ddf.minim.ugens.*;

// Configuration
int imgcount = 20;
boolean usePort = false;
boolean fullScreen = false;
int keyboardStep = 10;

// Serial port, for reading distance from ultrasonic sensor.
// Optional.
Serial serialPort;

// Images that make up the animation sequence
PImage images[] = new PImage[imgcount];

// HUD
PFont f;

// Animation draw state
int distance = 0;
long previousMillis = 0;
int frame = 16;
int max = 1;
float period = 1;

void setup() {
  size(displayWidth, displayHeight); //Use entire screen size.
  smooth(); // draws all shapes with smooth edges.

  for (int i = 0; i < imgcount; i++) {
    images[i] = loadImage(str(i) + ".jpg");
  }

  if (usePort) {
    serialPort = new Serial(this, "/dev/tty.usbmodem1411", 9600);
    serialPort.bufferUntil('\n'); // Trigger a SerialEvent on new line
  }
  
  if (!usePort) {
    distance = 400; // fake initial distance for simulation
  }
  
  f = createFont("Arial",16,true); // Arial, 16 point, anti-aliasing on
}

void draw() {
  if (distance > max) {
    max = distance;
  }
  
  long now = millis();
  long n = now - previousMillis;
  if (n > 200) {
    int newFrame = frame;
    if (distance > 0) {
      period = max / float(imgcount);
      newFrame = int(distance / period);
    }

    println(millis(), "period", period, "distance", distance, "frame", frame, "newframe", newFrame, "max", max);
   
    int change = 0; 
    // animation direction
    if (newFrame > frame) {
      // moving away
      change = 1;
    } else if (newFrame < frame) {
      // moving closer
      change = -1;
    }
    
    frame = frame + change;

    // stay within bounds    
    if (frame < 0) {
      frame = 0;
    }
    if (frame >= imgcount) {
      frame = imgcount-1;
    }
    
    PImage img = images[frame];
    image(img, 0, 0, displayWidth, displayHeight);
    
    tint(255, 127);  // Display at half opacity

    image(img, 0, 0, displayWidth, displayHeight);
    
    previousMillis = now;
  }
  
  // Update HUD
  textFont(f,36);
  fill(255);
  text("Distance" + nfs(distance, 2), 10, 100);
}

void keyPressed() {
  int newDistance = 0;
  if (UP == keyCode) {
    newDistance = distance + keyboardStep;
  } else if (DOWN == keyCode) {
    newDistance = distance - keyboardStep;
  }
  if (newDistance < 0) {
    newDistance = 0;
  }
  distance = newDistance;
}

void serialEvent(Serial cPort) {
  String s = cPort.readStringUntil('\n');
  if (s != null) {
    s = trim(s);
    println("distance", s);
    distance = int(Integer.parseInt(s));
    if (distance < 0) {
      distance = 0;
    }
  }
}

boolean sketchFullScreen() {
  return fullScreen;
}

