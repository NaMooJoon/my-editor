#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios orig_termios;

void disableRawMode() {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

/*
	ECHO - turn off echoing
	ICANON - turn off canonical mode(read by line)
	ISIG - turn off `Ctrl-C` and `Ctrl-Z` signals
	IEXTEN - diable `Ctrl-V`
	IXON - turn off `Ctrl-S` and `Ctrl-Q` software flow control
	ICRNL - CR: carrige return, NL: new line
	OPOST - turn off output processing / "\n" to "\r\n"
	
	<Miscellaneous flags>
	BRKINT - break condition
	INPCK - parity checking
	ISRTRIP - the 8th bit to be stripped
	CS8 - character size to 8 bits per bytes. 
*/
void enableRawMode() {
	tcgetattr(STDIN_FILENO, &orig_termios);
	atexit(disableRawMode);

	struct termios raw = orig_termios;
	
	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN); 

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
	enableRawMode();

	char c;
	while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
		if (iscntrl(c)) {
			printf("%d\r\n", c);
		} else {
			printf("%d ('%c')\r\n", c, c);
		}
	}
	return 0;
}
