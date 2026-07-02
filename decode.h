#ifndef DECODE_H
#define DECODE_H

#include <stdio.h>
#include "types.h"

// #define MAGIC_STRING "#*"
// #define MAX_SECRET_BUF_SIZE 1
// #define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
// #define MAX_FILE_SUFFIX 10

typedef struct _DecodeInfo
{
    // char image_data[MAX_IMAGE_BUF_SIZE];

    /* Secret File Info */
    char output_fname[100];
    FILE *fptr_output;

    // char extn_secret_file[MAX_FILE_SUFFIX];
    // int extn_size;
    // long size_secret_file;

    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

}DecodeInfo;

/* Function declarations */

/* Read and validate decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_file(DecodeInfo *decInfo);

/*skip the header file*/
Status skip_bmp_header(FILE *fptr);

Status decode_byte_from_lsb(char *image_buffer, char *data);

Status decode_integer_from_lsb(int *data,FILE *fptr_stego_image);

Status decode_string_from_image(char *str, int size, FILE *fptr_stego_image);

/* Store Magic String */
Status decode_magic_string(DecodeInfo *decInfo);

/* decode secret file extenstion size*/
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/* decode secret file extenstion */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Encode secret file size*/
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Encode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

#endif