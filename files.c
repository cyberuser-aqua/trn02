#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define PRINTE(err) write(2, err, strlen(err));

static void write_text(char *filename, long offset, long nbytes, char *text)
{
    ssize_t written = 0;

    int fd = open(filename, O_WRONLY | O_CREAT);
    if (fd == -1)
    {
        PRINTE("ERR: Could not open file for writing\n");
        goto CLEANUP;
    }

    if (lseek(fd, (off_t)offset, SEEK_SET) < 0)
    {
        PRINTE("Could not seek\n");
        goto CLEANUP;
    }

    // written = write(text, 1, MIN(nbytes, strlen(text)), fd);
    written = write(fd, text, MIN(nbytes, strlen(text)));
    if (written != nbytes)
    {
        char buff[50]; // Assuming I can't use fprintf for this...
        snprintf(buff, sizeof(buff), "WARN: Only %d bytes were written of %ld\n", written, nbytes);
        PRINTE(buff);
    }

CLEANUP:
    if (fd != -1)
    {
        close(fd);
        fd = -1;
    }
}

static void read_text(char *filename, long offset, long nbytes)
{
    ssize_t read_ = 0;
    ssize_t written = 0;

    char *buff = (char *)malloc(nbytes * sizeof(char));
    if (buff == NULL)
    {
        PRINTE("ERR: Could not allocate buffer");
        goto CLEANUP;
    }

    int fd = open(filename, O_RDONLY | O_CREAT);
    if (fd == -1)
    {
        PRINTE("ERR: Could not open file for reading\n");
        goto CLEANUP;
    }

    if (lseek(fd, (off_t)offset, SEEK_SET) < 0)
    {
        PRINTE("Could not seek\n");
        goto CLEANUP;
    }

    read_ = read(fd, buff, nbytes);
    if (read_ != nbytes)
    {
        char buff[50];
        snprintf(buff, sizeof(buff), "WARN: Only %d bytes were read of %ld\n", read_, nbytes);
        PRINTE(buff);
    }

    // written = write(buff, 1, read_, stdout);
    written = write(1, buff, read_);
    if (read_ != written)
    {
        char buff[73];
        snprintf(buff, sizeof(buff), "WARN: Only %d bytes were written of %d that were read\n", written, read_);
        PRINTE(buff);
    }

CLEANUP:
    if (buff != NULL)
    {
        free(buff);
        buff = NULL;
    }
    if (fd != -1)
    {
        close(fd);
        fd = -1;
    }
}

static int invalid_commandline(void)
{
    PRINTE("ERR: Invalid command line\n");
    exit(1);
}

int main(int argc, char **argv, char **envp)
{
    if (argc < 5)
        invalid_commandline();
    char *filename = argv[1];
    char *rw = argv[2];
    long offset = strtol(argv[3], NULL, 10);
    if (offset < 0)
        invalid_commandline();
    long nbytes = strtol(argv[4], NULL, 10);
    if (nbytes < 0)
        invalid_commandline();

    if (strcmp(rw, "r") == 0)
        read_text(filename, offset, nbytes);
    else if (strcmp(rw, "w") == 0 && argc == 6)
        write_text(filename, offset, nbytes, argv[5]);
    else
        invalid_commandline();

    return 0;
}
