#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "pcm.h"

#define RV_SUCC 0
#define RV_FAIL_NO_OUTPUT_FILE 1
#define RV_FAIL_BAD_FILE_EXT 2
#define RV_FAIL_CANT_OPEN_FILE 3

#define BIT_DEPTH 16      // # of bits of information per sample
#define SAMPLE_RATE 44100 // number of samples per second (Hz)
#define ONE_WAVE_PHASE (2.0 * M_PI)

typedef enum {
	AMP_PULSE,
	AMP_INCREASING,
	AMP_DECREASING,
	AMP_CONSTANT
} AmplitudeOption;

typedef enum {
	FREQ_INCREASING,
	FREQ_DECREASING,
	FREQ_CONSTANT
} FrequencyOption;

int get_frequency(int, int, int (*)(int, int));
int get_incr_frequency(int, int);
int get_decr_frequency(int, int);
int get_const_frequency(int, int);

int get_amplitude(int, int, int (*)(int, int));
int get_incr_amplitude(int, int);
int get_decr_amplitude(int, int);
int get_const_amplitude(int, int);
int get_pulse_amplitude(int, int);

int main(int argc, char** argv) {

	FILE * fp;    // the output file
	int duration; // TODO: assign a value based on user input


	int num_samples = duration * SAMPLE_RATE;  // Total samples in the entire audio file
	int sample_size = num_samples * BIT_DEPTH;



	// Generate the samples and write to output file
	for (int i = 0; i < num_samples; i++) {

		int frequency = get_frequency(i, num_samples, freq_function);
		int amplitude = get_amplitude(i, num_samples, amp_function);

		// generate the next sample based on sin() function
		int16_t sample = (int16_t) (amplitude * sin(frequency * i * (ONE_WAVE_PHASE / SAMPLE_RATE)));

	}


  return RV_SUCC;
}
