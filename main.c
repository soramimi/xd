#include <stdio.h>
#include <memory.h>
#include <ctype.h>

/**
 * @brief Dumps binary data in hexadecimal format with ASCII representation
 * @param fp File pointer to read data from
 * 
 * Reads data from the file pointer in 16-byte chunks and displays each chunk
 * in hexadecimal format with corresponding ASCII characters. Non-printable
 * characters are displayed as dots.
 */
void dump(FILE *fp)
{
	unsigned int offset = 0;  // Current byte offset in file
	while (1) {
		char data[16];  // Buffer for 16 bytes of input data
		int n = fread(data, 1, 16, fp);  // Read up to 16 bytes
		if (n < 1) break;  // End of file reached
		char line[100];  // Output line buffer
		memset(line, 0, sizeof(line));
		sprintf(line, "%08x", offset);  // Format offset as 8-digit hex
		char *p = line + 9;  // Position after offset and space
		// Convert bytes to hex representation
		for (int i = 0; i < n; i++) {
			sprintf(p + 3 * i, "%02x", (unsigned char)data[i]);
		}

		p += 3 * 16;  // Move to ASCII section
		*p++ = '|';  // ASCII section separator

		// Convert bytes to printable ASCII characters
		for (int i = 0; i < n; i++) {
			int c = (unsigned char)data[i];
			p[i] = isprint(c) ? c : '.';  // Use '.' for non-printable chars
		}
		p += 16;
		*p++ = '|';  // End ASCII section
		// Fill null bytes with spaces for proper formatting
		for (char *q = line; q < p; q++) {
			if (*q == 0) {
				*q = ' ';
			}
		}
		puts(line);  // Output the formatted line
		offset += n;  // Update offset counter
	}
}

/**
 * @brief Converts binary data to C array format
 * @param fp File pointer to read data from
 * @param name Variable name for the generated C array
 * 
 * Reads binary data and outputs it as a C language array declaration
 * with hexadecimal byte values. Also generates a size constant.
 */
void cdata(FILE *fp, char const *name)
{
	unsigned int offset = 0;  // Current byte offset
	printf("const char %s_data[] = {\n", name);  // Start array declaration
	while (1) {
		char data[16];  // Buffer for input data
		int n = fread(data, 1, 16, fp);  // Read up to 16 bytes
		if (n < 1) break;  // End of file
		char line[100];  // Output line buffer
		memset(line, 0, sizeof(line));
		// Format each byte as 0xXX, hex constant
		for (int i = 0; i < n; i++) {
			sprintf(line + 4 + 5 * i, "0x%02x,", (unsigned char)data[i]);
		}
		char *p = line + 4 + 5 * 16;
		sprintf(p + 1, "// %08x", offset);  // Add offset comment
		p += 12;
		// Replace null bytes with spaces for formatting
		for (char *q = line; q < p; q++) {
			if (*q == 0) {
				*q = ' ';
			}
		}
		puts(line);  // Output the formatted line
		offset += n;  // Update byte counter
	}
	printf("};\n");  // Close array declaration
	printf("const unsigned int %s_size = %u;\n", name, offset);  // Output size constant
}

/**
 * @brief Main function - parses command line arguments and processes input
 * @param argc Number of command line arguments
 * @param argv Array of command line argument strings
 * @return 0 on success, 1 on error
 * 
 * Processes command line options:
 * -c <name>: Generate C array format with specified variable name
 * filename: Input file (uses stdin if not specified)
 */
int main(int argc, char **argv)
{
	char const *name = nullptr;      // Variable name for C array output
	char const *infile = nullptr;    // Input filename
	bool opt_c = false;              // Flag for C array output mode

	int argi = 1;  // Current argument index
	// Parse command line arguments
	while (argi < argc) {
		char const *arg = argv[argi++];
		if (*arg == '-') {  // Option argument
			if (strcmp(arg, "-c") == 0) {
				opt_c = true;
				// Next argument should be the variable name
				if (argi < argc) {
					name = argv[argi++];
				} else {
					fprintf(stderr, "expected: -c <name>\n");
					return 1;
				}

			}
		} else {  // Filename argument
			if (!infile) {
				infile = arg;
			} else {
				fprintf(stderr, "invalid argument: %s\n", arg);
				return 1;
			}
		}
	}

	// Open input file or use stdin
	FILE *fp = stdin;
	if (infile) {
		fp = fopen(infile, "rb");  // Open in binary mode
	}

	if (fp) {
		// Process the input based on selected mode
		if (opt_c) {
			cdata(fp, name);  // Generate C array format
		} else {
			dump(fp);         // Generate hex dump format
		}
		// Close file if we opened one
		if (infile) {
			fclose(fp);
		}
	} else {
		fprintf(stderr, "failed to open file: %s\n", infile);
	}

	return 0;
}

