//Student: Yang Kai Yam
//SID: 55676537

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){

	// write your code here

        if(argc < 2)    {
                printf("czip:file1[file2...]\n");
                exit(1);
         }

        for(int i = 0; i < argc; i++) {

                char current_char;
                char prev_char;
                int num = 0;

                FILE *fp = fopen(argv[i], "rb");
                if(fp == NULL)
                {
                        printf("cannot open file: %s\n", argv[i]);
                        exit(1);
                }
                while(1)
                {
			current_char = fgetc(fp);
                        if(current_char == EOF)
                        {
                                if(num > 0)
                                {
                                        fwrite(&num, 1, sizeof(int), stdout);
                                        fwrite(&prev_char, 1, sizeof(char), stdout);
                                }
                                break; //leave the loop
                        }
                        if(current_char-prev_char != 0) //the char is changed
                        {
                                if(num > 0)
                                {
                                        fwrite(&num, 1, sizeof(int), stdout);
                                        fwrite(&prev_char, 1, sizeof(char), stdout);
                                }
                                num = 0;
                        }
                        num ++; //count the repeated number of char
                        prev_char = current_char;
                }
                fclose(fp);
        }

	return 0;
}
