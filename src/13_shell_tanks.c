/*
 * Possibly the worst tank game in existence.
 */

#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#define SCREEN_W 80
#define SCREEN_H 40
char *SCREEN[SCREEN_H];
static int rendered = 0;

#define FPS 20

enum GameState {
  INIT,
  PLAYING,
  WON,
  LOST,
  ENDED,
};
enum GameState G_STATE = INIT;

static struct termios saved_term;

#define IN_BUF_LEN 256
char IN_BUF[IN_BUF_LEN];
char VOID_BUF[IN_BUF_LEN];

#define BORDER_SIZE 3
#define HF_LEN SCREEN_W *BORDER_SIZE
#define BD_LEN SCREEN_W + (BORDER_SIZE - 1) * 4
#define SCREEN_TOP 2
#define SCREEN_BOTTOM 37
#define SCREEN_LEFT 6
#define SCREEN_RIGHT 81
const char G_FRAME_H1[] = "╔╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╗";
const char G_FRAME_H2[] = "╠╬╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╬╣";
const char G_FRAME_BD[] = "╠╣                                                                            ╠╣";
const char G_FRAME_F1[] = "╠╬╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╬╣";
const char G_FRAME_F2[] = "╚╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╝";

#define TITLE_START 8
#define TITLE_TICK 8
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

#define ENEMY_SIZE 10
#define ENEMY_W 6
#define ENEMY_H 3
#define ENEMY_CD 1
#define ENEMY_STB 10
#define ENEMY_LIFE 2
const char *const ENEMY_FRAME[] = {" ,--. ", "| oo |", "|/\\/\\|"};

enum Direction {
  STATIONARY,
  DOWN,
  UP,
  LEFT,
  RIGHT,
  NO_DIR,
};

enum ObjectType {
  TANK,
  ENEMY,
  BULLET,
};

struct Enemy {
  enum ObjectType ot;
  int id;
  int row;
  int col;
  int life;
  enum Direction dir;
  int cd;
};

#define TANK_W 5
#define TANK_H 3
#define TANK_CD 3
#define TANK_CHAR 126
const char *const TANK_FRAME_U[] = {"  @  ", "+|-|+", "+   +"};
const char *const TANK_FRAME_D[] = {"+   +", "+|-|+", "  @  "};
const char *const TANK_FRAME_L[] = {"  + +", "@|-| ", "  + +"};
const char *const TANK_FRAME_R[] = {"+ +  ", " |-|@", "+ +  "};

struct Tank {
  enum ObjectType ot;
  int row;
  int col;
  int cd;
  enum Direction dir;
  int move;
};

#define BULLET_LIMIT 4
#define BULLET_CHAR '*'

struct Bullet {
  enum ObjectType ot;
  int active;
  int row;
  int col;
  enum Direction dir;
};

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
void frame_pause() {
  /* Clear input buffer. */
  read(STDIN_FILENO, VOID_BUF, IN_BUF_LEN - 1);
  usleep(1000000 / FPS);
}

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
void run_title_screen(const char *const text[]) {
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
    if (tick % TITLE_TICK == 0) {
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
          G_STATE = ENDED;
          return;
        case 'e':
          G_STATE = PLAYING;
          return;
        default:
          continue;
      }
    }
    render();
    frame_pause();
  }
}

/* Randomly selects a new direction different from the current direction. */
enum Direction get_rand_dir(enum Direction cur_dir) {
  enum Direction dir = cur_dir;
  while (dir == cur_dir) {
    /* Any valid direction. */
    dir = rand() % 5;
  }
  return dir;
}

/* Draws the game object by filling the memory with appropriate data. */
void draw_object(void *obj) {
  enum ObjectType ot = *(enum ObjectType *)obj;
  if (ot == ENEMY) {
    if (((struct Enemy *)obj)->life <= 0) {
      return;
    }
    int row = ((struct Enemy *)obj)->row;
    int col = ((struct Enemy *)obj)->col;
    int di = 0;
    for (; di < ENEMY_H; ++di) {
      memcpy(SCREEN[row + di] + col, ENEMY_FRAME[di], ENEMY_W);
    }
  } else if (ot == TANK) {
    int row = ((struct Tank *)obj)->row;
    int col = ((struct Tank *)obj)->col;
    int di = 0;
    switch (((struct Tank *)obj)->dir) {
      case UP:
        for (; di < TANK_H; ++di) {
          memcpy(SCREEN[row + di] + col, TANK_FRAME_U[di], TANK_W);
        }
        break;
      case DOWN:
        for (; di < TANK_H; ++di) {
          memcpy(SCREEN[row + di] + col, TANK_FRAME_D[di], TANK_W);
        }
        break;
      case LEFT:
        for (; di < TANK_H; ++di) {
          memcpy(SCREEN[row + di] + col, TANK_FRAME_L[di], TANK_W);
        }
        break;
      case RIGHT:
        for (; di < TANK_H; ++di) {
          memcpy(SCREEN[row + di] + col, TANK_FRAME_R[di], TANK_W);
        }
        break;
      default:
        break;
    }
  } else if (ot == BULLET) {
    if (((struct Bullet *)obj)->active) {
      SCREEN[((struct Bullet *)obj)->row][((struct Bullet *)obj)->col] = BULLET_CHAR;
    }
  }
}

/* Performs an action on an active enemy. */
void enemy_act(struct Enemy *enemy) {
  if (enemy->life <= 0) {
    return;
  } else if (enemy->cd > 0) {
    --enemy->cd;
    return;
  }
  enemy->cd = ENEMY_CD;

  if (enemy->dir == STATIONARY) {
    enemy->dir = get_rand_dir(NO_DIR);
    return;
  }

  int collision = 0;
  int destroy = 0;
  if (enemy->dir == UP) {
    int c = enemy->col;
    for (; c < enemy->col + ENEMY_W; ++c) {
      char obj = SCREEN[enemy->row - 1][c];
      if (obj == TANK_CHAR) {
        /* Destroy the tank. */
        destroy = 1;
      } else if (obj != ' ') {
        collision = 1;
        break;
      }
    }
    if (!collision) {
      /* Fill the space with dummy values so no other objects will collide into it. */
      c = enemy->col;
      for (; c < enemy->col + ENEMY_W; ++c) {
        SCREEN[enemy->row - 1][c] = 'E';
      }
      /* Move up. */
      --enemy->row;
    }
  } else if (enemy->dir == DOWN) {
    int c = enemy->col;
    for (; c < enemy->col + ENEMY_W; ++c) {
      char obj = SCREEN[enemy->row + ENEMY_H][c];
      if (obj == TANK_CHAR) {
        destroy = 1;
      } else if (obj != ' ') {
        collision = 1;
        break;
      }
    }
    if (!collision) {
      c = enemy->col;
      for (; c < enemy->col + ENEMY_W; ++c) {
        SCREEN[enemy->row + ENEMY_H][c] = 'E';
      }
      ++enemy->row;
    }
  } else if (enemy->dir == LEFT) {
    int r = enemy->row;
    for (; r < enemy->row + ENEMY_H; ++r) {
      char obj = SCREEN[r][enemy->col - 1];
      if (obj == TANK_CHAR) {
        destroy = 1;
      } else if (obj != ' ') {
        collision = 1;
        break;
      }
    }
    if (!collision) {
      r = enemy->row;
      for (; r < enemy->row + ENEMY_H; ++r) {
        SCREEN[r][enemy->col - 1] = 'E';
      }
      --enemy->col;
    }
  } else if (enemy->dir == RIGHT) {
    int r = enemy->row;
    for (; r < enemy->row + ENEMY_H; ++r) {
      char obj = SCREEN[r][enemy->col + ENEMY_W];
      if (obj == TANK_CHAR) {
        destroy = 1;
      } else if (obj != ' ') {
        collision = 1;
        break;
      }
    }
    if (!collision) {
      r = enemy->row;
      for (; r < enemy->row + ENEMY_H; ++r) {
        SCREEN[r][enemy->col + ENEMY_W] = 'E';
      }
      ++enemy->col;
    }
  }

  if (!collision && destroy) {
    G_STATE = LOST;
    return;
  }

  if (collision || (rand() % ENEMY_STB) == 0) {
    enemy->dir = get_rand_dir(enemy->dir);
  }
}

/* Fills the enemy frame with dummy chars for destruction detection. */
void fill_enemy(struct Enemy *enemy) {
  if (enemy->life <= 0) {
    return;
  }
  int i = enemy->row;
  for (; i < enemy->row + ENEMY_H; ++i) {
    int j = enemy->col;
    for (; j < enemy->col + ENEMY_W; ++j) {
      SCREEN[i][j] = enemy->id;
    }
  }
}

/* Moves the tank based on instruction. */
void tank_move(struct Tank *tank) {
  if (!tank->move) {
    return;
  }
  tank->move = 0;

  if (tank->dir == UP) {
    /* Check boundary. */
    if (tank->row == SCREEN_TOP) {
      return;
    }
    int c = tank->col;
    for (; c < tank->col + TANK_W; ++c) {
      if (SCREEN[tank->row - 1][c] != ' ') {
        /* Hit an enemy. */
        G_STATE = LOST;
        return;
      }
    }
    --tank->row;
  } else if (tank->dir == DOWN) {
    if (tank->row == SCREEN_BOTTOM - TANK_H + 1) {
      return;
    }
    int c = tank->col;
    for (; c < tank->col + TANK_W; ++c) {
      if (SCREEN[tank->row + TANK_H][c] != ' ') {
        G_STATE = LOST;
        return;
      }
    }
    ++tank->row;
  } else if (tank->dir == LEFT) {
    if (tank->col == SCREEN_LEFT) {
      return;
    }
    int r = tank->row;
    for (; r < tank->row + TANK_H; ++r) {
      if (SCREEN[r][tank->col - 1] != ' ') {
        G_STATE = LOST;
        return;
      }
    }
    --tank->col;
  } else if (tank->dir == RIGHT) {
    if (tank->col == SCREEN_RIGHT - TANK_W + 1) {
      return;
    }
    int r = tank->row;
    for (; r < tank->row + TANK_H; ++r) {
      if (SCREEN[r][tank->col + TANK_W] != ' ') {
        G_STATE = LOST;
        return;
      }
    }
    ++tank->col;
  }
}

/* Fills the tank frame with dummy chars for collision detection. */
void fill_tank(struct Tank *tank) {
  int i = tank->row;
  for (; i < tank->row + TANK_H; ++i) {
    int j = tank->col;
    for (; j < tank->col + TANK_W; ++j) {
      SCREEN[i][j] = TANK_CHAR;
    }
  }
}

/* Fires a new bullet from the tank's position. */
void fire(struct Tank *tank, struct Bullet *bullets) {
  if (tank->cd > 0) {
    return;
  }
  tank->cd = TANK_CD;

  int i = 0;
  for (; i < BULLET_LIMIT; ++i) {
    if (!bullets[i].active) {
      bullets[i].active = 1;
      bullets[i].dir = tank->dir;
      switch (tank->dir) {
        case UP:
          bullets[i].row = tank->row - 1;
          bullets[i].col = tank->col + 2;
          break;
        case DOWN:
          bullets[i].row = tank->row + TANK_H;
          bullets[i].col = tank->col + 2;
          break;
        case LEFT:
          bullets[i].row = tank->row + 1;
          bullets[i].col = tank->col - 1;
          break;
        case RIGHT:
          bullets[i].row = tank->row + 1;
          bullets[i].col = tank->col + TANK_W;
          break;
        default:
          break;
      }
      break;
    }
  }
}

/* Performs action on a bullet object in each frame. */
void bullet_act(struct Bullet *bullet, struct Enemy *enemies) {
  if (!bullet->active) {
    return;
  }

  switch (bullet->dir) {
    case UP:
      --bullet->row;
      break;
    case DOWN:
      ++bullet->row;
      break;
    case LEFT:
      --bullet->col;
      break;
    case RIGHT:
      ++bullet->col;
      break;
    default:
      break;
  }

  int i = bullet->row;
  int j = bullet->col;
  if (i < SCREEN_TOP || i > SCREEN_BOTTOM || j < SCREEN_LEFT || j > SCREEN_RIGHT) {
    /* Bullet is out of bounds and destroyed. */
    bullet->active = 0;
    return;
  }

  if (SCREEN[i][j] != ' ') {
    /* Hit an enemy. */
    bullet->active = 0;
    --enemies[SCREEN[i][j] - 1].life;
  }
}

/* Runs the main game routine. */
void run_game() {
  /* Create enemies. */
  struct Enemy enemies[ENEMY_SIZE];
  int i = 0;
  for (; i < ENEMY_SIZE; ++i) {
    enemies[i].ot = ENEMY;
    enemies[i].id = i + 1;
    enemies[i].dir = STATIONARY;
    enemies[i].life = ENEMY_LIFE;
    enemies[i].cd = ENEMY_CD;
    enemies[i].col = SCREEN_LEFT + i * (ENEMY_W + 1);
    enemies[i].row = SCREEN_TOP + rand() % 10;
  }

  /* Create the player tank. */
  struct Tank tank = {
      .ot = TANK,
      .dir = UP,
      .cd = TANK_CD,
      .row = SCREEN_BOTTOM - TANK_H + 1,
      .col = SCREEN_RIGHT - TANK_W,
      .move = 0,
  };

  /* Initialize the bullet cabinet. */
  struct Bullet bullets[BULLET_LIMIT];
  for (i = 0; i < BULLET_LIMIT; ++i) {
    bullets[i].ot = BULLET;
    bullets[i].active = 0;
    bullets[i].dir = STATIONARY;
    bullets[i].row = 0;
    bullets[i].col = 0;
  }

  while (1) {
    switch (get_input_char()) {
      case 'q':
        G_STATE = ENDED;
        return;
      case 'k':
        fire(&tank, bullets);
        break;
      case 'w':
        if (tank.dir == UP) {
          tank.move = 1;
        } else {
          tank.dir = UP;
        }
        break;
      case 's':
        if (tank.dir == DOWN) {
          tank.move = 1;
        } else {
          tank.dir = DOWN;
        }
        break;
      case 'a':
        if (tank.dir == LEFT) {
          tank.move = 1;
        } else {
          tank.dir = LEFT;
        }
        break;
      case 'd':
        if (tank.dir == RIGHT) {
          tank.move = 1;
        } else {
          tank.dir = RIGHT;
        }
        break;
      default:
        tank.move = 0;
        break;
    }

    /* Logic to process objects. */
    for (i = 0; i < BULLET_LIMIT; ++i) {
      bullet_act(&bullets[i], enemies);
    }

    --tank.cd;
    tank_move(&tank);

    int win = 1;
    for (i = 0; i < ENEMY_SIZE; ++i) {
      enemy_act(&enemies[i]);
      if (enemies[i].life > 0) {
        win = 0;
      }
    }

    if (win) {
      G_STATE = WON;
    }

    if (G_STATE != PLAYING) {
      return;
    }

    /* Draw all objects. */
    screen_fill_def();
    draw_object(&tank);
    for (i = 0; i < ENEMY_SIZE; ++i) {
      draw_object(&enemies[i]);
    }
    for (i = 0; i < BULLET_LIMIT; ++i) {
      draw_object(&bullets[i]);
    }

    render();

    /* Fill in dummy values for detecting object collision. */
    fill_tank(&tank);
    for (i = 0; i < ENEMY_SIZE; ++i) {
      fill_enemy(&enemies[i]);
    }
    frame_pause();
  }
}

int main(int argc, char const *argv[]) {
  /* Test code ONLY. */

  /* Save the terminal state. */
  tcgetattr(STDIN_FILENO, &saved_term);
  set_non_canonical_mode();
  /* TURN OFF STDOUT BUFFER: setbuf(stdout, NULL); */
  srand(time(NULL));

  /* Initialize memory for the screen. */
  int i = 0;
  for (; i < SCREEN_H; ++i) {
    SCREEN[i] = calloc(SCREEN_W + 1, sizeof(BORDER_SIZE));
  }

  while (G_STATE != ENDED && G_STATE != PLAYING) {
    screen_fill_def();
    render();
    switch (G_STATE) {
      case WON:
        run_title_screen(G_FRAME_WIN);
        break;
      case LOST:
        run_title_screen(G_FRAME_LOSS);
        break;
      default:
        run_title_screen(G_FRAME_TITLE);
    }

    if (G_STATE == PLAYING) {
      run_game();
    } else {
      break;
    }
  }

  printf("Quitting...\n");
  for (i = 0; i < SCREEN_H; ++i) {
    free(SCREEN[i]);
  }

  /* Restore the terminal to normal mode. */
  tcsetattr(STDIN_FILENO, TCSANOW, &saved_term);
  return 0;
}
