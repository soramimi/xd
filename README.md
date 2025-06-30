# xd - Hexadecimal Dump Tool

A simple and efficient hexadecimal dump utility written in C that displays binary data in both hexadecimal and ASCII formats.

## Features

- **Hexadecimal Dump**: Display file contents in traditional hex dump format with ASCII representation
- **C Array Output**: Convert binary data to C language array declarations
- **Stdin Support**: Read from standard input when no file is specified
- **Lightweight**: Simple, fast implementation with minimal dependencies

## Usage

### Basic Hex Dump
```bash
xd [filename]
```

Display file contents in hexadecimal format:
```
00000000  48 65 6c 6c 6f 20 57 6f 72 6c 64 21 0a           |Hello World!.|
```

### C Array Output
```bash
xd -c <variable_name> [filename]
```

Generate C array declaration from binary data:
```c
const char hello_data[] = {
    0x48,0x65,0x6c,0x6c,0x6f,0x20,0x57,0x6f,0x72,0x6c,0x64,0x21,0x0a, // 00000000
};
const unsigned int hello_size = 13;
```

### Examples

```bash
# Dump a file
xd myfile.bin

# Read from stdin
cat myfile.bin | xd

# Generate C array from file
xd -c my_data myfile.bin

# Generate C array from stdin
cat myfile.bin | xd -c my_data
```

## Building

### Prerequisites
- GCC compiler
- Make

### Compile
```bash
make
```

### Install
```bash
make install
```

This installs the binary to `/usr/local/bin/xd`.

## Output Format

### Hex Dump Format
```
OFFSET    HEX BYTES (up to 16)                     |ASCII CHARS|
00000000  48 65 6c 6c 6f 20 57 6f 72 6c 64 21 0a  |Hello World!.|
```

- **OFFSET**: 8-digit hexadecimal byte offset
- **HEX BYTES**: Up to 16 bytes displayed as 2-digit hex values
- **ASCII CHARS**: Printable ASCII characters (non-printable shown as '.')

### C Array Format
- Generates `const char <name>_data[]` array with hex byte values
- Includes `const unsigned int <name>_size` with total byte count
- Each line contains up to 16 bytes with offset comments

## Alternative Usage

The `alias/` directory contains a shell alias that provides similar functionality using the standard `od` command:

```bash
source alias/xd.sh
```

This creates an alias: `alias xd="od -tx1z -Ax"`

