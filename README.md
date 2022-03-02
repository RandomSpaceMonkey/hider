# HIDER

Hider is a tool to encode and decode text into png files.

### Basic Usage
```sh
./hider encode linux.png
```

hider accepts 2 positional parameters:
1. The mode - encode/decode
2. The name of the png file to encode or decode


There are 2 scripts in the project:

1. encode.sh - creates a copy of linux.png called linux.copy.png, then compiles `hider.c` and then runs hider in `encode` mode on linux.copy.png
2. decode.sh - compiles `hider.c` and runs hider in decode mode on linux.copy.png

hider was compiled and developed using gcc version 9.3.0 on WSL running Ubuntu 20.

### *References*

1. PNG file format - [https://en.wikipedia.org/wiki/Portable_Network_Graphics](https://en.wikipedia.org/wiki/Portable_Network_Graphics)
2. Original idea - [https://www.youtube.com/watch?v=M9ZwuIv3xz8&amp;t=1736s](https://www.youtube.com/watch?v=M9ZwuIv3xz8&t=1736s)
