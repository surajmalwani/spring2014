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
	long int size;	
	char* fileString;

	fseek(file, 0, SEEK_END);
	size = ftell(file);
	rewind(file);

	fileString = calloc(size + 1, 1);
	fread(fileString, 1, size, file);

  printf("File String read: %s", fileString);
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
	
  memset(keybuffer,0x0,sizeof(keybuffer));

  generate_key(pwd, keybuffer);
  //TODO: Error checking
	
  crypt_err = gcry_cipher_setkey(handle, (const void *)&keybuffer[0],KEY_SIZE);
  //TODO: Error checking

  //to accomodate the macString
  contentSize = contentSize + MAC_SIZE;

  encryptedContents = (char *)malloc(contentSize);

  gcry_cipher_setiv(handle, &IV[0], 16);   
  
  errorCode = gcry_cipher_encrypt(handle, encryptedContents, contentSize, inputString, contentSize);

  if(errorCode)
  {
  	fprintf(stderr, "Failed to encrypt file: %s %s\n", gcry_strsource(errorCode), gcry_strerror(errorCode));
	  exit(ENCRYPTION_FAILED);
  }

  printf("\nEncrypted Contents: %s", encryptedContents); 
  macString = generateHMAC(encryptedContents, keybuffer);
  
  strcat(encryptedContents, macString);
  printf("\nEncrypted Content after MAC: %s", encryptedContents);

  return encryptedContents;  
}

long getFileEncryptedLength2(long fileLength)
{

  printf("\nOriginal File Length: %ld", fileLength);
   //TODO: Replace with block size/KEY SIze 
	if(fileLength < KEY_SIZE)
		fileLength = KEY_SIZE;
	else
	{
    printf("\nFile Length Remainder: %d", fileLength%KEY_SIZE); 

      
		fileLength = KEY_SIZE + ((fileLength/KEY_SIZE) * KEY_SIZE);
	}
	
	printf("\nFile length : %ld ", fileLength);

  return fileLength;
}

/**
* encrypt_file
*
**/
char* encrypt_file(FILE* file, const char* pwd)
{
  char *fileContents = NULL, *encryptedContents = NULL;
  int errorCode, i = 0;
  long size, contentSize;

  size = getFileSize(file);
  contentSize = getFileEncryptedLength2(size);
  fileContents = malloc(contentSize);
  fileContents = readFileIntoString(file);
  //TODO: Test fileContents for empty or invalid

  for (i = 0; i < strlen(fileContents); i++)
  {
      printf("%02X ", fileContents[i]);
  }

  printf("\nCount: %d\n", i);
  
  encryptedContents = encryptString(fileContents, contentSize, pwd);
  
  return encryptedContents;
}


/**
 *Test Function
 */
void encryptFile()
{
	const char* password = "trial";
	char* filepath = "test.txt";
	init_gcrypt();
	encrypt_file_inpath(filepath, password);
	finalize_gcrypt();

}

