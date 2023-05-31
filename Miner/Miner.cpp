#include <iostream>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE); //Дескриптор активного окна



enum Color { Black, Blue, Green, Cyan, Red, Magenta, Brown, LightGray, DarkGray, LightBlue = 9, LightGreen, LightCyan, LightRed, LightMagenta, Yellow, White, Orange };
enum Napravlenie { Up = 72, Left = 75, Right = 77, Down = 80, Enter = 13, escepushka = 27, BackSpace = 8, Q = 113, Space = 32 };


int const SIZE_x = 15, SIZE_y = 15; //размер поля

int random(int SIZE) {
    int r = rand() % SIZE;
    return r;
}
void setColor(Color text, Color background) //функция с помощью которой происходит изменение цвета текста
{
    SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text)); //установка параметров текста
}
void SetCursor(int x, int y) //функция для того чтобы устанавливать позицию курсора в консоли по оси Х и Y
{
    COORD myCoords = { x,y }; //инициализация координат
    SetConsoleCursorPosition(hStdOut, myCoords); //Способ перемещения курсора на нужные координаты
}


class Miner { //класс, в котором происходит обработка игрового поля
public:
    Miner(int Start_value) {
        for (int i = 0; i < SIZE_x * SIZE_y; i++) arr[i] = Start_value;
    }


    void Create_Field(int x, int y, int clr) { //метод, который выводит в консоль игровое поле
        int xo = x, yo = y;
        SetCursor(x, y);

        for (int j = 1; j <= (SIZE_y + 2); j++) {
            for (int i = 2; i <= 2 * (SIZE_x + 2); i += 2) {
                if ((i == 2) || (i == 2 * (SIZE_x + 2)) || (j == 1) || (j == (SIZE_y + 2))) {
                    setColor(Color(clr), Color(clr));
                    cout << "++";

                }
                else {
                    setColor(White, White);
                    if (get_value((i / 2) - 2, j - 2) >= 10) cout << "++";
                    else
                        if (get_value((i / 2) - 2, j - 2) == 0) {
                            setColor(LightGray, LightGray);
                            cout << "  ";
                        }
                        else
                            if (get_value((i / 2) - 2, j - 2) < 0) {


                                setColor(Magenta, Magenta);
                                cout << "++";
                            }
                            else {
                                setColor(Red, LightGray);
                                if (get_value((i / 2) - 2, j - 2) == 9)  cout << "Q ";
                                else {
                                    setColor(Black, LightGray);
                                    cout << get_value((i / 2) - 2, j - 2) << " ";
                                }
                            }

                }
            }
            y += 1;
            SetCursor(x, y);
        }
        if (clr != 1) {
            setColor(Black, Color(clr));
            SetCursor(xo, yo);
            cout << "осталось мин: " << count_bomb;
            SetCursor(x, y - 1);
        }
        setColor(White, Black);

    }
    void Open_All() { //метод, который открывает все ячейки, чтобы отрисовать поле в конце игры
        for (int i = 0; i < SIZE_x * SIZE_y; i++) {
            if (arr[i] < 0) arr[i] = -1 * arr[i];
            if (arr[i] >= 10) arr[i] -= 10;
        }

    }
    void PC_step() { //метод, обрабатывающий ход компьютера
        int min_numb = 0, min_mass = 10, r = random(10);
        Miner PC_Field_Mass(0);
        Miner PC_Field_Open(0);
        PC_Field_Mass.clear_all(0);
        Scan_Mass(PC_Field_Mass);
        Scan_Open(PC_Field_Open);


        for (int i = 0; i < SIZE_x * SIZE_y; i++) {
            if (((arr[i] != 19) || (r == 5)) && (PC_Field_Mass.arr[i] != 0) && (PC_Field_Open.arr[i] >= 3)) {
                min_mass = PC_Field_Mass.arr[i];
                min_numb = i;
            }

        }
        open(min_numb % SIZE_x, (min_numb - (min_numb % SIZE_x)) / SIZE_y);

    }
    void Hu_step(int& x, int& y, bool player, bool cnt_players) { //метод обрабатывающий ход человека
        int key = 0;
        bool f = true;
        while ((f)) {
            Create_cursor(x, y, player);
            key = _getch();
            SetCursor(x, y);
            setColor(White, White);
            if (get_value((x / 2) - 1, y - 1) >= 10) cout << "++";
            else
                if (get_value((x / 2) - 1, y - 1) == 0) {
                    setColor(LightGray, LightGray);
                    cout << "++";
                }
                else
                    if (get_value((x / 2) - 1, y - 1) < 0) {

                        setColor(Magenta, Magenta);
                        cout << "++";

                    }
                    else {
                        setColor(Black, LightGray);
                        cout << get_value((x / 2) - 1, y - 1) << " ";
                    }
            switch (key) //Выполняем различные функции в соответствии с нажатой клавишью
            {
            case Enter:
                if (get_value((x / 2) - 1, y - 1) >= 10) {
                    open((x / 2) - 1, y - 1);
                    f = false;
                }

                break;
            case Space:
                if (cnt_players) {
                    if (player)
                        Flag((x / 2) - 1, y - 1, 2);
                    else Flag((x / 2) - 1, y - 1, 3);
                }
                else Flag((x / 2) - 1, y - 1, 2);
                break;
            case Left: //если нажали "Стрелочка влево"
                if (x > 2) //позиция до которой разрешено перемещение в лево
                {
                    x -= 2; //меняем значение по Х на -2 так как одна клеточка в игре равна двум в консоли
                }
                break;
            case Right: //если нажали "Стрелочка вправо"


                if (x < (2 * SIZE_x)) // проверяем не достиг ли он позиции до которой ему можно передвигаться(чтобы он не вышел за границы поля)
                {
                    x += 2; //меняем значение переменной Х на +2 так как одна клеточка в игре равна двум в консоли
                }


                break; //закрыли этот кейс
            case Up: //если нажали "Стрелочка вверх"
                if (y > 1) //позиция до которой разрешено перемещение вверх
                {
                    y--; //меняем значение по Y на -1
                }
                break; //закрыли этот кейс
            case Down: //если нажали "Стрелочка вних"
                if (y < SIZE_y) //позиция до которой разрешено перемещение вверх
                {
                    y++; //меняем значение по Y на -1
                }


            }
            Create_cursor(x, y, player);
        }
    }
    bool End_game(bool& win) { //метод, который определяет закончилась игра или нет
        bool f = true;
        win = true;
        for (int i = 0; i < SIZE_x * SIZE_y; i++) {
            if ((arr[i] >= 10) && (arr[i] != 19)) {
                f = false;
            }
            if (arr[i] == 9) {
                win = false;
                return true;
            }
        }
        return f;
    }
    void set_bombs(int count) { //метод, который случайным образом раскладывает бомбы
        int x, y;
        count_bomb = count;
        while (count > 0) {

            x = random(SIZE_x);
            y = random(SIZE_y);
            if (!(get_value(x, y) == 19) && !((x == 6) && (y == 6)) && !((x == 0) && (y == 0))) {
                bomb(x, y);
                count -= 1;
            }
        }
    }
    void Create_cursor(int x, int y, bool player) { //метод, который выводит местоположение курсора в консоль
        SetCursor(x, y);
        if (player)
            setColor(LightGreen, LightGreen);
        else     setColor(LightBlue, LightBlue);
        if (get_value((x / 2) - 1, y - 1) >= 10) cout << "++";
        else
            if (get_value((x / 2) - 1, y - 1) <= 0) {
                if (player)
                    setColor(LightGreen, LightGreen);
                else     setColor(LightBlue, LightBlue);

                cout << "++";
            }
            else {
                if (player)
                    setColor(Black, LightGreen);
                else     setColor(Black, LightBlue);
                if (get_value((x / 2) - 1, y - 1) == 9) {
                    if (player)
                        setColor(Red, LightGreen);
                    else     setColor(Red, LightBlue);
                    setColor(Red, LightGreen);
                    cout << "Q ";
                }
                else
                    cout << get_value((x / 2) - 1, y - 1) << " ";
            }
    }
private:
    int arr[SIZE_x * SIZE_y];
    int count_bomb;
    void open(int x, int y) { //метод, который обрабатывает открытие ячейки по координатам
        if (get_value(x, y) >= 10) {
            value_add(x, y, -10);
            if (get_value(x, y) == 0) {
                for (int i = y - 1; i <= y + 1; i++)
                    for (int j = x - 1; j <= x + 1; j++)
                        if ((i >= 0) && (i < SIZE_y) && (j >= 0) && (j < SIZE_x) && !(get_value(j, i) == 9)) {
                            open(j, i);
                        }
            }
        }
    }
    void open_numb(int numb) { //метод, который обрабатывает открытие ячейки по номеру
        open(numb % SIZE_x, (numb - (numb % SIZE_x)) / SIZE_y);

    }
    int get_value(int x, int y) { //метод, который принимает координаты и возвращает номер ячейки
        return arr[SIZE_x * (y)+x];
    }

    void value_add(int x, int y, int val) { //метод добавления числа к ячейке
        arr[get_numb(x, y)] += val;

    }
    void Flag(int x, int y, int color) { //метод, который ставит флажок на полученных координатах
        if ((get_value(x, y) >= 10) || get_value(x, y) < 0) {
            value_set(x, y, -1 * (get_value(x, y)));
            if (get_value(x, y) < 0) count_bomb -= 1; else count_bomb += 1;
            Create_Field(0, 0, color);
        }
    }
    void Scan_Mass(Miner& PC_Field_Mass) { //метод, который считает вероятность расположения бомбы в каждой ячейке
        int mass = 0;
        for (int i = 0; i < SIZE_x; i++)
            for (int j = 0; j < SIZE_y; j++)
                if (get_value(i, j) < 9) {
                    mass = get_value(i, j);
                    for (int k = j - 1; k <= j + 1; k++)
                        for (int l = i - 1; l <= i + 1; l++)
                            if ((k >= 0) && (k < SIZE_y) && (l >= 0) && (l < SIZE_x) && (get_value(l, k) >= 10))
                                PC_Field_Mass.value_add(l, k, mass);
                }
    }

    void Scan_Open(Miner& PC_Field_Open) { //метод, который считает количесво открытых ячеек рядом с каждой ячейкой
        for (int i = 0; i < SIZE_x * SIZE_y; i++)
            PC_Field_Open.arr[i] = Near_open(i % SIZE_x, (i - (i % SIZE_x)) / SIZE_y);
    }
    void clear_all(int value) { //метод, который полностью очищает поле
        for (int i = 0; i < SIZE_x * SIZE_y; i++)
            if (arr[i] > 0)
                arr[i] = value;
    }
    int Near_open(int x, int y) { //метод который считает количество открытых ячеек в окрестности полученной ячейки
        int k = 0;
        for (int i = y - 1; i <= y + 1; i++)
            for (int j = x - 1; j <= x + 1; j++) {
                if (!((i >= 0) && (i < SIZE_y) && (j >= 0) && (j < SIZE_x))) k++;
                if ((i >= 0) && (i < SIZE_y) && (j >= 0) && (j < SIZE_x) && (get_value(j, i) < 10)) {
                    k++;
                }
            }
        return k;
    }

    void bomb(int x, int y) { //метод, который ставит мину на полученные координаты
        for (int i = y - 1; i <= y + 1; i++)
            for (int j = x - 1; j <= x + 1; j++)
                if ((i >= 0) && (i < SIZE_y) && (j >= 0) && (j < SIZE_x) && !(get_value(j, i) == 19)) {
                    value_add(j, i, 1);
                }
        value_set(x, y, 19);

    }
    void value_set(int x, int y, int val) { //метод, который задает ячейке значение
        arr[get_numb(x, y)] = val;
    }
    int get_numb(int x, int y) { //метод, который вычисляет номер ячейки по ее кординатам
        return SIZE_x * (y)+x;
    }
};
void Game_hum_pc(Miner& Field) { //функция, которая обрабатывает игру человека с компьютером
    int x = 14, y = 7;
    bool player = true, win = false;
    while (!Field.End_game(win)) {

        if (player) {
            Field.Create_cursor(x, y, player);
            Field.Hu_step(x, y, player, true);
            player = false;
            Field.Create_Field(0, 0, 3);
        }
        else {
            Sleep(400);
            Field.PC_step();
            player = true;
            Field.Create_Field(0, 0, 2);
        }


    }

    Field.Open_All();
    Field.Create_Field(2 * SIZE_x + 4, 0, 1);
    cout << endl;
    setColor(Black, LightRed);
    if (win) {
        SetCursor(31, 18);
        cout << "Победа";
    }
    else
        if (player) {
            SetCursor(21, 19);
            cout << "Бездушная машина попалась";
        }
        else {
            SetCursor(23, 19);
            cout << "Игрок был не осторожен";
        }
    setColor(White, Black);



}
void Game_hum_hum(Miner& Field) {//функция, которая обрабатывает игру человека с человеком
    int x = 14, y = 7;
    SetCursor(x, y);
    setColor(LightMagenta, LightMagenta);
    cout << "++";
    bool player = true, win = false;
    while (!Field.End_game(win)) {

        if (player) {
            Field.Create_cursor(x, y, player);
            Field.Hu_step(x, y, player, true);
            player = false;
            Field.Create_Field(0, 0, 3);
        }
        else {
            Field.Create_cursor(x, y, player);
            Sleep(400);
            Field.Hu_step(x, y, player, true);
            player = true;
            Field.Create_Field(0, 0, 2);
        }


    }

    Field.Open_All();
    Field.Create_Field(2 * SIZE_x + 4, 0, 1);
    cout << endl;
    setColor(Black, LightRed);
    if (win) {
        SetCursor(31, 18);
        cout << "Победа";
    }
    else
        if (!player) {
            SetCursor(19, 19);
            cout << "Первый игрок был не осторожен";
            // cout<<"Бездушная машина попалась";
        }
        else {
            SetCursor(19, 19);
            cout << "Второй игрок был не осторожен";
            //   cout<<"Игрок был не осторожен";
        }
    setColor(White, Black);


}
void Game_hum(Miner& Field) {//функция, которая обрабатывает одиночную игру человека
    int x = 14, y = 7;
    bool win = false;
    Field.Create_cursor(x, y, true);
    while (!Field.End_game(win)) {

        Field.Hu_step(x, y, true, false);
        Field.Create_Field(0, 0, 2);



    }

    Field.Open_All();
    Field.Create_Field(2 * SIZE_x + 4, 0, 1);
    cout << endl;
    setColor(Black, LightRed);
    if (win) {
        SetCursor(31, 18);
        cout << "Победа";
    }
    else {

        SetCursor(23, 19);
        cout << "Игрок был не осторожен";
    }
    setColor(White, Black);




}
bool Begin_game_two_pl() {//функция выбора второго игрока

    int key = 0;
    bool hum_or_pc = true;
    while (true) {
        SetCursor(25, 10);
        if (!hum_or_pc) {
            setColor(White, Black);
            cout << "Бездушная машина";
            SetCursor(25, 11);
            setColor(Black, White);
            cout << "Милый человек";
        }
        else {
            SetCursor(25, 10);
            setColor(Black, White);
            cout << "Бездушная машина";
            SetCursor(25, 11);
            setColor(White, Black);
            cout << "Милый человек";

        }
        key = _getch();

        switch (key) //Выполняем различные функции в соответствии с нажатой клавишью
        {
        case Enter:

            return hum_or_pc;



            break;

        case Up: //если нажали "Стрелочка вверх"
            hum_or_pc = true;
            break; //закрыли этот кейс
        case Down: //если нажали "Стрелочка вних"
            hum_or_pc = false;


        }

    }
}
bool Begin_game() {//функция выбора количесва игроков


    int key = 0;
    bool one_two = false;
    while (true) {
        SetCursor(32, 8);
        if (one_two) {
            setColor(White, Black);
            cout << "Один";
            SetCursor(32, 9);
            setColor(Black, White);
            cout << "Два";
        }
        else {

            setColor(Black, White);
            SetCursor(32, 8);
            cout << "Один";
            SetCursor(32, 9);
            setColor(White, Black);
            cout << "Два";

        }
        key = _getch();

        switch (key) //Выполняем различные функции в соответствии с нажатой клавишью
        {
        case Enter:

            return one_two;



            break;

        case Up: //если нажали "Стрелочка вверх"
            one_two = false;
            break; //закрыли этот кейс
        case Down: //если нажали "Стрелочка вних"
            one_two = true;


        }

    }
}
int Cnt_bombs() { //Функция выбора количеста мин
    setColor(White, Black);
    system("cls");
    int key = 0;
    int cnt = 30;
    while (true) {
        setColor(Black, Black);
        SetCursor(33, 8);
        cout << "++";
        SetCursor(44, 10);
        setColor(Black, Black);
        cout << "+";
        SetCursor(14, 5);
        setColor(White, Black);
        cout << "Выберете количесво мин для поля 15х15";
        SetCursor(16, 6);
        cout << "(используйте стрелочки вверх/вниз)";
        SetCursor(32, 8);
        cout << cnt;
        SetCursor(23, 10);
        cout << "Это " << ((cnt * 100) / (SIZE_x * SIZE_y)) << "% от всего поля.";
        SetCursor(27, 12);
        cout << "Новичок - 10%";
        SetCursor(27, 13);
        cout << "Бывалый - 15%";
        SetCursor(27, 14);
        cout << "Эксперт - 20%";



        key = _getch();

        switch (key) //Выполняем различные функции в соответствии с нажатой клавишью
        {
        case Enter:

            return cnt;



            break;

        case Up: //если нажали "Стрелочка вверх"
            if (cnt < SIZE_x * SIZE_y - 2)
                cnt++;
            break;
        case Down: //если нажали "Стрелочка вних"
            if (cnt > 0)
                cnt--;


        }

    }
}

void hidecursor() //функция, которая скрывает мигающий курсор консоли
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}
int main() {
    system("mode con cols=68 lines=25");



    HWND hWnd;
    HMENU hm;
    hWnd = GetConsoleWindow();

    hm = GetSystemMenu(hWnd, FALSE);
    RemoveMenu(hm, SC_SIZE, MF_BYCOMMAND | MF_REMOVE);
    RemoveMenu(hm, SC_MAXIMIZE, MF_BYCOMMAND | MF_REMOVE);
    DrawMenuBar(hWnd);







    hidecursor();
    srand(time(NULL));
    setlocale(LC_ALL, "Russian");
    int x = 2, y = 1, key = 1, Player_win = 0, miners_count = 0, test, min_mass, min_numb, k = 0;
    bool player = false, win = false, hum_or_pc = false;
    system("cls");
    SetCursor(26, 3);
    cout << "Это игра сапер.";
    SetCursor(10, 5);
    cout << "Выбор клетки осуществляется с помощью стрелочек";
    SetCursor(10, 6);
    cout << "Чтобы открыть клетку нажмите Enter";
    SetCursor(10, 7);
    cout << "Флажок ставится на пробел";
    SetCursor(10, 12);
    cout << "Клетка, на которой курсор стоит изначально без мины.";
    SetCursor(10, 13);
    cout << "То есть начать игру можно с нажатия Enter";
    SetCursor(10, 14);
    cout << "Но игрок имеет право выбрать и другую клетку";
    SetCursor(38, 17);
    cout << "Для продолжения нажмите Enter";
    while (true) {
        key = _getch();
        if (key == Enter) break;
    }
    SetCursor(10, 9);
    miners_count = Cnt_bombs();
    if (SIZE_x * SIZE_y < miners_count) return 1;

    Miner Field(10);
    Field.set_bombs(miners_count);
    cout << endl;
    system("cls");
    SetCursor(20, 5);
    cout << "Выберете количество игроков";
    SetCursor(16, 6);
    cout << "(используйте стрелочки вверх/вниз)";
    SetCursor(21, 11);
    cout << "Правила для двух игроков:";
    SetCursor(17, 13);
    cout << "Проигрывает тот, кто открыл мину.";
    SetCursor(17, 15);
    cout << "Когда открыто все кроме мин,";
    SetCursor(17, 16);
    cout << "выигрываю оба игрока.";

    if (Begin_game()) {
        setColor(White, Black);
        system("cls");
        SetCursor(19, 5);
        cout << "Кто будет вашим противником?";
        SetCursor(16, 6);
        cout << "(используйте стрелочки вверх/вниз)";
        SetCursor(24, 13);
        cout << "Как играет машина:";
        SetCursor(16, 14);
        cout << "Выбирает клетку, если рядом открыто";
        SetCursor(16, 15);
        cout << "Иногда может ошибаться";
        SetCursor(16, 16);
        cout << "Не ставит флаги";
        SetCursor(16, 17);
        cout << "Не открывает клетки с флагом";
        if (Begin_game_two_pl()) {
            setColor(White, Black);
            system("cls");
            Field.Create_Field(0, 0, 2);
            Game_hum_pc(Field);
        }
        else {
            setColor(White, Black);
            system("cls");
            Field.Create_Field(0, 0, 2);
            Game_hum_hum(Field);
        }
    }
    else {
        setColor(White, Black);
        system("cls");
        Field.Create_Field(0, 0, 2);
        Game_hum(Field);
    }
    cout << endl;
    return 0;
}
