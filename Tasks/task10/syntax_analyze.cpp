#include <iostream>
#include <string.h>
#include "lex_analyze.h"
#define NUM_OF_SPEC_WORDS 33
#define NUM_OF_SPEC_SYMBOLS 33
using namespace std;

typedef enum {NULL_T, INT_T, REAL_T, STR_T, LIST_T, BOOL_T, NORETURN_T} types;

struct Lexem {
    char elem[100], type_name[50], type[50];
    int begin_row_num, end_row_num;
    
    Lexem(const char elem[] = "~", const char type_name[] = "NULL", const char type[] = "NULL", int row = -1) {
        strcpy(this->elem, elem);
        strcpy(this->type_name, type_name);
        strcpy(this->type, type);
        begin_row_num = row;
        end_row_num = -1;
    }
};

struct TID {
    char name[50];
    types type;
    int int_val;
    double real_val;
    char str_val[100];
    bool bool_val;
    TID *list_val[100];
    TID() {
        memset(name, 0, sizeof(name));
        type = NULL_T;
        int_val = 0;
        real_val = 0.0;
        memset(str_val, 0, sizeof(str_val));
        bool_val = false;
        for (int i = 0; i < 100; ++i) {
            list_val[i] = nullptr;
        }
    }
    TID(int a) {
        memset(name, 0, sizeof(name));
        type = INT_T;
        int_val = a;
        real_val = 0.0;
        memset(str_val, 0, sizeof(str_val));
        bool_val = false;
        for (int i = 0; i < 100; ++i) {
            list_val[i] = nullptr;
        }
    }
    TID(double t) {
        memset(name, 0, sizeof(name));
        type = REAL_T;
        int_val = 0;
        real_val = t;
        memset(str_val, 0, sizeof(str_val));
        bool_val = false;
        for (int i = 0; i < 100; ++i) {
            list_val[i] = nullptr;
        }
    }
    TID(bool t) {
        memset(name, 0, sizeof(name));
        type = BOOL_T;
        int_val = 0;
        real_val = 0.0;
        memset(str_val, 0, sizeof(str_val));
        bool_val = t;
        for (int i = 0; i < 100; ++i) {
            list_val[i] = nullptr;
        }
    }
    TID(char *s) {
        memset(name, 0, sizeof(name));
        type = STR_T;
        int_val = 0;
        real_val = 0.0;
        memset(str_val, 0, sizeof(str_val));
        strcpy(str_val,s);
        bool_val = false;
        for (int i = 0; i < 100; ++i) {
            list_val[i] = nullptr;
        }
    }
    TID(const TID& other) {
        type = other.type;
        int_val = other.int_val;
        real_val = other.real_val;
        bool_val = other.bool_val;
        memcpy(name, other.name, sizeof(name));
        memcpy(str_val, other.str_val, sizeof(str_val));
        for (int i = 0; i < 100; ++i) {
            if (other.list_val[i] != nullptr) {
                list_val[i] = new TID(*other.list_val[i]);
            } else {
                list_val[i] = nullptr;
            }
        }
    }
    TID& operator=(const TID& other) {
        if (this == &other) {
            return *this;
        }
        type = other.type;
        int_val = other.int_val;
        real_val = other.real_val;
        bool_val = other.bool_val;
        memcpy(name, other.name, sizeof(name));
        memcpy(str_val, other.str_val, sizeof(str_val));
        for (int i = 0; i < 100; ++i) {
            delete list_val[i];
        }
        for (int i = 0; i < 100; ++i) {
            if (other.list_val[i] != nullptr) {
                list_val[i] = new TID(*other.list_val[i]);
            } else {
                list_val[i] = nullptr;
            }
        }

        return *this;
    }
    ~TID() {
        for (int i = 0; i < 100; ++i) {
            if (list_val[i] != nullptr) {
                delete list_val[i];
                list_val[i] = nullptr;
            }
        }
    }
};

struct FUNC_TABLE {
    char name[100];
    int argc;
    types type;
    TID args_mas[10];
    TID loc_var[20];
    
    FUNC_TABLE() : argc(0), type(NORETURN_T) {
        memset(name, 0, sizeof(name));
        for (int i = 0; i < 10; ++i) {
            args_mas[i] = TID();
        }
        for (int i = 0; i < 20; ++i) {
            loc_var[i] = TID();
        }
    }
};

FUNC_TABLE func_mas[20];
int func_end = 0;

TID tid_mas[100];
int tid_end = 0;

int func_level = 0, cycle_level = 0;

Lexem matrix[100][100];
int row_num = 0, column_num = 0;
char real_number[100], cur_string[100];

char **lst_copy, **lst_gr;
char spec_symbols[NUM_OF_SPEC_SYMBOLS][4] = {",", ".", "/", "//", "'", "\"", "[", "]", "<", "<=", ">", ">=", "=", "==", 
    "!=", "%", "*", "**","+", "-", "+=", "-=", "*=", "/=", "//=", "(", ")", "'''", "\"\"\"", " ", "\n", ":", "#"};
char spec_words[NUM_OF_SPEC_WORDS][9] = {"print", "for", "while", "if", "def", "int", "real", "bool", "str", "list", "input", 
    "range", "elif", "else", "break", "continue", "split", "sort", "reverse", "type", "and", "or", "not", "in",
    "global", "True", "False", "return", "range", "len", "append", "pop", "insert"}; // пока не нужен, но пусть будет
int size_l, comment_flag = 0, str_flag = 0, lst_gr_i = 0, lst_gr_len, newline_flag = 1, recursion_deep = 0;

int is_end() { // проверка не дошли ли мы до конца списка, проводится перед каждой новой проверкой лексемы
    return (lst_gr_i == lst_gr_len);
}

int is_letter(char c) {
    //cout<<"is_letter"<<endl;
    return ((('a' <= c) && (c <= 'z')) || (('A' <= c) && (c <= 'Z')) || (c == '_')) ? 1 : 0;
}

int is_num(char c) {
    //cout<<"is_num"<<endl;
    return (('0' <= c) && (c <= '9')) ? 1 : 0;
}

int is_identifier() { // проверка первого символа лексемы
    // cout<<"is_identifier "<<lst_gr_i<<endl;
    if (is_end()) {
        return 0;
    }
    if (is_letter(lst_gr[lst_gr_i][0]) == 0)
        return 0;
    lst_gr_i++; // каждый раз если проверка успешная, сдвикаемся на следующую лексему, иначе - остаемся на текущей
    return NORETURN_T;
}

int is_bool() {
    //cout<<"is_bool"<<endl;
    if (is_end()) {
        return 0;
    }
    if (((strcmp(lst_gr[lst_gr_i], "True") == 0) || (strcmp(lst_gr[lst_gr_i], "False") == 0)) == 0)
        return 0;
    matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "bool", "bool");
    lst_gr_i++;
    return BOOL_T;
}

// предописания функций, которые нужны раньше, чем они описаны
int is_expression();

int is_statement(int deep);

int is_list_methods() {
    //cout<<"is_list_methods"<<endl;
    if (is_end()) {
        return 0;
    }
    int lst_gr_i_old = lst_gr_i; // далее почти всегда сохраняем начальное состояние
    if (is_identifier() == 0) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (strcmp(lst_gr[lst_gr_i], ".") != 0) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    lst_gr_i++;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (strcmp(lst_gr[lst_gr_i], "append") == 0) { // блок проверки на <identifier>.append(<expression>)
        lst_gr_i++;
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (strcmp(lst_gr[lst_gr_i], "(") != 0) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        lst_gr_i++;
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 4], "variable", "list");
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 3], "dot", "service");
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 2], "list_method", "method");
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 1], "open_circle_bracket", "service");
        if (is_expression() == 0) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (strcmp(lst_gr[lst_gr_i], ")") != 0) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "close_circle_bracket", "service");
        lst_gr_i++;
        return NORETURN_T;
    }
    else if (strcmp(lst_gr[lst_gr_i], "insert") == 0) { // аналогично
        lst_gr_i++;
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (strcmp(lst_gr[lst_gr_i], "(") != 0) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        lst_gr_i++;
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 4], "variable", "list");
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 3], "dot", "service");
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 2], "list_method", "method");
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 1], "open_circle_bracket", "service");
        if (is_expression() == 0) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (strcmp(lst_gr[lst_gr_i], ",") != 0) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        lst_gr_i++;
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 1], "comma", "service");
        if (is_expression() == 0) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (strcmp(lst_gr[lst_gr_i], ")") != 0) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "close_circle_bracket", "service");
        lst_gr_i++;
        return NORETURN_T;
    }
    else if (strcmp(lst_gr[lst_gr_i], "pop") == 0) {
        lst_gr_i++;
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (strcmp(lst_gr[lst_gr_i], "(") != 0) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        lst_gr_i++;
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 4], "variable", "list");
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 3], "dot", "service");
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 2], "list_method", "method");
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 1], "open_circle_bracket", "service");
        if (is_expression() != 0) { // expression может быть, а может и не быть
            if (is_end()) {
                lst_gr_i = lst_gr_i_old;
                return 0;
            }
        }
        if (strcmp(lst_gr[lst_gr_i], ")") != 0) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "close_circle_bracket", "service");
        lst_gr_i++;
        return NORETURN_T;
    }
    else if (strcmp(lst_gr[lst_gr_i], "sort") == 0) {
        lst_gr_i++;
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (strcmp(lst_gr[lst_gr_i], "(") != 0) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        lst_gr_i++;
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (strcmp(lst_gr[lst_gr_i], ")") != 0) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 4], "variable", "list");
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 3], "dot", "service");
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 2], "list_method", "method");
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 1], "open_circle_bracket", "service");
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "close_circle_bracket", "service");
        lst_gr_i++;
        return NORETURN_T;
    }
    else if (strcmp(lst_gr[lst_gr_i], "reverse") == 0) {
        lst_gr_i++;
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (strcmp(lst_gr[lst_gr_i], "(") != 0) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        lst_gr_i++;
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (strcmp(lst_gr[lst_gr_i], ")") != 0) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 4], "variable", "list");
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 3], "dot", "service");
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 2], "list_method", "method");
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 1], "open_circle_bracket", "service");
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "close_circle_bracket", "service");
        lst_gr_i++;
        return NORETURN_T;
    }
    lst_gr_i = lst_gr_i_old;
    return 0;
}

int is_list() {
    // cout<<"is_list"<<endl;
    if (is_end()) {
        return 0;
    }
    int lst_gr_i_old = lst_gr_i;
    if (strcmp(lst_gr[lst_gr_i], "[") != 0) { // начинается с [
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "open_square_bracket", "service");
    lst_gr_i++;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (is_expression() != 0) { // далее может быть выражение
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        while (strcmp(lst_gr[lst_gr_i], ",") == 0) { // пока запятая, может быть выражение
            matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "comma", "service");
            lst_gr_i++;
            if (is_end()) {
                lst_gr_i = lst_gr_i_old;
                return 0;
            }
            if (is_expression() == 0) {
                break;
            }
            if (is_end()) {
                lst_gr_i = lst_gr_i_old;
                return 0;
            }
        }
        if (strcmp(lst_gr[lst_gr_i], "]") != 0) { // в конце (после последнего выражения или запятой - ]
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
    }
    else if (strcmp(lst_gr[lst_gr_i], "]") != 0) { // случай с пустым списком
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "close_square_bracket", "service");
    lst_gr_i++;
    return LIST_T;
}

int is_string() {
    // cout<<"is_string "<<lst_gr_i<<endl;
    if (is_end()) {
        return 0;
    }
    int lst_gr_i_old = lst_gr_i;
    if (strcmp(lst_gr[lst_gr_i], "'") == 0) {
        memset(cur_string, 0, sizeof(cur_string));
        strcpy(cur_string, "'");
        lst_gr_i++;
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        while ((strcmp(lst_gr[lst_gr_i], "'") != 0)) { // в строке - любые символы кроме ее границ
            strcat(cur_string, lst_gr[lst_gr_i]);
            lst_gr_i++;
            if (is_end()) {
                lst_gr_i = lst_gr_i_old;
                return 0;
            }
        }
        strcat(cur_string, "'");
        matrix[row_num][column_num++] = Lexem(cur_string, "string", "str");
        lst_gr_i++;
        return STR_T;
    }
    else if (strcmp(lst_gr[lst_gr_i], "\"") == 0) {
        memset(cur_string, 0, sizeof(cur_string));
        strcpy(cur_string, "\"");
        lst_gr_i++;
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        while ((strcmp(lst_gr[lst_gr_i], "\"") != 0)) { // аналогично
            strcat(cur_string, lst_gr[lst_gr_i]);
            lst_gr_i++;
            if (is_end()) {
                lst_gr_i = lst_gr_i_old;
                return 0;
            }
        }
        strcat(cur_string, "\"");
        matrix[row_num][column_num++] = Lexem(cur_string, "string", "str");
        lst_gr_i++;
        return STR_T;
    }
    else {
        return 0;
    }
}

int is_string_methods() {
    // cout<<"is_string_methods "<<lst_gr_i<<endl;
    int lst_gr_i_old = lst_gr_i;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    // len - в list_methods
    if (is_identifier() == 0) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (strcmp(lst_gr[lst_gr_i], ".") != 0) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    lst_gr_i++;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (strcmp(lst_gr[lst_gr_i], "split") == 0) {
        lst_gr_i++;
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (strcmp(lst_gr[lst_gr_i], "(") != 0) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        lst_gr_i++;
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 4], "variable", "str");
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 3], "dot", "service");
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 2], "str_method", "method");
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 1], "open_circle_bracket", "service");
        if (is_string() != 0) {
            if (is_end()) {
                lst_gr_i = lst_gr_i_old;
                return 0;
            }
            matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 1], "string", "str");
        }
        if (strcmp(lst_gr[lst_gr_i], ")") != 0) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "close_circle_bracket", "service");
        lst_gr_i++;
        return LIST_T;
    }
    else if (strcmp(lst_gr[lst_gr_i], "count") == 0) {
        lst_gr_i++;
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (strcmp(lst_gr[lst_gr_i], "(") != 0) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        lst_gr_i++;
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 4], "variable", "str");
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 3], "dot", "service");
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 2], "str_method", "method");
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 1], "open_circle_bracket", "service");
        if (is_string() == 0) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (strcmp(lst_gr[lst_gr_i], ")") != 0) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "close_circle_bracket", "service");
        lst_gr_i++;
        return INT_T;
    }
    else if (strcmp(lst_gr[lst_gr_i], "find") == 0) {
        lst_gr_i++;
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (strcmp(lst_gr[lst_gr_i], "(") != 0) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        lst_gr_i++;
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 4], "variable", "str");
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 3], "dot", "service");
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 2], "str_method", "method");
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 1], "open_circle_bracket", "service");
        if (is_string() == 0) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (strcmp(lst_gr[lst_gr_i], ")") != 0) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "close_circle_bracket", "service");
        lst_gr_i++;
        return BOOL_T;
    }
    else {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
}

int is_int() { // первый символ лексемы должен быть цифрой, а следующий - не точкой
    //cout<<"is_int"<<endl;
    int lst_gr_i_old = lst_gr_i;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (is_num(lst_gr[lst_gr_i][0]) == 0) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    lst_gr_i++;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (strcmp(lst_gr[lst_gr_i], ".") == 0) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    return INT_T;
}

int is_real() { // должно быть число (цифра), потом точка, потом еще число
    //cout<<"is_real"<<endl;
    int lst_gr_i_old = lst_gr_i;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (is_num(lst_gr[lst_gr_i][0]) == 0) {
        return 0;
    }
    lst_gr_i++;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (strcmp(lst_gr[lst_gr_i], ".") == 0) {
        lst_gr_i++;
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (is_num(lst_gr[lst_gr_i][0]) == 0) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        lst_gr_i++;
        return REAL_T;
    }
    else {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
}

int is_number() { // здесь и далее - проверяем по очереди, так как при успешной проверке мы уже находимся на следующем символе
    // cout<<"is_number "<<lst_gr_i<<endl;
    int lst_gr_i_old = lst_gr_i;
    if (is_int()) {
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i_old], "int_number", "int");
        return INT_T;
    }
    if (is_real()) {
        memset(real_number, 0, sizeof(real_number));
        strcpy(real_number, lst_gr[lst_gr_i_old]);
        strcat(real_number, lst_gr[lst_gr_i_old + 1]);
        strcat(real_number, lst_gr[lst_gr_i_old + 2]);
        matrix[row_num][column_num++] = Lexem(real_number, "real_number", "real");
        return REAL_T;
    }
    return 0;
}

int is_literal() {
    int ret_t;
    // cout<<"is_literal"<<endl;
    if ((ret_t = is_number()))
        return ret_t;
    if ((ret_t = is_string()))
        return ret_t;
    if ((ret_t = is_string_methods()))
        return ret_t;
    if ((ret_t = is_list()))
        return ret_t;
    if ((ret_t = is_list_methods()))
        return ret_t;
    return (is_bool());
}

int is_block(int deep) {
    // cout<<"is_block"<<endl;
    int i;
    int lst_gr_i_old = lst_gr_i;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (strcmp(lst_gr[lst_gr_i], "\n") != 0) { // начинается с \n
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    matrix[row_num++][column_num] = Lexem("\\n", "NEWLINE", "service");
    column_num = 0;
    lst_gr_i++;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    char space_indent[20];
    memset(space_indent, 0, sizeof(space_indent));
    for (i = 0; i < 4 * deep; i++) { // далее нужное число провелов - 4 * deep
        if (strcmp(lst_gr[lst_gr_i], " ") != 0) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        strcat(space_indent, " ");
        lst_gr_i++;
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
    }
    matrix[row_num][column_num++] = Lexem(space_indent, "space_indent", "service");
    if (is_statement(deep) == 0) { // потом - statement
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    lst_gr_i_old = lst_gr_i; // обновляем точку возврата, так как уже считали часть блока
    if (is_end()) { // если уже конец - нормально
        lst_gr_i = lst_gr_i_old;
        return NORETURN_T;
    }
    while (1) { // далее если что-то не сходится - нормально, откатываемся к последней точке
        for (i = 0; i < 4 * deep; i++) {
            if (strcmp(lst_gr[lst_gr_i], " ") != 0) {
                lst_gr_i = lst_gr_i_old;
                return NORETURN_T;
            }
            lst_gr_i++;
            if (is_end()) {
                lst_gr_i = lst_gr_i_old;
                return NORETURN_T;
            }
        }
        matrix[row_num][column_num++] = Lexem(space_indent, "space_indent", "service");
        if (is_statement(deep) == 0) {
            lst_gr_i = lst_gr_i_old;
            return NORETURN_T;
        }
        lst_gr_i_old = lst_gr_i;
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return NORETURN_T;
        }
    }
}

int is_parameter() { // поработать с типами
    // cout<<"is_parameter"<<endl;
    int lst_gr_i_old = lst_gr_i;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (is_identifier() == 0) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 1], "local_variable"); // тип
    if (strcmp(lst_gr[lst_gr_i], "=") != 0) {
        return 1;
    }
    matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "equal_by_default", "service");
    lst_gr_i++;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (is_expression() == 0) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    return 1;
}

int is_parameters() { // поработать с типами
    // cout<<"is_parameters"<<endl;
    int lst_gr_i_old = lst_gr_i;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (is_parameter() == 0) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (strcmp(lst_gr[lst_gr_i], ",") != 0) {
        return 1;
    }
    while (1) { // пока запятая - должен быть параметр
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "comma", "service");
        lst_gr_i++;
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (is_parameter() == 0) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (strcmp(lst_gr[lst_gr_i], ",") != 0) {
            return 1;
        }
    }
}

int is_function_call() { // поработать с типом
    // cout<<"is_function_call"<<endl;
    int lst_gr_i_old = lst_gr_i;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (is_identifier() == 0) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (strcmp(lst_gr[lst_gr_i], "(") != 0) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    lst_gr_i++;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 2], "function_name"); // вставить тип
    matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 1], "open_circle_bracket", "service");
    if (is_expression() != 0) { // вызов с параметрами
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        while (strcmp(lst_gr[lst_gr_i], ",") == 0) {
            matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "comma", "service");
            lst_gr_i++;
            if (is_end()) {
                lst_gr_i = lst_gr_i_old;
                return 0;
            }
            if (is_expression() == 0) {
                lst_gr_i = lst_gr_i_old;
                return 0;
            }
            if (is_end()) {
                lst_gr_i = lst_gr_i_old;
                return 0;
            }
        }
    }
    if (strcmp(lst_gr[lst_gr_i], ")") != 0) { // вызов с пустыми скобками
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "close_circle_bracket", "service");
    lst_gr_i++;
    return 1;
}

int is_function_definition(int deep) {
    // cout<<"is_function_definition"<<endl;
    int lst_gr_i_old = lst_gr_i;
    // int i;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (strcmp(lst_gr[lst_gr_i], "def") != 0) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    lst_gr_i++;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (is_identifier() == 0) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (strcmp(lst_gr[lst_gr_i], "(") != 0) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    lst_gr_i++;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    int start_row = row_num, start_column = column_num;
    matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 3], "func_def", "service_word", row_num + 1);
    matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 2], "function_name"); // тип
    /*
    for (i = 0; i < tid_end; i++) {
        if (strcmp(tid_mas[i].name, lst_gr[lst_gr_i - 2]) == 0) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
    }
    strcpy(func_mas[func_end].name, lst_gr[lst_gr_i - 2]);
    */
    matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 1], "open_circle_bracket", "service");
    if (is_parameters() != 0) {
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
    }
    if (strcmp(lst_gr[lst_gr_i], ")") != 0) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "close_circle_bracket", "service");
    lst_gr_i++;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (strcmp(lst_gr[lst_gr_i], ":") != 0) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "double_dot", "service");
    lst_gr_i++;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    func_level++;
    if (is_block(deep + 1) == 0) { // блок - более высокого уровня вложенности
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    func_level--;
    matrix[start_row][start_column].end_row_num = row_num - 1;
    return NORETURN_T;
}

int is_while_statement(int deep) {
    // cout<<"is_while_statement"<<endl;
    int lst_gr_i_old = lst_gr_i;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (strcmp(lst_gr[lst_gr_i], "while") != 0) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    int start_row = row_num, start_column = column_num;
    matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "while", "service_word", row_num + 1);
    lst_gr_i++;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (is_expression() != BOOL_T) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (strcmp(lst_gr[lst_gr_i], ":") != 0) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "double_dot", "service");
    lst_gr_i++;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    cycle_level++;
    if (is_block(deep + 1) == 0) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    cycle_level--;
    matrix[start_row][start_column].end_row_num = row_num - 1;
    return NORETURN_T;
}

int is_for_statement(int deep) {
    // cout<<"is_for_statement"<<endl;
    int i;
    int lst_gr_i_old = lst_gr_i;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (strcmp(lst_gr[lst_gr_i], "for") != 0) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    int start_row = row_num, start_column = column_num;
    matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "for", "service_word", row_num + 1);
    lst_gr_i++;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (is_identifier() == 0) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 1], "variable");
    for (i = 0; i < tid_end; i++) {
        if (strcmp(tid_mas[i].name, matrix[row_num][column_num - 1].elem) == 0) {
            if (tid_mas[i].type != INT_T) {
                lst_gr_i = lst_gr_i_old;
                return 0;
            }
            break;
        }
    }
    if (i == tid_end) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    switch(tid_mas[i].type) {
        case INT_T: 
            strcpy(matrix[row_num][column_num - 1].type, "int");
            break;
        case REAL_T: 
            strcpy(matrix[row_num][column_num - 1].type, "real");
            break;
        case STR_T: 
            strcpy(matrix[row_num][column_num - 1].type, "str");
            break;
        case LIST_T: 
            strcpy(matrix[row_num][column_num - 1].type, "list");
            break;
        case BOOL_T: 
            strcpy(matrix[row_num][column_num - 1].type, "bool");
            break;
        default:
            lst_gr_i = lst_gr_i_old;
            return 0;
    }
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (strcmp(lst_gr[lst_gr_i], "in") != 0) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "in", "service_word");
    lst_gr_i++;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (strcmp(lst_gr[lst_gr_i], "range") == 0) { // вариант с in range
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "range", "service_word");
        lst_gr_i++;
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (strcmp(lst_gr[lst_gr_i], "(") != 0) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "open_circle_bracket", "service");
        lst_gr_i++;
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (is_int() == 0) {
            if (is_identifier() == 0) {
                lst_gr_i = lst_gr_i_old;
                return 0;
            }
            matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 1], "variable", "int");
        }
        else {
            matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 1], "int_number", "int");
        }
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (strcmp(lst_gr[lst_gr_i], ",") == 0) { // с двумя параметрами
            matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "comma", "service");
            lst_gr_i++;
            if (is_end()) {
                lst_gr_i = lst_gr_i_old;
                return 0;
            }
            if (is_int() == 0) {
                if (is_identifier() == 0) {
                    lst_gr_i = lst_gr_i_old;
                    return 0;
                }
                matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 1], "variable", "int");
            }
            else {
                matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 1], "int_number", "int");
            }
            if (is_end()) {
                lst_gr_i = lst_gr_i_old;
                return 0;
            }
            if (strcmp(lst_gr[lst_gr_i], ",") == 0) { // с тремя
                matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "comma", "service");
                lst_gr_i++;
                if (is_end()) {
                    lst_gr_i = lst_gr_i_old;
                    return 0;
                }
                if (is_int() == 0) {
                    if (is_identifier() == 0) {
                        lst_gr_i = lst_gr_i_old;
                        return 0;
                    }
                    matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 1], "variable", "int");
                }
                else {
                    matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 1], "int_number", "int");
                }
                if (is_end()) {
                    lst_gr_i = lst_gr_i_old;
                    return 0;
                }
            }
        }
        if (strcmp(lst_gr[lst_gr_i], ")") != 0) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "close_circle_bracket", "service");
        lst_gr_i++;
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (strcmp(lst_gr[lst_gr_i], ":") != 0) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "double_dot", "service");
        lst_gr_i++;
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        cycle_level++;
        if (is_block(deep + 1) == 0) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        cycle_level--;
        matrix[start_row][start_column].end_row_num = row_num - 1;
        return NORETURN_T;
    }
    else if (is_identifier() == 0) { // вариант с in <variable>
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 1], "variable"); // тип (список или строка)
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (strcmp(lst_gr[lst_gr_i], ":") != 0) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "double_dot", "service");
    lst_gr_i++;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    cycle_level++;
    if (is_block(deep + 1) == 0) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    cycle_level--;
    matrix[start_row][start_column].end_row_num = row_num - 1;
    return NORETURN_T;
}

int is_if_statement(int deep) {
    // cout<<"is_if_statement"<<endl;
    int i;
    int lst_gr_i_old = lst_gr_i;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (strcmp(lst_gr[lst_gr_i], "if") != 0) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    int start_row = row_num, start_column = column_num;
    matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "if", "service_word", row_num + 1);
    lst_gr_i++;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (is_expression() != BOOL_T) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (strcmp(lst_gr[lst_gr_i], ":") != 0) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "double_dot", "service");
    lst_gr_i++;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (is_block(deep + 1) == 0) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    matrix[start_row][start_column].end_row_num = row_num - 1;
    if (is_end()) {
        return NORETURN_T;
    }
    lst_gr_i_old = lst_gr_i; // обновляем точку возврата, т.к. уже считали правильную часть
    char space_indent[20];
    while (1) {
        memset(space_indent, 0, sizeof(space_indent));
        for (i = 0; i < 4 * deep; i++) { // далее нужное число провелов - 4 * deep
            if (strcmp(lst_gr[lst_gr_i], " ") != 0) {
                lst_gr_i = lst_gr_i_old;
                return 0;
            }
            strcat(space_indent, " ");
            lst_gr_i++;
            if (is_end()) {
                lst_gr_i = lst_gr_i_old;
                return NORETURN_T;
            }
        }
        matrix[row_num][column_num++] = Lexem(space_indent, "space_indent", "service");
        if (strcmp(lst_gr[lst_gr_i], "elif") == 0) {
            start_row = row_num;
            start_column = column_num;
            matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "elif", "service_word", row_num + 1);
            lst_gr_i++;
            if (is_end()) {
                lst_gr_i = lst_gr_i_old;
                return 0;
            }
            if (is_expression() != BOOL_T) {
                lst_gr_i = lst_gr_i_old;
                return 0;
            }
            if (is_end()) {
                lst_gr_i = lst_gr_i_old;
                return 0;
            }
            if (strcmp(lst_gr[lst_gr_i], ":") != 0) {
                lst_gr_i = lst_gr_i_old;
                return 0;
            }
            matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "double_dot", "service");
            lst_gr_i++;
            if (is_end()) {
                lst_gr_i = lst_gr_i_old;
                return 0;
            }
            if (is_block(deep + 1) == 0) {
                lst_gr_i = lst_gr_i_old;
                return 0;
            }
            matrix[start_row][start_column].end_row_num = row_num - 1;
            if (is_end()) {
                return NORETURN_T;
            }
        }
        else if (strcmp(lst_gr[lst_gr_i], "else") == 0) { // в конце будем сразу выходить
            start_row = row_num;
            start_column = column_num;
            matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "else", "service_word", row_num + 1);
            lst_gr_i++;
            if (is_end()) {
                lst_gr_i = lst_gr_i_old;
                return 0;
            }
            if (strcmp(lst_gr[lst_gr_i], ":") != 0) {
                lst_gr_i = lst_gr_i_old;
                return 0;
            }
            matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "double_dot", "service");
            lst_gr_i++;
            if (is_end()) {
                lst_gr_i = lst_gr_i_old;
                return 0;
            }
            if (is_block(deep + 1) == 0) {
                lst_gr_i = lst_gr_i_old;
                return 0;
            }
            matrix[start_row][start_column].end_row_num = row_num - 1;
            return NORETURN_T;
        }
        else {
            lst_gr_i = lst_gr_i_old;
            return NORETURN_T;
        }
    }
    
}

int is_assignment() {
    int ret_t, i;
    // cout<<"is_assignment"<<endl;
    int lst_gr_i_old = lst_gr_i;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (is_identifier() == 0) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (strcmp(lst_gr[lst_gr_i], "=") == 0) {
        int saved_column_num = column_num;
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 1], "variable");
        for (i = 0; i < func_end; i++) {
            if (strcmp(func_mas[i].name, lst_gr[lst_gr_i - 1]) == 0) {
                lst_gr_i = lst_gr_i_old;
                return 0;
            }
        }
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "assign_operation", "service");
        lst_gr_i++;
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        ret_t = is_expression();
        if (ret_t == 0) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        for (i = 0; i < tid_end; i++) {
            if (strcmp(tid_mas[i].name, matrix[row_num][saved_column_num].elem) == 0) {
                if (tid_mas[i].type != ret_t) {
                    lst_gr_i = lst_gr_i_old;
                    return 0;
                }
                break;
            }
        }
        if (i == tid_end) {
            switch(ret_t) {
                case 1: 
                    strcpy(matrix[row_num][saved_column_num].type, "int");
                    tid_mas[tid_end].type = INT_T;
                    break;
                case 2: 
                    strcpy(matrix[row_num][saved_column_num].type, "real");
                    tid_mas[tid_end].type = REAL_T;
                    break;
                case 3: 
                    strcpy(matrix[row_num][saved_column_num].type, "str");
                    tid_mas[tid_end].type = STR_T;
                    break;
                case 4: 
                    strcpy(matrix[row_num][saved_column_num].type, "list");
                    tid_mas[tid_end].type = LIST_T;
                    break;
                case 5: 
                    strcpy(matrix[row_num][saved_column_num].type, "bool");
                    tid_mas[tid_end].type = BOOL_T;
                    break;
            }
            strcpy(tid_mas[tid_end].name, matrix[row_num][saved_column_num].elem);
            tid_end++;
        }
        else {
            switch(ret_t) {
                case 1: 
                    strcpy(matrix[row_num][saved_column_num].type, "int");
                    break;
                case 2: 
                    strcpy(matrix[row_num][saved_column_num].type, "real");
                    break;
                case 3: 
                    strcpy(matrix[row_num][saved_column_num].type, "str");
                    break;
                case 4: 
                    strcpy(matrix[row_num][saved_column_num].type, "list");
                    break;
                case 5: 
                    strcpy(matrix[row_num][saved_column_num].type, "bool");
                    break;
            }
        }
        return NORETURN_T;
    }
    else if ((strcmp(lst_gr[lst_gr_i], "+=") == 0) || 
    (strcmp(lst_gr[lst_gr_i], "-=") == 0) || (strcmp(lst_gr[lst_gr_i], "*=") == 0) || 
    (strcmp(lst_gr[lst_gr_i], "/=") == 0) || (strcmp(lst_gr[lst_gr_i], "//=") == 0)) {
        int saved_column_num = column_num;
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 1], "variable");
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "assign_operation", "service");
        lst_gr_i++;
        for (i = 0; i < tid_end; i++) {
            if (strcmp(tid_mas[i].name, matrix[row_num][saved_column_num].elem) == 0) {
                break;
            }
        }
        if (i == tid_end) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        ret_t = is_expression();
        if (ret_t == 0) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (tid_mas[i].type != ret_t) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        switch(ret_t) {
            case 1: 
                strcpy(matrix[row_num][saved_column_num].type, "int");
                break;
            case 2: 
                strcpy(matrix[row_num][saved_column_num].type, "real");
                break;
            case 3: 
                strcpy(matrix[row_num][saved_column_num].type, "str");
                break;
            case 4: 
                strcpy(matrix[row_num][saved_column_num].type, "list");
                break;
            case 5: 
                strcpy(matrix[row_num][saved_column_num].type, "bool");
                break;
        }
        return NORETURN_T;
    }
    else {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
}

int is_comp_op() {
    // cout<<"is_comp_op"<<endl;
    if ((strcmp(lst_gr[lst_gr_i], "==") == 0) || (strcmp(lst_gr[lst_gr_i], "!=") == 0) || (strcmp(lst_gr[lst_gr_i], ">") == 0) || 
    (strcmp(lst_gr[lst_gr_i], ">=") == 0) || (strcmp(lst_gr[lst_gr_i], "<") == 0) || (strcmp(lst_gr[lst_gr_i], "<=") == 0)) {
        lst_gr_i++;
        return NORETURN_T;
    }
    return 0;
}

int is_primary() {
    int ret_t, i;
    // cout<<"is_primary "<<lst_gr_i<<endl;
    int lst_gr_i_old = lst_gr_i;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (strcmp(lst_gr[lst_gr_i], "(") == 0) {
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "open_circle_bracket", "service");
        lst_gr_i++;
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if ((ret_t = is_expression()) == 0) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (strcmp(lst_gr[lst_gr_i], ")") != 0) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "close_circle_bracket", "service");
        lst_gr_i++;
        return ret_t;
    }
    if ((ret_t = is_function_call()) != 0) {
        return ret_t;
    }
    if ((ret_t = is_literal()) != 0) {
        // cout<<"         "<<ret_t<<endl;
        return ret_t;
    }
    if ((ret_t = is_identifier()) != 0) {
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 1], "variable");
        for (i = 0; i < tid_end; i++) {
            if (strcmp(tid_mas[i].name, matrix[row_num][column_num - 1].elem) == 0) {
                break;
            }
        }
        if (i == tid_end) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        switch(tid_mas[i].type) {
            case INT_T: 
                strcpy(matrix[row_num][column_num - 1].type, "int");
                break;
            case REAL_T: 
                strcpy(matrix[row_num][column_num - 1].type, "real");
                break;
            case STR_T: 
                strcpy(matrix[row_num][column_num - 1].type, "str");
                break;
            case LIST_T: 
                strcpy(matrix[row_num][column_num - 1].type, "list");
                break;
            case BOOL_T: 
                strcpy(matrix[row_num][column_num - 1].type, "bool");
                break;
            default:
                lst_gr_i = lst_gr_i_old;
                return 0;
        }
        return tid_mas[i].type;
    }
    lst_gr_i = lst_gr_i_old;
    return 0;
}

int is_power() {
    int ret_t;
    int lst_gr_i_old = lst_gr_i;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    ret_t = is_primary();
    if (ret_t == 0) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (is_end()) {
        return ret_t;
    }
    lst_gr_i_old = lst_gr_i;
    while (strcmp(lst_gr[lst_gr_i], "**") == 0) {
        if (ret_t != INT_T) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "arith_operation", "service");
        lst_gr_i++;
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if ((ret_t = is_power()) != INT_T) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (is_end()) {
            return INT_T;
        }
        // lst_gr_i_old = lst_gr_i;
    }
    return ret_t;
}

int is_factor() {
    int ret_t;
    int lst_gr_i_old = lst_gr_i;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if ((strcmp(lst_gr[lst_gr_i], "+") == 0) || (strcmp(lst_gr[lst_gr_i], "-") == 0)) {
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "unary_operation", "service");
        lst_gr_i++;
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        ret_t = is_power();
        if (ret_t != INT_T && ret_t != REAL_T) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        return ret_t;
    }
    if ((ret_t = is_power()) == 0) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    return ret_t;
}

int is_term() {
    int saved_lst_gr_i;
    int ret_t, cur_ret_t;
    int lst_gr_i_old = lst_gr_i;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if ((ret_t = is_factor()) == 0) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (is_end()) {
        return ret_t;
    }
    lst_gr_i_old = lst_gr_i;
    cur_ret_t = ret_t;
    while ((strcmp(lst_gr[lst_gr_i], "*") == 0) || (strcmp(lst_gr[lst_gr_i], "/") == 0) || 
    (strcmp(lst_gr[lst_gr_i], "//") == 0) || (strcmp(lst_gr[lst_gr_i], "%") == 0)) {
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "arith_operation", "service");
        saved_lst_gr_i = lst_gr_i;
        lst_gr_i++;
        if (is_end()) {
            return 0;
        }
        if (ret_t != INT_T && ret_t != REAL_T) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        ret_t = is_factor();
        if (ret_t != INT_T && ret_t != REAL_T) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        // cout<<"now "<<lst_gr_i<<endl;
        if (strcmp(lst_gr[saved_lst_gr_i], "*") == 0) {
            cur_ret_t = (cur_ret_t == REAL_T || ret_t == REAL_T) ? REAL_T : INT_T;
        }
        else if (strcmp(lst_gr[saved_lst_gr_i], "/") == 0) {
            cur_ret_t = REAL_T;
        }
        else if (strcmp(lst_gr[saved_lst_gr_i], "//") == 0) {
            if (ret_t != INT_T) {
                lst_gr_i = lst_gr_i_old;
                return 0;
            }
            cur_ret_t = INT_T;
        }
        else {
            if (ret_t * cur_ret_t != 1) {
                lst_gr_i = lst_gr_i_old;
                return 0;
            }
            cur_ret_t = INT_T;
        }
        if (is_end()) {
            return cur_ret_t;
        }
        // lst_gr_i_old = lst_gr_i;
    }
    return cur_ret_t;
}

int is_arith_expr() {
    int saved_lst_gr_i;
    int ret_t, cur_ret_t;
    int lst_gr_i_old = lst_gr_i;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if ((ret_t = is_term()) == 0) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (is_end()) {
        return ret_t;
    }
    lst_gr_i_old = lst_gr_i;
    cur_ret_t = ret_t;
    while ((strcmp(lst_gr[lst_gr_i], "+") == 0) || (strcmp(lst_gr[lst_gr_i], "-") == 0)) {
        saved_lst_gr_i = lst_gr_i;
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "arith_operation", "service");
        lst_gr_i++;
        if (is_end()) {
            return 0;
        }
        if (ret_t < 1 || ret_t > 4) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        ret_t = is_term();
        if (ret_t < 1 || ret_t > 4) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (strcmp(lst_gr[saved_lst_gr_i], "-") == 0) {
            if (cur_ret_t < 1 || cur_ret_t > 2) {
                lst_gr_i = lst_gr_i_old;
                return 0;
            }
            if (ret_t < 1 || ret_t > 2) {
                lst_gr_i = lst_gr_i_old;
                return 0;
            }
            cur_ret_t = (cur_ret_t == REAL_T || ret_t == REAL_T) ? REAL_T : INT_T;
        }
        else {
            if (cur_ret_t == STR_T || cur_ret_t == LIST_T) {
                if (ret_t != cur_ret_t) {
                    lst_gr_i = lst_gr_i_old;
                    return 0;
                }
            }
            else {
                if (ret_t != INT_T && ret_t != REAL_T) {
                    lst_gr_i = lst_gr_i_old;
                    return 0;
                }
                cur_ret_t = (cur_ret_t == REAL_T || ret_t == REAL_T) ? REAL_T : INT_T;
            }
        }
        if (is_end()) {
            return cur_ret_t;
        }
        // lst_gr_i_old = lst_gr_i;
    }
    return cur_ret_t;
}

int is_comparisson() {
    int ret_t, cur_ret_t;
    int comp_op_counter = 0, lst_gr_i_old = lst_gr_i;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    ret_t = is_arith_expr();
    if (ret_t == 0) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (is_end()) {
        return ret_t;
    }
    cur_ret_t = ret_t;
    lst_gr_i_old = lst_gr_i;
    while (is_comp_op() != 0) {
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i - 1], "comp_operation", "service");
        comp_op_counter++; // позволяет выделить потом конструкцию с цепочками сравнения
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (ret_t > 5 || ret_t < 1) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        ret_t = is_arith_expr();
        if (ret_t > 5 || ret_t < 1) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (cur_ret_t != ret_t) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if ((strcmp(lst_gr[lst_gr_i - 2], "==") == 0) || (strcmp(lst_gr[lst_gr_i - 2], "!=") == 0)) {
            cur_ret_t = BOOL_T;
        }
        else {
            if (ret_t > 2) {
                lst_gr_i = lst_gr_i_old;
                return 0;
            }
            cur_ret_t = BOOL_T;
        }
        if (is_end()) {
            return cur_ret_t;
        }
        // lst_gr_i_old = lst_gr_i;
    }
    return cur_ret_t;
}

int is_logical_not_expression() {
    int ret_t;
    int lst_gr_i_old = lst_gr_i;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (strcmp(lst_gr[lst_gr_i], "not") == 0) {
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "unary_operation", "service");
        lst_gr_i++;
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (is_logical_not_expression() != BOOL_T) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        return BOOL_T;
    }
    ret_t = is_comparisson();
    if (ret_t == 0) {
        lst_gr_i = lst_gr_i_old;
            return 0;
    }
    return ret_t;
}

int is_logical_and_expression() {
    int ret_t;
    int lst_gr_i_old = lst_gr_i;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    ret_t = is_logical_not_expression();
    if (ret_t == 0) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (is_end()) {
        return ret_t;
    }
    lst_gr_i_old = lst_gr_i;
    while (strcmp(lst_gr[lst_gr_i], "and") == 0) {
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "bool_operation", "service");
        lst_gr_i++;
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (ret_t != BOOL_T) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        ret_t = is_logical_not_expression();
        if (ret_t != BOOL_T) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (is_end()) {
            return ret_t;
        }
        // lst_gr_i_old = lst_gr_i;
    }
    return ret_t;
}

int is_logical_or_expression() {
    int ret_t;
    int lst_gr_i_old = lst_gr_i;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    ret_t = is_logical_and_expression();
    if (ret_t == 0) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (is_end()) {
        return ret_t;
    }
    lst_gr_i_old = lst_gr_i;
    while (strcmp(lst_gr[lst_gr_i], "or") == 0) {
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "bool_operation", "service");
        lst_gr_i++;
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (ret_t != BOOL_T) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        ret_t = is_logical_and_expression();
        if (ret_t != BOOL_T) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (is_end()) {
            return ret_t;
        }
        // lst_gr_i_old = lst_gr_i;
    }
    return ret_t;
}

int is_expression() {
    int ret_t;
    // cout<<"is_expression "<<lst_gr_i<<endl;
    int lst_gr_i_old = lst_gr_i;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    ret_t = is_logical_or_expression();
    if (ret_t != 0) {
        return ret_t;
    }
    lst_gr_i = lst_gr_i_old;
    return 0;
}

int is_compound_statement(int deep) {
    // cout<<"is_compound_statement"<<endl;
    int lst_gr_i_old = lst_gr_i;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (is_if_statement(deep) != 0) {
        return 1;
    }
    if (is_for_statement(deep) != 0) {
        return 1;
    }
    return is_while_statement(deep);
}

int is_simple_statement() {
    // cout<<"is_simple_statement"<<endl;
    int lst_gr_i_old = lst_gr_i;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (is_assignment() != 0) {
        return 1;
    }
    if (is_expression() != 0) {
        return 1;
    }
    if (strcmp(lst_gr[lst_gr_i], "return") == 0) {
        if (func_level == 0) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "return", "service_command");
        lst_gr_i++;
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (is_expression() == 0) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        return 1;
    }
    else if (strcmp(lst_gr[lst_gr_i], "break") == 0) {
        if (cycle_level == 0) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "break", "service_command");
        lst_gr_i++;
        return 1;
    }
    else if (strcmp(lst_gr[lst_gr_i], "continue") == 0) {
        if (cycle_level == 0) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        matrix[row_num][column_num++] = Lexem(lst_gr[lst_gr_i], "continue", "service_command");
        lst_gr_i++;
        return 1;
    }
    lst_gr_i = lst_gr_i_old;
    return 0;
}

int is_statement(int deep) {
    // cout<<"is_statement"<<endl;
    int lst_gr_i_old = lst_gr_i;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    if (is_function_definition(deep) != 0) {
        return 1;
    }
    if (is_compound_statement(deep) != 0) {
        return 1;
    }
    if (is_simple_statement() != 0) {
        if (is_end()) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (strcmp(lst_gr[lst_gr_i], "\n") == 0) { // после простого выражения должен быть \n
            lst_gr_i++;
            matrix[row_num++][column_num] = Lexem("\\n", "NEWLINE", "service");
            column_num = 0;
            return 1;
        }
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    lst_gr_i = lst_gr_i_old;
    return 0;
}

int is_program() {
    // cout<<"is_program"<<endl;
    int lst_gr_i_old = lst_gr_i;
    if (is_end()) {
        lst_gr_i = lst_gr_i_old;
        return 0;
    }
    while (1) {
        while (strcmp(lst_gr[lst_gr_i], "\n") == 0) { // пропускаем незначащие \n
            lst_gr_i++;
            if (is_end()) {
                return 1;
            }
        }
        if (is_statement(0) == 0) {
            lst_gr_i = lst_gr_i_old;
            return 0;
        }
        if (is_end()) { // верный выход - только если в итоге попали на конец
            return 1;
        }
    }
}


int main() {
    int i, j;
    char str_test[100];
    cout<<"Введите тест: ";
    cin>>str_test;
    lst_copy = lex_analyzer(str_test); // получаем список лексем
    // printlist();
    size_l = ret_sizelst();
    
    strcpy(func_mas[func_end].name, "print");
    func_mas[func_end].argc = -1; // для любого кол-ва - пока так
    func_mas[func_end].type = NORETURN_T;
    func_end++;
    
    strcpy(func_mas[func_end].name, "len");
    func_mas[func_end].argc = 1;
    func_mas[func_end].type = INT_T;
    strcpy(func_mas[func_end].args_mas[0].name, "x");
    func_mas[func_end].args_mas[0].type = STR_T;
    func_end++;
    
    strcpy(func_mas[func_end].name, "len");
    func_mas[func_end].argc = 1;
    func_mas[func_end].type = INT_T;
    strcpy(func_mas[func_end].args_mas[0].name, "x");
    func_mas[func_end].args_mas[0].type = LIST_T;
    func_end++;
    
    strcpy(func_mas[func_end].name, "input");
    func_mas[func_end].argc = 0;
    func_mas[func_end].type = STR_T;
    func_end++;
    
    strcpy(func_mas[func_end].name, "int");
    func_mas[func_end].argc = 1;
    func_mas[func_end].type = INT_T;
    strcpy(func_mas[func_end].args_mas[0].name, "x");
    func_mas[func_end].args_mas[0].type = STR_T;
    func_end++;
    
    strcpy(func_mas[func_end].name, "reverse");
    func_mas[func_end].argc = 0;
    func_mas[func_end].type = NORETURN_T;
    func_mas[func_end].args_mas[0].type = LIST_T;
    func_end++;
    
    strcpy(func_mas[func_end].name, "pop");
    func_mas[func_end].argc = 1;
    func_mas[func_end].type = NORETURN_T;
    strcpy(func_mas[func_end].args_mas[0].name, "i");
    func_mas[func_end].args_mas[0].type = LIST_T;
    func_mas[func_end].args_mas[1].type = INT_T;
    func_end++;
    
    strcpy(func_mas[func_end].name, "insert");
    func_mas[func_end].argc = 2;
    func_mas[func_end].type = NORETURN_T;
    strcpy(func_mas[func_end].args_mas[0].name, "i");
    strcpy(func_mas[func_end].args_mas[1].name, "x");
    func_mas[func_end].args_mas[0].type = LIST_T;
    func_mas[func_end].args_mas[1].type = INT_T;
    func_mas[func_end].args_mas[2].type = NULL_T; // VAR
    func_end++;
    
    strcpy(func_mas[func_end].name, "append");
    func_mas[func_end].argc = 1;
    func_mas[func_end].type = NORETURN_T;
    strcpy(func_mas[func_end].args_mas[0].name, "x");
    func_mas[func_end].args_mas[0].type = LIST_T;
    func_mas[func_end].args_mas[1].type = NULL_T; // VAR
    func_end++;
    
    strcpy(func_mas[func_end].name, "split");
    func_mas[func_end].argc = 0;
    func_mas[func_end].type = LIST_T;
    func_mas[func_end].args_mas[0].type = STR_T;
    func_end++;
    
    strcpy(func_mas[func_end].name, "count");
    func_mas[func_end].argc = 1;
    func_mas[func_end].type = INT_T;
    func_mas[func_end].args_mas[0].type = STR_T;
    func_mas[func_end].args_mas[1].type = STR_T;
    func_end++;
    
    strcpy(func_mas[func_end].name, "find");
    func_mas[func_end].argc = 1;
    func_mas[func_end].type = INT_T;
    func_mas[func_end].args_mas[0].type = STR_T;
    func_mas[func_end].args_mas[1].type = STR_T;
    func_end++;
    
    strcpy(func_mas[func_end].name, "sort");
    func_mas[func_end].argc = 0;
    func_mas[func_end].type = NORETURN_T;
    func_mas[func_end].args_mas[0].type = LIST_T;
    func_end++;
    
    try {
        for (i = 0; i < size_l - 1; i++) { // в цикле обрабатываем все лексемы, причем на каждом шаге - ровно одна лексема
            // проверка, установлены ли сейчас флаги комментариев или строк
            if (comment_flag == 1) {
                if (strcmp(lst_copy[i], "'''") == 0) {
                    comment_flag = 0;
                }
                continue;
            }
            else if (comment_flag == 2) {
                if (strcmp(lst_copy[i], "\"\"\"") == 0) {
                    comment_flag = 0;
                }
                continue;
            }
            else if (comment_flag == 3) { // значение флага при #
                if (strcmp(lst_copy[i], "\n") == 0) {
                    comment_flag = 0;
                }
                continue;
            }
            else if (str_flag == 1) {
                if (strcmp(lst_copy[i], "'") == 0) {
                    str_flag = 0;
                    lst_gr = (char**)realloc(lst_gr, (++lst_gr_i)*sizeof(*lst_gr)); // сразу сохраняем лексему в новый список - без лишних символов
                    lst_gr[lst_gr_i-1] = lst_copy[i];
                    continue;
                }
            }
            else if (str_flag == 2) {
                if (strcmp(lst_copy[i], "\"") == 0) {
                    str_flag = 0;
                    lst_gr = (char**)realloc(lst_gr, (++lst_gr_i)*sizeof(*lst_gr)); // аналогично
                    lst_gr[lst_gr_i-1] = lst_copy[i];
                    continue;
                }
            }
            
            if (str_flag > 0) { // если есть флаг строки - сохраняем любую лексему
                lst_gr = (char**)realloc(lst_gr, (++lst_gr_i)*sizeof(*lst_gr));
                lst_gr[lst_gr_i-1] = lst_copy[i];
                continue;
            }
            
            // установка флагов
            if (strcmp(lst_copy[i], "'''") == 0) {
                comment_flag = 1;
                continue;
            }
            else if (strcmp(lst_copy[i], "\"\"\"") == 0) {
                comment_flag = 2;
                continue;
            }
            else if (strcmp(lst_copy[i], "#") == 0) {
                comment_flag = 3;
                continue;
            }
            else if (strcmp(lst_copy[i], "'") == 0) {
                str_flag = 1;
            }
            else if (strcmp(lst_copy[i], "\"") == 0) {
                str_flag = 2;
            }
            
            // установка флага перевода строки, чтобы отбрасывать ненужные пробелы
            if (strcmp(lst_copy[i], "\n") == 0)
                newline_flag = 1;
            else if (strcmp(lst_copy[i], " ") != 0)
                newline_flag = 0;
            
            if (strcmp(lst_copy[i], " ") == 0 && newline_flag == 0) // если не было перевода строки, то пробелы будут пропускаться
                continue;
            
            // если лексема - спец. символ, то сохраняем
            for (j = 0; j < NUM_OF_SPEC_SYMBOLS; j++) {
                if (strcmp(lst_copy[i], spec_symbols[j]) == 0) {
                    break;
                }
            }
            if (j < NUM_OF_SPEC_SYMBOLS) {
                if (strcmp(lst_copy[i], spec_symbols[j]) == 0) {
                    lst_gr = (char**)realloc(lst_gr, (++lst_gr_i)*sizeof(*lst_gr));
                    lst_gr[lst_gr_i-1] = lst_copy[i];
                    continue;
                }
            }
            
            // если лексема начинается как слово, то сохраняем (это идентификатор или спец. слово)
            if (is_letter(lst_copy[i][0])) {
                lst_gr = (char**)realloc(lst_gr, (++lst_gr_i)*sizeof(*lst_gr));
                lst_gr[lst_gr_i-1] = lst_copy[i];
                continue;
            }
            
            j = 0;
            if ('0' > lst_copy[i][0] || lst_copy[i][0] > '9') { // если лексема начинается и не с цифры, то эта лексема - не часть грамматики - выбрасываем исключение
                throw lst_copy[i];
            }
            while (lst_copy[i][j] != '\0') {
                if (is_letter(lst_copy[i][j])) { // если же лексема начинается с цифер, но потом встречается буква - тоже не часть грамматики
                    throw lst_copy[i];
                }
                j++;
            }
            
            lst_gr = (char**)realloc(lst_gr, (++lst_gr_i)*sizeof(*lst_gr)); // если же все в порядке - сохраняем в новый массив
            lst_gr[lst_gr_i-1] = lst_copy[i];
        } // конец цикла обработки лексем
        
        if (comment_flag + str_flag > 0) // если на выходе строки или комментарии не зкрыты - ошибка
            throw comment_flag * 10 + str_flag;
        
        lst_gr_len = lst_gr_i;
        lst_gr_i = 0;
        // cout<<"LEN = "<<lst_gr_len<<endl;
        if (is_program() != 0) { // запуск синтаксического анализатора
            cout<<"Program is OK!"<<endl;
        }
        else {
            cout<<"ERROR!"<<endl;
        }
        /*
        for (i=0; i<=lst_gr_len-1; i++) {
            if (lst_gr[i][0] == '\n') {
                cout<<"\\n"<<" "<<i<<endl;
            }
            else {
                if (lst_gr[i][0] == ' ') {
                    cout<<"@"<<" "<<i<<endl;
                }
                else {
                    cout<<lst_gr[i]<<" "<<i<<endl;
                }
            }
        }
        */
    } // далее - различные реакции на пойманные исключения
    catch (int flag_sym) {
        cout<<"ERROR - unclosed comment/string ("<<flag_sym<<")"<<endl;
    }
    catch (char *error_word) {
        cout<<"ERROR on symbol: "<<error_word<<endl;
    }
    
    for (i = 0; i < row_num; i++) {
        j = 0;
        cout<<i<<"   ";
        while (strcmp(matrix[i][j].type_name, "NULL") != 0) {
            cout<<"{"<<matrix[i][j].elem<<"; "<<matrix[i][j].type_name<<"; "<<matrix[i][j].type;
            if (matrix[i][j].begin_row_num != -1)
                cout<<"; "<<matrix[i][j].begin_row_num<<"; "<<matrix[i][j].end_row_num;
            cout<<"} ";
            j++;
        }
        cout<<endl;
    }
    cout<<endl;
    for (i = 0; i < tid_end; i++) {
        cout<<tid_mas[i].name<<" "<<tid_mas[i].type<<endl;
    }
    
    free(lst_gr);
    clearlist();
    return 0;
}

// # , . / // ' " [ ] < <= > >= = == != % * ** + - += -= *= /= //= ( ) # ''' """
