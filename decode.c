#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "decode.h"
#include "common.h"
#include "types.h"

/* Function Definitions */

/* Validate decoding arguments */

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    // Check operation argument
    if (argv[2] == NULL)
    {
        return e_failure;
    }

    // Check stego image extension
    if (strstr(argv[2], ".bmp") == NULL)
    {
        return e_failure;
    }

    decInfo->stego_image_fname = argv[2];

    // Check output filename
    if (argv[3] != NULL)
    {
        decInfo->output_fname[0] = '\0';

        if (strlen(argv[3]) >= sizeof(decInfo->output_fname))
        {
            return e_failure;
        }

        strcpy(decInfo->output_fname, argv[3]);
    }
    else
    {
        strcpy(decInfo->output_fname, "decoded");
    }

    return e_success;
}


/* Open files */

Status open_decode_files(DecodeInfo *decInfo)
{
    // Open stego image
    decInfo->fptr_stego_image =
        fopen(decInfo->stego_image_fname, "rb");

    if (decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "Unable to open stego image\n");
        return e_failure;
    }

    return e_success;
}


/* Decode one byte from image LSB */

Status decode_byte_from_lsb(char *data, char *image_buffer)
{
    *data = 0;

    for (int i = 0; i < 8; i++)
    {
        *data = (*data << 1) | (image_buffer[i] & 1);
    }

    return e_success;
}


/* Decode 32-bit size from image LSB */

Status decode_size_from_lsb(int *size, char *image_buffer)
{
    unsigned int value = 0;

    for (int i = 0; i < 32; i++)
    {
        value = (value << 1) | (image_buffer[i] & 1);
    }

    *size = (int)value;

    return e_success;
}


/* Decode magic string */

Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo)
{
    char buffer[8];
    char data;

    for (int i = 0; i < strlen(magic_string); i++)
    {
        if (fread(buffer, 1, 8, decInfo->fptr_stego_image) != 8)
        {
            return e_failure;
        }

        decode_byte_from_lsb(&data, buffer);

        if (data != magic_string[i])
        {
            return e_failure;
        }
    }

    return e_success;
}


/* Decode secret file extension size */

Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    char buffer[32];

    if (fread(buffer, 1, 32, decInfo->fptr_stego_image) != 32)
    {
        return e_failure;
    }

    decode_size_from_lsb(&decInfo->extn_size, buffer);

    if (decInfo->extn_size <= 0 ||
        decInfo->extn_size >= sizeof(decInfo->extn_secret_file))
    {
        return e_failure;
    }

    return e_success;
}


/* Decode secret file extension */

Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char buffer[8];

    for (int i = 0; i < decInfo->extn_size; i++)
    {
        if (fread(buffer, 1, 8, decInfo->fptr_stego_image) != 8)
        {
            return e_failure;
        }

        decode_byte_from_lsb(&decInfo->extn_secret_file[i], buffer);
    }

    decInfo->extn_secret_file[decInfo->extn_size] = '\0';

    return e_success;
}


/* Decode secret file size */

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char buffer[32];
    int size;

    if (fread(buffer, 1, 32, decInfo->fptr_stego_image) != 32)
    {
        return e_failure;
    }

    decode_size_from_lsb(&size, buffer);

    if (size < 0)
    {
        return e_failure;
    }

    decInfo->size_secret_file = size;

    return e_success;
}


/* Decode secret file data */

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char buffer[8];
    char data;

    for (long i = 0; i < decInfo->size_secret_file; i++)
    {
        if (fread(buffer, 1, 8, decInfo->fptr_stego_image) != 8)
        {
            return e_failure;
        }

        decode_byte_from_lsb(&data, buffer);

        if (fwrite(&data, 1, 1, decInfo->fptr_output) != 1)
        {
            return e_failure;
        }
    }

    return e_success;
}


/* Main decoding function */

Status do_decoding(DecodeInfo *decInfo)
{
    // Step 1: Open stego image
    if (open_decode_files(decInfo) == e_failure)
    {
        printf("Error opening stego image\n");
        return e_failure;
    }

    printf("Stego image opened successfully\n");

    // Step 2: Skip BMP header (54 bytes)
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);

    // Step 3: Decode magic string
    if (decode_magic_string(MAGIC_STRING, decInfo) == e_failure)
    {
        printf("Magic string not found\n");
        return e_failure;
    }

    printf("Magic string verified successfully\n");

    // Step 4: Decode extension size
    if (decode_secret_file_extn_size(decInfo) == e_failure)
    {
        printf("Extension size decoding failed\n");
        return e_failure;
    }

    // Step 5: Decode extension
    if (decode_secret_file_extn(decInfo) == e_failure)
    {
        printf("Extension decoding failed\n");
        return e_failure;
    }

    printf("Secret file extension: %s\n", decInfo->extn_secret_file);

    // Step 6: Decode secret file size
    if (decode_secret_file_size(decInfo) == e_failure)
    {
        printf("Secret file size decoding failed\n");
        return e_failure;
    }

    printf("Secret file size: %ld bytes\n", decInfo->size_secret_file);

    // Step 7: Create output filename
    if (strlen(decInfo->output_fname) +
        strlen(decInfo->extn_secret_file) >= sizeof(decInfo->output_fname))
    {
        printf("Output filename too long\n");
        return e_failure;
    }

    strcat(decInfo->output_fname, decInfo->extn_secret_file);

    // Step 8: Open output file
    decInfo->fptr_output = fopen(decInfo->output_fname, "wb");

    if (decInfo->fptr_output == NULL)
    {
        perror("fopen");
        return e_failure;
    }

    // Step 9: Decode secret file data
    if (decode_secret_file_data(decInfo) == e_failure)
    {
        printf("Secret data decoding failed\n");
        return e_failure;
    }

    printf("Secret data decoded successfully\n");

    fclose(decInfo->fptr_stego_image);
    fclose(decInfo->fptr_output);

    decInfo->fptr_stego_image = NULL;
    decInfo->fptr_output = NULL;

    printf("Decoding completed successfully\n");

    return e_success;
}