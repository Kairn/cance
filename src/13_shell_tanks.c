/*
 * Possibly the worst tank game in existence.
 */

#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define SCREEN_W 80
#define SCREEN_H 40
#define FPS 10

static struct termios saved_term;

#define IN_BUF_LEN 256
char IN_BUF[IN_BUF_LEN];

const char G_FRAME_H1[] = "╔╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╗";
const char G_FRAME_H2[] = "╠╬╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╬╣";
const char G_FRAME_BD[] = "╠╣                                                                            ╠╣";
const char G_FRAME_F1[] = "╠╬╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╬╣";
const char G_FRAME_F2[] = "╚╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╝";

const char *const G_FRAME_TITLE[] = {
    "╠╣               _____ __         ____ ______            __                   ╠╣",
    "╠╣              / ___// /_  ___  / / //_  __/___ _____  / /_______            ╠╣",
    "╠╣              \\__ \\/ __ \\/ _ \\/ / /  / / / __ `/ __ \\/ //_/ ___/            ╠╣",
    "╠╣             ___/ / / / /  __/ / /  / / / /_/ / / / / ,< (__  )             ╠╣",
    "╠╣            /____/_/ /_/\\___/_/_/  /_/  \\__,_/_/ /_/_/|_/____/              ╠╣"};

const char *const G_FRAME_WIN[] = {
    "╠╣               _    ______________________  ______  __   ____               ╠╣",
    "╠╣              | |  / /  _/ ____/_  __/ __ \\/ __ \\ \\/ /  / / /               ╠╣",
    "╠╣              | | / // // /     / / / / / / /_/ /\\  /  / / /                ╠╣",
    "╠╣              | |/ // // /___  / / / /_/ / _, _/ / /  /_/_/                 ╠╣",
    "╠╣              |___/___/\\____/ /_/  \\____/_/ |_| /_/  (_|_)                  ╠╣"};

const char *const G_FRAME_LOSS[] = {"╠╣                   ____  _____________________  ______                      ╠╣",
                                    "╠╣                  / __ \\/ ____/ ____/ ____/   |/_  __/                      ╠╣",
                                    "╠╣                 / / / / __/ / /_  / __/ / /| | / /                         ╠╣",
                                    "╠╣                / /_/ / /___/ __/ / /___/ ___ |/ /    _ _ _                 ╠╣",
                                    "╠╣               /_____/_____/_/   /_____/_/  |_/_/    (_|_|_)                ╠╣"};

const char *const G_FRAME_INS[] = {"╠╣                          MOVE: WASD, FIRE: K                               ╠╣",
                                   "╠╣                                                                            ╠╣",
                                   "╠╣                          Press E to Continue                               ╠╣",
                                   "╠╣                                                                            ╠╣",
                                   "╠╣                          Press Q to Quit                                   ╠╣"};

/* Sets the terminal to non-canonical mode to read keyboard inputs. */
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

/* Pauses the program between frames. */
void frame_pause() { usleep(1000000 / FPS); }

/* Reads an input from the player; the last valid input will be used. */
char getInputChar() {
  ssize_t len = read(STDIN_FILENO, IN_BUF, IN_BUF_LEN - 1);
  if (len) {
    /* TODO: Filter invalid chars if present. */
    return IN_BUF[len - 1];
  }
  return 0;
}

int main(int argc, char const *argv[]) {
  /* Save the terminal state. */
  tcgetattr(STDIN_FILENO, &saved_term);
  setNonCanonicalMode();
  /* TURN OFF STDOUT BUFFER: setbuf(stdout, NULL); */

  /* Restore the terminal to normal mode. */
  tcsetattr(STDIN_FILENO, TCSANOW, &saved_term);
  return 0;
}
