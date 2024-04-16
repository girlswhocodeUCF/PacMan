

#include <stdio.h>
#include <stdlib.h>
// Colors
#define BLK "\e[0;30m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[1;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"
#define WHT "\e[0;37m"
#define BLKB "\e[40m"
#define REDB "\e[41m"
#define GRNB "\e[42m"
#define YELB "\e[43m"
#define BLUB "\e[44m"
#define MAGB "\e[45m"
#define CYNB "\e[46m"
#define WHTB "\e[47m"
#define YELBLINK "\e[5;33m"

int step_count = 0;
int score = 0;
int dots_count;
int isTaken1 = 0;
int isTaken2 = 0;

struct character {
  int x, y;
  int powerUp;
};

void printColors(char c) {
  switch (c) {
  case '#':
    printf(BLU "%c", c);
    break;
  case '.':
    printf(MAG "%c", c);
    break;
  case 'C':
    printf(YEL "%c", c);
    break;
  case 'G':
    printf(RED "%c", c);
    break;
  case 'o':
    printf(WHT "%c", c);
    break;
  case 'X':
    printf(YEL "%c", c);
    break;
  case ' ':
    printf(" ");
    break;
  }
}

void updatePosition(char move, struct character *pacMan, char layout[10][15],
                    int visited[10][15]) {

  switch (move) {
  case 'W':
  case 'w':
    step_count++;
    if (layout[pacMan->x - 1][pacMan->y] != '#') {
      pacMan->x--;
      visited[pacMan->x][pacMan->y] = 1;
    }
    break;

  case 'A':
  case 'a':
    step_count++;
    if (layout[pacMan->x][pacMan->y - 1] != '#') {
      pacMan->y--;
      visited[pacMan->x][pacMan->y] = 1;
    }
    break;

  case 'S':
  case 's':
    step_count++;
    if (layout[pacMan->x + 1][pacMan->y] != '#') {
      pacMan->x++;
      visited[pacMan->x][pacMan->y] = 1;
    }
    break;

  case 'D':
  case 'd':
    step_count++;
    if (layout[pacMan->x][pacMan->y + 1] != '#') {
      pacMan->y++;
      visited[pacMan->x][pacMan->y] = 1;
    }
    break;
  }
}

void updateGhosts(struct character *ghost, struct character *pacMan,
                  char layout[10][15], int poweredUp) {
  int direction = rand() % 4;

  // if pacman within 3x3 of ghost, move in pacmans direction
  if (pacMan->powerUp != 1) {
    if (ghost->x - 3 == pacMan->x) { // up check
      direction = 0;
    }
    if (ghost->x + 3 == pacMan->x) { // down check
      direction = 2;
    }
    if (ghost->y - 3 == pacMan->y) { // left check
      direction = 1;
    }
    if (ghost->y + 3 == pacMan->y) { // right check
      direction = 3;
    }
  }

  // if pacman is powered up (within 2x2), move away
  if (pacMan->powerUp == 1) {
    if (ghost->x - 2 == pacMan->x) { // up check
      direction = 2;
    }                                // go down
    if (ghost->x + 2 == pacMan->x) { // down check
      direction = 0;
    }                                // go up
    if (ghost->y - 2 == pacMan->y) { // left check
      direction = 3;
    }                                // go right
    if (ghost->y + 2 == pacMan->y) { // right check
      direction = 1;
    } // go left
  }

  switch (direction) {
  case 0: // up
    if (layout[ghost->x - 1][ghost->y] != '#' &&
        layout[ghost->x - 1][ghost->y] != 'G') {
      ghost->x--;
    }
    break;
  case 1: // left
    if (layout[ghost->x][ghost->y - 1] != '#' &&
        layout[ghost->x][ghost->y - 1] != 'G') {
      ghost->y--;
    }
    break;
  case 2: // down
    if (layout[ghost->x + 1][ghost->y] != '#' &&
        layout[ghost->x + 1][ghost->y] != 'G') {
      ghost->x++;
    }
    break;
  case 3: // right
    if (layout[ghost->x][ghost->y + 1] != '#' &&
        layout[ghost->x][ghost->y + 1] != 'G') {
      ghost->y++;
    }
    break;
  }
}

void display(char layout[10][15], struct character *pacMan,
             struct character *ghost1, struct character *ghost2,
             int visited[10][15], int *gameover) {
  system("clear");
  int num = 0;

  // i = y value, j = x value

  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 15; j++) {
      if (i == 0 || i == 9 || j == 0 || j == 14) {
        layout[i][j] = '#';
      } else if ((i == 3 && j == 5) || (i == 4 && j == 5) ||
                 (i == 5 && j == 5) || (i == 5 && j == 7) ||
                 (i == 5 && j == 8) || (i == 5 && j == 9) ||
                 (i == 4 && j == 9) || (i == 3 && j == 9) ||
                 (i == 5 && j == 6) || (i == 2 && j == 2) ||
                 (i == 2 && j == 2) || (i == 2 && j == 3) ||
                 (i == 5 && j == 2) || (i == 5 && j == 3) ||
                 (i == 6 && j == 3) || (i == 7 && j == 3) ||
                 (i == 1 && j == 7) || (i == 8 && j == 7) ||
                 (i == 2 && j == 11) || (i == 2 && j == 12) ||
                 (i == 5 && j == 11) || (i == 5 && j == 12) ||
                 (i == 6 && j == 11) || (i == 7 && j == 11)) {

        layout[i][j] = '#';
      } else if (i == pacMan->x && j == pacMan->y) {
        if (pacMan->powerUp == 1) {
          layout[i][j] = 'X';
        } else {
          layout[i][j] = 'C';
        }
      }

      else if (i == ghost1->x && j == ghost1->y ||
               i == ghost2->x && j == ghost2->y) {
        layout[i][j] = 'G'; //
      }

      else if ((i == 2 && j == 5) || (i == 2 && j == 6) || (i == 2 && j == 7) ||
               (i == 2 && j == 8) || (i == 2 && j == 9) || (i == 3 && j == 6) ||
               (i == 3 && j == 7) || (i == 3 && j == 8) || (i == 4 && j == 7) ||
               (i == 7 && j == 7) || (i == 4 && j == 6) || (i == 4 && j == 8)) {
        layout[i][j] = ' ';
      }

      else if (visited[i][j] == 1) {
        layout[i][j] = ' ';
      }
      // If the ghost reaches pacman and its not powered up, then game over

      // If the ghost reaches pacman and is powered up, then eat ghost
      else if ((ghost1->x == pacMan->x) && (ghost1->y == pacMan->y) &&
               (pacMan->powerUp == 1)) {
        score += 500;
        ghost1->x = 4;
        ghost1->y = 6;
        layout[ghost1->x][ghost1->y] = 'G';
      } else if (ghost2->x == pacMan->x && ghost2->y == pacMan->y &&
                 pacMan->powerUp == 1) {
        score += 500;
        ghost2->x = 4;
        ghost2->y = 8;
        layout[ghost2->x][ghost2->y] = 'G';
      }

      else if ((ghost1->x == pacMan->x) && (ghost1->y == pacMan->y) &&
               (pacMan->powerUp != 1)) {
        *gameover = 1;
      } else if ((ghost2->x == pacMan->x) && (ghost2->y == pacMan->y) &&
                 (pacMan->powerUp != 1)) {
        *gameover = 1;
      }

      else if ((i == 1 && j == 1) || (i == 8 && j == 13)) {
        layout[i][j] = 'o';
      }

      else {
        layout[i][j] = '.';
      }

      printColors(layout[i][j]);
    }
    printf("\n");
  }

  dots_count = 0;
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 15; j++) {
      if (layout[i][j] == '.') {
        dots_count++;
      }
    }
  }

  if (dots_count == 0) {
    *gameover = 2;
  }
}

int calculateScore(int dot_count) {

  int eaten = 67 - dots_count;
  eaten *= 50;

  score += eaten;

  return score;
}

int main(void) {
  char layout[10][15];
  int visited[10][15];
  int gameover = 0;

  struct character pacMan;
  struct character ghost1;
  struct character ghost2;
  char move;
  pacMan.x = 7;
  pacMan.y = 7;
  pacMan.powerUp = 0;
  ghost1.x = 4;
  ghost1.y = 6;
  ghost1.powerUp = 0;
  ghost2.x = 4;
  ghost2.y = 8;
  ghost2.powerUp = 0;

  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 15; j++) {
      visited[i][j] = 0;
    }
  }

  display(layout, &pacMan, &ghost1, &ghost2, visited, &gameover);
  while (gameover == 0) {
    scanf("%c", &move);
    updatePosition(move, &pacMan, layout, visited);
    updateGhosts(&ghost1, &pacMan, layout, 0);
    updateGhosts(&ghost2, &pacMan, layout, 0);
    display(layout, &pacMan, &ghost1, &ghost2, visited, &gameover);

    if (visited[1][1] == 1 && pacMan.x == 1 && pacMan.y == 1 && isTaken1 == 0) {
      pacMan.powerUp = 1;
      step_count = 0;
      isTaken1 = 1;
      score += 300;
    }

    if (visited[8][13] == 1 && pacMan.x == 8 && pacMan.y == 13 &&
        isTaken2 == 0) {
      pacMan.powerUp = 1;
      step_count = 0;
      isTaken2 = 1;
      score += 300;
    }

    if (step_count == 30 && pacMan.powerUp == 1) {
      pacMan.powerUp = 0;
      step_count = 0;
    }
    if (move == 'Q') {
      gameover = 1;
    }
  }

  if (gameover == 1) {
    printf("GAME OVER\n");
  } else if (gameover == 2) {
    printf("YOU WIN!\n");
  }

  printf("Score: %d\n", calculateScore(dots_count));
  return 0;
}
