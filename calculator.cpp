#include "stack.cpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
enum symbolType
{
    OPERATOR,
    OPERAND,
    UNKNOWN
}; // defining symbol types in expression
enum preced
{
    PCPAR = 0,
    PADD = 10,
    PMULT = 15,
    PEXP = 20,
    PSIGN = 25, // SIN COS TAN
    POPAR = 30
}; // defining precedance of operators
enum Operators
{
    CPAR = ')',
    ADD = '+',
    SUB = '-',
    MULT = '*',
    DIV = '/',
    EXP = '^',
    OPAR = '(',
    ABS = 'a',
    SQRT = '#',
    SIN = 's',
    COS = 'c',
    TAN = 't'
}; // operators
int symbolType(char symbol);
int precValue(char symbol);
int cmpPrec(char oprStk, char oprInp);
int associativity(char opr);
int toPostfix(std::string infix, std::string &postfix);
int evaluate(std::string postfix);

int main()
{
    // Stack<double> s;
    // s.push(12);
    // s.push(41.22);
    // s.push(14566.3);
    // std::cout<<s.peep();
    //  std::cout<<symbolType('a')<<std::endl;
    // std::string i = "1+3";
    std::string i = "1+2^3-(4+5*6)*7+1";
    //std::string i = "1-2^3^3-(4+5*6)*7";
    std::string p;
    std::cout << toPostfix(i, p) << std::endl;
    std::cout << p;
}

int symbolType(char symbol)
{
    if (
        symbol == CPAR || symbol == ADD || symbol == SUB || symbol == MULT ||
        symbol == DIV || symbol == EXP || symbol == OPAR || symbol == SIN ||
        symbol == COS || symbol == TAN || symbol == SQRT || symbol == ABS)
        return OPERATOR;
    else if (isdigit(symbol))
        return OPERAND;
    return UNKNOWN;
}
int precValue(char symbol)
{
    if (symbol == CPAR)
        return PCPAR;
    else if (symbol == ADD || symbol == SUB)
        return PADD;
    else if (symbol == MULT || symbol == DIV)
        return PMULT;
    else if (symbol == EXP)
        return PEXP;
    else if (symbol == SIN || symbol == COS || symbol == TAN || symbol == SQRT || symbol == ABS)
        return PSIGN;
    return -1;
}
int cmpPrec(char oprStk, char oprInp)
{
    if (precValue(oprStk) < precValue(oprInp))
        return -1;
    else if (precValue(oprStk) == precValue(oprInp))
        return 0;
    return 1;
}
int associativity(char opr)
{
    if (opr == EXP || opr == SIN || opr == COS || opr == TAN || opr == SQRT)
        return 1;
    return 2;
}
int toPostfix(std::string infix, std::string &postfix)
{
    postfix = "";
    std::stringstream inp;
    inp.str(infix);
    char ch;
    Stack<char> op;
    int type, cmp;

    while (inp.get(ch))
    {
        std::cout<<ch<<" ";
        // skip white spaces
        if (ch == ' ')
            continue;
        type = symbolType(ch);
        if (type == OPERAND)
            postfix += ch;
        else if (type == UNKNOWN)
            return -1;
        else
        {
            if (ch == OPAR)
                op.push(ch);
            else if (ch == CPAR)
            {
                while (op.peep() != OPAR)
                    postfix += op.pop();
                op.pop();
            }
            else
            {
                while (!op.isEmpty() && cmpPrec(op.peep(), ch) >= 0)
                {
                    if (associativity(ch) == 1 && associativity(op.peep()) == 1)
                        break;
                    else
                        postfix += op.pop();
                }
                op.push(ch);
            }
        }
    }
    while (!op.isEmpty())
        postfix += op.pop();
    if (inp.bad())
        return -1;
    return 0;
}
// int evaluate(string  postfix);