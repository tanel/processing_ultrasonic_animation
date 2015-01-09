import processing.serial.*;
import ddf.minim.*;
import ddf.minim.signals.*;
import ddf.minim.analysis.*;
import ddf.minim.effects.*;
import ddf.minim.ugens.*;

Serial myPort;

int imgcount = 20;
PImage images[] = new PImage[imgcount];

Minim minim;
AudioOutput out;

// the Oscil we use for modulating frequency.
Oscil fm;

void setup() {
  size(displayWidth, displayHeight); //Use entire screen size.
  smooth(); // draws all shapes with smooth edges.

  for (int i = 0; i < imgcount; i++) {
    images[i] = loadImage(str(i) + ".jpg");
  }

  // initialize the minim and out objects
  minim = new Minim( this );
  out   = minim.getLineOut();
  
  // make the Oscil we will hear.
  // arguments are frequency, amplitude, and waveform
  Oscil wave = new Oscil( 200, 0.8, Waves.TRIANGLE );
  // make the Oscil we will use to modulate the frequency of wave.
  // the frequency of this Oscil will determine how quickly the
  // frequency of wave changes and the amplitude determines how much.
  // since we are using the output of fm directly to set the frequency 
  // of wave, you can think of the amplitude as being expressed in Hz.
  fm   = new Oscil( 10, 2, Waves.SINE );
  // set the offset of fm so that it generates values centered around 200 Hz
  fm.offset.setLastValue( 50 );
  // patch it to the frequency of wave so it controls it
  fm.patch( wave.frequency );
  // and patch wave to the output
  wave.patch( out );

  myPort = new Serial(this, "/dev/tty.usbmodem1411", 9600);
  myPort.bufferUntil('\n'); // Trigger a SerialEvent on new line
}

int distance = 0;
long previousMillis = 0;
int frame = 16;
int max = 1;
float period = 1;

void draw() {
  if (distance > max) {
    max = distance;
  }
  
  long now = millis();
  long n = now - previousMillis;
  if (n > 200) {
    float modulateFrequency = map( distance, 0, max, 0.1, 100 );
    fm.setFrequency( modulateFrequency );

//    float modulateAmount = map( distance, 0, max, 220, 1 );
//    fm.setAmplitude( modulateAmount );
    
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
  return true;
}

