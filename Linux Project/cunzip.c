//Student:Yang Kai Yam
//SID: 55676537

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){

	// write your code here
	if(argc<2)
	{
		printf("cunzip: file1 [file2 ...]\n");
		exit(1);
	}

	for(int i=1; i<argc; i++)
	{
		char current_char;
		int num = 0;
		FILE *fp = fopen(argv[i], "r");
		
		if(fp == NULL)
		{
			printf("cannot open file: %s\n", argv[i]);
			exit(1);
		}
		
		while(1)
		{
			int bytes = fread(&num, 1, sizeof(int), fp); // record the number of char
			if(bytes != sizeof(int)){
				break;
			}
			fread(&current_char, 1, sizeof(char), fp);

			int loop = 0;
			while(loop<num)
			{
				fwrite(&current_char, 1, sizeof(char), stdout); //expand the char
				loop++;
			}
		}

		fclose(fp);
	}

	return 0;
}
