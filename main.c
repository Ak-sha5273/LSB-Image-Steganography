/*Name:Akshata Babaleshwar
  Date: 13-11-26
  Description: Developed an image-based Steganography application in C using the Least Significant Bit (LSB) technique to hide secret messages inside BMP images.
               Implemented encoding and decoding functionalities using file handling, pointers, and bitwise operations to hide and retrieve messages.  */

#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

// Function declaration
OperationType check_operation_type(char *symbol);

// MAIN FUNCTION
int main(int argc, char *argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;

    if (argc < 3 || argc > 5)
    {
        printf("Usage:\n");
        printf("Encoding: ./steganography -e source.bmp secret.txt [output.bmp]\n");
        printf("Decoding: ./steganography -d stego.bmp [output_filename]\n");
        return 1;
    }

    // Encoding
    if (check_operation_type(argv[1]) == e_encode)
    {
        if (argc < 4)
        {
            printf("Missing secret file\n");
            return 1;
        }

        if (read_and_validate_encode_args(argv, &encInfo) == e_success)
        {
            if (do_encoding(&encInfo) == e_success)
            {
                printf("Encoding successful\n");
            }
            else
            {
                printf("Encoding failed\n");
                return 1;
            }
        }
        else
        {
            printf("Invalid encoding arguments\n");
            return 1;
        }
    }

    // Decoding
    else if (check_operation_type(argv[1]) == e_decode)
    {
        if (argc > 4)
        {
            printf("Too many decoding arguments\n");
            return 1;
        }

        if (read_and_validate_decode_args(argv, &decInfo) == e_success)
        {
            if (do_decoding(&decInfo) == e_success)
            {
                printf("Decoding successful\n");
            }
            else
            {
                printf("Decoding failed\n");
                return 1;
            }
        }
        else
        {
            printf("Invalid decoding arguments\n");
            return 1;
        }
    }

    else
    {
        printf("Unsupported operation\n");
        return 1;
    }

    return 0;
} // IMPORTANT: main() ends here


// Function definition must be OUTSIDE main()

OperationType check_operation_type(char *symbol)
{
    if (symbol == NULL)
    {
        return e_unsupported;
    }

    if (strcmp(symbol, "-e") == 0)
    {
        return e_encode;
    }

    if (strcmp(symbol, "-d") == 0)
    {
        return e_decode;
    }

    return e_unsupported;
}