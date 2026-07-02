#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <stdlib.h>
#include <string.h>

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    /* Check source image file (.bmp) */
    if (argv[2] != NULL && strstr(argv[2], ".bmp") != NULL)
    {
        encInfo->src_image_fname = argv[2];
    }
    else
    {
        printf("Error: Source image must be .bmp file\n");
        return e_failure;
    }

    /* Check secret file */
    if (argv[3] != NULL)
    {
        encInfo->secret_fname = argv[3];

        char *extn = strstr(argv[3], ".");

        if (extn == NULL)
        {
            printf("Error: Secret file must have extension\n");
            return e_failure;
        }

        /* Validate extension */
        if (strcmp(extn, ".txt") == 0)
        {
            strcpy(encInfo->extn_secret_file, ".txt");
            encInfo->extn_size = 32;
        }
        else if (strcmp(extn, ".sh") == 0)
        {
            strcpy(encInfo->extn_secret_file, ".sh");
            encInfo->extn_size = 24;
        }
        else if (strcmp(extn, ".c") == 0)
        {
            strcpy(encInfo->extn_secret_file, ".c");
            encInfo->extn_size = 16;
        }
        else
        {
            printf("Error: Unsupported secret file format\n");
            return e_failure;
        }
    }
    else
    {
        printf("Error: Secret file not provided\n");
        return e_failure;
    }

    /* Check stego image name */
    if (argv[4] != NULL)
    {
        if (strstr(argv[4], ".bmp") != NULL)
        {
            encInfo->stego_image_fname = argv[4];
        }
        else
        {
            printf("Error: Stego image must be .bmp\n");
            return e_failure;
        }
    }
    else
    {
        encInfo->stego_image_fname = "stego.bmp";
    }

    return e_success;
}
// Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
// {
//     if (argv[2] != NULL && strstr(argv[2], ".bmp") != NULL) // Check if source image is a valid .bmp file
//     {
//         encInfo->src_image_fname = argv[2];

//         if (argv[3] != NULL && strstr(argv[3], ".") != NULL) // Check if secret file is provided and is valid
//         {
//             encInfo->secret_fname = argv[3];

//             if (argv[4] != NULL) // Check if stego image is provided; if not, use default name
//             {
//                 if (strstr(argv[4], ".bmp") != NULL)
//                 {
//                     encInfo->stego_image_fname = argv[4];
//                 }
//                 else
//                 {
//                     return e_failure;
//                 }
//             }
//             else
//             {
//                 encInfo->stego_image_fname = "stego.bmp"; // Default stego image filename
//             }
//             return e_success;
//         }
//     }
//     return e_failure;
// }

uint get_image_size_for_bmp(FILE *fptr_image) // Function to get the image size of a BMP file
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{

    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");// Open the source image file in binary read mode
   
    if (encInfo->fptr_src_image == NULL)   // Check if the source image file was opened successfully
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    encInfo->fptr_secret = fopen(encInfo->secret_fname, "rb"); // Open the secret file in binary read mode
    
    if (encInfo->fptr_secret == NULL) // Check if the secret file was opened successfully
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

   
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");  // Open the stego image file in binary write mode
 
    if (encInfo->fptr_stego_image == NULL)  // Check if the stego image file was opened successfully
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;\

}

uint get_file_size(FILE *fptr) // Function to get the size of a file
{
    fseek(fptr, 0, SEEK_END);
    uint size = ftell(fptr);// Get the current position, which is the size of the file
    rewind(fptr);  // Rewind the file pointer to the beginning
    return size;
}
Status check_capacity(EncodeInfo *encInfo) // Function to check if the image can hold the secret file
{
    // Get the capacity of the image and the size of the secret file
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);

    if((encInfo->image_capacity-54) > (strlen(MAGIC_STRING) + sizeof(int) + strlen(encInfo->extn_secret_file) + sizeof(int) + encInfo->size_secret_file )*8)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image) // Function to copy the BMP header from source to destination
{
    fseek(fptr_src_image,0, SEEK_SET);
    char arr[54];
    fread(arr,54,1,fptr_src_image);
    fwrite(arr,54,1,fptr_dest_image);
    return e_success;
}
Status encode_byte_to_lsb(char data, char *image_buffer) // Function to encode a byte of data into the least significant bit of image pixels
{
    for(int i = 0; i < 8; i++)
    {
        image_buffer[i] = image_buffer[i]&(~1);// Clear the least significant bit
        char get = (data & 1<<i)>>i;// Extract the bit from the integer
        image_buffer[i] = image_buffer[i] | get;// Set the least significant bit to the extracted bit
    }
    return e_success;
}

Status encode_string_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char arr[8];
    for(int i = 0; i < size; i++)
    {
        fread(arr, 8, 1, fptr_src_image);
        encode_byte_to_lsb(data[i], arr);
        fwrite(arr, 8, 1, fptr_stego_image);
    }
    return e_success;
}
Status encode_integer_to_lsb(int data, char *image_buffer)// Function to encode an integer into the least significant bits of the image
{
    for(int i = 0; i < 32; i++)   
    {
        image_buffer[i] = image_buffer[i]&(~1);// Clear the least significant bit
        char get = (data & 1<<i)>>i;// Extract the bit from the integer
        image_buffer[i] = image_buffer[i] | get;// Set the least significant bit to the extracted bit
    }
   return e_success;
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo) // Function to encode the magic string into the image
{
    encode_string_to_image(MAGIC_STRING, strlen(MAGIC_STRING), encInfo->fptr_src_image, encInfo->fptr_stego_image);
    return e_success;
}

Status encode_secret_file_extn_size(const char *file_extn, EncodeInfo *encInfo)// Function to encode the size of the secret file extension into the image
{
    char arr[32];
    int size = strlen(file_extn);

    fread(arr,32,1,encInfo->fptr_src_image);
    encode_integer_to_lsb(size,arr);
    fwrite(arr,32,1,encInfo->fptr_stego_image);

    return e_success;
}
Status encode_secret_file_extn(EncodeInfo *encInfo)// Function to encode the secret file extension into the image
{
    encode_string_to_image(encInfo->extn_secret_file,strlen(encInfo->extn_secret_file),encInfo->fptr_src_image,encInfo->fptr_stego_image);

    return e_success;
}
Status encode_secret_file_size(EncodeInfo *encInfo)
{
    char arr[32];
 
    fread(arr, 32, 1, encInfo->fptr_src_image);
    encode_integer_to_lsb(encInfo->size_secret_file, arr);
    fwrite(arr, 32, 1, encInfo->fptr_stego_image);

    return e_success;
}
Status encode_secret_file_data(EncodeInfo *encInfo)// Function to encode the secret file data into the image
{
    rewind(encInfo->fptr_secret); // Rewind the secret file pointer

    char data_str[encInfo->size_secret_file];

    fread(data_str, encInfo->size_secret_file, 1, encInfo->fptr_secret);

    encode_string_to_image(data_str,encInfo->size_secret_file,encInfo->fptr_src_image,encInfo->fptr_stego_image);

    return e_success;
}
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest) 
// Function to copy the remaining image data (after encoding) to the stego image
{
    char arr;
    while(fread(&arr,1,1,fptr_src) == 1)
    {
    fwrite(&arr, 1, 1,fptr_dest);
    }
    return e_success;
}
Status do_encoding(EncodeInfo *encInfo)
{
    if (open_files(encInfo) == e_failure) // Open the necessary files for encoding
    {
        return e_failure;
    }
    printf("opened file successfully\n");

    if(check_capacity(encInfo) == e_failure)  // Check if the image has enough capacity to hold the secret file
    {
        return e_failure;
    }

    if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image)  == e_failure)    // Copy the BMP header from source to stego image
    {
        return e_failure;
    }
    printf("bmp header copied successfully\n");

    if(encode_magic_string(MAGIC_STRING, encInfo) == e_failure)    // Encode the magic string into the image
    {
        return e_failure;
    }
    printf("magic string encoded successfully\n"); 

    if(encode_secret_file_extn_size(encInfo->extn_secret_file, encInfo) == e_failure)  // Encode the secret file extension size
    {
        return e_failure;
    }
    printf("secret file extn size encoded successfully\n");

    if(encode_secret_file_extn(encInfo) == e_failure)  // Encode the secret file extension
    {
        return e_failure;
    }
    printf("secret file extn encoded successfully\n");

    if(encode_secret_file_size(encInfo) == e_failure)  
    // Encode the secret file size
    {
        return e_failure;
    }
    printf("secret file data size encoded successfully\n");
    
    if(encode_secret_file_data(encInfo) == e_failure)  // Encode the secret file data
    {
        return e_failure;
    }
    printf("secret file data encoded successfully\n");

    if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_failure)  // Copy the remaining image data after encoding
    {
        return e_failure;
    }
    printf("remaining_img_data copied successfully\n");

    // Close all the opened files
    fclose(encInfo->fptr_src_image);
    fclose(encInfo->fptr_stego_image);
    fclose(encInfo->fptr_secret);

    return e_success;
}