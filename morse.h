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
	0xF9, 0xE8, 0xEA, 0xF4, 0xFC, 0xE2, 0xF6, 0xEF, 0xF8, 0xE7, 0xF5, 0xE4,
	0xFB, 0xFA, 0xF7, 0xE6, 0xED, 0xF2, 0xF0, 0xFD, 0xF1, 0xE1, 0xF3, 0xE9,
	0xEB, 0xEC, 0xCF, 0xC7, 0xC3, 0xC1, 0xC0, 0xD0, 0xD8, 0xDC, 0xDE, 0xDF
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