#ifndef DECODE_H
#define DECODE_H

#include <stdio.h>
#include "types.h"

typedef struct _DecodeInfo
{
    /* Stego Image Information */
    char *stego_image_fname;
    FILE *fptr_stego_image;

    /* Output Secret File Information */
    char output_fname[100];
    FILE *fptr_output;

    /* Secret File Information */
    char extn_secret_file[5];
    int extn_size;
    long size_secret_file;

} DecodeInfo;

/* Validate decoding arguments */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Open files */
Status open_decode_files(DecodeInfo *decInfo);

/* Decode magic string */
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo);

/* Decode extension size */
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/* Decode extension */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Decode secret file data */
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Decode one byte from image LSB */
Status decode_byte_from_lsb(char *data, char *image_buffer);

/* Decode size from image LSB */
Status decode_size_from_lsb(int *size, char *image_buffer);

#endif