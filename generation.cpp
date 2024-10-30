#include "generation.h"
void sudoku:: generator(){
    std::random_device rd; // Получаем случайное число из устройства
    std::mt19937 gen(rd()); // Инициализируем генератор (Mersenne Twister)
    // Задаем диапазон случайных чисел
    std::uniform_int_distribution<> distrib(1, 7); // От 1 до 100
    for(int i=0;i<10;i++){
    operation=distrib(gen);
    std::cout << distrib(gen) << ' ';
    switch(operation){
    case 1:
        swap_lines_or_columns(true);
        break;
    case 2:
        swap_lines_or_columns(false);
        break;
    case 3:
        swap_horizontal_or_vertical_sectors(true);
        break;
    case 4:
        swap_horizontal_or_vertical_sectors(false);
        break;
    case 5:
        transposition(true);
        break;
    case 6:
        transposition(false);
        break;
    case 7:
        add_number();
        break;
    }
    }
}
void sudoku::swap_lines_or_columns(bool lines){
    int sector, line1=0,line2=0,bub;
      std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 2);
    sector=distrib(gen);
    while(line1==line2){
    line1=distrib(gen)+(sector)*3;
    line2=distrib(gen)+(sector)*3;
    }
    if(lines==true){
    for(int i=0;i<size;i++){
        bub = basic_field[line1][i];
        basic_field[line1][i]=basic_field[line2][i];
        basic_field[line2][i]=bub;
    }
    }
    else{
        for(int i=0;i<size;i++){
            bub = basic_field[i][line1];
            basic_field[i][line1]=basic_field[i][line2];
            basic_field[i][line2]=bub;
        }
    }
}
void sudoku::swap_horizontal_or_vertical_sectors(bool horizontal){
    int sector1=0,sector2=0,bub;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 2);
    while(sector1==sector2){
        sector1=distrib(gen);
        sector2=distrib(gen);
    }
    if(horizontal==true){
        for(int j=0;j<3;j++){
        for(int i=0;i<size;i++){
            bub = basic_field[sector1*3+j][i];
            basic_field[sector1*3+j][i]=basic_field[sector2*3+j][i];
            basic_field[sector2*3+j][i]=bub;
        }
    }
    }
    else{
        for(int j=0;j<3;j++){
            for(int i=0;i<size;i++){
                bub = basic_field[i][sector1*3+j];
                basic_field[i][sector1*3+j]=basic_field[i][sector2*3+j];
                basic_field[i][sector2*3+j]=bub;
            }
        }
    }
}
void sudoku::transposition(bool left){
    int bub;
    if(left==true){
    for(int i=0;i<size;i++){
        for(int j=0;j<i;j++){
            bub=basic_field[i][j];
            basic_field[i][j]=basic_field[j][i];
            basic_field[j][i]=bub;
        }
    }
    }
    else{
        for(int i=0;i<size;i++){
            for(int j=0;j<i;j++){
                bub=basic_field[8-i][j];
                basic_field[8-i][j]=basic_field[j][8-i];
                basic_field[j][8-i]=bub;
            }
        }
    }
}
void sudoku::add_number(){
    int number;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1,8);
    number=distrib(gen);
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            basic_field[i][j]+=number;
            if(basic_field[i][j]>9){
                basic_field[i][j]-=9;
            }
        }
    }
}
