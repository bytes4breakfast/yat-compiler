#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

//Various ASCII codes as constants.
static char TAB = (char) 9;

//Global variable to store the character currently being processed
char lookahead;

//Read a character from input stream
void ReadChar(char* c1)
{
   *c1 = getchar();

}

//Notify of an error
void ErrorNotify( char* message)
{
    printf("%s\n", message);
}

//Notify and exit the program
void ErrorHalt(char* message)
{
    ErrorNotify(message);
    exit(1);
}

//Exit, notifying of an unmet expectation in the input file
void Expected_msg(char* expectation)
{
    char expected[] = " expected.";
    char* message = malloc(strlen(expectation)+10);
    strcpy(message, expectation);
    strcat(message, expected);
    ErrorHalt(message);
}

//Compare two characters
void Match(char x)
{
    if(lookahead == x)
        ReadChar(&lookahead);
    else
        Expected_msg(&x);

}

//Determine if a character is alphabetic
int IsAlpha(char x)
{
    unsigned int dec =  x;
    if( ((dec>64)&&(dec<90)) || ((dec>96)&&(dec<123)) )
    //If x is either in the ascii uppercase o lowercase range
        return TRUE;
    else
        return FALSE;

}

//Determine is a character is a numeral
int IsDigit(char x)
{
    unsigned int ascii_code = x;
    if((ascii_code >= 48) && (ascii_code <= 57))
        return TRUE;
    else
        return FALSE;
}

//Get a name token and update the lookahead character
void GetName(char* name)
{
    if (IsAlpha(lookahead)== FALSE)
    {
        Expected_msg("Name");
    }

    *name= toupper(lookahead);
    ReadChar(&lookahead);
}

void GetNum(char* num)
{
    if(IsDigit( lookahead)==FALSE)
        Expected_msg("Integer");
    *num = lookahead;
    ReadChar(&lookahead);

}
//Print a string preceded by tabulation.
void Emit(char* s)
{
printf("%c%s", TAB, s);

}

//Emit() a line followed by CRLF
void EmitLn(char* s)
{
    Emit(s);
    printf("\n");
}

//Start reading from input stream

void Init()
{
    ReadChar(&lookahead);

}
void Term()
{
    char buffer[64];
    char num;
    GetNum(&num);
    int length=0;
    length+=sprintf(buffer,"li");
    length+=sprintf(buffer+length," $t0");
    length+=sprintf(buffer+length,", %c", num );
    EmitLn(buffer);

}

void Add()
{
    char plus = '+';
    Match(plus);
    Term();
    EmitLn("add $t0, $t0, $t1");

}

void Substract()
{
    char minus = '-';
    Match(minus);
    Term();
    EmitLn("sub $t0, $t0, $t1");
    EmitLn("neg $t0, $t0");

}


void Expression()
{
    Term();
    while((lookahead=='+')||(lookahead == '-'))
          {
        EmitLn("move $t1, $t0");
        switch(lookahead)
        {
            case '+':
                Add();
                break;
            case '-':
                Substract();
                break;
            default:
                Expected_msg("Addop");

        }

          }
}


void main()
{
    Init();
    Expression();
}
