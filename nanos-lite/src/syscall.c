#include "common.h"
#include "syscall.h"
#include "fs.h"

extern int mm_brk(uint32_t new_brk);
extern char _end;
extern ssize_t fs_read(int fd, void *buf, size_t len);
extern ssize_t fs_write(int fd, const void *buf, size_t len);
extern int fs_open(const char *pathname, int flags, int mode);
extern off_t fs_lseek(int fd, off_t offset, int whence);
extern int fs_close(int fd);

uintptr_t sys_write(int fd, const void *buf, size_t count) {
	uintptr_t i = 0;
	if (fd == 1 || fd == 2) {
		for(; count > 0; count--) {
			_putc(((char*)buf)[i]);
			i++;
		}
	}
	Log("'Hello World!' from SYS_write");
	return i;
}

_RegSet* do_syscall(_RegSet *r) {
  uintptr_t a[4], result = -1;
  a[0] = SYSCALL_ARG1(r);
	a[1] = SYSCALL_ARG2(r);
	a[2] = SYSCALL_ARG3(r);
	a[3] = SYSCALL_ARG4(r);

  switch (a[0]) {
		case SYS_none:
			result = 1;
			break;
        case SYS_exit:
			_halt(a[1]);
			break;
		case SYS_write:
			//result = sys_write(a[1], (void *)a[2], a[3]);
			result = fs_write(a[1], (void *)a[2], a[3]);
			break;
		case SYS_read:
			result = fs_read(a[1], (void *)a[2], a[3]);
			break;
		case SYS_brk:
			result = mm_brk(a[1]);;
			break;
		case SYS_open:
			result = fs_open((char *)a[1], a[2], a[3]);
			break;
		case SYS_close:
			result = fs_close(a[1]);
			break;
		case SYS_lseek:
			result = fs_lseek(a[1], a[2], a[3]);
			break;
		default: panic("Unhandled syscall ID = %d", a[0]);
  }
	
  SYSCALL_ARG1(r) = result;

  return NULL;
}
