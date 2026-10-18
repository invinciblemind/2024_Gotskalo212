#include "matrix.h"
#include <iostream>
using namespace std;
// using namespace Class_def;

int main() {
    try {
        // Test 1
        Matrix m1(3, 7);
        cout<<"Test 1 - m1(3, 7):\n"<<m1;
        
        // Test 2
        Matrix m2(4.76);
        cout<<"Test 2 - m2(4.76):\n"<<m2;
        
        // Test 3
        double vals3[] = {1, 2.3, 4.56, -0.001, 999.999};
        Matrix m3(vals3, 5);
        cout<<"Test 3 - m3(vals3, 5):\n"<<m3;
        
        // Test 4
        double vals4[] = {1, -1, 4.76};
        Matrix m4(3, vals4);
        cout<<"Test 4 - m4(3, vals4):\n"<<m4;
        
        // Test 5
        Matrix m5("{{1, 0, 0}, {0, 1, 0.5}}");
        cout<<"Test 5 - m5(\"{{1, 0, 0}, {0, 1, 0.5}}\"):\n"<<m5;
        
        // Test 6
        Matrix m6;
        m6 = Matrix::identity(10);
        cout<<"Test 6 - identity(10):\n"<<m6;
        
        // Test 7
        Matrix m7;
        double vals7[] = {4, 2, 1, 0.5, 0.25, 0, -1, -4};
        m7 = Matrix::diagonal(vals7, 8);
        cout<<"Test 7 - diagonal(vals7, 8):\n"<<m7;
        
        // Test 8
        cout<<"Test 8 - rows of m1:\n"<<m1.rows()<<endl<<endl;
        
        // Test 9
        cout<<"Test 9 - columns of m1:\n"<<m1.columns()<<endl<<endl;
        
        // Test 10
        Matrix m10(m1);
        m10.set(1, 1, 1);
        m10.set(1, 7, 2);
        m10.set(3, 1, 0.5);
        m10.set(3, 7, -1);
        cout<<"Test 10 - set:\n"<<m10;
        
        // Test 11
        cout<<"Test 11 - operation [] on m5:\n"<<m5[1]<<m5[2]<<m5[3]<<m5[1][1]<<m5[2][3];
        
        // Test 12
        Matrix m12;
        m12 = m5;
        cout<<"Test 12 - operation * and *= on m5:\n"<<m12*1<<m12*2<<m12*0.5;
        m12 *= 4;
        cout<<m12;
        m12 *= -0.25;
        cout<<m12;
        
        int num_of_test = 0;
        cout<<"Enter 0 or number of test to try exception (you can try test 1, 5, 10 and 11): ";
        cin>>num_of_test;
        int m, n, k, num, i, j, i1, i2;
        char s[256];
        if (num_of_test == 1) {
                cout<<"Enter the sizes of massive: ";
                cin>>n>>m;
                Matrix m1_1(n, m);
                printf("Test 1_1 - m1_1(%d, %d):\n", n, m);
                cout<<m1_1;
        }
        else if (num_of_test == 5) {
                cout<<"Enter the string form of massive: ";
                cin.getline(s, 256);
                cin.getline(s, 256);
                cout<<"**"<<s<<"**";
                Matrix m5_1(s);
                printf("Test 5_1 - m5_1(\"%s\"):\n", s);
                cout<<m5_1;
        }
        else if (num_of_test == 10) {
                Matrix m10_1(10, 10);
                cout<<"Enter the num of set elements: ";
                cin>>k;
                for (i = 1; i <= k; i++) {
                    cout<<"Enter the indexes and new number of set element (base matrix iz zero-filled, 10x10): ";
                    cin>>n>>m>>num;
                    m10_1.set(n, m, num);
                }
                cout<<"Test 10_1 - set:\n"<<m10_1;
        }
        else if (num_of_test == 11) {
                Matrix m11_1("{{1, 2, 3, 4, 5, 6, 7}, {0.1, 0.2, 0.4, 0.8, 1.6, 3.2, 6.4}, {0, 0, 0, 0, 0, 0, 0}, {-0.009, -0.09, -0.9, -9, -99, -999, -9999}, {1, 0, 1, 0, 1, 0, 1}}");
                cout<<"Enter the index of row or column (twice) and two indexes of one element (you have matrix 5x7): ";
                cin>>i1>>i2>>i>>j;
                cout<<"Test 11_1 - []:\n"<<m11_1[i1]<<m11_1[i2]<<m11_1[i][j];
        }
    }
    catch (Errors & exception) {
        cerr<<exception.get_error_msg();
    }
    
    return 0;
}
