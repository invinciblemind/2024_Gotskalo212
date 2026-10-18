#include <iostream>

#ifndef __CLASS
#define __CLASS

// namespace Class_def
// {

struct Cell {
        double number;
        Cell* down;
        Cell* right;
        Cell (double num = 0.0, Cell* dwn = nullptr, Cell* rght = nullptr) {
            this->number = num;
            this->down = dwn;
            this->right = rght;
        }
};

class Errors {
    char error_msg[256];
public:
    Errors (): error_msg(" ") {};
    
    Errors (const char *buf);
    
    char* get_error_msg();
};

class Matrix: public Errors {
    Cell* first_cell;
    
public:
    
    Cell* get_first_cell() const;
    
    Matrix (): first_cell(nullptr) {}
    
    void MakeMatrix (const Matrix &m0);
    
    void clear();
    
    Matrix (const Matrix &m0);
    
    Matrix& operator= (const Matrix m0);
    
    Matrix (const int vert_len, const int horiz_len, const double num = 0.0);
    
    Matrix (const double num);
    
    Matrix (const double* num, const int horiz_len);
    
    Matrix (const int vert_len, const double* num);
    
    Matrix (const char *str_matrix);
    
    friend std::ostream& operator<< (std::ostream& s, const Matrix& m0);
    
    ~Matrix();
    
    static Matrix identity(const int n);
    
    static Matrix diagonal(const double* vals, const int n);
    
    int rows() const;
    
    int columns() const;
    
    void set(const int i, const int j, const double val);
    
    Matrix operator[](const int index) const;
    
    Matrix operator* (const double scalar) const;
    
    Matrix& operator*= (const double scalar);
};

std::ostream& operator<< (std::ostream& s, const Matrix& m0);

// }

#endif
