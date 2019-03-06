all: SAL_To_SML.o SAL_Checker.o
	gcc SAL_To_SML.o SAL_Checker.o -o SAL_to_SML_Conversion 
SAL_To_SML.o: SAL_To_SML.c
		gcc -c SAL_To_SML.c
SAL_Checker.o: SAL_Checker.c
		gcc -c SAL_Checker.c
clean:
	rm -rf *.o
	rm -rf SAL_to_SML_Conversion
