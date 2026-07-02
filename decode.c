#include <stdio.h>
#include "decode.h"
#include "types.h"
#include <string.h>
#include "common.h"

Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo)
{
    //source image check
    if(argv[2] != NULL && strstr(argv[2],".bmp")!=NULL)  //check if a source image is a .bmp file
    {
      decInfo -> stego_image_fname = argv[2];  //if it is .bmp means store it
    }
    else 
    {
    return e_failure; //if not .bmp return it
    }

    //Default output file
    strcpy(decInfo -> output_fname,"decoded.txt"); //strcpy bcz it is storing in array(char output_fname[20])

    return e_success;
}

Status open_decode_files(DecodeInfo *decInfo)
{
   decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "rb"); //open the stego image in binary read mode
    // Do Error handling
    if (decInfo->fptr_stego_image == NULL)
    {
    	perror("fopen"); //print error message
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);

    	return e_failure; //return failure if the file can't be open
    }

    decInfo->fptr_output = fopen(decInfo->output_fname,"wb");  
    if(decInfo->fptr_output == NULL) //if file doesn't exist or can't open
    {
        perror("fopen"); //print error msg
        return e_failure;
    }
    return e_success;
}

//Decode 1 byte
Status decode_byte_from_lsb(char *data,char *image_buffer)
{
    *data = 0; //Initilize data to 0
    for(int i=0;i<8;i++)
    {
        *data |= (image_buffer[i] & 1)<<i; //set each bit in data
    }
    return e_success;
}

//Function to Decode a string from the image
Status decode_string_from_image(char *data, int size,FILE *fptr_stego_image) 
{
    char arr[8];// Array to hold byte data for decoding
    for(int i=0;i<size;i++)
    {
        fread(arr,8,1,fptr_stego_image);
        decode_byte_from_lsb(&data[i],arr);
    }
    data[size] = '\0';  // Add a null terminator at the end of the string
    return e_success;
}

// Function to decode an integer from the image using LSB
Status decode_integer_from_lsb(int *data,FILE *fptr_stego_image)
{
    char arr[32]; // Array to hold 32 bits
    fread(arr,32,1,fptr_stego_image); // Read 32 bits from the image

    *data = 0;
    for(int i=0;i<32;i++)
    {
        *data |= (arr[i] & 1) <<i;// Set each bit in the integer
    }
    return e_success;
}

 Status do_decoding(DecodeInfo *decInfo)
 {
     if(open_decode_files(decInfo) == e_failure)  
    {
         printf("ERROR : opening files\n"); // Error message if files cannot be opened
         return e_failure;
    }
    printf("INFO : files opened successfully\n");

    //skip 54-byte header
    fseek(decInfo->fptr_stego_image,54,SEEK_SET);
 
    //decode magic string
    char magic_string[3];
    decode_string_from_image(magic_string,strlen(MAGIC_STRING),decInfo->fptr_stego_image);
    
    if(strcmp(magic_string,MAGIC_STRING) != 0)  //checking magicstring or not 
    {
        printf("ERROR : Magic string mismatch\n");
        return e_failure;
    }

    //2.Decode extension size
    int extn_size;
    decode_integer_from_lsb(&extn_size,decInfo->fptr_stego_image);

    //3.decode extension
    char extn[10];
    decode_string_from_image(extn,extn_size,decInfo->fptr_stego_image);
    extn[extn_size] = '\0';

    //4.Decode secret file size
    int secret_size;
    decode_integer_from_lsb(&secret_size,decInfo->fptr_stego_image);
    printf("Decoded secret size : %d\n",secret_size);

    //5.Decode secret data
    char data;
    for(int i=0;i<secret_size;i++)
    {
        char arr[8];
        fread(arr,8,1,decInfo->fptr_stego_image);
        decode_byte_from_lsb(&data,arr);
        fwrite(&data,1,1,decInfo->fptr_output);
    }

    fclose(decInfo->fptr_stego_image);
    fclose(decInfo->fptr_output);

    printf("INFO : Decoding completed successfully\n");

    return e_success;
 }