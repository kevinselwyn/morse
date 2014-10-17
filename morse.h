#ifndef _MORSE_H
#define _MORSE_H

#define ALPHA_LENGTH 36
#define SAMPLE_RATE 44100
#define DOT 0.25
#define SPEED 0.25
#define PITCH 1000.0
#define VOLUME 32000

#ifndef M_PI
	#define M_PI 3.1415
#endif

int alpha[ALPHA_LENGTH] = {
	0x41, 0x88, 0x8A, 0x64, 0x20, 0x82, 0x66, 0x80, 0x40, 0x87, 0x65, 0x84,
	0x43, 0x42, 0x67, 0x86, 0x8D, 0x62, 0x60, 0x21, 0x61, 0x81, 0x63, 0x89,
	0x8B, 0x8C, 0xAF, 0xA7, 0xA3, 0xA1, 0xA0, 0xB0, 0xB8, 0xBC, 0xBE, 0xBF
};

struct wav_header {
	char riff_tag[4];
	int riff_length;
	char wave_tag[4], fmt_tag[4];
	int fmt_length;
	short audio_format, num_channels;
	int sample_rate, byte_rate;
	short block_align, bits_per_sample;
	char data_tag[4];
	int data_length;
};

void num2word(char **word, int num);
char num2ascii(int num);
int word2num(char *word);
char word2ascii(char *word);
void ascii2word(char **word, char ascii);
void morse2text(char **text, char *morse);
void text2morse(char **morse, char *text);
void wav_open(FILE **wav, char *filename);
void wav_note(FILE **wav, double duration, double pitch);
void wav_close(FILE **wav);
void morse2wav(char *filename, char *morse);
void usage(char *exec);

#endif /* _MORSE_H */