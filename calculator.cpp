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

    std::string i = "1-2^3^3-(4+5*6)*7";
    //std::string i = "1+7";
    std::string p;
    std::cout << toPostfix(i, p) << std::endl;
    std::cout << p;
    std::cout << evaluate(p) << std::endl;
}

int symbolType(char symbol)
{
    if (
        symbol == CPAR || symbol == ADD || symbol == SUB || symbol == MULT ||
        symbol == DIV || symbol == EXP || symbol == OPAR || symbol == SIN ||
        symbol == COS || symbol == TAN || symbol == SQRT || symbol == ABS)
        return OPERATOR;
    else if (isdigit(symbol) || symbol == '.')
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
    char ch;
    Stack<char> op;
    int type, cmp;
    for (int i = 0; i < infix.length(); i++)
    {
        ch = infix[i];
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
                {
                    postfix += " ";
                    postfix += op.pop();
                }
                op.pop();
            }
            else
            {
                while (!op.isEmpty() && cmpPrec(op.peep(), ch) >= 0)
                {
                    if (associativity(ch) == 1 && associativity(op.peep()) == 1)
                        break;
                    else
                    {
                        postfix += " ";
                        postfix += op.pop();
                    }
                }
                postfix += " ";
                op.push(ch);
            }
        }
    }
    while (!op.isEmpty())
    {
        postfix += " ";
        postfix += op.pop();
        postfix += " ";
    }
    return 0;
}
int evaluate(std::string postfix)
{
    std::stringstream var(postfix);
    std::string s;
    char op;
    Stack<double> result;
    while (var >> s)
    {
        if (isdigit(s[0]))
        {
            result.push(stod(s));
        }
        else
        {
            op = s[0];
            switch (op)
            {
            case ADD:
                result.push(result.pop() + result.pop());
                break;
            case SUB:
                result.push(result.pop() - result.pop());
                break;
            case MULT:
                result.push(result.pop() * result.pop());
                break;
            case DIV:
                result.push(result.pop() / result.pop());
                break;
            case EXP:
                result.push(pow(result.pop(), result.pop()));
                break;
            case ABS:
                result.push(abs(result.pop()));
                break;
            case SQRT:
                result.push(sqrtf(result.pop()));
                break;
            case SIN:
                result.push(sin(result.pop()));
                break;
            case COS:
                result.push(cos(result.pop()));
                break;
            case TAN:
                result.push(tan(result.pop()));
                break;

            default:
                break;
            }
        }
    }

    std::cout<<"Result = "<<result.pop();
    return 0;
}