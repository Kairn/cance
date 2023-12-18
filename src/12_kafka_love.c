/*
 * A program that renders ASCII texts in the shell frame by frame.
 *
 * Concepts:
 * Basic screen rendering with frame rate control.
 */

#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SCREEN_H 28
#define SCREEN_W 78
#define TEXT_H 5
#define FPS 45

/* Critical text rows of the frame. */
char const *TL_1[] = {
    "/ /\\ \\             ___   _                                              "
    "/ /\\ \\",
    "\\ \\/ /            |_ _| | |    _____   _____                            "
    "\\ \\/ /",
    " \\/ /              | |  | |   / _ \\ \\ / / _ \\                         "
    "   \\/ / ",
    " / /\\              | |  | |__| (_) \\ V /  __/                           "
    " / /\\ ",
    "/ /\\ \\            |___| |_____\\___/ \\_/ \\___|                        "
    "   / /\\ \\"};

char const *TL_2[] = {
    " \\/ /              _  __      __ _                                       "
    "\\/ / ",
    " / /\\             | |/ /__ _ / _| | ____ _                               "
    "/ /\\ ",
    "/ /\\ \\            | ' // _` | |_| |/ / _` |                             "
    "/ /\\ \\",
    "\\ \\/ /            | . \\ (_| |  _|   < (_| |                            "
    " \\ \\/ /",
    " \\/ /             |_|\\_\\__,_|_| |_|\\_\\__,_|                          "
    "    \\/ / "};

char const *TL_3[] = {
    "/ /\\ \\             _____                                                "
    "/ /\\ \\",
    "\\ \\/ /            |  ___|__  _ __ _____   _____ _ __                    "
    "\\ \\/ /",
    " \\/ /             | |_ / _ \\| '__/ _ \\ \\ / / _ \\ '__|                "
    "    \\/ / ",
    " / /\\             |  _| (_) | | |  __/\\ V /  __/ |                      "
    " / /\\ ",
    "/ /\\ \\            |_|  \\___/|_|  \\___| \\_/ \\___|_|                  "
    "    / /\\ \\",
    "\\ \\/ /                                                                  "
    "\\ \\/ /",
    " \\/ /                                                                    "
    "\\/ / "};

void render(char *frame[]);
void frame_pause();
void clear_screen();
void roll_line(char *frame[], int frame_row_start, char const *tl[]);

int main(int argc, char const *argv[]) {
  /* Initialize an empty frame. */
  char const *init_frame[] = {
      " .--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--"
      "..--. ",
      "/ .. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. "
      "\\.. \\.. \\.. \\.. \\.. \\",
      "\\ \\/\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ "
      "`'\\ `'\\ `'\\ `'\\ `'\\ \\/ /",
      " \\/ "
      "/`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'\\/"
      " / ",
      " / /\\                                                                  "
      "  / /\\ ",
      "/ /\\ \\                                                                "
      "  / /\\ \\",
      "\\ \\/ /                                                                "
      "  \\ \\/ /",
      " \\/ /                                                                  "
      "  \\/ / ",
      " / /\\                                                                  "
      "  / /\\ ",
      "/ /\\ \\                                                                "
      "  / /\\ \\",
      "\\ \\/ /                                                                "
      "  \\ \\/ /",
      " \\/ /                                                                  "
      "  \\/ / ",
      " / /\\                                                                  "
      "  / /\\ ",
      "/ /\\ \\                                                                "
      "  / /\\ \\",
      "\\ \\/ /                                                                "
      "  \\ \\/ /",
      " \\/ /                                                                  "
      "  \\/ / ",
      " / /\\                                                                  "
      "  / /\\ ",
      "/ /\\ \\                                                                "
      "  / /\\ \\",
      "\\ \\/ /                                                                "
      "  \\ \\/ /",
      " \\/ /                                                                  "
      "  \\/ / ",
      " / /\\                                                                  "
      "  / /\\ ",
      "/ /\\ \\                                                                "
      "  / /\\ \\",
      "\\ \\/ /                                                                "
      "  \\ \\/ /",
      " \\/ /                                                                  "
      "  \\/ / ",
      " / "
      "/\\.--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--./"
      " /\\ ",
      "/ /\\ \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. "
      "\\.. \\.. \\.. \\.. \\/\\ \\",
      "\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ "
      "`'\\ `'\\ `'\\ `'\\ `'\\ `' /",
      " `--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--"
      "'`--' "};

  /* Copy the init frame into dynamic memory which will be modified during frame
   * updates. */
  char *frame[SCREEN_H];
  int row = 0;
  for (; row < SCREEN_H; ++row) {
    frame[row] = calloc(SCREEN_W + 1, sizeof(char));
    strcpy(frame[row], init_frame[row]);
  }

  render(frame);

  roll_line(frame, 5, TL_1);
  roll_line(frame, 11, TL_2);
  roll_line(frame, 17, TL_3);

  row = 0;
  for (; row < SCREEN_H; ++row) {
    free(frame[row]);
  }

  return 0;
}

/* Renders the frame to the screen with the given data pointer. */
void render(char *frame[]) {
  printf("\n");
  int row = 0;
  for (; row < SCREEN_H; ++row) {
    printf("%s\n", frame[row]);
  }
  printf("\n");
}

/* Pauses the rendering between frames based on the FPS. */
void frame_pause() { usleep(1000000 / FPS); }

/* Clears the screen for re-rendering. */
void clear_screen() {
  int i = 0;
  /* Erase every line in the frame and the leading/trailing newlines. */
  for (; i < SCREEN_H + 2; ++i) {
    printf("\x1b[1F");
    printf("\x1b[2K");
  }
}

/* Rolls out a line of ASCII text in the main frame at the given row. */
void roll_line(char *frame[], int frame_row_start, char const *tl[]) {
  int col = 0;
  for (; col < SCREEN_W; ++col) {
    clear_screen();
    int i = 0;
    for (; i < TEXT_H; ++i) {
      frame[frame_row_start + i][col] = tl[i][col];
    }
    render(frame);
    frame_pause();
  }
}
