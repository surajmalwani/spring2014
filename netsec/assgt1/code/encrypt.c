#include<gcrypt.h>
#include<stdio.h>
#include<encrypt.h>
#include<wchar.h>
#include<stdint.h>
#include<string.h>
#include<cryptcommon.h>
#include<errorcodes.h>

/**
*
**/
char* readFileIntoString(FILE* file)
{
	long int size = ftell(file);	
	char* fileString;

	fseek(file, 0, SEEK_END);
	size = ftell(file);
	rewind(file);

	fileString = calloc(size + 1, 1);
	fread(fileString, 1, size, file);

	return fileString;	
}


/**
* encrypt_file_inpath
*
**/
void encrypt_file_inpath(char* filePath, const char* pwd)
{
	//TODO: Validate if file exists and if filepath is valid
	
	FILE* f = fopen(filePath, "r, ccs=UTF-8");
	
	if(!f)
	{
		fprintf(stderr, "\nUnable to create new file");
	}

	encrypt_file(f, pwd);

	//TODO: Error Checking
	fclose(f);

	return;
}

/**
 * Get File Size as a multiple of block size
 * **/
long getFileEncryptedLength(long fileLength)
{

   //TODO: Replace with block size/KEY SIze 
	if(fileLength < 16)
		fileLength = 16;
	else
	{
		fileLength = fileLength%16 + fileLength;
	}
	
	printf("\nFile length : %ld ", fileLength);

  return fileLength;
}

/**
 * Get File Size
 */
long getFileSize(FILE* file)
{
  long size;
  fseek(file, 0L, SEEK_END);
  size = ftell(file);
  fseek(file, 0L, SEEK_SET);

  return size;
}

/**
* Encrypt String
*
**/
char* encryptString(char* inputString, long contentSize, const char* pwd)
{
  
  char* encryptedContents = NULL;
  gcry_cipher_hd_t handle;
  gcry_error_t crypt_err;
  gpg_error_t keygen_err;
  uint8_t keybuffer[KEY_SIZE];
  int errorCode;
  char* macString;

  crypt_err = gcry_cipher_open(&handle, ENCRYPTION_ALGO, ENCRYPTION_MODE, GCRY_CIPHER_SECURE);
  //TODO: Perform error checking      
	
  generate_key(pwd, keybuffer);
  //TODO: Error checking
	
  crypt_err = gcry_cipher_setkey(handle, (const void *)&keybuffer[0],KEY_SIZE);
  //TODO: Error checking

  //to accomodate the macString
  encryptedContents = (char *)malloc(contentSize + 64);

  gcry_cipher_setiv(handle, &IV, 16);   
  
  errorCode = gcry_cipher_encrypt(handle, encryptedContents, contentSize + 64, inputString, contentSize);

  if(errorCode)
  {
	fprintf(stderr, "Failed to encrypt file: %s %s\n", gcry_strsource(errorCode), gcry_strerror(errorCode));
	exit(ENCRYPTION_FAILED);
  }
 
  macString = generateHMAC(encryptedContents, keybuffer);
  printf("\nMAC LENGTH: %d " , strlen(macString));
  printf("\nLength of Encrypted Content: %d", strlen(encryptedContents));

  strcat(encryptedContents, macString);

  return encryptedContents;  
}

/**
* encrypt_file
*
**/
void encrypt_file(FILE* file, const char* pwd)
{
  char *fileContents = NULL, *encryptedContents = NULL;
  int errorCode;
  long size, contentSize;

  size = getFileSize(file);
  contentSize = getFileEncryptedLength(size);
  fileContents = malloc(contentSize);
  fileContents = readFileIntoString(file);
  //TODO: Test fileContents for empty or invalid

  printf("\nBefore Encryption: %s", fileContents);
  encryptedContents = encryptString(fileContents, contentSize, pwd);
  printf("\nAfter Encryption: %s", encryptedContents); 
  
}



void encryptFile()
{
	const char* password = "trial";
	char* filepath = "test.txt";
	init_gcrypt();
	encrypt_file_inpath(filepath, password);
	finalize_gcrypt();

}

/**
Start Function
**/

int main()
{
	
	encryptFile();

}


