#define FUSE_USE_VERSION 29

#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h> 
#include <unistd.h>

#define MAXPATH 1024

#define SYS_CALL(expr) do { if ((expr) == -1) { perror(#expr); exit(1); } } while (0)
#define C_CALL(expr)   do { if ((expr) == 0) { perror(#expr); exit(1); } } while (0)


static char turtle_mirror_path[MAXPATH];
static int turtle_delay;

static void
turtle_realpath(char *buff, const char *given_path)
{
    strcpy(buff, turtle_mirror_path);
    strcat(buff, given_path);
}

static int
turtle_getattr(const char *given_path, struct stat *stbuf)
{
    char path[MAXPATH];
    turtle_realpath(path, given_path);

    printf("%s() path=%s realpath=%s\n", __func__, given_path, path);

    SYS_CALL(stat(path, stbuf));

    return 0; // return -ENOENT;
}

static int
turtle_readdir(const char *given_path, void *buff, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    char path[MAXPATH];
    turtle_realpath(path, given_path);

    printf("%s() path=%s realpath=%s offset=%ld\n", __func__, path, path, offset);

    filler(buff, ".", NULL, 0);
    filler(buff, "..", NULL, 0);

    DIR *d;
    C_CALL(d = opendir(path));

    struct dirent *de;

    while ((de = readdir(d)) != NULL) {
        filler(buff, de->d_name, NULL, 0);
    }

    if (errno) {
        perror("readdir"); exit(1);
    }

    return 0;
}

static int
turtle_read(const char *given_path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    char path[MAXPATH];
    turtle_realpath(path, given_path);

    printf("%s() path=%s realpath=%s size=%ld offset=%ld\n", __func__, given_path, path, size, offset);

    sleep(turtle_delay);

    int fd;
    ssize_t r;
    SYS_CALL(fd = open(path, O_RDONLY));
    SYS_CALL(r = read(fd, buf, size));
    return r;
}

static struct fuse_operations fuse_my_operations = {
    .readdir = turtle_readdir,
    .getattr = turtle_getattr,
    .read = turtle_read,
};

int
main(int argc, char *argv[])
{
    // read configuration
    strcpy(turtle_mirror_path, getenv("HOME"));

    if (getenv("TURTLEFS_DELAY")) {
        turtle_delay = atoi(getenv("TURTLEFS_DELAY"));
    }
    else {
        turtle_delay = 1;
    }

    printf("turtle_mirror_path=%s\n", turtle_mirror_path);
    printf("turtle_dalay=%d\n", turtle_delay);

    return fuse_main(argc, argv, &fuse_my_operations, NULL);
}
