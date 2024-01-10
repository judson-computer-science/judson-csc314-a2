# Assignment 2 - Audio Encoder
In this assignment you will create an Audio Encoding program that will generate an audio file
encoded in PCM format (a `.wav` file) with 16 bit encoding at 44100 Hz. Your program will take 3 command line
parameters indicating the name of the output file, the Amplitude Option and the Frequency Option.
The Amplitued Option determines how the volume of the audio track will vary over the duration of
the clip and the Frequency Option determines how the frequency of the audio track will vary. The
sound will be generated using a `sin()` function to generate a sine wave. The amplitude (height) of
the sine wave determines the volume of the sound and the period determines the pitch, where a shorter
period mean a higher frequency, and therefore a higher pitch. Your program will prompt the user for the
duration of the audio clip and it will apply the amplitude and frequency options to the sine wave to
create the desired effect. Before generating the audio file, your program will also print a summary of
the file's properties as shown in the output below. A PCM file contains meta data (information) about
the encoding that is found in the file. This meta data is stored in key/value pairs (headers) at the
beginning of the file. Two functions have been provided for you (in `pcm.h`) to generate these headers.

Once your file has been created, you can listen to it by double-clicking it (inside your VM). You can
also see a visualization of the audio track by opening the file in **Audacity** (note, if you zoom in
close enough, you can see each individual sample plotted on the graph). Finally, you can use the command
line tool **Octal Dump** (using the command `od -A x -t x <output file>`) to see a hex dump of the file
contents. This can be used to see how the bits are being written to the file.

The program will be written in C.

## Input
Three command line parameters will be required as shown in the Output section below.
Additionally, the program will prompt the user for the duration of the audio
clip to be generated.

## Output
Several example files are provided for some of the scenarios. They can be found in the
`examples` directory of your assignment repo. You can use **Octal Dump** and **Audacity** to open
these files and see what your output should contain when the same options are selected
while running your program. The interaction with the program is shown below.

### Example 1 - Incorrect Usage
Your program should require the user to provide exactly 3 command line parameters. The first
command line parameter is the name of the output file and it MUST have the .wav file extension.
If a different file extension is used, the program should print the Usage message as shown below.
Also, if any number of parameters are provided other than 3, your program should print the Usage
message show below.

	>$ ./encoder
	Usage: ./encoder <audio_file>.wav <amplitude_option> <frequency_option>

		amplitude_option:
			0 - Pulse
			1 - Gradually Increasing
			2 - Gradually Decreasing
			3 - Constant

		frequency_option:
			0 - Gradually Increasing
			1 - Gradually Decreasing
			2 - Constant

### Example 2 - Generating 15 sec Clip, Increasing Amplitude, Decreasing Frequency
Your program should generate the following output in the case where 3 command line parameters
have been provided. Note that in the Summary section, the labels on each line (before the colon)
are 17 character-wide fields. You must use the correct format specifiers to generate them.

	>$ ./encoder test.wav 1 1
	Enter duration (sec): 15
	
	Summary
	=======
	Generating 15 sec audio file
	Amplitude Option: 1, Frequency Option: 1
	     Sample Rate: 44100 samples per second (Hz)
	       Bit Depth: 16 bits
	        Bit Rate: 705600
	   Total Samples: 661500
	Total Audio Data: 10584000 bits (1323000 bytes)


## Implementation
 - Use the `get_pcm_header()` and `write_pcm_header()` functions (provided in `pcm.h`) to generate the PCM file's
headers and write the headers to the output file.
 - After generating the headers, you can start writing your audio samples (the actual sound) by calling
the `sin()` function.
   - Your program will calculate the total number of samples to be generated (based on the sample rate
and the duration selected by the user). Each sample is then generated using the loop counter and a multiplication
factor that is derived by calling the `get_frequency()` function. These factors will determine the frequency of
the sine wave generated. Since the amplitude of the sine wave is always in the range of -1 to 1, a multiplier will
need to be applied to scale the value between the max and min values that can be represented in 16 bits. This
multiplier is derived using the `get_amplitude()` function.
 - Because you are using 16 bit encoding, each audio sample will be a positive or negative number represented
by 16 bits. Be sure to use appropriately sized datatypes.
 - To write data into a binary file, you will open the file with `fopen()` using the `wb` option. You will then
use the `frwite()` function to write 16 bits of data at a time. Remember that there is a C reference bookmarked
in the browser of your virtual machine.
 - You will create a function for each Amplitude Option and Frequency Option that the user can select. Based on the
Amplitude Option and Frequency Option selections, you will then pass the appropriate function to `get_amplitude()`
and `get_frequency()` functions respectively (as a function pointer). Review the lesson on Function Pointers on
the class E-Learning site if necessary.

### Functions
Your program MUST include implementations for the functions corresponding to each function prototype listed here:
 - `int get_frequency(int, int, int (*)(int, int))`
   - This function calls the function that is passed as the last parameter. The first and second parameter values
are used as the parameters to the function passed in.
   - **Returns**:  a value that is used to scale the sine wave (higher values mean a shorter sine wave period)
   - **Parameters**:
      1. the current sample number
      2. the total number of samples to be used throughout the entire audio file
      3. a function pointer pointing to the fuction that will be used to generate the frequency

 - `int get_amplitude(int, int, int (*)(int, int))`
   - This function calls the function that is passed as the last parameter. The first and second parameter values
are used as the parameters to the function passed in. Since the `sin()` function will always generate a number
between -1 and 1, the output of this function will be used to scale that result between the max and min values that
can be represented in 16 bits.
   - **Returns**:  a value that is used to magnify the sine wave (higher values means a greater amplitude)
   - **Parameters**:
      1. the current sample number
      2. the total number of samples to be used throughout the entire audio file
      3. a function pointer pointing to the fuction that will be used to generate the amplitude multiplier

 - `int get_const_frequency(int, int)`
   - This function generates a constant frequency multiplier of 400 (ultimately contributing to the creation of
a sine wave having 400 periods (cycles) per second (i.e. 400 Hz). This is one of the functions that will be
passed to `get_frequency()` depending on the user selection.
   - **Returns**:  an integer (hard-coded to 400)
   - **Parameters**:
      1. the current sample this multiplier will be applied to
      2. the total number of samples in the entire file

 - `int get_incr_frequency(int, int)`
   - This function generates an increasing frequency multiplier in the range of 100 to 1000. If the current
sample is within the first 10% of the total samples, it will return 100. The values will increase by 100 for
each additional 10% of progress throughout the file ending in a value of 1000 for the final 10%. This is
one of the functions that will be passed to `get_frequency()` depending on the user selection.
   - **Returns**:  an integer (the frequency multiplier)
   - **Parameters**:
      1. the current sample this multiplier will be applied to
      2. the total number of samples in the entire file

 - `int get_decr_frequency(int, int)`
   - This function does the same as `get_incr_frequency()` except the values decrease from 1000
to 100 over time. This is one of the functions that will be passed to `get_frequency()` depending on the
user selection.
   - **Returns**:  an integer (the frequency multiplier)
   - **Parameters**:
      1. the current sample this multiplier will be applied to
      2. the total number of samples in the entire file

 - `int get_const_amplitude(int, int)`
   - This function generates a constant amplitude multiplier equal to the max positive value that can be
stored in 16 bits (`0x7FFF`). This is one of the function that will be passed to `get_amplitude()` depending
on user selection.
   - **Returns**:  an integer (hard-coded to `0x7FFF`)
   - **Parameters**:
      1. the current sample this multiplier will be applied to
      2. the total number of samples in the entire file

 - `int get_incr_amplitude(int, int)`
   - This function generates an increasing amplitude multiplier in the range of `0x1FFF` to `0x7FFF`. If the current
sample is within the first 25% of the total samples, it will return `0x1FFF`. The values will increase by `0x2000` for
each additionaly 25% of progress throughout the file ending in a value of `0x7FFF` for the final 25%. This is
one of the functions that will be passed to `get_amplitude()` depending on the user selection.
   - **Returns**:  an integer (the amplitude multiplier)
   - **Parameters**:
      1. the current sample this multiplier will be applied to
      2. the total number of samples in the entire file

 - `int get_decr_amplitude(int, int)`
   - This function does the same as `get_incr_amplitude()` except the values decrease from `0x7FFF` to `0x1FFF`.
This is one of the functions that will be passed to `get_amplitude()` depending on the user selection.
   - **Returns**:  an integer (the amplitude multiplier)
   - **Parameters**:
      1. the current sample this multiplier will be applied to
      2. the total number of samples in the entire file

 - `int get_pulse_amplitude(int, int)`
   - This function generates an alternating amplitude multiplier of either `0x0000` (silence) or `0x7FFF` (loudest volume).
If the current sample is within the first 10% of the total samples, it will return `0x0000`. The values will alternate
for each additional 10% of progress throughout the file. This is one of the functions that will be passed to
`get_amplitude()` depending on the user selection.
   - **Returns**:  an integer (the amplitude multiplier)
   - **Parameters**:
      1. the current sample this multiplier will be applied to
      2. the total number of samples in the entire file

## Checking your results
In addition to listening to your output file you can compare your output file to the examples provided by using
the sum command. The command will compute a number (a checksum) based on the contents of the file such that any
two files having identical contents will produce the same checksum (see the man pages for sum for more details).
In the two examples below, the first shows an output file having a different result than the example file and
the second shows an output file having the same result.

	>$ sum my_file.wav 1-2-10.wav 
	40650   862 my_file.wav
	29513   862 1-2-10.wav

	>$ sum my_file.wav 0-1-10.wav 
	40650   862 my_file.wav
	40650   862 0-1-10.wav

Opening your file in **Audacity** will also provide insight into how the sine wave looks visually. This can help if you
are not getting the result you expect because you will be able to see a visual depiction of what the samples
look like when plotted as a function.

Finally, to see the actual values the were written to the file, use the **Octal Dump** tool.
