import processing.serial.*;
import ddf.minim.*;
import ddf.minim.signals.*;
import ddf.minim.analysis.*;
import ddf.minim.effects.*;
import ddf.minim.ugens.*;

// Configuration
int imgcount = 326;
boolean usePort = false;
boolean fullScreen = false;
int keyboardStep = 10;
boolean hud = true;

// Serial port, for reading distance from ultrasonic sensor.
// Optional.
Serial serialPort;

// Images that make up the animation sequence
PImage images[] = new PImage[imgcount];
boolean loadedImages[] = new boolean[imgcount];

// HUD
PFont f;

// Animation draw state
int distance = 0;
int lastDistance = distance;
long previousMillis = 0;
int frame = 16;
int max = 1;
float period = 1;

void setup() {
  long now = millis();
  
  size(displayWidth, displayHeight); //Use entire screen size.
  smooth(); // draws all shapes with smooth edges.

  if (usePort) {
    serialPort = new Serial(this, "/dev/tty.usbmodem1411", 9600);
    serialPort.bufferUntil('\n'); // Trigger a SerialEvent on new line
  }
  
  if (!usePort) {
    distance = 400; // fake initial distance for simulation
  }
  
  f = createFont("Arial",16,true); // Arial, 16 point, anti-aliasing on
  
  int spent = int(millis() - now);
  
  println("setup took " + nf(spent, 0) + " seconds");
}

void draw() {
  if (distance > max) {
    max = distance;
  }
  
  long now = millis();

  long timePassed = now - previousMillis;
  int newFrame = frame;
  if (timePassed > 200) {
    if (distance > 0 && distance != lastDistance) {
      period = max / float(imgcount);
      newFrame = int(distance / period);
      lastDistance = distance;
    }

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
    
    PImage img = getImage(frame);
    image(img, 0, 0, displayWidth, displayHeight);
    
    tint(255, 127);  // Display at half opacity

    image(img, 0, 0, displayWidth, displayHeight);
    
    previousMillis = now;
  }
  
  // Update HUD
  String debugInfo = 
      " distance=" + nf(distance, 0)
    + " frame=" + nf(frame, 0) + "/" + nf(imgcount, 0)
    +  " max=" + nf(max, 0);  
  if (hud) {
    textFont(f,36);
    fill(0);
    text(debugInfo, 10, 100);
  } else {
    println(debugInfo);
  }
}

PImage getImage(int i) {
  if (!loadedImages[i]) {
    println("loading image " + nf(i, 0) + "/" + nf(imgcount, 0));
    images[i] = loadImage(str(i) + ".jpg");
    loadedImages[i] = true;
  }
  return images[i];
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
    distance = int(Integer.parseInt(s));
    if (distance < 0) {
      distance = 0;
    }
  }
}

boolean sketchFullScreen() {
  return fullScreen;
}

