#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <errno.h>

#define HOOK_LIB_PATH NULL
#define HOOK_INIT(fname) static void *handle, (*f)(); if (!handle) handle = hook_init_ptr(&f, fname);

static void *hook_init_ptr(void (**f)(), const char *fname)
{
  static void *handle;

  handle = dlopen(HOOK_LIB_PATH, RTLD_LAZY);
  if (!handle)
    err(1, "dlopen %s", HOOK_LIB_PATH);
  *f = dlsym(RTLD_NEXT, fname);
  if (!*f)
    err(1, "dlsym %s", fname);
  return handle;
}

int setuid(uid_t uid)
{
  HOOK_INIT("setuid");
  int ret;

  ret = ((int (*)())f)(uid);

  do
    {
      struct rlimit rlim;
      int fd;
      char buf[20];

      if (uid == 0)
	break;

      if (getrlimit(RLIMIT_NPROC, &rlim) != 0)
	break;

      if ((fd = open("/proc/flexnproc", O_RDONLY)) < 0)
	break;

      if (read(fd, buf, sizeof(buf)) <= 0)
	goto out_open;

      int cur_processes = strtoul(buf, NULL, 16);
      printf("cur_processes : %d (RLIMIT_NPROC hard : %ld)\n",
	     cur_processes, rlim.rlim_max);

      if (cur_processes >= rlim.rlim_max)
	{
	  exit(1);
	  return -EAGAIN;
	}

    out_open:
      close(fd);
    }
  while (0);

  return ret;
}
