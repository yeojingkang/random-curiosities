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
    auto *win = newwin(Tetris::HEIGHT + 2, Tetris::WIDTH + 2, starty, startx);
    box(win, 0, 0); // Use default chars for vertical & horizontal lines
    return win;
}

WINDOW *CreateGameOverWin(int starty, int startx)
{
    auto *win = newwin(3, 14, starty, startx);
    box(win, 0, 0);
    mvwprintw(win, 1, 2, "Game Over!");
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

    Tetris::Instance tetris;
    auto *tetWin = CreateTetrisWin(1, 0);
    auto *gameOverWin = CreateGameOverWin(11, 5);

    bool gameOpen = true;

    Time time;
    float dtAccum = 0.0f;
    constexpr auto INTERVAL = 0.5f; // Game step interval

    time.Start();
    while (gameOpen)
    {
        dtAccum += time.Lap();

        // Input
        timeout(1); // Block and wait for input for 1ms
        switch (getch())
        {
        case 'd':
        case KEY_RIGHT:
            tetris.MovePlayerRight();
            break;
        case 'a':
        case KEY_LEFT:
            tetris.MovePlayerLeft();
            break;
        case 's':
        case KEY_DOWN:
            tetris.MovePlayerDown();
            break;
        case 'w':
        case KEY_UP:
            tetris.RotatePlayerCW();
            break;
        }

        // Logic
        while (dtAccum >= INTERVAL)
        {
            tetris.Step();
            dtAccum -= INTERVAL;
        }

        // Render
        mvprintw(0, 7, "%u", tetris.Score());
        tetris.PrintBoard(tetWin);

        refresh();
        if (tetris.GameOver())
            wrefresh(gameOverWin);
        else
            wrefresh(tetWin);
    }

    DestroyWin(tetWin);
    endwin(); // End curses mode
}
