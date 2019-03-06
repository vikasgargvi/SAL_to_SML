#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int count(FILE *);
int is_Spel_Correct(char *, char *);
int val_Const(char *);
int is_Branch_Valid(char *, int);
int valid_Non_Branch(char *, char [], int);

int SAL_Syn_Check(char *fileName)
{
	FILE *fin = NULL;
	fin = fopen(fileName, "r");

	if(fin == NULL)
	{
		printf("\nError opening file...");
		return -1;
	}

	int line_count = count(fin);
	fclose(fin); //since after returning frmom the function fin is now pointing to the end of the file
	fin = fopen(fileName, "r");

	if(fin == NULL)
	{
		printf("\nError opening file...");
		return -1;
	}

	char str[50];
	char *token;
	char prev_token[10];
	int wc, lno, l;
	int plno = -1;
	char var_Array[26];
	int i;
	int var_Arr_size = 0;
	int err_bit = 0;
	int var_flag = 0; //flag for repeated or valid/invalid variable name
	int inst_flag = 0; //flag for instruction spelling and capitalizing
	int var; //if second token is variable or not
	int const_flag = 0; //if constant is valid or not
	int val_addr_flag = 0; //if the address location after any branch instruction is valid or not

	while(!feof(fin))
	{
		while(fgets(str,30,fin) != NULL)
		{
			//first token : line no.
			token = strtok(str, " ");
			l = strlen(token);
			if(l > 2)
			{
				printf("\nBlank line not accepted...");
				plno++;
				err_bit++;
				continue;
			}
			for(i = 0 ; i < l ; i++)
			{
				if(!(str[i] >= '0' && str[i] <= '9'))
				{
					printf("\n%d : Integer token expected in place of %s...\n", token);
					err_bit++;
					break;
				}
			}

			if(i == l)
			{
				lno = atoi(token);
				if(lno >= 0)
				{
					if(lno != (plno + 1))
					{
						printf("\n%d : Line no. not ordered...", lno);
						err_bit++;
					}
					else
						plno++;
				}
			}

			wc++;
				
			//second token : variable or instruction
			token = strtok(NULL," ");
			l = strlen(token);
	
			if(l == 2)
			{
				if(token[0] >= 'A' && token[0] <= 'Z')
				{
					for(i = 0 ; i < var_Arr_size ; i++)
					{
						if(token[0] == var_Array[i])
						{
							var_flag = 1;
							break;
						}
					}
					if(var_flag == 1)
					{
						printf("\nVariable %c reinitialized....", token[0]);
						var_flag = 0;
						err_bit++;
					}
					else
					{
						var_Array[i] = token[0];
						var_Arr_size++;
					}

					if(token[1] != ':')
					{
						printf("\nExpected ':' after variable name %c...", token[0]);
						err_bit++;
					}
				}
				
				else
				{
					printf("\nVariables name '%c' not accepted...", token[0]);
					err_bit++;
				}

				var = 1;
			}

			else
			{
				inst_flag = is_Spel_Correct(token, prev_token);
				if(inst_flag == 1)
				{
					printf("\n%s : No such instruction...", token);
					inst_flag = 0;
					err_bit++;
				}
			}
			wc++;
				
			//third token : valid address/variable/constant
			token = strtok(NULL, " ");

			if(var == 1)
			{
				if(token == NULL)
				{
					printf("\nInteger value expected...");
					err_bit++;
				}
				else
				{
					const_flag = val_Const(token);
					
					if(const_flag == 1)
					{
						printf("\n%s : Invalid integer value...", token);
						const_flag = 0;
						err_bit++;
					}
				}
				var = 0;
			}
			
			else
			{
				if(strcmp(prev_token, "HALT") == 0)
				{
					if(token != NULL)
					{
						printf("\nNo argument/parameter expected after '%s'...", prev_token);													err_bit++;
					}	
					wc--;
				}
				
				else if( (strcmp(prev_token, "BRAN") == 0) || (strcmp(prev_token, "BRANN") ==0) || (strcmp(prev_token, "BRANZ") == 0) )
				{
					if(token == NULL)
					{
						printf("\nAddress location expected after %s...", prev_token);
						err_bit++;
					}
		
					else
					{
						val_addr_flag = is_Branch_Valid(token, line_count);
						if(val_addr_flag == 1)
						{
							printf("\nNo location exists with address %s", token);
							val_addr_flag = 0;
							err_bit++;
						}
					}
				}
					
				else if( (strcmp(prev_token, "ADD") == 0) || (strcmp(prev_token, "SUB") == 0) || (strcmp(prev_token, "MULT") == 0) || (strcmp(prev_token, "DIV") == 0) )
				{
					if(token == NULL)
					{
						printf("\nVariable/constant expected after %s...", prev_token);
						err_bit++;
					}
						
					else
					{
						const_flag = val_Const(token);
				
						var_flag = valid_Non_Branch(token, var_Array, var_Arr_size);
							
						if(const_flag == 1 && var_flag == 1)
						{
							printf("\n%s : Not a valid constant/variable...", token);
							err_bit++;
						}
			
						else if(strcmp(prev_token, "DIV") == 0)
						{
							if(const_flag == 0)
							{
								int x = atoi(token);
									if(x == 0)
								{
									printf("\nDivision by zero error...");
									err_bit++;
								}
							}
						}
					
						const_flag = 0;
						var_flag = 0;
					}
				}
				
				else
				{
					if(token == NULL)
					{
						printf("\nVariable expected after %s...", prev_token);
						err_bit++;
					}
					else
					{
						var_flag = valid_Non_Branch(token, var_Array, var_Arr_size);
						if(var_flag == 1)
						{
							printf("\n%s : No such variable exists...", token);
							var_flag = 0;
							err_bit++;
						}
					}
				}	
			}
			wc++;

			token = strtok(NULL, " ");
			if(token != NULL)
			{
				printf("\nToo many arguments...");
				err_bit++;
			}

			if(wc == 2)
			{
				if(strcmp(prev_token, "HALT") != 0)
					printf("\nToo few arguments...");
			}
		
			else if(wc < 3)
				printf("\nToo few arguments...");

			wc = 0;
		}	
	}

	fclose(fin);

	printf("\nTotal no. of errors : %d\n", err_bit);

	return err_bit;
}

int count(FILE *fp)
{
	int lc = 0;
	char *str;

	while(!feof(fp))
	{
		while(fgets(str,30,fp) != NULL)
			lc++;
	}

	return lc;
}

int is_Spel_Correct(char *inst, char *str)

{
	/*String is compared character-by-character because in case if instruction is not followed by any variable/constant/address/etc. then
	  token is not only the string but it is the string appended with newline character which doesn't lie between A-Z and thus creating
	  errors though the token must be accepted without any error*/
 
	if(inst[0] == 'R' && inst[1] == 'E' && inst[2] == 'A' && inst[3] == 'D')
	{
		strcpy(str, "READ");
		return 0;
	}
	else if(inst[0] == 'W' && inst[1] == 'R' && inst[2] == 'I' && inst[3] == 'T' && inst[4] == 'E')
	{
		strcpy(str, "WRITE");
		return 0;
	}
	else if(inst[0] == 'L' && inst[1] == 'O' && inst[2] == 'A' && inst[3] == 'D')
	{
		strcpy(str, "LOAD");
		return 0;
	}
	else if(inst[0] == 'S' && inst[1] == 'T' && inst[2] == 'O' && inst[3] == 'R' && inst[4] == 'E')
	{
		strcpy(str, "STORE");
		return 0;
	}
	else if(inst[0] == 'A' && inst[1] == 'D' && inst[2] == 'D')
	{
		strcpy(str, "ADD");
		return 0;
	}
	else if(inst[0] == 'S' && inst[1] == 'U' && inst[2] == 'B')
	{
		strcpy(str, "SUB");
		return 0;
	}
	else if(inst[0] == 'D' && inst[1] == 'I' && inst[2] == 'V')
	{
		strcpy(str, "DIV");
		return 0;
	}
	else if(inst[0]  =='M' && inst[1] == 'U' && inst[2] == 'L' && inst[3] == 'T')
	{
		strcpy(str, "MULT");
		return 0;
	}
	else if(inst[0] == 'B' && inst[1] == 'R' && inst[2] == 'A' && inst[3] == 'N')
	{
		strcpy(str, "BRAN");
		return 0;
	}
	else if(inst[0] == 'B' && inst[1] == 'R' && inst[2] == 'A' && inst[3] == 'N' && inst[4] == 'N')
	{
		strcpy(str, "BRANN");
		return 0;
	}
	else if(inst[0] == 'B' && inst[1] == 'R' && inst[2] == 'A' && inst[3] == 'N' && inst[4] == 'Z')
	{
		strcpy(str, "BRANZ");
		return 0;
	}
	else if(inst[0] == 'H' && inst[1] == 'A' && inst[2] == 'L' && inst[3] == 'T')
	{
		strcpy(str, "HALT");
		return 0;
	}
	strcpy(str, inst);
	return 1;
}



int val_Const(char *str)
{
	if(str[0] == '+' || str[0] == '-')
	{
		for(int i = 1; i < 5 ; i++)
		{
			if(!(str[i] >= '0' && str[i] <= '9'))
				return 1;
		}
		return 0;
	}

	else
		return 1;
}

int is_Branch_Valid(char *token3 , int lcount)

{
	if(token3 != NULL)
	
{
		int i ;

		i = atoi(token3);

	    	
		if(i < lcount)

			return 0;
	
	    	else
	
		return 1;

	
}
	
	return 1;	


}


int valid_Non_Branch(char *s, char arr[], int size)
{
	for(int i = 0 ; i < size ; i++)
	{
		if(arr[i] == s[0])
			return 0;
	}
	return 1;
}
