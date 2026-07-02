# LSB Image Digital Steganography in C

A command-line application developed in **C** that securely hides and retrieves secret messages within **24-bit BMP images** using the **Least Significant Bit (LSB)** steganography technique. The project demonstrates low-level file handling, bit manipulation, and binary data processing while preserving the visual quality of the original image.

---

## Overview

Steganography is the practice of concealing information within another medium so that the existence of the hidden data is not apparent. This project implements **Least Significant Bit (LSB) Image Steganography**, where each bit of the secret message is embedded into the least significant bits of the image pixels.

The application supports both **encoding** (hiding a secret message inside an image) and **decoding** (retrieving the hidden message from the encoded image) through a simple command-line interface.

---

## Features

* Encode secret text into a 24-bit BMP image
* Decode hidden text from an encoded BMP image
* Preserve the visual appearance of the original image
* Validate BMP image format before processing
* Verify image capacity before encoding
* Support text files as secret messages
* Handle invalid inputs with appropriate error messages
* Modular and maintainable source code

---

## Technologies Used

| Category             | Technology                                                                        |
| -------------------- | --------------------------------------------------------------------------------- |
| Programming Language | C                                                                                 |
| Compiler             | GCC                                                                               |
| Platform             | Linux / Unix                                                                      |
| Concepts             | File Handling, Bit Manipulation, Binary Files, Structures, Command-Line Arguments |

---

## Project Structure

```text
LSB-Image-Steganography/
├── main.c
├── encode.c
├── decode.c
├── common.c
├── encode.h
├── decode.h
├── common.h
├── types.h
├── Makefile
├── sample.bmp
├── secret.txt
├── README.md
```

---

## Working Principle

### Encoding

1. Read the source BMP image.
2. Validate the BMP format.
3. Check whether the image has sufficient capacity.
4. Embed each bit of the secret message into the least significant bit of image bytes.
5. Generate the encoded output image.

### Decoding

1. Read the encoded BMP image.
2. Extract the least significant bits from image bytes.
3. Reconstruct the original secret message.
4. Save or display the decoded message.

---

## Build Instructions

Clone the repository:

```bash
git clone https://github.com/Shrinidhi-Khanchanale/LSB-image-digital-Stegnography.git
cd LSB-image-digital-Stegnography
```

Compile the project:

```bash
gcc *.c -o steganography
```

Or use Makefile (if available):

```bash
make
```

---

## Usage

### Encode a Secret Message

```bash
./steganography -e sample.bmp secret.txt output.bmp
```

### Decode a Secret Message

```bash
./steganography -d output.bmp decoded.txt
```

---

## Sample Output

### Encoding

```text
------------------------------------------
Encoding Started...

✔ Source image validated
✔ Secret file loaded
✔ Capacity verification successful
✔ Data encoded successfully

Output Image : output.bmp

Encoding Completed Successfully
------------------------------------------
```

### Decoding

```text
------------------------------------------
Decoding Started...

✔ Encoded image validated
✔ Secret message extracted successfully

Decoded Message:
Hello Embedded Systems!

Decoding Completed Successfully
------------------------------------------
```

---

## Concepts Demonstrated

* Least Significant Bit (LSB) Steganography
* Binary File Processing
* Bitmap (BMP) Image Format
* Bitwise Operations
* File Pointer Manipulation
* Command-Line Argument Parsing
* Modular Programming
* Error Handling
* Memory Management

---

## Learning Outcomes

This project provided practical experience in:

* Binary image processing
* Data hiding techniques
* Bit-level manipulation in C
* Reading and writing BMP image files
* Developing modular applications
* Designing efficient command-line utilities
* Debugging and testing system-level software

---

## Future Enhancements

* Password-protected encoding and decoding
* AES encryption before embedding
* Support for PNG and other image formats
* Hide multiple files within a single image
* Graphical User Interface (GUI)
* Support for audio and video steganography

---

## Author

**Shrinidhi Khanchanale**

Embedded Systems Trainee | C Programmer | IoT & Embedded Systems Enthusiast

GitHub: https://github.com/Shrinidhi-Khanchanale

---

## License

This project is intended for educational and learning purposes.

---

## Acknowledgements

This project was developed to strengthen understanding of systems programming concepts including binary file handling, bit manipulation, and secure information hiding techniques using the C programming language.
