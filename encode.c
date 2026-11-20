#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "common.h"
#include <string.h>

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
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

uint get_file_size(FILE *fptr)
{
    // Find the size of secret file data
    // return secret file size


    fseek(fptr, 0, SEEK_END);

    uint size = ftell(fptr);

    rewind(fptr);

    return size;




}

/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    // step 1 -> check argv[2] is having .bmp or not
        //yes -> store the file name into encInfo -> src_image_fname = argv[2]
        //no - > return e_failure

    // step 2 -> check argv[3] is having extn there or not
        // yes - > store the file name into encInfo -> secret_fname = argv[3]
        // no -> return e_failure

    // step 3 -> check argv[4] is NULL or not
        //yes -> check argv[4] is having .bmp or not
                //yes -> store the file name into encInfo -> stego_image_fname = argv[4]
                //no - > return e_failure

        //no -> store default name the encInfo -> stego_image_fname = "stego.bmp"

    // return e_success;

    // Check source image extension
    if (strstr(argv[2], ".bmp") == NULL)
    {
        return e_failure;
    }

    encInfo->src_image_fname = argv[2];

    // Check secret file extension
    char *ptr = strrchr(argv[3], '.');

    if (ptr == NULL)
    {
        return e_failure;
    }

    encInfo->secret_fname = argv[3];

    if (strlen(ptr) >= sizeof(encInfo->extn_secret_file))
    {
        return e_failure;
    }

    strcpy(encInfo->extn_secret_file, ptr);

    // Check output file
    if (argv[4] != NULL)
    {
        if (strstr(argv[4], ".bmp") == NULL)
        {
            return e_failure;
        }

        encInfo->stego_image_fname = argv[4];
    }
    else
    {
        encInfo->stego_image_fname = "stego.bmp";
    }

    return e_success;
}

Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
   encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

        return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

        return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

        return e_failure;
    }

    // No failure return e_success
    return e_success;
}

Status check_capacity(EncodeInfo *encInfo)
{
    // Magic                 = 2 × 8 = 16
    // Extension size        = 4 × 8 = 32
    // Extension             = 4 × 8 = 32
    // Secret size           = 4 × 8 = 32
    // Secret data           = 9 × 8 = 72

    //step 1 -> encInfo -> image_capacity = call get_image_size_for_bmp(encoInfo -> fptr_src_image)

    //step 2 -> encInfo -> size_secret_file = call get_file_size(encInfo -> fptr_secret)

    //step 3 -> check encInfo -> image_capacity > 16 + 32 + (strlen(encInfo -> extn_secret_file) * 8) + 32 + (encInfo -> size_secret_file * 8)
                //t -> return e_success;
                //f -> return e_failure


    encInfo->image_capacity =
        get_image_size_for_bmp(encInfo->fptr_src_image);

    encInfo->size_secret_file =
        get_file_size(encInfo->fptr_secret);

    if (encInfo->image_capacity >=
        (16 + 32 + (strlen(encInfo->extn_secret_file) * 8)
        + 32 + (encInfo->size_secret_file * 8)))
    {
        return e_success;
    }

    return e_failure;



   
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    // step 1 -> rewind source file pointer
    // declare one character buffer of size 54

    // step 2 -> read 54 bytes from source file fread()

    // step 3 -> write 54 bytes to dest file fwrite()

    // return e_success


    char buffer[54];
   rewind(fptr_src_image);

if (fread(buffer, 1, 54, fptr_src_image) != 54)
{
    return e_failure;
}

if (fwrite(buffer, 1, 54, fptr_dest_image) != 54)
{
    return e_failure;
}

return e_success;




}
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    // char buffer[8];

    //step 1 -> read 8 bytes from source file

    //step 2 -> call encode_byte_to_lsb(magic_string[i], buffer)

    //step 3 -> write the buffer into dest file

    // repeat this for strlen(magic_string) times from step 1

    char buffer[8];

    for (int i = 0; i < strlen(magic_string); i++)
    {
        if (fread(buffer, 1, 8, encInfo->fptr_src_image) != 8)
        {
            return e_failure;
        }

        encode_byte_to_lsb(magic_string[i], buffer);

        if (fwrite(buffer, 1, 8, encInfo->fptr_stego_image) != 8)
        {
            return e_failure;
        }
    }

    return e_success;
}
Status encode_secret_file_extn_size(int size, EncodeInfo *encInfo)
{
    // char buffer[32];

    //step 1 -> read 32 bytes from source file

    //step 2 -> call encode_size_to_lsb(size, buffer)

    //step 3 -> write the buffer into dest file

    
    char buffer[32];

    if (fread(buffer, 1, 32, encInfo->fptr_src_image) != 32)
{
    return e_failure;
}

encode_size_to_lsb(size, buffer);

if (fwrite(buffer, 1, 32, encInfo->fptr_stego_image) != 32)
{
    return e_failure;
}
return e_success;

}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    // char buffer[8];

    //step 1 -> read 8 bytes from source file

    //step 2 -> call encode_byte_to_lsb(file_extn[i], buffer)

    //step 3 -> write the buffer into dest file

    // repeat this for strlen(file_extn) times from step 1  
    
    char buffer[8];

    for (int i = 0; i < strlen(file_extn); i++)
{
        if (fread(buffer, 1, 8, encInfo->fptr_src_image) != 8)
{
    return e_failure;
}

encode_byte_to_lsb(file_extn[i], buffer);

if (fwrite(buffer, 1, 8, encInfo->fptr_stego_image) != 8)
{
    return e_failure;
}


    }

    return e_success;
}


Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    // char buffer[32];

    //step 1 -> read 32 bytes from source file

    //step 2 -> call encode_size_to_lsb(file_size, buffer)

    //step 3 -> write the buffer into dest file
   

    char buffer[32];

    if (fread(buffer, 1, 32, encInfo->fptr_src_image) != 32)
{
    return e_failure;
}

encode_size_to_lsb(file_size, buffer);

if (fwrite(buffer, 1, 32, encInfo->fptr_stego_image) != 32)
{
    return e_failure;
}
 return e_success;

}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    // Move pointer to beginning of secret file

    rewind(encInfo->fptr_secret);

    // step 1 -> read secret data from file and store it into encInfo -> secret_data

    // char buffer[8];

    char buffer[8];

    // step 2 -> read 8 bytes from source file

    // step 3 -> call encode_byte_to_lsb(encInfo -> secret_data[i], buffer)

    // step 4 -> write the buffer into dest file

    // repeat this for size_secret_file times from step 2

    for (int i = 0; i < encInfo->size_secret_file; i++)
    {
        if (fread(&encInfo->secret_data[0], 1, 1, encInfo->fptr_secret) != 1)
        {
            return e_failure;
        }

        if (fread(buffer, 1, 8, encInfo->fptr_src_image) != 8)
        {
            return e_failure;
        }

        encode_byte_to_lsb(encInfo->secret_data[0], buffer);

        if (fwrite(buffer, 1, 8, encInfo->fptr_stego_image) != 8)
        {
            return e_failure;
        }
    }

    return e_success;
}


Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for (int i = 0; i < 8; i++)
    {
        image_buffer[i] = (image_buffer[i] & 0xFE) |
                          (((unsigned char)data >> (7 - i)) & 1);
    }

    return e_success;
}

Status encode_size_to_lsb(int size, char *imageBuffer)
{
    for (int i = 0; i < 32; i++)
    {
        imageBuffer[i] = (imageBuffer[i] & 0xFE) |
                         (((unsigned int)size >> (31 - i)) & 1);
    }

    return e_success;
}


Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;

    while (fread(&ch, 1, 1, fptr_src) == 1)
    {
        if (fwrite(&ch, 1, 1, fptr_dest) != 1)
        {
            return e_failure;
        }
    }

    if (feof(fptr_src))
    {
        return e_success;
    }

    return e_failure;
}
Status do_encoding(EncodeInfo *encInfo)
{
    //step 1 -> check open_files(encInfo) is returning e_success or not
            //no -> print error return e_failure
            //yes -> print success and goto next step

    //step 2 -> check check_capacity(encInfo) is returning e_success or not
            //no -> print error return e_failure
            //yes -> print success and goto next step

    //step 3 -> call copy_bmp_header(encInfo -> fptr_src_image, encInfo -> fptr_stego_image)is returning e_success or not
            //no -> print error return e_failure
            //yes -> print success and goto next step

    //step 4 -> call encode_magic_string(MAGIC_STRING, encInfo)

    //step 5 -> call encode_secret_file_extn_size(strlen(encInfo -> extn_secret_file), encInfo)

    //step 6 -> call encode_secret_file_extn(encInfo -> extn_secret_file, encInfo)

    //step 7 -> call encode_secret_file_size(encInfo ->  size_secret_file, encInfo)

    //step 8 -> call encode_secret_file_data(encInfo)

    //step 9 -> copy_remaining_img_data(encInfo -> fptr_src_image, encInfo -> fptr_stego_image)is returning e_success or not
            //no -> print error return e_failure
            //yes -> print success


    // return e_success

    // Step 1: Open files
    if (open_files(encInfo) == e_failure)
    {
        printf("Error opening files\n");
        return e_failure;
    }

    printf("Files opened successfully\n");

    // Step 2: Check capacity
    if (check_capacity(encInfo) == e_failure)
    {
        printf("Insufficient image capacity\n");
        return e_failure;
    }

    printf("Capacity checked successfully\n");

    // Step 3: Copy BMP header
    if (copy_bmp_header(encInfo->fptr_src_image,
                        encInfo->fptr_stego_image) == e_failure)
    {
        printf("BMP header copying failed\n");
        return e_failure;
    }

    printf("BMP header copied successfully\n");

    // Step 4: Encode magic string
    if (encode_magic_string(MAGIC_STRING, encInfo) == e_failure)
    {
        printf("Magic string encoding failed\n");
        return e_failure;
    }

    // Step 5: Encode extension size
    if (encode_secret_file_extn_size(strlen(encInfo->extn_secret_file), encInfo) == e_failure)
    {
        printf("Extension size encoding failed\n");
        return e_failure;
    }

    // Step 6: Encode extension
    if (encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_failure)
    {
        printf("Extension encoding failed\n");
        return e_failure;
    }

    // Step 7: Encode secret file size
    if (encode_secret_file_size(encInfo->size_secret_file, encInfo) == e_failure)
    {
        printf("Secret file size encoding failed\n");
        return e_failure;
    }

    // Step 8: Encode secret file data
    if (encode_secret_file_data(encInfo) == e_failure)
    {
        printf("Secret file data encoding failed\n");
        return e_failure;
    }

    // Step 9: Copy remaining image data
    if (copy_remaining_img_data(encInfo->fptr_src_image,
                                encInfo->fptr_stego_image) == e_failure)
    {
        printf("Remaining image data copying failed\n");
        return e_failure;
    }

    printf("Encoding completed successfully\n");

    return e_success;
}