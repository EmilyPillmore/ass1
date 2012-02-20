#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <string.h>

#define length 4

bool decrypt(char*,char,char,char,char);
int main(int argc, char **argv){

	for(char first = '0'; first < 'z'; first ++){
		for(char second = '0'; second  < 'z'; second++){
			for(char third = '0'; third  < 'z'; third++){
				for(char fourth = '0'; fourth  < 'z'; fourth++){
					if(decrypt(argv[1],first,second,third,fourth)){
						printf("Key: %c%c%c%c\n",first,second,third,fourth);
						first = 'z';
						second = 'z';
						third = 'z';
						fourth = 'z';
					}
					if(fourth == '9')
						fourth = '@';
					else if(fourth == 'Z')
						fourth = 'a';
				}
				if(third == '9')
					third = '@';
				else if(third== 'Z')
					third = 'a';

			}
			if(second == '9')
				second = '@';
				else if(second == 'Z')
					second = 'a';

		}
		if(first == '9')
			first = '@';
		else if(first == 'Z')
			first = 'a';
	} 
}

bool decrypt(char* pathname,char first,char second, char third, char fourth){
	char input_key[length] = {first,second,third,fourth};
	char key[length] = {first,second,third,fourth};
	char in_buf[1024];
	int cipherfd;
	int tcount = 0;
	int count;
	char c;
	char out[50] = {NULL};
	char word1[50] = {NULL};
	char word2[50] = {NULL};
	char word3[50] = {NULL};
	int counter=0;
	FILE *fp;
	char command[90];
	char temp[50];

	if( (cipherfd = open(pathname,O_RDONLY)) == NULL){
		printf("Error");
		exit(-1);
	}

	while( ((count = read( cipherfd, in_buf, length) ) > 0) && counter < 30){
		for( int i=0; i<count; i++){
			c=(in_buf[i]-key[i]-i)%128; 
			while( c<0)
				c+=128;

			if((int)c > 31 && (int)c < 123 &&
				(c != ' ' || c != '\t' || c != '\n')){
				out[counter] = c;
				counter++;
			}
			else if(c == ' ' || c == '\t' || c == '\n'){
				out[counter] = (char)27;
				counter++;
			}

			key[i]=((int) c + input_key[i]+ 53) % 128;
		}
				
	}

	close(cipherfd);

	for(int i = 0, k = 0, j = 1;(i < counter && j < 4);i++){
		switch(j){
		case 1:
			if(out[i] == (char)27){
				word1[k] = '\0';
				k = 0;
				j++;
			}
			else if(out[i] == '*' || out[i] == '[' || out[i] == ']' || out[i] == '^' || out[i] == '$' || out[i] == '.' || out[i] == '{' || out[i] == '}'|| out[i] == '\\'){
				word1[k] = '\\';
				k++;
				word1[k] = out[i];
				k++;
			}
			else{
				word1[k] = out[i];
				k++;
			}
			break;
		case 2:
			if(out[i] == (char)27){
				word2[k] = '\0';
				k = 0;
				j++;
			}
			else if(out[i] == '[' || out[i] == ']' || out[i] == '^' || out[i] == '$' || out[i] == '.' || out[i] == '{' || out[i] == '}'){
				word2[k] = '\\';
				k++;
				word2[k] = out[i];
				k++;
			}
			else{
				word2[k] = out[i];
				k++;
			}
			break;

		case 3:
			if(out[i] == (char)27){
				word3[k] = '\0';
				k = 0;
				j++;
			}
			else if(out[i] == '[' || out[i] == ']' || out[i] == '^' || out[i] == '$' || out[i] == '.' || out[i] == '{' || out[i] == '}'){
				word3[k] = '\\';
				k++;
				word3[k] = out[i];
				k++;
			}
			else{
				word3[k] = out[i];
				k++;
			}
		break;
		}
	}

	if(word1[0] == NULL || word2[0] == NULL || word3[0] == NULL)
		return false;

	if(strlen(word1) == 1 && (
		(word1 != 'a' || word1 != 'A' || word1 != 'I')))
		return false; 
	if(strlen(word2) == 1 && (
		(word2 != 'a' || word2 != 'A' || word2 != 'I')))
		return false; 
	if(strlen(word3) == 1 && (
		(word3 != 'a' || word3 != 'A' || word3 != 'I')))
		return false; 



	sprintf(command,"cat /usr/dict/words |grep -i -w  '%s'",word1);
	fp = popen(command, "r");
	if( (fgets(temp,sizeof temp, fp)) == 0){
		pclose(fp); 
		return false;
	}
	pclose(fp);
	printf("\t\tFirst Word\n");

	sprintf(command,"cat /usr/dict/words |grep -i -w  '%s'",word2);
	fp = popen(command, "r");
	if( (fgets(temp,sizeof temp, fp)) == 0){
		pclose(fp); 
		return false;
	}
	pclose(fp);

	printf("\t\tSecond Word\n");
	sprintf(command,"cat /usr/dict/words |grep -i -w  '%s'",word3);
	fp = popen(command, "r");
	if( (fgets(temp,sizeof temp, fp)) == 0){
		pclose(fp); 
		return false;
	}
	pclose(fp);
	return true;
}
