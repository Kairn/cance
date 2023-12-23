/*
 * Possibly the worst tank game in existence.
 */

#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

void setNonCanonicalMode() {
  struct termios term;
  tcgetattr(STDIN_FILENO, &term);
  term.c_lflag &= ~(ICANON | ECHO | IEXTEN | IXON);
  term.c_cflag &= ~(CSIZE | PARENB | ISTRIP);
  term.c_cflag |= CS8;
  term.c_cc[VMIN] = 0;
  term.c_cc[VTIME] = 0;
  tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

int main(int argc, char const *argv[]) {
  setNonCanonicalMode();
  setbuf(stdout, NULL);
  int buf_len = 256;
  char buf[buf_len];

  while (1) {
    usleep(100000);
    ssize_t res = read(STDIN_FILENO, buf, buf_len - 1);
    if (res) {
      printf("%s", buf);
    } else {
      printf("_");
    }
    memset(buf, 0, buf_len);
  }

  return 0;
}
