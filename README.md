# LSB-Image-Steganography
A C-based Image Steganography project using the Least Significant Bit (LSB) technique to encode and decode secret messages within BMP images.

## 1. Project Description

This project implements an Image Steganography application using the Least Significant Bit (LSB) technique in C programming.

The application hides secret text or file data inside a BMP image by modifying the least significant bits of image data. The hidden information can later be extracted using the decoding process.

The project demonstrates the use of file handling, pointers, structures, and bitwise operations in C.

## 2. Features

- Encode secret files into BMP images.
- Decode and retrieve hidden data from stego images.
- Uses the Least Significant Bit (LSB) technique.
- Stores a magic string to identify encoded images.
- Stores the secret file extension and file size.
- Validates command-line arguments.
- Checks image capacity before encoding.
- Supports a default output image filename (`stego.bmp`).

## 3. Technologies Used

- Programming Language: C
- Compiler: GCC
- Image Format: BMP
- Operating System: Linux / Ubuntu

## 4. Concepts Used

- File Handling
- Pointers
- Structures
- Functions
- Command-Line Arguments
- Bitwise Operators
- Arrays and Strings
- Header Files

## 5. Project Structure

```text
LSB-Image-Steganography/
│
├── main.c
├── encode.c
├── encode.h
├── decode.c
├── decode.h
├── common.h
├── types.h
│
├── beautiful.bmp
│
├── screenshots/
│   ├── encoding.png
│   └── decoding.png
│
├── README.md
└── .gitignore
```

## 6. Working Principle

This project uses the Least Significant Bit (LSB) technique to hide secret information inside a BMP image.

### Encoding Process

1. Read the source BMP image and secret file.
2. Validate the input files and arguments.
3. Check whether the image has sufficient capacity.
4. Copy the BMP header to the output image.
5. Encode the magic string.
6. Encode the secret file extension and its size.
7. Encode the secret file size.
8. Encode the secret file data into the image's least significant bits.
9. Copy the remaining image data.

### Decoding Process

1. Open the stego image.
2. Skip the BMP header.
3. Verify the magic string.
4. Extract the secret file extension.
5. Extract the secret file size.
6. Retrieve the hidden file data.
7. Reconstruct the original secret file.

## 7. Compilation

Compile the source files using GCC:

```bash
gcc main.c encode.c decode.c -o steganography
```

## 8. Execution

### Encoding

```bash
./steganography -e beautiful.bmp secret.txt stego.bmp
```

Here:
- `-e` specifies the encoding operation.
- `beautiful.bmp` is the source image.
- `secret.txt` is the secret file.
- `stego.bmp` is the output image containing hidden data.

The output image filename is optional. If omitted, the program uses `stego.bmp`.

### Decoding

```bash
./steganography -d stego.bmp decoded
```

Here:
- `-d` specifies the decoding operation.
- `stego.bmp` is the encoded image.
- `decoded` is the output filename prefix.

The decoded file will be saved as `decoded.txt` when the original secret file extension is `.txt`.

## 9. Applications

- Hiding confidential text information inside images.
- Secure data communication demonstrations.
- Understanding digital information hiding.
- Learning image-based steganography techniques.

## 10. Advantages

- Simple implementation using C.
- Secret data is hidden inside an image.
- Demonstrates practical use of bitwise operations.
- Supports extraction of the original hidden file data.

## 11. Limitations

- The project works with BMP images.
- Encoding depends on the available image capacity.
- Modifying the encoded image may damage the hidden information.
- This implementation is not encryption; the hidden message is not encrypted.

## 12. Output Screenshots

### Encoding Process
<img width="637" height="427" alt="image" src="https://github.com/user-attachments/assets/c79b4bf3-491a-4abe-81b3-7026b0acd349" />

<img width="506" height="357" alt="image" src="https://github.com/user-attachments/assets/a461503f-fa42-4281-9f65-d796d6ab3220" />



### Decoding Process
<img width="642" height="237" alt="image" src="https://github.com/user-attachments/assets/18c0e090-140b-4fe4-aa02-cb7c5f37af12" />

<img width="591" height="132" alt="image" src="https://github.com/user-attachments/assets/f9461852-b639-47e6-8a45-4b02b18ccc0c" />



## 13. Learning Outcomes

- Understanding the LSB steganography technique.
- Learning how image data can store hidden information.
- Practicing file handling in C.
- Understanding pointers and structures.
- Implementing encoding and decoding operations.
- Improving logical thinking and problem-solving skills.

## 14. Author

**Akshata Babaleshwar**

C Programming Project – Image Steganography
