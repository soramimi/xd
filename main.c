#include <stdio.h>
#include <memory.h>
#include <ctype.h>

void dump(FILE *fp)
{
	unsigned int offset = 0;
	while (1) {
		char data[16];
		int n = fread(data, 1, 16, fp);
		if (n < 1) break;
		char line[100];
		memset(line, 0, sizeof(line));
		sprintf(line, "%08x", offset);
		char *p = line + 9;
		for (int i = 0; i < n; i++) {
			sprintf(p + 3 * i, "%02x", data[i]);
		}

		p += 3 * 16;
		*p++ = '|';

		for (int i = 0; i < n; i++) {
			int c = (unsigned char)data[i];
			p[i] = isprint(c) ? c : '.';
		}
		p += 16;
		*p++ = '|';
		for (char *q = line; q < p; q++) {
			if (*q == 0) {
				*q = ' ';
			}
		}
		puts(line);
		offset += n;
	}
}

void cdata(FILE *fp, char const *name)
{
	unsigned int offset = 0;
	printf("const char %s_data[] = {\n", name);
	while (1) {
		char data[16];
		int n = fread(data, 1, 16, fp);
		if (n < 1) break;
		char line[100];
		memset(line, 0, sizeof(line));
		for (int i = 0; i < n; i++) {
			sprintf(line + 4 + 5 * i, "0x%02x,", data[i]);
		}
		char *p = line + 4 + 5 * 16;
		sprintf(p + 1, "// %08x", offset);
		p += 12;
		for (char *q = line; q < p; q++) {
			if (*q == 0) {
				*q = ' ';
			}
		}
		puts(line);
		offset += n;
	}
	printf("};\n");
	printf("const unsigned int %s_size = %u;\n", name, offset);
}

int main(int argc, char **argv)
{
	char const *name = nullptr;
	char const *infile = nullptr;
	bool opt_c = false;

	int argi = 1;
	while (argi < argc) {
		char const *arg = argv[argi++];
		if (*arg == '-') {
			if (strcmp(arg, "-c") == 0) {
				opt_c = true;
				if (argi < argc) {
					name = argv[argi++];
				} else {
					fprintf(stderr, "expected: -c <name>\n");
					return 1;
				}

			}
		} else {
			if (!infile) {
				infile = arg;
			} else {
				fprintf(stderr, "invalid argument: %s\n", arg);
				return 1;
			}
		}
	}

	FILE *fp = stdin;
	if (infile) {
		fp = fopen(infile, "rb");
	}

	if (fp) {
		if (opt_c) {
			cdata(fp, name);
		} else {
			dump(fp);
		}
		if (infile) {
			fclose(fp);
		}
	} else {
		fprintf(stderr, "failed to open file: %s\n", infile);
	}

	return 0;
}

