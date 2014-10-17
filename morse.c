#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "morse.h"

#if !defined(__APPLE__)
	#include <malloc.h>
#endif

void num2word(char **word, int num) {
	int length = 0, counter = 0, bit = 0, i = 0, l = 0;
	char *tmp = NULL;

	for (i = 7, l = 0; i >= l; i--) {
		bit = (num >> i) & 0x01;

		if (!bit) {
			length = i;
			i = l;
		}
	}

	tmp = malloc(sizeof(char) * length + 1);

	for (i = length - 1, l = 0; i >= l; i--) {
		bit = (num >> i) & 0x01;
		tmp[counter++] = (bit) ? '-' : '.';
	}

	tmp[counter] = '\0';

	strcpy(*word, tmp);

	if (tmp) {
		free(tmp);
	}
}

char num2ascii(int num) {
	int index = 0, offset = 0x41, i = 0, l = 0;
	char ascii;

	for (i = 0, l = ALPHA_LENGTH; i < l; i++) {
		if (num == alpha[i]) {
			index = i;
			i = l;
		}
	}

	if (index >= 0x1A) {
		offset -= 0x2A;
	}

	if (index == ALPHA_LENGTH - 1) {
		offset -= 0x0A;
	}

	ascii = offset + index;

	return ascii;
}

int word2num(char *word) {
	int length = 0, num = 0, bit = 0, i = 0, l = 0;

	length = (int)strlen(word);
	num = 0xFF - (0x01 << length);

	for (i = 0, l = length; i < l; i++) {
		bit = (word[i] == '.') ? 1 : 0;
		num -= (bit << (length - i - 1));
	}

	return num;
}

char word2ascii(char *word) {
	return num2ascii(word2num(word));
}

void ascii2word(char **word, char ascii) {
	int charCode = ascii, offset = -0x41;

	if (!*word) {
		*word = malloc(sizeof(char) * 5 + 1);
	}

	strcpy(*word, "");

	if (charCode < 0x41) {
		offset += 0x2A;
	}

	if (charCode < 0x31) {
		offset += 0x0A;
	}

	num2word(&*word, alpha[offset + charCode]);
}

void morse2text(char **text, char *morse) {
	int rc = 0, counter = 0, start = 0, finish = 0, i = 0, j = 0, k = 0, l = 0;
	size_t length = 0, total = 0;
	char chr, ascii;
	char *clean_morse = NULL, *word = NULL;

	length = strlen(morse);

	clean_morse = malloc(sizeof(char) * length + 1);
	word = malloc(sizeof(char) * 5 + 1);
	*text = malloc(sizeof(char) * length + 1);

	strcpy(*text, "");

	for (i = 0, l = (int)length; i < l; i++) {
		chr = morse[i];

		if (chr == '.' || chr == '-' || chr == ' ' || chr == '/') {
			clean_morse[counter++] = chr;
		}
	}

	clean_morse[counter] = '\0';

	length = strlen(clean_morse);
	counter = 0;

	start = 0;
	finish = 0;

	for (i = 0, j = (int)length; i < j; i++) {
		if (clean_morse[i + 1] != ' ' && clean_morse[i + 1] != '\0') {
			finish++;
		} else {
			counter = 0;

			for (k = start, l = finish + 1; k < l; k++) {
				if (clean_morse[k] != ' ' && clean_morse[k] != '\0') {
					word[counter++] = clean_morse[k];
				}
			}

			word[counter] = '\0';

			if (strcmp(word, "/") == 0) {
				ascii = ' ';
			} else {
				ascii = word2ascii(word);
			}

			total = strlen(*text);
			sprintf(*text + total, "%c", ascii);

			start = finish + 1;
			finish = start;
		}
	}

	if (clean_morse) {
		free(clean_morse);
	}

	if (word) {
		free(word);
	}

	if (rc) {
		exit(EXIT_FAILURE);
	}
}

void text2morse(char **morse, char *text) {
	int rc = 0, counter = 0, total = 0, i = 0, l = 0;
	size_t length = 0;
	char chr;
	char *clean_text = NULL, *word = NULL;

	length = strlen(text);

	clean_text = malloc(sizeof(char) * length + 1);
	*morse = malloc(sizeof(char) * (6 * length) + 1);
	word = malloc(sizeof(char) * 5 + 1);

	strcpy(*morse, "");

	for (i = 0, l = (int)length; i < l; i++) {
		chr = text[i];

		if ((chr >= 'a' && chr <= 'z')) {
			chr -= 0x20;
		}

		if ((chr >= 'A' && chr <= 'Z') || (chr >= '0' && chr <= '9') || chr == ' ') {
			clean_text[counter++] = chr;
		}
	}

	clean_text[counter] = '\0';

	length = strlen(clean_text);

	for (i = 0, l = (int)length; i < l; i++) {
		if (clean_text[i] == ' ') {
			strcpy(word, "/");
		} else {
			ascii2word(&word, clean_text[i]);
		}

		total = (int)strlen(*morse);
		sprintf(*morse + total, "%s ", word);
	}

	if (clean_text) {
		free(clean_text);
	}

	if (word) {
		free(word);
	}

	if (rc) {
		exit(EXIT_FAILURE);
	}
}

void wav_open(FILE **wav, char *filename) {
	int rc = 0, sample_rate = SAMPLE_RATE;
	short bits_per_sample = 16;
	struct wav_header header;

	strncpy(header.riff_tag, "RIFF", 4);
	strncpy(header.wave_tag, "WAVE", 4);
	strncpy(header.fmt_tag, "fmt ", 4);
	strncpy(header.data_tag, "data", 4);

	header.riff_length = 0;
	header.fmt_length = 16;
	header.audio_format = 1;
	header.num_channels = 1;
	header.sample_rate = sample_rate;
	header.byte_rate = sample_rate * (bits_per_sample / 8);
	header.block_align = bits_per_sample / 8;
	header.bits_per_sample = bits_per_sample;
	header.data_length = 0;

	*wav = fopen(filename, "w+");

	if (!*wav) {
		printf("Could not create wav \"%s\"\n", filename);

		rc = 1;
		goto cleanup;
	}

	if (fwrite(&header, sizeof(header), 1, *wav) != 1) {
		printf("Could not write wav header\n");

		rc = 1;
		goto cleanup;
	}

	if (fflush(*wav)) {
		printf("Could not flush\n");

		rc = 1;
		goto cleanup;
	}

cleanup:
	if (rc) {
		exit(EXIT_FAILURE);
	}
}

void wav_note(FILE **wav, double duration, double pitch) {
	int rc = 0, i = 0, l = 0;
	short waveform[SAMPLE_RATE * 2];
	double time = 0.0;

	duration *= SPEED;

	for (i = 0, l = (int)(SAMPLE_RATE * duration); i < l; i++) {
		time = (double)i / SAMPLE_RATE;
		waveform[i] = (short)(VOLUME * sin(pitch * time * 2 * M_PI));
	}

	if (fwrite(waveform, sizeof(short), (size_t)l, *wav) != (size_t)l) {
		printf("Could not write waveform\n");

		rc = 1;
		goto cleanup;
	}

cleanup:
	if (rc) {
		exit(EXIT_FAILURE);
	}
}

void wav_close(FILE **wav) {
	int rc = 0, file_length = (int)ftell(*wav), data_length = file_length - 44;
	int riff_length = file_length - 8;

	if (fseek(*wav, 44 - sizeof(int), SEEK_SET)) {
		printf("Could not seek\n");

		rc = 1;
		goto cleanup;
	}

	if (fwrite(&data_length, sizeof(data_length), 1, *wav) != 1) {
		printf("Could not write data length\n");

		rc = 1;
		goto cleanup;
	}

	if (fseek(*wav, 4, SEEK_SET)) {
		printf("Could not seek\n");

		rc = 1;
		goto cleanup;
	}

	if (fwrite(&riff_length, sizeof(riff_length), 1, *wav) != 1) {
		printf("Could not write RIFF length\n");

		rc = 1;
		goto cleanup;
	}

	if (fclose(*wav)) {
		printf("Could not close WAV\n");

		rc = 1;
		goto cleanup;
	}

cleanup:
	if (rc) {
		exit(EXIT_FAILURE);
	}
}

void morse2wav(char *filename, char *morse) {
	int counter = 0, i = 0, l = 0;
	size_t length = 0;
	char chr;
	char *clean_morse = NULL;
	FILE *wav = NULL;

	length = strlen(morse);
	clean_morse = malloc(sizeof(char) * length + 1);

	for (i = 0, l = (int)length; i < l; i++) {
		chr = morse[i];

		if (chr == '.' || chr == '-' || chr == ' ' || chr == '/') {
			clean_morse[counter++] = chr;
		}
	}

	clean_morse[counter] = '\0';
	length = strlen(clean_morse);

	wav_open(&wav, filename);

	for (i = 0, l = (int)length; i < l; i++) {
		switch (clean_morse[i]) {
		case '.':
			wav_note(&wav, DOT, PITCH);
			wav_note(&wav, DOT, 0.0);
			break;
		case '-':
			wav_note(&wav, DOT * 3, PITCH);
			wav_note(&wav, DOT, 0.0);
			break;
		case '/':
			wav_note(&wav, DOT, 0.0);
			break;
		case ' ':
		default:
			wav_note(&wav, DOT * 3, 0.0);
			break;
		}
	}

	wav_close(&wav);

	if (clean_morse) {
		free(clean_morse);
	}
}

void usage(char *exec) {
	int length = 0;

	length = (int)strlen(exec) + 7;
	
	printf("Usage: %s -t, --text  \"<text>\"     Converts ASCII text to Morse code\n", exec);
	printf("%*s -m, --morse \"<morse>\"    Converts Morse code to ASCII text\n", length, " ");
	printf("%*s -w, --wav   <wav>        Exports Morse code as a WAV file\n", length, " ");
}

int main(int argc, char *argv[]) {
	int rc = 0, i = 0, l = 0;
	bool has_text = false, has_morse = false, has_wav = false;
	size_t length = 0;
	char *exec = argv[0], *action = NULL, *input = NULL, *output = NULL;
	char *morse = NULL, *filename = NULL, *text_input = NULL, *morse_input = NULL;

	if (argc < 3) {
		usage(exec);

		rc = 1;
		goto cleanup;
	}

	for (i = 1, l = argc; i < l; i += 2) {
		action = argv[i];
		input = argv[i + 1];

		if (!input) {
			printf("Provide parameter for %s\n", action);

			rc = 1;
			goto cleanup;
		}

		if (strcmp(action, "-t") == 0 || strcmp(action, "--text") == 0) {
			has_text = true;
			has_morse = false;

			text_input = input;
		} else if (strcmp(action, "-m") == 0 || strcmp(action, "--morse") == 0) {
			has_morse = true;
			has_text = false;

			morse_input = input;
		} else if (strcmp(action, "-w") == 0 || strcmp(action, "--wav") == 0) {
			has_wav = true;

			filename = input;
		} else {
			printf("Unknown action %s\n", action);

			rc = 1;
			goto cleanup;
		}
	}

	if (has_text) {
		text2morse(&output, text_input);
		length = strlen(output);

		morse = malloc(sizeof(char) * length + 1);

		strcpy(morse, output);

		printf("%s\n", output);
		free(output);
	}

	if (has_morse) {
		morse = morse_input;
		morse2text(&output, morse_input);

		printf("%s\n", output);
		free(output);
	}

	if (has_wav) {
		if (has_text || has_morse) {
			morse2wav(filename, morse);

			if (has_text) {
				free(morse);
			}
		} else {
			usage(exec);

			rc = 1;
			goto cleanup;
		}
	}

cleanup:
	return rc;
}