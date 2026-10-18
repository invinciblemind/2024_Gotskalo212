#include "matrix.h"
#include <iostream>
#include <cstring>
#include <iomanip>
using namespace std;
// using namespace Class_def;


Errors::Errors (const char *buf) {
        strcpy(error_msg, buf);
}

char* Errors::get_error_msg() {
    return error_msg;
}

Cell* Matrix::get_first_cell() const {
        return first_cell;
}

void Matrix::MakeMatrix (const Matrix &m0) {
        if (m0.first_cell == nullptr) {
            this->first_cell = nullptr;
            return;
        }
        
        int i = 0;
        Cell *vert_pointer, *horiz_pointer, *vert_pointer0, *horiz_pointer0;
        vert_pointer0 = horiz_pointer0 = m0.first_cell;
        this->first_cell = vert_pointer = horiz_pointer = new Cell(horiz_pointer0->number);
        while (vert_pointer0 != nullptr) {
            if (i > 0)
                vert_pointer = vert_pointer->down = horiz_pointer = new Cell(horiz_pointer0->number);
            else
                i = 1;
            horiz_pointer0 = horiz_pointer0->right;
            while (horiz_pointer0 != nullptr) {
                horiz_pointer = horiz_pointer->right = new Cell(horiz_pointer0->number);
                horiz_pointer0 = horiz_pointer0->right;
            }
            horiz_pointer0 = vert_pointer0 = vert_pointer0->down;
        }
}
    
void Matrix::clear() {
        Cell *vert_pointer, *horiz_pointer;
        while (first_cell != nullptr) {
            vert_pointer = first_cell;
            first_cell = first_cell->down;
            while (vert_pointer != nullptr) {
                horiz_pointer = vert_pointer;
                vert_pointer = vert_pointer->right;
                delete horiz_pointer;
            }
        }
}
    
Matrix::Matrix (const Matrix &m0) {
        MakeMatrix(m0);
}
    
Matrix& Matrix::operator= (const Matrix m0) {
        if (&m0 == this) {
            return *this;
        }
        
        this->clear();
        if (m0.first_cell == nullptr) {
            return *this;
        }
        
        int i = 0;
        Cell *vert_pointer, *horiz_pointer, *vert_pointer0, *horiz_pointer0;
        vert_pointer0 = horiz_pointer0 = m0.first_cell;
        this->first_cell = vert_pointer = horiz_pointer = new Cell(horiz_pointer0->number);
        while (vert_pointer0 != nullptr) {
            if (i > 0)
                vert_pointer = vert_pointer->down = horiz_pointer = new Cell(horiz_pointer0->number);
            else
                i = 1;
            horiz_pointer0 = horiz_pointer0->right;
            while (horiz_pointer0 != nullptr) {
                horiz_pointer = horiz_pointer->right = new Cell(horiz_pointer0->number);
                horiz_pointer0 = horiz_pointer0->right;
            }
            horiz_pointer0 = vert_pointer0 = vert_pointer0->down;
        }
        
        return *this;
}
    
Matrix::Matrix (const int vert_len, const int horiz_len, const double num) {
        char buf[256];
        
        if (vert_len <= 0 || horiz_len <= 0) {
            sprintf(buf, "\nError in matrix.cpp, function Matrix(n, m), line %d: sizes of matrix must be positive\n\n", __LINE__);
            this->clear();
            throw Errors(buf);
        }
        
        int i, j;
        Cell *vert_pointer, *horiz_pointer;
        first_cell = vert_pointer = horiz_pointer = new Cell(num);
        
        for (i = 0; i < vert_len; i++) {
            if (i > 0)
                vert_pointer = vert_pointer->down = horiz_pointer = new Cell(num);
            for (j = 1; j < horiz_len; j++) {
                horiz_pointer = horiz_pointer->right = new Cell(num);
            }
        }
}
    
Matrix::Matrix (const double num) {
        this->first_cell = new Cell(num);
}
    
Matrix::Matrix (const double* nums, const int horiz_len) {
        char buf[256];
        
        if (horiz_len <= 0) {
            sprintf(buf, "\nError in matrix.cpp, function Matrix(vals, m), line %d: sizes of matrix must be positive\n\n", __LINE__);
            this->clear();
            throw Errors(buf);
        }
        
        int i;
        Matrix res(1, horiz_len);
        for (i = 0; i < horiz_len; i++) {
            res.set(1, i+1, nums[i]);
        }
        MakeMatrix(res);
}
    
Matrix::Matrix (const int vert_len, const double* nums) {
        char buf[256];
        
        if (vert_len <= 0) {
            sprintf(buf, "\nError in matrix.cpp, function Matrix(n, vals), line %d: sizes of matrix must be positive\n\n", __LINE__);
            this->clear();
            throw Errors(buf);
        }
        
        int i;
        Matrix res(vert_len, 1);
        for (i = 0; i < vert_len; i++) {
            res.set(i+1, 1, nums[i]);
        }
        MakeMatrix(res);
}
    
Matrix::Matrix (const char *str_matrix) {
        int flag = 0, i = 0;
        char buf[256];
        double num;
        const char *str_pointer = str_matrix;
        
        if (*(str_pointer+i) != '{') {
            sprintf(buf, "\nError in matrix.cpp, function Matrix(str), line %d: expected '{' in %d symbol of string\n\n", __LINE__, i);
            this->clear();
            throw Errors(buf);
        }
        
        if (*(str_pointer+i+1) != '{') {
            sprintf(buf, "\nError in matrix.cpp, function Matrix(str), line %d: expected '{' in %d symbol of string\n\n", __LINE__, i);
            this->clear();
            throw Errors(buf);
        }
        
        i+=2;
        if (*(str_pointer+i) == '}') {
            first_cell = nullptr;
            return;
        }
        
        Cell *vert_pointer, *horiz_pointer;
        
        if ((sscanf(str_pointer+i, "%lg", &num)) == 0 || *(str_pointer+i) == ' ') {
            sprintf(buf, "\nError in matrix.cpp, function Matrix(str), line %d: expected a number in %d symbol of string\n\n", __LINE__, i);
            this->clear();
            throw Errors(buf);
        }
        
        first_cell = vert_pointer = horiz_pointer = new Cell(num);
        
        while (1) {
            if (flag > 0) {
                if ((sscanf(str_pointer+i, "%lg", &num)) == 0 || *(str_pointer+i) == ' ') {
                    sprintf(buf, "\nError in matrix.cpp, function Matrix(str), line %d: expected a number in %d symbol of string\n\n", __LINE__, i);
                    this->clear();
                    throw Errors(buf);
                }
                
                vert_pointer = vert_pointer->down = horiz_pointer = new Cell(num);
            }
            else
                flag = 1;
            
            sprintf(buf, "%lg", num);
            i += strlen(buf);
            if (*(str_pointer+i) != '}' && *(str_pointer+i) != ',') {
                sprintf(buf, "\nError in matrix.cpp, function Matrix(str), line %d: expected ',' or '}' in %d symbol of string\n\n", __LINE__, i);
                this->clear();
                throw Errors(buf);
            }
            i++;
            
            if (*(str_pointer+i) == '}')
                break;
            else if (*(str_pointer+i) == ',') {
                i += 3;
                continue;
            }
            else if (*(str_pointer+i) == ' ') {
                i++;
            }
            else {
                sprintf(buf, "\nError in matrix.cpp, function Matrix(str), line %d: expected ',' or '}' or ' ' in %d symbol of string\n\n", __LINE__, i);
                this->clear();
                throw Errors(buf);
            }
            
            while (1) {
                if ((sscanf(str_pointer+i, "%lg", &num)) == 0 || *(str_pointer+i) == ' ') {
                    sprintf(buf, "\nError in matrix.cpp, function Matrix(str), line %d: expected a number in %d symbol of string\n\n", __LINE__, i);
                    this->clear();
                    throw Errors(buf);
                }
                
                horiz_pointer = horiz_pointer->right = new Cell(num);
                
                sprintf(buf, "%lg", num);
                i += strlen(buf);
                
                if (*(str_pointer+i) != '}' && *(str_pointer+i) != ',') {
                    sprintf(buf, "\nError in matrix.cpp, function Matrix(str), line %d: expected ',' or '}' in %d symbol of string\n\n", __LINE__, i);
                    this->clear();
                    throw Errors(buf);
                }
                i++;
                
                if (*(str_pointer+i) == ',' || *(str_pointer+i) == '}')
                    break;
                else if (*(str_pointer+i) == ' ') {
                    i++;
                }
                else {
                    sprintf(buf, "\nError in matrix.cpp, function Matrix(str), line %d: expected ',' or '}' or ' ' in %d symbol of string\n\n", __LINE__, i);
                    this->clear();
                    throw Errors(buf);
                }
            }
            
            if (*(str_pointer+i) == '}')
                break;
            else {
                if (*(str_pointer+i+1) != ' ' || *(str_pointer+i+2) != '{') {
                    sprintf(buf, "\nError in matrix.cpp, function Matrix(str), line %d: expected ' {' in %d symbol of string\n\n", __LINE__, i);
                    this->clear();
                    throw Errors(buf);
                }
                i += 3;
            }
        }
}
    
Matrix::~Matrix() {
        this->clear();
}
    
Matrix Matrix::identity(const int n) {
        char buf[256];
        
        if (n <= 0) {
            sprintf(buf, "\nError in matrix.cpp, function identity, line %d: sizes of matrix must be positive\n\n", __LINE__);
            throw Errors(buf);
        }
        
        Matrix res(n, n);
        Cell* vert_pointer = res.first_cell, *horiz_pointer;
        int i, j;
        for (i = 0; i < n; i++) {
            horiz_pointer = vert_pointer;
            for (j = 0; j < i; j++)
                horiz_pointer = horiz_pointer->right;
            horiz_pointer->number = 1.0;
            vert_pointer = vert_pointer->down;
        }
        return res;
}
    
Matrix Matrix::diagonal(const double* vals, const int n) {
        char buf[256];
        
        if (n <= 0) {
            sprintf(buf, "\nError in matrix.cpp, function diagonal, line %d: sizes of matrix must be positive\n\n", __LINE__);
            throw Errors(buf);
        }
        
        Matrix res(n, n);
        Cell* vert_pointer = res.first_cell, *horiz_pointer;
        const double* vals2 = vals;
        int i, j;
        for (i = 0; i < n; i++) {
            horiz_pointer = vert_pointer;
            for (j = 0; j < i; j++)
                horiz_pointer = horiz_pointer->right;
            horiz_pointer->number = *vals2;
            vals2++;
            vert_pointer = vert_pointer->down;
        }
        
        return res;
}
    
int Matrix::rows() const {
        Cell *vert_pointer = this->first_cell;
        int num_of_rows = 0;
        while (vert_pointer != nullptr) {
            vert_pointer = vert_pointer->down;
            num_of_rows++;
        }
        return num_of_rows;
}
    
int Matrix::columns() const{
        Cell *horiz_pointer = this->first_cell;
        int num_of_columns = 0;
        while (horiz_pointer != nullptr) {
            horiz_pointer = horiz_pointer->right;
            num_of_columns++;
        }
        return num_of_columns;
}
    
void Matrix::set(const int i, const int j, const double val) {
        Cell *cell_pointer = this->first_cell;
        int k;
        char buf[256];
        
        if ((*this).rows() < i || i <= 0) {
            
            sprintf(buf, "\nError in matrix.cpp, function set, line %d: incorrect index i\n\n", __LINE__);
            this->clear();
            throw Errors(buf);
        }
        if ((*this).columns() < j || j <= 0) {
            sprintf(buf, "\nError in matrix.cpp, function set, line %d: incorrect index j\n\n", __LINE__);
            this->clear();
            throw Errors(buf);
        }
        
        for (k = 1; k < i; k++)
            cell_pointer = cell_pointer->down;
        for (k = 1; k < j; k++)
            cell_pointer = cell_pointer->right;
        cell_pointer->number = val;
}
    
Matrix Matrix::operator[](const int index) const {
    char buf[256];
        if ((*this).rows() >= index && (*this).rows() > 1 && index > 0) {
            Matrix res(1, (*this).columns());
            Cell *res_pointer = res.first_cell, *this_pointer = (*this).first_cell;
            int i;
            
            for (i = 1; i < index; i++) {
                this_pointer = this_pointer->down;
            }
            
            for (i = 1; i <= (*this).columns(); i++) {
                res_pointer->number = this_pointer->number;
                res_pointer = res_pointer->right;
                this_pointer = this_pointer->right;
            }
            
            return res;
        }
        else if ((*this).columns() >= index && index > 0) {
            Matrix res((*this).rows(), 1);
            Cell *res_pointer = res.first_cell, *vert_pointer, *horiz_pointer;
            int i, j;
            
            vert_pointer = (*this).first_cell;
            for (i = 1; i <= (*this).rows(); i++) {
                horiz_pointer = vert_pointer;
                for (j = 1; j < index; j++) {
                    horiz_pointer = horiz_pointer->right;
                }
                res_pointer->number = horiz_pointer->number;
                res_pointer = res_pointer->down;
                vert_pointer = vert_pointer->down;
            }
            
            return res;
        }
        else {
            sprintf(buf, "\nError in matrix.cpp, function operator[], line %d: index is bigger than number of rows and columns of matrix\n\n", __LINE__); 
            throw Errors(buf);
        }
}
    
Matrix Matrix::operator* (const double scalar) const{
        Matrix res(*this);
        Cell *vert_pointer, *horiz_pointer;
        vert_pointer = res.first_cell;
        
        while (vert_pointer != nullptr) {
            horiz_pointer = vert_pointer;
            while (horiz_pointer != nullptr) {
                horiz_pointer->number *= scalar;
                horiz_pointer = horiz_pointer->right;
            }
            vert_pointer = vert_pointer->down;
        }
        
        return res;
}
    
Matrix& Matrix::operator*= (const double scalar) {
        Cell *vert_pointer, *horiz_pointer;
        vert_pointer = (*this).first_cell;
        
        while (vert_pointer != nullptr) {
            horiz_pointer = vert_pointer;
            while (horiz_pointer != nullptr) {
                horiz_pointer->number *= scalar;
                horiz_pointer = horiz_pointer->right;
            }
            vert_pointer = vert_pointer->down;
        }
        
        return *this;
}

ostream& operator<< (ostream& s, const Matrix& m0) {
        if (m0.get_first_cell() == nullptr) {
            s<<"";
            return s;
        }
        
        Cell *vert_pointer, *horiz_pointer;
        vert_pointer = m0.get_first_cell();
        
        while (vert_pointer != nullptr) {
            horiz_pointer = vert_pointer;
            while (horiz_pointer != nullptr) {
                s<<setw(7)<<horiz_pointer->number<<" ";
                horiz_pointer = horiz_pointer->right;
            }
            s<<endl;
            vert_pointer = vert_pointer->down;
        }
        
        s<<endl;
        return s;
}
