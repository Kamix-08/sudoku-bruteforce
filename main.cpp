#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <utility>
#include <stack>
#include <unordered_set>
#include <ctime>

void print_board(bool add=false);
void process_inputs();
bool verify(std::pair<int, int> pos);

HANDLE hConsole;
std::vector<std::vector<char>> board;

bool running = true;
std::pair<int, int> _pos = std::make_pair(0,0);

HANDLE hInput;
DWORD events;
INPUT_RECORD input_record;
KEY_EVENT_RECORD key_event;
CHAR ascii;

bool change = true;

std::stack<std::pair<int, int>> empty;
std::stack<std::pair<int, int>> answers;

bool display_time = false;
std::time_t time_start;

int main()
{
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    hInput = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(hConsole, ENABLE_WINDOW_INPUT | ENABLE_PROCESSED_INPUT);

    board = std::vector<std::vector<char>>(9, std::vector<char>(9, '0'));

    while(running)
    {
        if(change)
            print_board();

        process_inputs();
    }

    time_start = std::time(nullptr);
    display_time = true;
    
    print_board(true);

    char counter = '0';
    std::pair<int, int> pos;
    while(!empty.empty())
    {
        _pos = pos = empty.top();

        if(counter > '9')
            counter = board[pos.first][pos.second];
        else
            counter = '0';

        do
        {
            if(++counter > '9')
                break;
            
            board[pos.first][pos.second] = counter;
            print_board();

        } while (!verify(pos));

        if(counter <= '9')
        {
            answers.push(pos);
            empty.pop();
            
            continue;
        }

        board[pos.first][pos.second] = '0';

        if(answers.empty())
        {
            SetConsoleTextAttribute(hConsole, 0x4);
            std::cout << "[x] Failed to find the solution...\n";
            SetConsoleTextAttribute(hConsole, 0xF);

            return 1;
        }

        empty.push(answers.top());
        answers.pop();
    }

    print_board();

    return 0;
}

void print_board(bool add)
{
    system("cls");

    bool c = false;
    char f;

    for(int y=0; y<9; y++)
    {
        for(int x=0; x<9; x++)
        {
            if(x == 3 || x == 6)
                std::cout << "| ";

            c = (y == _pos.second && x == _pos.first);
            if(c)
                SetConsoleTextAttribute(hConsole, 0xB);

            f = board[y][x];
            std::cout << (f == '0' ? '.' : f) << ' ';

            if(add && f == '0')
                empty.emplace(y,x);

            if(c)
                SetConsoleTextAttribute(hConsole, 0xF);
        }

        std::cout << '\n';

        if(y == 2 || y == 5)
        {
            for(int i=0; i<21; i++)
                std::cout << '-';

            std::cout << '\n';
        }
    }

    if(display_time)
        std::cout << "\nTime elapsed: " << std::time(nullptr) - time_start << "s";
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
        board[_pos.second][_pos.first] = ascii;
        return;
    }

    if(ascii >= 32)
    {
        change = true;  
        board[_pos.second][_pos.first] = '0';
        return;
    }

    switch (key_event.wVirtualKeyCode)
    {
    case VK_UP:
        modif(_pos.second, false); break;
    case VK_DOWN:
        modif(_pos.second, true);  break;
    case VK_LEFT:
        modif(_pos.first, false);  break;
    case VK_RIGHT:
        modif(_pos.first, true);   break;
    default:
        running = false;
        break;
    }   
}

bool verify_row(int y)
{
    std::unordered_set<char> values;
    int s;

    for(char& v : board[y])
    {
        if(v == '0')
            continue;

        s = values.size();
        values.emplace(v);

        if(s == values.size())
            return false;
    }

    return true;
}

bool verify_col(int x)
{
    std::unordered_set<char> values;
    int s;

    for(const std::vector<char>& row : board)
    {
        if(row[x] == '0')
            continue;

        s = values.size();
        values.emplace(row[x]);

        if(s == values.size())
            return false;
    }

    return true;
}

std::pair<int, int> get_square(std::pair<int, int> pos)
{
    return std::make_pair(pos.first/3, pos.second/3);
}

bool verify_square(std::pair<int, int> square)
{
    std::unordered_set<char> values;
    int s;

    square.first  *= 3;
    square.second *= 3;

    for(int y=square.first; y<square.first+3; y++)
    {
        for(int x=square.second; x<square.second+3; x++)
        {
            if(board[y][x] == '0')
                continue;

            s = values.size();
            values.emplace(board[y][x]);

            if(s == values.size())
                return false;
        }
    }

    return true;
}

bool verify(std::pair<int, int> pos)
{
    return verify_row(pos.first) && verify_col(pos.second) && verify_square(get_square(pos));
}