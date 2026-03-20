#include <iostream>

using namespace std;

// -- LOGINIAI ELEMENTAI --
bool NAND(bool x, bool y)
{
    if (x == 1 && y == 1)return 0;
    return 1;
}
bool NOT(bool x)
{
    return NAND(x, x);
}
bool AND(bool x, bool y)
{
    bool nand1 = NAND(x, y);
    return NAND(nand1, nand1);
}
bool OR(bool x, bool y)
{
    bool nand1 = NAND(x, x);
    bool nand2 = NAND(y, y);
    return NAND(nand1, nand2);
}
bool NOR(bool x, bool y)
{
    bool nand1 = NAND(x, x);
    bool nand2 = NAND(y, y);
    bool nand3 = NAND(nand1, nand2);
    return NAND(nand3, nand3);
}
bool XOR(bool x, bool y)
{
    bool nand2 = NAND(x, y);
    bool nand1 = NAND(x,nand2);
    bool nand3 = NAND(y,nand2);
    bool nand4 = NAND(nand1, nand3);
    return nand4;
}

bool Mux(bool fulladd, bool invers, bool oras, bool xoras, bool M0, bool M1)
{
    bool notM0 = NOT(M0);
    bool notM1 = NOT(M1);

    bool and1 = AND(fulladd, AND(M1, M0));
    bool and2 = AND(invers, AND(M1, notM0));
    bool and3 = AND(oras, AND(notM1, M0));
    bool and4 = AND(xoras, AND(notM1, notM0));

    bool ORx4 = OR(OR(and1, and2), OR(and3, and4));
    return ORx4;
}

// ----- 1 BITO SUMA ------

void Half_adder(bool x, bool y, bool &sum, bool &carry)
{
    sum = XOR(x, y);
    carry = AND(x, y);
}
void Full_adder(bool x, bool y, bool CarryIN, bool &CarryOUT, bool &SUM)
{
    bool carry1=0;
    bool carry2 = 0;
    bool sum1=0;

    Half_adder(x, y, sum1, carry1);
    Half_adder(sum1, CarryIN, SUM, carry2);

    CarryOUT = OR(carry1, carry2);
}

// ----- 1 BITO ALU ------

void ALU1bit(bool x, bool y, bool ENA, bool ENB, bool M0, bool M1, bool CarryIN, bool& CarryOUT, bool& Out)
{
    x = AND(x, ENA);
    y = AND(y, ENB);

    bool xoras = XOR(x, y);
    bool oras = OR(x, y);
    bool notas = NOT(y);

    bool SUM;
    Full_adder(x, y, CarryIN, CarryOUT, SUM);

    Out = Mux(SUM, notas, oras, xoras, M0, M1);
}

// ----- 8 BIT OP ------

void DECODER3to8(bool D[3], bool O[8])
{
    O[0]= AND(AND(NOT(D[0]),NOT(D[1])),NOT(D[2])); // 000

    O[1]= AND(AND(NOT(D[0]), NOT(D[1])), D[2]); // 001
    O[2]= AND(AND(NOT(D[0]), D[1]), NOT(D[2])); // 010
    O[3]= AND(AND(NOT(D[0]), D[1]), D[2]); // 011

    O[4]= AND(AND(D[0], NOT(D[1])), NOT(D[2])); // 100
    O[5]= AND(AND(D[0], NOT(D[1])), D[2]); // 101
    O[6]= AND(AND(D[0], D[1]), NOT(D[2])); // 110

    O[7]= AND(AND(D[0], D[1]), D[2]); // 111
}

void Shifter8bit(bool A[8], bool ctrl, bool O[8])
{
    O[0] = AND(NOT(ctrl), A[1]);
    O[1] = OR(AND(A[0],ctrl), AND(A[2],NOT(ctrl)));
    O[2] = OR(AND(A[1],ctrl), AND(A[3],NOT(ctrl)));
    O[3] = OR(AND(A[2], ctrl), AND(A[4], NOT(ctrl)));
    O[4] = OR(AND(A[3], ctrl), AND(A[5], NOT(ctrl)));
    O[5] = OR(AND(A[4], ctrl), AND(A[6], NOT(ctrl)));
    O[6] = OR(AND(A[5], ctrl), AND(A[7], NOT(ctrl)));
    O[7] = AND(ctrl, A[6]);

}

void Equal(bool A[8], bool B[8], bool CarryIN, bool CarryOUT, bool Out[8], bool &F)
{
    for (int i = 0; i < 8; i++)
    {
        ALU1bit(A[i], B[i], 1, 1, 0, 0, CarryIN, CarryOUT, Out[i]);
    }

    F = NOR(OR(Out[1], OR(Out[3], OR(Out[5], Out[7]))), OR(Out[2], OR(Out[4], OR(Out[6], Out[0]))));
}

// ----- 8 BIT ALU ------

void ALU8bit(bool A[8], bool B[8], bool D[3], bool Cntr, bool Out[8], bool& Cflag, bool& F)
{
    bool Dec[8];
    DECODER3to8(D, Dec);
    bool CarryIN = 0, CarryOUT = 0;

    if (Dec[0]) // 000 
    {
        cout << "000 - nera operacijos";
    }
    if (Dec[1]) // 001
    {
        cout << "001 - A==B";

        Equal(A, B, CarryIN, CarryOUT, Out, F);
    }
    if (Dec[2]) // 010
    {
        cout << "010 - nera operacijos";
    }
    if (Dec[3]) // 011
    {
        cout << "011 - A-B";

        bool notB[8];
        for (int i = 0; i < 8; i++)
        {
            ALU1bit(A[i], B[i], 0, 1, 0, 1, CarryIN, CarryOUT, notB[i]);
        }
        CarryIN = 1;
        for (int i = 7; i >= 0; i--)
        {
            ALU1bit(A[i], notB[i], 1, 1, 1, 1, CarryIN, CarryOUT, Out[i]);
            CarryIN = CarryOUT;
        }
        Cflag = CarryOUT;
    }
    if (Dec[4]) // 100
    {
        cout << "100 - nera operacijos";
    }
    if (Dec[5]) // 101
    {
        cout << "101 - A>>1, A<<1";
        Shifter8bit(A, Cntr, Out);
    }
    if (Dec[6]) // 110
    {
        cout << "110 - A+B";

        for (int i = 7; i >= 0; i--)
        {
            ALU1bit(A[i], B[i], 1, 1, 1, 1, CarryIN, CarryOUT,Out[i]);
            CarryIN = CarryOUT;
        }
        Cflag = CarryOUT;
    }
    if (Dec[7]) // 111
    {
        cout << "111 - A*B";
    }
}

int main()
{
    bool A[8] = { 0,0,0,1,1,1,1,1 };
    bool B[8] = { 0,0,0,1,1,1,1,1 };
    bool D[3] = { 0,0,1 };
    bool cntr = 1;

    bool O[8] = { 0,0,0,0,0,0,0,0 };
    bool F = 0;
    bool carry = 0;

    ALU8bit(A, B, D, cntr, O, carry, F);

    std::cout << "\nA: ";
    for (int i = 0; i < 8; i++)
        std::cout << A[i];

    std::cout << "\nB: ";
    for (int i = 0; i < 8; i++)
        std::cout << B[i];

    std::cout << "\n\nOutput: ";
    for (int i = 0; i < 8; i++)
        std::cout << O[i];

    std::cout << "\nF: " << F;
    std::cout << "\ncarry: " << carry;
    cout << endl;

    return 0;
}

