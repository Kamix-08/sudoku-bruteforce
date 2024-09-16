#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <utility>

void print_board();
void process_inputs();

HANDLE hConsole;
std::vector<std::vector<char>> board;

bool running = true;
std::pair<int, int> pos = std::make_pair(0,0);

HANDLE hInput;
DWORD events;
INPUT_RECORD input_record;
KEY_EVENT_RECORD key_event;
CHAR ascii;

bool change = true;

int main()
{
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    hInput = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(hConsole, ENABLE_WINDOW_INPUT | ENABLE_PROCESSED_INPUT);

    board = std::vector<std::vector<char>>(9, std::vector<char>(9, '.'));

    while(running)
    {
        if(change)
        {
            system("cls");
            print_board();
        }

        process_inputs();
    }

    return 0;
}

void print_board()
{
    bool c = false;
    for(int y=0; y<9; y++)
    {
        for(int x=0; x<9; x++)
        {
            if(x == 3 || x == 6)
                std::cout << "| ";

            c = (y == pos.second && x == pos.first);
            if(c)
                SetConsoleTextAttribute(hConsole, 11);

            std::cout << board[y][x] << ' ';

            if(c)
                SetConsoleTextAttribute(hConsole, 15);
        }

        std::cout << '\n';

        if(y == 2 || y == 5)
        {
            for(int i=0; i<21; i++)
                std::cout << '-';

            std::cout << '\n';
        }
    }
}

void modif(int& c, bool i)
{
    change = true;

    if(i)
        ++c %= 9;
    else
        c = (--c < 0 ? 9 + c : c);
}

void process_inputs()
{
    change = false;
    ReadConsoleInput(hInput, &input_record, 1, &events);

    if(input_record.EventType != KEY_EVENT)
        return;

    key_event = input_record.Event.KeyEvent;

    if(!key_event.bKeyDown)
        return;


    ascii = key_event.uChar.AsciiChar;
    if(ascii >= '0' && ascii <= '9')
    {
        change = true;  
        board[pos.second][pos.first] = (ascii == '0' ? '.' : ascii);
        return;
    }

    if(ascii >= 32)
    {
        change = true;  
        board[pos.second][pos.first] = '.';
        return;
    }

    switch (key_event.wVirtualKeyCode)
    {
    case VK_UP:
        modif(pos.second, false); break;
    case VK_DOWN:
        modif(pos.second, true);  break;
    case VK_LEFT:
        modif(pos.first, false);  break;
    case VK_RIGHT:
        modif(pos.first, true);   break;
    default:
        running = false;
        break;
    }   
}