#include<stdio.h>
#include<string.h>
#include<stdlib.h>
struct counts
{
    int lines;
    int varcount;
    int constant;
    char val[30][7];
};
typedef struct counts c;
c line_var(char *);

int SAL_Syn_Check(char *);
int count(FILE*);
int val_Const(char *);
int is_Spel_Correct(char *, char *);
int is_Branch_Valid(char *, int);
int valid_Non_Branch(char *, char[], int);

int main(int argc, char *argv[])
{
    if(argv[1] == '\0')
    {
        printf("INPUT AND OUTPUT FILES ARE MISSING");
        return 0;
    }
    if(argv[2] == '\0')
    {
        printf("OUTPUT FILE IS MISSING");
        return 0;
    }
    int flag = SAL_Syn_Check(argv[1]);
    if(flag == 0)
    {
        FILE *fin, *fout; //fin is input file pointer, fout is output file pointer
        char *opcode; //instruction code
        c s; // structure oblect
        int tno; //token number
        int i = 0; // variable array index
        int line = 0;
        int constArrIndex;
        int operand; //operand Address
        int ic = 0; // Instruction Counter
        int j = 0;
        int temp; //for temporary Array index
        char varcap; // Capital Variable
        char str[60]; //line string

        fin = fopen(argv[1], "r");
        fout = fopen(argv[2], "w");
        char *token;

        if(fin == NULL)
        {
            printf("error: FILE IS EMPTY");
            return (-1);
        }

        s = line_var(argv[1]);

        char var[s.varcount]; //variable array
        int varAddStart = s.lines - s.varcount;

        while(fgets(str, 60, fin) != NULL)
        {
            operand = 0;
            token = strtok(str, " ");
            tno = 1;
            while(token != NULL)
            {
                if(tno == 2)
                {
                    if(line == s.lines - 1 )
                    {
                        // length of last token is 2 more than the actual length because its accepting it as a token followed by new line character
                        if(strlen(token) == 6)
                            opcode = "+43";
                    }

                    // check for varibles
                    if(strlen(token) == 2)
                    {
                        var[i] = token[0];
                        i++;
                    }
                    else
                    {
                        if(strcmp(token, "READ") == 0 || strcmp("READ\n", token) == 0)
                        {
                            opcode = "+10";
                        }
                        else if(strcmp("WRITE", token) == 0 || strcmp("WRITE\n", token) == 0)
                        {
                                opcode = "+11";
                        }
                        else if(strcmp("LOAD", token) == 0 || strcmp("LOAD\n", token) == 0)
                        {
                                opcode = "+20";
                        }
                        else if(strcmp("STORE", token) == 0 || strcmp("STORE\n", token) == 0)
                        {
                                opcode = "+21";
                        }
                        else if(strcmp("ADD", token) == 0 || strcmp("ADD\n", token) == 0)
                        {
                                opcode = "+30";
                        }
                        else if(strcmp("SUB", token) == 0 || strcmp("SUB\n", token) == 0)
                        {
                                opcode = "+31";
                        }
                        else if(strcmp("MULT", token) == 0 || strcmp("MULT\n", token) == 0)
                        {
                                opcode = "+33";
                        }
                        else if(strcmp("DIV", token) == 0 || strcmp("DIV\n", token) == 0)
                        {
                                opcode = "+32";
                        }
                        else if(strcmp("BRAN", token) == 0 || strcmp("BRAN\n", token) == 0)
                        {
                                opcode = "+40";
                        }
                        else if(strcmp("BRANN", token) == 0 || strcmp("BRANN\n", token) == 0)
                        {
                                opcode = "+41";
                        }
                        else if(strcmp("BRANZ", token) == 0 || strcmp("BRANZ\n", token) == 0)
                        {
                                opcode = "+42";
                        }
                        else if(strcmp("HALT", token) == 0 || strcmp("HALT\n", token) == 0)
                        {
                                opcode = "+43";
                        }
                    }

                }
                if(tno == 3)
                {
                    if(line == s.lines - 1)
                    {
                        if(strlen(token) == 7 || strlen(token) == 6)
                        {
                            for(constArrIndex = 0; constArrIndex < 5; constArrIndex++)
                                s.val[j][constArrIndex] = token[constArrIndex];
                            operand = j + varAddStart;
                            j++;
                        }
                       else if(strlen(token) == 2 || (strlen(token) == 3 && atoi(token) == 0))
                        {
                            temp = 0;
                            while(temp < s.varcount)
                            {
                                if(token[0] == var[temp])
                                {
                                    operand = temp + varAddStart;
                                    break;
                                }
                                temp++;
                            }
                        }
                        else if(strlen(token) == 4 || strlen(token) == 3)
                        {
                            operand = atoi(token) - s.varcount;
                        }
                    }

                    else
                    {
                        if(strlen(token) == 6 || strlen(token) == 7)
                        {
                            for(constArrIndex = 0; constArrIndex < 5; constArrIndex++)
                                s.val[j][constArrIndex] = token[constArrIndex];

                            operand = j + varAddStart;
                            j++;
                        }
                       else if(strlen(token) == 2 || strlen(token) == 3)
                        {
                            temp = 0;
                            while(temp < s.varcount)
                            {
                                if(token[0] == var[temp])
                                {
                                    operand = temp + varAddStart;
                                    break;
                                }
                                temp++;
                            }
                        }
                        else if(strlen(token) == 3 || strlen(token) == 4)
                        {
                            operand = atoi(token) - s.varcount;
                        }
                    }
                }
                tno++;
                token = strtok(NULL, " ");
            }

            // insert converted string after variables
            if(line >= s.varcount && line < s.lines)
            {
                if(operand < 10)
                {
                    if(ic < 10)
                        fprintf(fout, "0%d %s0%d\n",ic ,opcode, operand);
                    else
                        fprintf(fout, "%d %s0%d\n",ic ,opcode, operand);
                }
                else
                {
                    if(ic < 10)
                        fprintf(fout, "0%d %s%d\n",ic ,opcode, operand);
                    else
                        fprintf(fout, "%d %s%d\n",ic ,opcode, operand);
                }
                ic++;
            }
            line++;

        }

        // inserting variable and constants in output file
        int v;
        for(v = 0 ; v < s.constant; v++)
        {
            if(varAddStart + v < 10)
                fprintf(fout, "0%d %s\n",varAddStart + v ,s.val[v]);
            else
                fprintf(fout, "%d %s\n",varAddStart + v ,s.val[v]);
        }

        fclose(fin);
        fclose(fout);
    }
    return 0;
}

c line_var(char *input)
{
    FILE *fp;
    c s;
    int tno;
    char *token, str[20], *opcode;

    s.lines = 0;
    s.constant = 0;
    s.varcount = 0;
    fp = fopen(input, "r");
    while(fgets(str, 60, fp) != NULL)
    {
        token = strtok(str, " ");
        tno = 1;
        while(token != NULL)
        {
            if(tno == 2)
            {
                if(strlen(token) == 2)
                {
                    s.varcount++;
                }
                s.lines++;
            }
            if(tno == 3)
            {
                if(strlen(token) == 6 || strlen(token) == 7)
                    {
                        s.constant++;
                    }
            }
            ++tno;
            token = strtok(NULL, " ");
        }
    }
    return s;
}
