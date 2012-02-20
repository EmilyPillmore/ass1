/* Copyright. For Edwin Sha's security course. Only encrypt ASCII files */
/* THis is only used to encrypt English file which does not habe any control characters below ASCII SPACE except LF*/

#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#define BUF_SZ 1024

/* Encryption Function
 * 	  Input:  input_key - key
 * 	  		  plaintext_file - plain text file name
 * 	  Output: cipher text (stdout)
 */	 
void encrypt( char * input_key, char * plaintext_file)
{
	int i;
	int ksz;
	int *key;
	int count;
	unsigned char in_buf[BUF_SZ];
	int	 fd;
	char  c;
	int  d;
	int	 offset=0;
        int tt; 


	/*initilize key*/
	ksz = strlen(input_key);
	if( ( key=(int *)malloc(ksz*sizeof(int)) ) == NULL ){
		printf("Error: malloc key\n");
		exit(-1);
	}

	for( i=0; i<ksz; i++){
		key[i]=(int) input_key[i];
	}
	
	
	/* open files */
	if( ( fd = open( plaintext_file, O_RDONLY ) )< 0 ){
		printf("Error: open for input file\n");
		exit(-1);
	}

	/* Read ksz char from inputfile and encrypt*/
	while( (count=read( fd, in_buf, ksz) ) > 0 ){
	
		for( i=0; i<count; i++){
			tt=(in_buf[i]+key[i]+i)%128;
			while(tt<0) tt+=128; 
                         
                        c = (char) tt;
                        key[i]=(int) (in_buf[i] + input_key[i] + 53) % 128;

			printf("%c",c);
	/*		printf("%c %d, cipher:%d, decode:%d\n",in_buf[i],in_buf[i],c,d);*/
		}
	}

	if( count < 0 ){
		printf("Error: read input file\n");
		exit(-1);
	}
		
	close(fd);
}


main(int argc, char **argv)
{
	/* Usage */
	if( argc != 3 ){
		printf("Usage: encrypt key file_name\n");
		exit(-1);
	}
	
	/* Encrypt: input key and plain text file
	 * 			argv[1] -- key
	 * 			argv[2] -- plaintext file
	 */
	encrypt(argv[1],argv[2]);
}
