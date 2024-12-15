#include <ncurses.h>

#include "tetris.h"
#include "time.h"

void InitNCurses()
{
    initscr();              // Start curses mode
    noecho();               // Disable user input echo
    cbreak();               // Disable line buffering (get user input ASAP instead of waiting for newline)
    keypad(stdscr, true);   // Enable function keys
    curs_set(false);        // Hide cursor
}

WINDOW *CreateTetrisWin(int starty, int startx)
{
    auto *win = newwin(Tetris::Board::HEIGHT + 2, Tetris::Board::WIDTH + 2, starty, startx);
    box(win, 0, 0); // Use default chars for vertical & horizontal lines
    return win;
}

void DestroyWin(WINDOW *win)
{
    wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '); // Clear all borders
    wrefresh(win);
    delwin(win);
}

int main()
{
    InitNCurses();

    mvprintw(0, 0, "Score: ");

    Tetris::Board tetris;
    auto *tetWin = CreateTetrisWin(1, 0);

    Time time;
    float dtAccum = 0.0f;
    constexpr auto INTERVAL = 0.5f; // Game step interval

    time.Start();
    while (!tetris.GameOver())
    {
        dtAccum += time.Lap();

        // TODO: Get user input

        while (dtAccum >= INTERVAL)
        {
            // TODO: Perform simulation step
            dtAccum -= INTERVAL;
        }

        mvprintw(0, 7, "%u", tetris.Score());
        tetris.PrintBoard(tetWin);

        refresh();
        wrefresh(tetWin);
    }

    DestroyWin(tetWin);
    endwin(); // End curses mode
}
