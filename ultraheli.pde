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
int maxDistance = 1000;
int sleepMillis = 200;

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
int frame = 0;
float period = 1;
float distanceOfFrame = maxDistance / imgcount;

void setup() {
  size(displayWidth, displayHeight); //Use entire screen size.
  smooth(); // draws all shapes with smooth edges.

  if (usePort) {
    serialPort = new Serial(this, "/dev/tty.usbmodem1411", 9600);
    serialPort.bufferUntil('\n'); // Trigger a SerialEvent on new line
  }
  
  if (!usePort) {
    distance = maxDistance; // fake initial distance for simulation
  }

  if (hud) {
    f = createFont("Arial",16,true); // Arial, 16 point, anti-aliasing on
  }
}

void draw() {
  // distance is read somewhere else. here we just make sure
  // it does not overstep the bounds.
  if (distance > maxDistance) {
    distance = maxDistance;
  } else if (distance < 0) {
    distance = 0;
  }

  // Dont do anything unless n millis have passed.  
  long now = millis();
  long timePassed = now - previousMillis;
  if (timePassed < sleepMillis) {
    return;
  }
  
  // Draw the current animation frame
  PImage img = getImage(frame);
  image(img, 0, 0, displayWidth, displayHeight);
  tint(255, 127);  // Display at half opacity
  image(img, 0, 0, displayWidth, displayHeight);

  // If distance has not changed, we're done
  if (lastDistance == distance) {
    return;
  }

  calculateFrame();

  previousMillis = now;

  updateHUD();
}

void calculateFrame() {
  int destination = frameForDistance();
  
  // are we already there?
  if (destination == frame) {
    return;
  }
  
  // move back to destination
  if (destination < frame) {
    frame = frame - 1;
    return;
  }
  
  // move towards destination
  frame = frame + 1;
}

// Frame for current distance
// Note that this is not the actual frame that will be animated.
// Instead will start to animate towards this frame.
int frameForDistance() {
  int result = imgcount - int(distance / distanceOfFrame);
  if (result < 0) {
    result = 0;
  }
  if (result >= imgcount - 1) {
    result = imgcount - 1;
  }
  return result;
}

void updateHUD() {
  String debugInfo = 
      " distance=" + nf(distance, 0)
    + " frame=" + nf(frame, 0) + "/" + nf(imgcount, 0);
  if (!hud) {
    println(debugInfo);
    return;
  }
  textFont(f,36);
  fill(0);
  text(debugInfo, 10, 100);
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
    // distance decreases as viewer approaches
    newDistance = distance - keyboardStep;
  } else if (DOWN == keyCode) {
    // distance incrases as viewer steps back
    newDistance = distance + keyboardStep;
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

