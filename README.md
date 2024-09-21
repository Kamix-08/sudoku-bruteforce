# sudoku-bruteforce

![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Windows](https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)

Simple C++ script to simulate bruteforcing a sudoku puzzle in real time.

<br>

# the plan

- [x] create a functional version
- [x] add a timer
- [x] improve graphical representation
- [ ] add replay speed customization options
- [ ] add other input options
- [ ] make the code compatible with Linux and MacOS

_(all pull requests are welcome)_

<br>

# installation and execution

To install the script, run

```bash
git clone https://github.com/Kamix-08/sudoku-bruteforce
cd sudoku-bruteforce
```

<br>

To run the program, use the complier of your choice to compile the `main.cpp` file into an executable.

_(an example using `gcc`)_

```bash
g++ -o main main.cpp
./main
```

<br>

## using the program

After executing the program, you will be prompted with an empty sudoku board:

![1](https://github.com/user-attachments/assets/5add96c3-4f42-4fab-905a-7129bc2790ff)

<br>

You can use the arrow keys to move your pointer (highlighted in light blue), and the number keys to insert values.

![2](https://github.com/user-attachments/assets/cbe766b2-a8a8-496c-88cc-18d8213c174b)

<br>

To remove the value in a cell, press `0`.

After filling in the board, press `Enter` to let the code run.

<br>

After some time, the solution will be found:

![3](https://github.com/user-attachments/assets/a7be3dbe-b6a1-4b0c-805c-69697a33730b)

or in case of a failure, the following message will be displayed:

    [x] Failed to find the solution...
