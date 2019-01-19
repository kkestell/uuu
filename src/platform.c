#include <assert.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

static const char* mmapFile(size_t* len, const char* filename)
{
    FILE* f;
    long file_size;
    struct stat sb;
    char* p;
    int fd;

    (*len) = 0;

    f = fopen(filename, "r");

    if (f == NULL)
    {
        fprintf(stderr, "Couldn't fopen %s\n", filename);
        exit(1);
    }

    if (fseek(f, 0, SEEK_END))
    {
        fprintf(stderr, "Couldn't fseek %s\n", filename);
        exit(1);
    }

    file_size = ftell(f);

    if (file_size == -1)
    {
        fprintf(stderr, "Couldn't ftell %s\n", filename);
        exit(1);
    }

    if (fclose(f) == EOF)
    {
        fprintf(stderr, "Couldn't close %s\n", filename);
        exit(1);
    }

    fd = open(filename, O_RDONLY);
    
    if (fd == -1)
    {
        fprintf(stderr, "Couldn't open %s\n", filename);
        exit(1);
    }

    if (fstat(fd, &sb) == -1)
    {
        fprintf(stderr, "Couldn't fstat %s\n", filename);
        exit(1);
    }

    if (!S_ISREG(sb.st_mode))
    {
        fprintf(stderr, "%s isn't a regular file\n", filename);
        exit(1);
    }

    p = (char*)mmap(0, (size_t)file_size, PROT_READ, MAP_SHARED, fd, 0);

    if (p == MAP_FAILED)
    {
        fprintf(stderr, "Couldn't mmap %s\n", filename);
        exit(1);
    }

    if (close(fd) == -1)
    {
        fprintf(stderr, "Couldn't close %s\n", filename);
        exit(1);
    }

    *len = (size_t)file_size;

    return p;
}

const char* getFileData(size_t* len, const char* filename)
{
    size_t data_len = 0;
    const char* data = NULL;

    data = mmapFile(&data_len, filename);

    *len = data_len;
    return data;
}