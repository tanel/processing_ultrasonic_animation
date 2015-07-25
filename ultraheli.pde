import gab.opencv.*;

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
int maxDistance = 1000;
int sleepMillis = 100;

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

  f = createFont("Arial",16,true); // Arial, 16 point, anti-aliasing on
}

void draw() {
  // distance is read somewhere else. here we just make sure
  // it does not overstep the bounds.
  if (distance > maxDistance) {
    distance = maxDistance;
  } else if (distance < 0) {
    distance = 0;
  }
  
  drawFrame();
  calculateFrame();
}

void drawFrame() {
  // Draw the current animation frame
  PImage img = getImage(frame);
  image(img, 0, 0, displayWidth, displayHeight);
  tint(255, 127);  // Display at half opacity
  image(img, 0, 0, displayWidth, displayHeight);
}

void introduceRandomness() {
  return;
 /*
    int r = int(random(5));
    if (r == 0) {
      r = int(random(2));
      if (0 == r) {
        frame = frame - 1;
      } else {
        frame = frame + 1;
      }
    }
*/
}  

void calculateFrame() {
  long now = millis();
  
  long timePassed = now - previousMillis;
  
  int destinationFrame = frameForDistance();
  
  if (timePassed > sleepMillis) {
    // move towards destination
    if (destinationFrame > frame) {
      frame = frame + 1;
    } else if (destinationFrame > frame) {
      frame = frame + 1;
    }

    previousMillis = now;
  }

  // Update HUD  
  textFont(f,36);
  fill(0);
  text("distance=" + nf(distance, 0), 10, 100);
  text("distance of frame=" + nf(int(distanceOfFrame), 0), 10, 140);
  text("frame=" + nf(frame, 0) + "/" + nf(imgcount, 0), 10, 180);
  text("destination frame=" + nf(destinationFrame, 0), 10, 220);
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

PImage getImage(int i) {
  if (!loadedImages[i]) {
    println("loading image " + nf(i, 0) + "/" + nf(imgcount, 0));
    images[i] = loadImage(str(i) + ".jpg");
    loadedImages[i] = true;
  }
  return images[i];
}

void keyPressed() {
  int keyboardStep = 10 + int(random(50));

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

