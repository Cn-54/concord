# Concord
concord is a simple light weight file analyser written in C

it uses the extension and magic bytes of the file to flag mismatches


## Features

* File extension detection
* Magic byte detection
* File type identification
* Extension/type mismatch detection
* Basic file metadata
* Hexadecimal display of magic bytes

## Example

```text
╔══════════════════════════════════════════╗
║                 CONCORD                  ║
║          File Forensics Analysis         ║
╚══════════════════════════════════════════╝

File: tests/image.png

== File Identification ====================
  Extension:       png
  Magic bytes:     89 50 4E 47 0D 0A 1A 0A
  Detected:        PNG
  Extension match: YES

== Metadata ================================
  File size:       4506 bytes
  Permissions:     777
  Last modified:   Wed Sep 30 12:11:11 2026
```

## Building

```bash
make
```

The executable will be placed in:

```text
bin/concord
```

## Usage

```bash
./bin/concord <file>
```

Example:

```bash
./bin/concord tests/image.png
```

## Current File Types

* PNG
* JPEG
* PDF
* EXE

## License

MIT License
