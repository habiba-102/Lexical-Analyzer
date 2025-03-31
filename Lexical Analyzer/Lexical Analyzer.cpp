#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>

using namespace std;

int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;
ifstream in_fp;

#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26

/*These symbolic constants represent different types of tokens that can be recognized
during lexical analysis. Each constant is assigned a unique integer value for easy
identification and processing during tokenization.*/

void addChar();
void getChar();
void getNonBlank();
int lex();
int lookup(char ch);

void addChar()
{
    if (lexLen <= 98) //ensuring that lexeme doesn't exceed 99 characters is to prevent buffer overflow
    {
        lexeme[lexLen++] = nextChar; // Add the character to the lexeme
        lexeme[lexLen] = '\0'; // Null-terminate the string
    }
    else
        cout << "Error - lexeme is too long" << endl;
}

void getChar() {
    if (in_fp.get(nextChar)) // Read next character from file
    {
        if (isalpha(nextChar))  // If it's a letter (A-Z or a-z)
            charClass = LETTER;
        else if (isdigit(nextChar)) // If it's a digit (0-9)
            charClass = DIGIT;
        else
            charClass = UNKNOWN;  // If it's neither, classify as UNKNOWN
    }
    else
        charClass = EOF;  // If End Of File is reached
}

void getNonBlank()
{
    while (isspace(nextChar))
        getChar(); //skip space, tab, or newline
}

int lookup(char ch)
{
    /*Determines the token type for special characters
    such as +, -, *, /, (, and )*/

    switch (ch)
    {
    case '(':
        addChar();
        nextToken = LEFT_PAREN;
        break;
    case ')':
        addChar();
        nextToken = RIGHT_PAREN;
        break;
    case '+':
        addChar();
        nextToken = ADD_OP;
        break;
    case '-':
        addChar();
        nextToken = SUB_OP;
        break;
    case '*':
        addChar();
        nextToken = MULT_OP;
        break;
    case '/':
        addChar();
        nextToken = DIV_OP;
        break;
    default:
        addChar();
        nextToken = EOF;
        break;
    }
    return nextToken;
}

int lex()
{
    lexLen = 0;
    getNonBlank();
    switch (charClass)
    {
    case LETTER: // If the first character is a letter, it's an identifier
        addChar();
        getChar();
        while (charClass == LETTER || charClass == DIGIT)
            // Identifiers can contain letters and digits
        {
            addChar();
            getChar();
        }
        nextToken = IDENT;
        break;

    case DIGIT: // If the first character is a digit, it's an integer literal
        addChar();
        getChar();
        while (charClass == DIGIT)
            // Integer literals contain only digits
        {
            addChar();
            getChar();
        }
        nextToken = INT_LIT;
        break;

    case UNKNOWN:
        lookup(nextChar); //check if it's an operator
        getChar();
        break;

    case EOF: // End of file
        nextToken = EOF;
        strcpy_s(lexeme, sizeof(lexeme), "EOF"); // Set lexeme to "EOF"
        break;
    }
    cout << "Next token is: " << nextToken << ", Next lexeme is " << lexeme << endl;
    return nextToken;
}

int main()
{
    in_fp.open("front.in");

    if (!in_fp) // Check if file exists
    {
        cout << "ERROR - cannot open front.in" << endl;
        return 1; // Exit with error code
    }

    getChar(); //Get first character from file

    do
    {
        lex();
    } while (nextToken != EOF);

    in_fp.close();
    return 0;
}