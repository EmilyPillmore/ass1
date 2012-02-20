/* For Dr. Edwin Sha's class. It can be copied and revised
for students in Dr. Sha's class.  */

#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#define BUF_SZ 1024			/*This is the default size for the buffer during decrypting*/
							/*This value must be bigger than the key length            */

/* Decryption function
 *  Input: input_key   -  point to a string that is the key
 *  	   cipher_file -  point to a string that is cipher file name
 *	Output: the decrypted file ( to standout )
 */ 

void decrypt(char * input_key, char * cipher_file) 
{
	int i;
	int ksz;
	int *key;		
	int count;
	unsigned char in_buf[BUF_SZ];
	int	 fd;
	int  c;
	int	 offset=0;

	/*initilize key*/
	ksz = strlen(input_key);
	if( ( key=(int *)malloc(ksz*sizeof(int)) ) == NULL ){
		printf("Error: malloc key\n");
		exit(-1);
	}

	for( i=0; i<ksz; i++){
		key[i]= (int) input_key[i];

	}
	
	
	/* open files */
	if( ( fd = open( cipher_file, O_RDONLY ) )< 0 ){
		printf("Error: open for input file\n");
		exit(-1);
	}

	/* Read ksz char from inputfile and encrypt*/
	while( (count=read( fd, in_buf, ksz) ) > 0 ){
		
		for( i=0; i<count; i++){
			c=(in_buf[i]-key[i]-i)%128; 

			while( c<0)
				c+=128;
			key[i]=((int) c + input_key[i]+ 53) % 128;
			printf("%c",c);
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
	/* Usage*/
	if( argc != 3 ){
		printf("Usage: decrypt key cipher_text\n");
		exit(-1);
	}
	
	/* Call encrypt function: encrypt(argv[1],argv[2])   */
	/*    where: argv[1] - key      					 */
    /*           argv[2] - plaintext filename 			 */	
	decrypt( argv[1], argv[2] );
}

