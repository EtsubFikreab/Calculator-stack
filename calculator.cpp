/*
 ----------------------------
|Group Members (DRB 2002)	|
|---------------------------|
|                           |
|Bereket Alemayehu - GH7511	|
|                           |
|Etsub Fikreab - OG8653		|
|                           |
-----------------------------
 */
#include "stack.cpp"
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <sstream>
#include <cmath>
const float PI = 3.14159265;
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
int mainMenuChoice();
void help();
int main()
{
    int choice;
    do
    {
        choice = mainMenuChoice();
        if (choice == 1)
        {
            std::cout << "Enter a mathematical expression:\n";
            std::string i;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(std::cin, i);
            std::string p;
            if (toPostfix(i, p) == -1)
            {
                std::cout << "There is a syntactical error in the expression,\n"
                             "please rerun the program after correcting the expression\n"
                             "Press enter to continue... ";
                std::cin.get();
            }
            else if (evaluate(p) == -1)
            {
                std::cout << "Sorry, the expression could not be evaluated\n"
                             "Press enter to continue... ";
                std::cin.get();
            }
        }
        else if (choice == 2)
        {
            help();
        }
    } while (choice != 3);
}

int mainMenuChoice()
{
    int choice;
    do
    {
        std::cout << "Main Menu\n"
                     "----------\n"
                     "1. Evaluate Expression\n"
                     "2. Help\n"
                     "3. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        if (!std::cin)
        {
            std::cout << "\nInvalid input, press Enter to try again... ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
        }
        system("cls");
    } while (!std::cin);
    return choice;
}
void help()
{
    std::cout << "List of operators\n"
                 "------------------\n"
                 "+ addition\n"
                 "- subtraction\n"
                 "* multiplication\n"
                 "/ division\n"
                 "( open parenthesis\n"
                 ") close parenthesis\n"
                 "^ Exponent\n"
                 "s sin\n"
                 "c cos\n"
                 "t tan\n"
                 "a absolute\n"
                 "# square root\n";

    std::cout << "\nPress enter to continue... ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
    system("cls");
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
    double num;
    while (var >> s)
    {
        if (isdigit(s[0]))
        {
            result.push(stod(s));
        }
        else
        {
            op = s[0];
            num = result.pop();
            switch (op)
            {
            case ADD:
                result.push(num + result.pop());
                break;
            case SUB:
                result.push(result.pop() - num);
                break;
            case MULT:
                result.push(num * result.pop());
                break;
            case DIV:
                result.push(result.pop() / num);
                break;
            case EXP:
                result.push(pow(result.pop(), num));
                break;
            case ABS:
                result.push(abs(num));
                break;
            case SQRT:
                result.push(sqrtf(num));
                break;
            case SIN:
                result.push(sin(num * PI / 180));
                break;
            case COS:
                result.push(cos(num * PI / 180));
                break;
            case TAN:
                result.push(tan(num * PI / 180));
                break;

            default:
                return -1;
                break;
            }
        }
    }

    std::cout << "Result = " << result.pop() << std::endl;
    return 0;
}