# morse

Utility for converting to and from Morse code

## Usage

```bash
Usage: morse -t, --text  "<text>"     Converts ASCII text to Morse code
             -m, --morse "<morse>"    Converts Morse code to ASCII text
             -w, --wav   <wav>        Exports Morse code as a WAV file
```

## Install

```bash
make && sudo make install
```

## Example

```bash
$ morse -t "SOS"
... --- ...
$ morse -m ".... . .-.. .-.. --- / .-- --- .-. .-.. -.."
HELLO WORLD
```
