/*
 * Header file that provides operating system specific definitions for IO
 * functions, for clearing the terminal and getting key presses.
 */

#ifndef IO_SP_DEF_HPP
#define IO_SP_DEF_HPP

#ifdef __linux__
  #include <curses.h>

  #define CURSES_INIT initscr(); cbreak(); noecho(); keypad(stdscr, TRUE)
  #define CURSES_END endwin(); exit_curses(0)
  #define CLEAR_SCREEN wclear(stdscr)

  #define print(str) wprintw(stdscr, ((std::string)str).c_str()); wrefresh(stdscr)

  #define GETCH_ESC 224

#else
  #include <conio.h>

  #define CURSES_INIT
  #define CURSES_END
  #define CLEAR_SCREEN system("CLS")

  #define print(str) std::cout << str;

  #define GETCH_ESC 224
  #define KEY_UP GETCH_ESC * 72
  #define KEY_DOWN GETCH_ESC * 80
  #define KEY_RIGHT GETCH_ESC * 77
  #define KEY_LEFT GETCH_ESC * 75

#endif

/*
 * Function: getchEsc
 * Description: If the escape sequence is detected, queries getch() again for
 *    the second code. Useful for the Windows implementation of getch().
 * Returns (int): The result of getch(), or if the escape key was detected
 *    returns the escape key value times the result of the second getch().
 */
int getchEsc();

#endif
