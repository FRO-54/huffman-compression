# Huffman Coding Compression and Decompression

This project implements Huffman Coding, a lossless data compression algorithm, in C. The program reads an input file, compresses its contents using Huffman coding, and allows decompression to retrieve the original data.

## Features
- Reads a file and calculates character frequencies.
- Builds a Huffman tree to generate optimal prefix codes.
- Compresses the file by encoding characters into binary Huffman codes.
- Decompresses the encoded file back to the original content.
- Displays Huffman codes for each character.

## How It Works
1. The program reads the input file and determines character frequencies.
2. A Huffman tree is built based on the frequencies.
3. Huffman codes are generated and stored in a table.
4. The file is compressed by replacing characters with their Huffman codes.
5. The compressed file can be decompressed using the Huffman tree.

## Prerequisites
- A C compiler (GCC recommended)
- A text file to compress

## Compilation & Execution
### Compile the program:
```sh
gcc huffman.c -o huffman
```

### Run the program:
```sh
./huffman
```

### Input:
The program prompts the user to enter the name of the file to compress.

### Output:
- A compressed file is generated.
- The compressed file can then be decompressed back to its original form.
- Displays Huffman codes used for compression.

## Example
```
Enter the name of the file to compress: sample.txt
File sample.txt has been compressed to compressed.bin.
File compressed.bin has been decompressed to decompressed.txt.
```

## File Details
- `compressed.bin`: The encoded version of the input file.
- `decompressed.txt`: The reconstructed file after decompression.

## Notes
- Only ASCII characters (0-255) are considered.
- Compression efficiency depends on character distribution in the file.
- Ensure the input file exists before running the program.


