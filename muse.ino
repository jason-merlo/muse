/*
 *  Written by Jason Merlo
 *  Last update 9/15/2015
 *
 *  muse.ino
 *
 *  Main program for visuled visulizer
 */

struct audio_bins {
    // Audio bins
    int left[7];
    int right[7];
}

/**** Pin defines ****/
// Digital Outputs
const char rst = D0;
const char strobe = D1;

// Digital Inputs
const char pwr_sw = D2;

// Analog Inputs (0-4096)
const char audio_r = A0;
const char audio_l = A1;

void setup() {
    // Open debug terminal
    Serial.begin(57600);

    // Set pin modes
    pinMode(rst, OUTPUT);
    pinMode(strobe, OUTPUT);
    pinMode(pwr_sw, INPUT);
    pinMode(audio_l, INPUT);
    pinMode(audio_r, INPUT);

    // Declare memory for audio bin structure
    struct audio_bins bins;

    while (1) {
        Serial.print("Testing...\n");
        delay(1000);
    }
}

// Appease Arduino :p
void loop();

void init_eq() {
    /*  Reset sequence:
     *
     *  rst     _|‾‾‾‾‾|__
     *
     *  strobe  _|‾‾|__|‾‾
     */
    digitalWrite(rst, HIGH);
    digitalWrite(strobe, HIGH);
    delay(1);
    digitalWrite(strobe, LOW);
    delay(1);
    digitalWrite(rst, LOW);
    digitalWrite(strobe, HIGH);
    delay(1);
}

void sample_freq() {
    for (int i = 0; i < sizeof(bins_l)/sizeof(int); i++) {
        digitalWrite(strobe, LOW);
        delay(1); // allow for EQ mux to fully switch

        bins_l[i] = analogRead(audio_l);
        bins_r[i] = analogRead(audio_r);

        digitalWrite(strobe, HIGH);
        delay(1); // allow for EQ mux to fully switch
    }
}
