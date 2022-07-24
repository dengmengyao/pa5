#include "common.h"
#include "memory.h"
#include "fs.h"
#define DEFAULT_ENTRY ((void *)0x8048000)

extern void ramdisk_read(void *buf, off_t offset, size_t len);
extern size_t get_ramdisk_size();
extern ssize_t fs_read(int fd, void *buf, size_t len);
extern size_t fs_filesz(int fd);
extern int fs_open(const char *pathname, int flags, int mode);
extern int fs_close(int fd);

uintptr_t loader( _Protect *as, const char *filename) {
    //ramdisk_read(DEFAULT_ENTRY, 0, get_ramdisk_size());//PA3.1loader
	int fd = fs_open(filename, 0, 0);
	size_t nbyte = fs_filesz(fd);
    void *pa;
    void *va;
	//printf("fd = %d\n", fd);
    //fs_read(fd, DEFAULT_ENTRY, fs_filesz(fd)); 
    void *end = DEFAULT_ENTRY + nbyte;
    for (va = DEFAULT_ENTRY; va < end; va += PGSIZE) {
       pa = new_page();
       _map(as, va, pa);
       fs_read(fd, pa, (end - va) < PGSIZE ? (end - va) : PGSIZE);
    }
	fs_close(fd); 
	return (uintptr_t)DEFAULT_ENTRY;
}
