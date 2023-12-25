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
char *SCREEN[SCREEN_H];
static int rendered = 0;

#define FPS 10

static struct termios saved_term;

#define IN_BUF_LEN 256
char IN_BUF[IN_BUF_LEN];

#define BORDER_SIZE 3
#define HF_LEN SCREEN_W *BORDER_SIZE
#define BD_LEN SCREEN_W + (BORDER_SIZE - 1) * 4
const char G_FRAME_H1[] = "╔╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╗";
const char G_FRAME_H2[] = "╠╬╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╬╣";
const char G_FRAME_BD[] = "╠╣                                                                            ╠╣";
const char G_FRAME_F1[] = "╠╬╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╬╣";
const char G_FRAME_F2[] = "╚╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╝";

#define TITLE_START 8
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

#define INS_START 23
#define E_LINE INS_START + 2
const char *const G_FRAME_INS[] = {"╠╣                          MOVE: WASD, FIRE: K                               ╠╣",
                                   "╠╣                                                                            ╠╣",
                                   "╠╣                          Press E to Continue                               ╠╣",
                                   "╠╣                                                                            ╠╣",
                                   "╠╣                          Press Q to Quit                                   ╠╣"};

/* Sets the terminal to non-canonical mode to read keyboard inputs. */
void set_non_canonical_mode() {
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
char get_input_char() {
  ssize_t len = read(STDIN_FILENO, IN_BUF, IN_BUF_LEN - 1);
  if (len) {
    /* TODO: Filter invalid chars if present. */
    return IN_BUF[len - 1];
  }
  return 0;
}

/* Clears the screen for re-rendering. */
void clear_screen() {
  int i = 0;
  for (; i < SCREEN_H + 1; ++i) {
    printf("\x1b[1F");
    printf("\x1b[2K");
  }
  rendered = 0;
}

/* Renders the screen from memory. */
void render() {
  if (rendered) {
    clear_screen();
  }
  int i = 0;
  for (; i < SCREEN_H; ++i) {
    printf("%s\n", SCREEN[i]);
  }
  rendered = 1;
}

/* Fills the screen memory with default border. */
void screen_fill_def() {
  memcpy(SCREEN[0], G_FRAME_H1, HF_LEN);
  memcpy(SCREEN[1], G_FRAME_H2, HF_LEN);
  int i = 2;
  for (; i < SCREEN_H - 2; ++i) {
    memcpy(SCREEN[i], G_FRAME_BD, BD_LEN);
  }
  memcpy(SCREEN[SCREEN_H - 2], G_FRAME_F1, HF_LEN);
  memcpy(SCREEN[SCREEN_H - 1], G_FRAME_F2, HF_LEN);
}

/* Runs the title screen with certain title text and waits for user input. */
int run_title_screen(const char *const text[]) {
  screen_fill_def();
  int i = 0;
  for (; i < SCREEN_H; ++i) {
    if (i >= TITLE_START && i < TITLE_START + 5) {
      memcpy(SCREEN[i], text[i - TITLE_START], BD_LEN);
    }
    if (i >= INS_START && i < INS_START + 5) {
      memcpy(SCREEN[i], G_FRAME_INS[i - INS_START], BD_LEN);
    }
    render();
    frame_pause();
  }

  int tick = 0;
  int show_e = 1;
  while (1) {
    ++tick;
    if (tick % 5 == 0) {
      show_e = !show_e;
      if (show_e) {
        memcpy(SCREEN[E_LINE], G_FRAME_INS[2], BD_LEN);
      } else {
        memcpy(SCREEN[E_LINE], G_FRAME_BD, BD_LEN);
      }
    }

    char ui = get_input_char();
    if (ui) {
      switch (ui) {
        case 'q':
          return 0;
        case 'e':
          return 1;
        default:
          continue;
      }
    }
    render();
    frame_pause();
  }
}

int main(int argc, char const *argv[]) {
  /* Save the terminal state. */
  tcgetattr(STDIN_FILENO, &saved_term);
  set_non_canonical_mode();
  /* TURN OFF STDOUT BUFFER: setbuf(stdout, NULL); */

  /* Initialize memory for the screen. */
  int i = 0;
  for (; i < SCREEN_H; ++i) {
    SCREEN[i] = calloc(SCREEN_W + 1, sizeof(BORDER_SIZE));
  }

  screen_fill_def();
  render();

  int start = run_title_screen(G_FRAME_TITLE);
  if (start) {
    printf("Game starting...\n");
  } else {
    printf("Quitting...\n");
  }

  for (i = 0; i < SCREEN_H; ++i) {
    free(SCREEN[i]);
  }

  /* Restore the terminal to normal mode. */
  tcsetattr(STDIN_FILENO, TCSANOW, &saved_term);
  return 0;
}
