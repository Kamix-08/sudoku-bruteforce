#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <utility>
#include <stack>
#include <unordered_set>
#include <ctime>

void print_board(bool add=false, bool pt=true);
void print_time();
void process_inputs();
bool verify(std::pair<int, int> pos);
bool process_flags(int argc, char* argv[]);

HANDLE hConsole;
CONSOLE_CURSOR_INFO cursor_info;

std::vector<std::vector<char>> board;
std::vector<std::pair<int, int>> fixed;

bool running = true;
std::pair<int, int> _pos = std::make_pair(0,0);

HANDLE hInput;

bool change = true;

std::stack<std::pair<int, int>> empty;
std::stack<std::pair<int, int>> answers;

std::time_t time_start;

bool print = true;
bool display = true;

int main(int argc, char* argv[])
{
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    hInput = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleCursorInfo(hConsole, &cursor_info);

    SetConsoleMode(hConsole, ENABLE_WINDOW_INPUT | ENABLE_PROCESSED_INPUT);
    cursor_info.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursor_info);

    board = std::vector<std::vector<char>>(9, std::vector<char>(9, '0'));

    if(!process_flags(argc, argv))
        return 0;

    while(running)
    {
        if(change)
        {
            system("cls");
            print_board(false, false);
        }

        process_inputs();
    }

    time_start = std::time(nullptr);
    
    print_board(true);
    print = display;

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

            if(print)
                system("cls");

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

            return 0;
        }

        empty.push(answers.top());
        answers.pop();
    }

    print = true;

    system("cls");
    print_board();

    return 0;
}

void print_board(bool add, bool pt)
{
    if(!print)
        return;

    bool c = false;
    bool d = false;
    char f;
    int ptr = 0;

    for(int y=0; y<9; y++)
    {
        for(int x=0; x<9; x++)
        {
            if(x == 3 || x == 6)
                std::cout << "| ";

            c = (y == _pos.first && x == _pos.second);
            
            if(c)
                SetConsoleTextAttribute(hConsole, 0xB);
            else
            {
                if(ptr < fixed.size() && (y == fixed[ptr].first && x == fixed[ptr].second))
                {
                    SetConsoleTextAttribute(hConsole, 0x5);
                    ++ptr;
                    d = true;
                }
            }

            f = board[y][x];
            std::cout << (f == '0' ? '.' : f) << ' ';

            if(add)
            {
                if(f == '0')
                    empty.emplace(y,x);
                else
                    fixed.emplace_back(y,x);
            }

            if(c || d)
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

    if(pt)
        print_time();
}

void print_time()
{
    std::time_t t = std::time(nullptr) - time_start;

    std::cout << "\nTime elapsed: ";

    // i know it's bad, but it's not THAT bad 
    // right?
    if(t >= 3600)
        std::cout << t/3660 << "h ";

    if(t >= 60)
        std::cout << (t/60)%3600 << "m ";
    std::cout << t%60 << "s";
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
    DWORD events;
    INPUT_RECORD input_record;

    change = false;
    ReadConsoleInput(hInput, &input_record, 1, &events);

    if(input_record.EventType != KEY_EVENT)
        return;

    KEY_EVENT_RECORD key_event = input_record.Event.KeyEvent;

    if(!key_event.bKeyDown)
        return;


    CHAR ascii = key_event.uChar.AsciiChar;
    if(ascii >= '0' && ascii <= '9')
    {
        change = true;  
        board[_pos.first][_pos.second] = ascii;
        return;
    }

    if(ascii >= 32)
    {
        change = true;  
        board[_pos.first][_pos.second] = '0';
        return;
    }

    switch (key_event.wVirtualKeyCode)
    {
    case VK_UP:
        modif(_pos.first, false); break;
    case VK_DOWN:
        modif(_pos.first, true);  break;
    case VK_LEFT:
        modif(_pos.second, false);  break;
    case VK_RIGHT:
        modif(_pos.second, true);   break;
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

bool process_flags(int argc, char* argv[])
{
    if(argc == 1)
        return true;

    // -- "-h" flag --
    for(int i=1; i<argc; i++)
    {
        if(_stricmp(argv[i], "-h") == 0)
        {
            std::cout << "\nsudoku-bruteforce\n"
                      << "-----------------\n\n"
                      << "Usage:\n"
                      << "\t./main [-h] [-s] [-b <board>]\n\n"
                      << "Options:\n"
                      << "\t-h           Show this help message and exit\n"
                      << "\t-n           Don't print the board while bruteforcing\n"
                      << "\t-s           Start the program without prompting for user input\n"
                      << "\t-b <board>   Setup the board with the specified 81 characters\n\n"
                      << "Examples:\n"
                      << "\t./main -s -b ..43..2.9..5..9..1.7..6..43..6..2.8719...74...5..83...6.....1.5..35.869..4291.3..\n"
                      << "\t\tSolves the sudoku board passed as a string.\n\n";

            return false;
        }
    }

    // -- "-s" flag --
    for(int i=1; i<argc; i++)
    {
        if(_stricmp(argv[i], "-s") == 0)
        {
            running = false;
            break;
        }
    }

    // -- "-b" flag --
    for(int i=1; i<argc; i++)
    {
        if(_stricmp(argv[i], "-b") == 0)
        {
            if(i == argc-1)
            {
                SetConsoleTextAttribute(hConsole, 0x4);
                std::cout << "[x] The -b flag cannot be the last argument.\n";
                SetConsoleTextAttribute(hConsole, 0xF);

                return false;
            }

            std::string b = argv[i+1];

            if(b.length() != 81)
            {
                SetConsoleTextAttribute(hConsole, 0x4);
                std::cout << "[x] The argument directly after the -b flag has to be exactly 81 characters.\n";
                SetConsoleTextAttribute(hConsole, 0xF);

                return false;
            }

            for(int y=0; y<9; y++)
            {
                for(int x=0; x<9; x++)
                {
                    char c = b[y*9 + x];
                    board[y][x] = (c >= '0' && c <= '9' ? c : '0');
                }
            }

            break;
        }
    }

    // -- "-n" flag --
    for(int i=1; i<argc; i++)
    {
        if(_stricmp(argv[i], "-n") == 0)
        {
            display = false;
            break;
        }
    }

    return true;
}   