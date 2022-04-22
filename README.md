# TuttleFS

TurtleFS is Delay simulation file system using FUSE.

* Mirroring HOME directory.
* Read file with given delay.

It does not take care about security so you *SHOULD NOT* use it.

## Configuration

Use environment variables:

* `HOME`: Mirror path.
* `TURTLEFS_DELAY`: Delay on read operations in secornds. Default is 1 sec.

## Usage

```
$ sudo mkdir /mnt/turtle
$ gcc -Wall `pkg-config fuse --cflags --libs` turtlefs.c -o turtlefs -lfuse
$ sudo TURTLEFS_DELAY=5 HOME=/home/ko1 ./turtlefs -f /mnt/turtle
```

## Acknowledgement

* The article [FUSE 2でファイルシステムを作る 1｜May｜note](https://note.com/yamblue/n/n511e1c2529a0) helps me at bootstrap.
* FUSE documentation, especially [libfuse/fuse.h at fuse_2_9_bugfix ・ libfuse/libfuse](https://github.com/libfuse/libfuse/blob/fuse_2_9_bugfix/include/fuse.h#L88).
