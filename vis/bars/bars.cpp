#include "bars.h"

Bars::Bars() : Visualizer::Visualizer() {};

Bars::Bars(Adafruit_NeoPixel** bars, audio_bins * bins, Beat_Detection * bd, float in_factor, float out_factor)
: Visualizer(bars, bins, bd)
{
    this->in_factor = in_factor;
    this->out_factor = out_factor;
}

void Bars::tick(audio_bins * in_bins) {
    decay(out_factor);
    float bass_level = 0;
    int red, green, blue;

    // if (strobe)
    //   bass_level = (log(((in_bins->left[LEFT_63]+in_bins->right[LEFT_63])/2.0f)/BINS_MAX)+0.7f) * 5 * 255.0f;

    for (char i = 0; i < NUM_BARS; i++) {
      for (char j = 0; j < BAR_HEIGHT; j++) {
        // get bin
        //int level = (i < NUM_BARS/2) ? bar_levels[i] : bar_levels[7-i];

        // if (strobe)
        //   mix_pixel(i, j, 0.5f, bass_level, bass_level, bass_level);

        // Set bar levels
        int level = 0;
        switch(i) {
            case 0: level = in_bins->left[LEFT_160];     break;
            case 1: level = in_bins->left[LEFT_1000];    break;
            case 2: level = in_bins->left[LEFT_6250];    break;
            case 3: level = in_bins->left[LEFT_16000];   break;
            case 4: level = in_bins->right[RIGHT_16000]; break;
            case 5: level = in_bins->right[RIGHT_6250];  break;
            case 6: level = in_bins->right[RIGHT_1000];  break;
            case 7: level = in_bins->right[RIGHT_160];   break;
        }

        level *= FREQ_GAIN;
        // set bar
        if (j < (pow((float)(level)/(float)(BINS_MAX), 2)) * (STRIP_LENGTH)) {
        //if (j < (float)(level)/(float)(BINS_MAX) * (STRIP_LENGTH))

          float val = level*2*PI/BINS_MAX;

          // Select colors
          switch(bd->num_beats() % 3) {
            case 0:
              red = cos(val)*255;
              green = cos(val - 2*PI/3)*255;
              blue = cos(val - 4*PI/3)*255;
              break;
            case 1:
              red = cos(val - 4*PI/3)*255;
              green = cos(val)*255;
              blue = cos(val - 2*PI/3)*255;
              break;
            case 2:
              red = cos(val - 2*PI/3)*255;
              green = cos(val - 4*PI/3)*255;
              blue = cos(val)*255;
              break;
          }

          mix_pixel(i, j, in_factor, red, green, blue);
          /*mix_pixel(i, j, in_factor, cos(val)*255, cos(val - 2*pi/3)*255, cos(val - 4*pi/3)*255);*/

          /*mix_pixel(i, j, in_factor, in_bins->left[0]/16,
                                     in_bins->left[1]/16,
                                     in_bins->left[2]/16);*/

          /*mix_pixel(i, j, in_factor, in_bins->left[0]/(64-(in_bins->left[1]/128)),
                                     in_bins->left[1]/(64-(in_bins->left[2]/128)),
                                     in_bins->left[2]/(64-(in_bins->left[0]/128)));*/
        }
      }
    }
}
