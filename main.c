/* 
   Name:Shrinidhi Khanchanale
   Date:12/03/26
   Batch id:25040_055
   Title:LSB image digital stegonography
   Description:A practice of concealing a file,message , image or video within another file

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "decode.h"

OperationType check_operation_type(char *argv[])
{
    if (strcmp(argv[1], "-e") == 0)
        return e_encode;
    else if (strcmp(argv[1], "-d") == 0)
        return e_decode;
    else
        return e_unsupported;
}
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Use:\n");
        printf("./a.out -e <input.bmp> <secret.txt/.c/.sh> <output.bmp>\n");
        printf("./a.out -d <stego.bmp> [output file name]\n");
        return 1;
    }

    OperationType res = check_operation_type(argv);

    /* ---------------- ENCODING ---------------- */

    if (res == e_encode)
    {
        printf("Selected encoding\n");

        EncodeInfo encInfo;

        if (read_and_validate_encode_args(argv, &encInfo) == e_success)
        {
            if (do_encoding(&encInfo) == e_success)
            {
                printf("Encoding successful\n");
            }
            else
            {
                fprintf(stderr,"Error: Encoding failed\n");
                return 1;
            }
        }
        else
        {
            fprintf(stderr,"Error: Invalid encoding arguments\n");
            return 1;
        }
    }

    /* ---------------- DECODING ---------------- */

    else if (res == e_decode)
    {
        printf("Selected decoding\n");

        DecodeInfo decInfo;

        if (read_and_validate_decode_args(argv, &decInfo) == e_success)
        {
            if (do_decoding(&decInfo) == e_success)
            {
                printf("Decoding successful\n");
            }
            else
            {
                fprintf(stderr,"Error: Decoding failed\n");
                return 1;
            }
        }
        else
        {
            fprintf(stderr,"Error: Invalid decoding arguments\n");
            return 1;
        }
    }

    else
    {
        fprintf(stderr,"Invalid option\n");
        return 1;
    }

    return 0;
}
