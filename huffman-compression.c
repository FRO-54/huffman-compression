#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_HT 100

struct MinHeapNode {
    char data;
    unsigned freq;
    struct MinHeapNode *left, *right;
};

struct MinHeap {
    unsigned size;
    unsigned capacity;
    struct MinHeapNode** nodes;
};

struct MinHeapNode* newNode(char data, unsigned freq) {
    struct MinHeapNode* node = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    node->data = data;
    node->freq = freq;
    node->left = node->right = NULL;
    return node;
}

struct MinHeap* createMinHeap(unsigned capacity) {
    struct MinHeap* heap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    heap->size = 0;
    heap->capacity = capacity;
    heap->nodes = (struct MinHeapNode**)malloc(heap->capacity * sizeof(struct MinHeapNode*));
    return heap;
}

void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) {
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(struct MinHeap* heap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < heap->size && heap->nodes[left]->freq < heap->nodes[smallest]->freq)
        smallest = left;

    if (right < heap->size && heap->nodes[right]->freq < heap->nodes[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swapMinHeapNode(&heap->nodes[smallest], &heap->nodes[idx]);
        minHeapify(heap, smallest);
    }
}

int isSizeOne(struct MinHeap* heap) {
    return (heap->size == 1);
}

struct MinHeapNode* extractMin(struct MinHeap* heap) {
    struct MinHeapNode* temp = heap->nodes[0];
    heap->nodes[0] = heap->nodes[--heap->size];
    minHeapify(heap, 0);
    return temp;
}

void insertMinHeap(struct MinHeap* heap, struct MinHeapNode* node) {
    ++heap->size;
    int i = heap->size - 1;

    while (i && node->freq < heap->nodes[(i - 1) / 2]->freq) {
        heap->nodes[i] = heap->nodes[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    heap->nodes[i] = node;
}

void buildMinHeap(struct MinHeap* heap) {
    int n = heap->size - 1;
    int i;
    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(heap, i);
}

struct MinHeap* createAndBuildMinHeap(char data[], int freq[], int size) {
    struct MinHeap* heap = createMinHeap(size);
    int i;

    for (i = 0; i < size; ++i)
        heap->nodes[i] = newNode(data[i], freq[i]);

    heap->size = size;
    buildMinHeap(heap);
    return heap;
}

struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size) {
    struct MinHeapNode *left, *right, *top;
    struct MinHeap* heap = createAndBuildMinHeap(data, freq, size);

    while (!isSizeOne(heap)) {
        left = extractMin(heap);
        right = extractMin(heap);

        top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;

        insertMinHeap(heap, top);
    }
    return extractMin(heap);
}

int isLeaf(struct MinHeapNode* node) {
    return !(node->left) && !(node->right);
}

void printCodes(struct MinHeapNode* root, int arr[], int top, char** huffmanTable) {
    int i;

    if (root->left) {
        arr[top] = 0;
        printCodes(root->left, arr, top + 1, huffmanTable);
    }

    if (root->right) {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1, huffmanTable);
    }

    if (isLeaf(root)) {
        char* code = (char*)malloc(top + 1);
        for (i = 0; i < top; i++) {
            code[i] = arr[i] + '0';
        }
        code[top] = '\0';
        huffmanTable[(int)root->data] = code;
        printf("%c: %s\n", root->data, code);
    }
}

void HuffmanCodes(char data[], int freq[], int size, char** huffmanTable) {
    struct MinHeapNode* root = buildHuffmanTree(data, freq, size);
    int arr[MAX_TREE_HT], top = 0;
    printCodes(root, arr, top, huffmanTable);
}

void calculateFrequencies(const char* filename, int freq[], char data[], int* size) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Could not open file %s\n", filename);
        exit(1);
    }

    char c;
    int uniqueChars = 0;
    while ((c = fgetc(file)) != EOF) {
        if (freq[(int)c] == 0) {
            data[uniqueChars++] = c;
        }
        freq[(int)c]++;
    }
    *size = uniqueChars;
    fclose(file);
}

void compressFile(const char* filename, const char* compressedFilename, char** huffmanTable) {
    FILE* file = fopen(filename, "r");
    FILE* outputFile = fopen(compressedFilename, "wb");

    if (!file || !outputFile) {
        printf("Could not open files for compression.\n");
        exit(1);
    }

    char c;
    unsigned char byte = 0;
    int bitCount = 0;

    while ((c = fgetc(file)) != EOF) {
        char* code = huffmanTable[(int)c];
        int i;
        for (i = 0; code[i] != '\0'; i++) {
            if (code[i] == '1') {
                byte |= (1 << (7 - bitCount));
            }
            bitCount++;

            if (bitCount == 8) {
                fwrite(&byte, sizeof(unsigned char), 1, outputFile);
                bitCount = 0;
                byte = 0;
            }
        }
    }

    if (bitCount > 0) {
        fwrite(&byte, sizeof(unsigned char), 1, outputFile);
    }

    fclose(file);
    fclose(outputFile);
}

void decompressFile(const char* compressedFilename, const char* decompressedFilename, struct MinHeapNode* root) {
    FILE* file = fopen(compressedFilename, "rb");
    FILE* outputFile = fopen(decompressedFilename, "w");

    if (!file || !outputFile) {
        printf("Could not open files for decompression.\n");
        exit(1);
    }

    struct MinHeapNode* current = root;
    unsigned char byte;
    int bit;
    while (fread(&byte, sizeof(unsigned char), 1, file)) {
        for (bit = 7; bit >= 0; bit--) {
            if (byte & (1 << bit)) {
                current = current->right;
            } else {
                current = current->left;
            }

            if (isLeaf(current)) {
                fputc(current->data, outputFile);
                current = root;
            }
        }
    }

    fclose(file);
    fclose(outputFile);
}

int main() {
    int size, i;
    char inputFilename[256], outputFilename[256] = "compressed.bin", decompressedFilename[256] = "decompressed.txt";
    char data[256];
    int freq[256] = {0};
  
	printf("\n\n*****DATA COMPRESSION USING HUFFMAN CODING TECHNIQUE*****\n");
	
    printf("Enter the name of the file to compress: ");
    scanf("%s", inputFilename);
	
	// Calculate frequencies and unique characters
    calculateFrequencies(inputFilename, freq, data, &size);

    // Prepare actual data and frequencies for Huffman tree
    int actualFreq[size];
    char actualData[size];
    int index = 0;

    for (i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            actualData[index] = i;
            actualFreq[index] = freq[i];
            index++;
        }
    }

    // Generate Huffman codes
    char* huffmanTable[256] = {0};
    HuffmanCodes(actualData, actualFreq, size, huffmanTable);

    // Compress the file
    compressFile(inputFilename, outputFilename, huffmanTable);
    printf("File %s has been compressed to %s.\n", inputFilename, outputFilename);

    //  Decompress the file
    struct MinHeapNode* root = buildHuffmanTree(actualData, actualFreq, size);
    decompressFile(outputFilename, decompressedFilename, root);
    printf("File %s has been decompressed to %s.\n", outputFilename, decompressedFilename);

    return 0;
}

