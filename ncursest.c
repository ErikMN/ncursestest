#include <stdio.h>
#include <ncurses.h>

typedef struct _win_border_struct {
  chtype   ls, rs, ts, bs, tl, tr, bl, br;
} WIN_BORDER;

typedef struct _WIN_struct {
  int startx, starty;
  int height, width;
  WIN_BORDER border;
} WIN;

void init(WIN *p_win);
void draw_box(WIN *win, bool flag);

bool pen = false;

void
draw_box(WIN *p_win, bool flag)
{
  int i, j;
  int x, y, w, h;

  x = p_win->startx;
  y = p_win->starty;
  w = p_win->width;
  h = p_win->height;

  // draw box: mvaddch(row,col,ch);
  if (flag == TRUE) {
    attron(COLOR_PAIR(2));
    mvaddch(y, x, p_win->border.tl); // top left
    mvaddch(y, x + w, p_win->border.tr); // top right
    mvaddch(y + h, x, p_win->border.bl); // bottom left
    mvaddch(y + h, x + w, p_win->border.br); // bottom right
    mvhline(y, x + 1, p_win->border.ts, w - 1); // top side
    mvhline(y + h, x + 1, p_win->border.bs, w - 1); // bottom side
    mvvline(y + 1, x, p_win->border.ls, h - 1); // left side
    mvvline(y + 1, x + w, p_win->border.rs, h - 1); // rigth side
    attroff(COLOR_PAIR(2));
  } else {
    // clean up after the box:
    for (j = y; j <= y + h; ++j) {
      for (i = x; i <= x + w; ++i) {
        if (pen) {
          attron(COLOR_PAIR(3));
          mvaddch(j, i, ' ');
          attroff(COLOR_PAIR(3));
        } else {
          mvaddch(j, i, ' ');
        }
      }
    }
  }
  refresh();
}

// initialize the box:
void
init(WIN *p_win)
{
  p_win->height = 2;
  p_win->width = 5;
  p_win->starty = (LINES - p_win->height)/2;
  p_win->startx = (COLS - p_win->width)/2;

  // set the characters to draw the box with:
  p_win->border.ls = ' ';
  p_win->border.rs = ' ';
  p_win->border.ts = ' ';
  p_win->border.bs = ' ';
  p_win->border.tl = ' ';
  p_win->border.tr = ' ';
  p_win->border.bl = ' ';
  p_win->border.br = ' ';
}

int
main(int argc, char *argv[])
{
  (void)argc;
  (void)argv;
  WIN win;
  int ch;

  initscr(); // start curses mode

  if (has_colors() == FALSE) {
    endwin();
    printf("Your terminal does not support color\n");
    return -1;
  }

  start_color(); // start the color functionality
  cbreak(); // line buffering disabled, pass on evertything to me
  keypad(stdscr, TRUE); // to use F3
  noecho();
  init_pair(1, COLOR_RED, COLOR_BLACK); // text color
  init_pair(2, COLOR_RED, COLOR_RED); // box color
  init_pair(3, COLOR_BLUE, COLOR_BLUE); // path color

  init(&win); // initialize the window parameters

  attron(COLOR_PAIR(1));
  attron(A_BOLD | A_UNDERLINE);
  printw("Press F3 to exit\nPress Backspace to paint");
  attroff(A_BOLD | A_UNDERLINE);
  refresh();
  attroff(COLOR_PAIR(1));

  draw_box(&win, TRUE);
  while ((ch = getch()) != KEY_F(3)) {
    switch(ch)
    {
      case KEY_LEFT:
        draw_box(&win, FALSE);
        --win.startx;
        draw_box(&win, TRUE);
        break;
      case KEY_RIGHT:
        draw_box(&win, FALSE);
        ++win.startx;
        draw_box(&win, TRUE);
        break;
      case KEY_UP:
        draw_box(&win, FALSE);
        --win.starty;
        draw_box(&win, TRUE);
        break;
      case KEY_DOWN:
        draw_box(&win, FALSE);
        ++win.starty;
        draw_box(&win, TRUE);
        break;
      case KEY_BACKSPACE:
        if (pen)
          pen = false;
        else
          pen = true;
        break;
    }
  }
  endwin(); // end curses mode

  return 0;
}
