#ifndef GENERATION_H
#define GENERATION_H
#include <random>
#include <iostream>
#endif // GENERATION_H
class sudoku{
public:
    const int size =9;
    void generator();
    int operation=1;
    int basic_field[9][9]={{3,8,9, 6,5,1, 2,4,7},
                           {4,5,2, 3,7,8, 6,1,9},
                           {1,6,7, 2,9,4, 8,3,5},

                           {6,1,4, 8,2,9, 5,7,3},
                           {9,3,5, 1,6,7, 4,8,2},
                           {2,7,8, 4,3,5, 1,9,6},

                           {7,4,3, 5,8,6, 9,2,1},
                           {8,9,6, 7,1,2, 3,5,4},
                           {5,2,1, 9,4,3, 7,6,8}};
    protected:
    void swap_lines_or_columns(bool lines);
    void swap_horizontal_or_vertical_sectors(bool horizontal);
    void transposition(bool left);
    void add_number();
};
/*class sudoku_check : public sudoku
{
private:
  int field_to_compare[9][9];
    void compare();
};*/
