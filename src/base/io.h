 
 

#ifndef BASE_IO_H
#define BASE_IO_H

#include "types.h"

#include <cstdint>

 \
\
\
\


 \
\

enum
{
	 \
\
\
\

	IOFLAG_READ = 1,
	 \
\
\
\

	IOFLAG_WRITE = 2,
	 \
\
\
\

	IOFLAG_APPEND = 4,
};

 \
\
\
\
\
\

enum class EIoSeekOrigin
{
	 \
\
\
\

	START,
	 \
\
\
\

	CURRENT,
	 \
\
\
\

	END,
};

 \
\
\
\
\
\
\
\
\
\
\

IOHANDLE io_open(const char *filename, int flags);

 \
\
\
\
\
\
\
\
\
\

unsigned io_read(IOHANDLE io, void *buffer, unsigned size);

 \
\
\
\
\
\
\
\
\
\
\
\
\
\
\

bool io_read_all(IOHANDLE io, void **result, unsigned *result_len);

 \
\
\
\
\
\
\
\
\
\
\
\
\
\
\

char *io_read_all_str(IOHANDLE io);

 \
\
\
\
\
\
\
\
\

int io_skip(IOHANDLE io, int64_t size);

 \
\
\
\
\
\
\
\
\
\

int io_seek(IOHANDLE io, int64_t offset, EIoSeekOrigin origin);

 \
\
\
\
\
\
\
\

int64_t io_tell(IOHANDLE io);

 \
\
\
\
\
\
\
\

int64_t io_length(IOHANDLE io);

 \
\
\
\
\
\
\
\
\
\

unsigned io_write(IOHANDLE io, const void *buffer, unsigned size);

 \
\
\
\
\
\
\
\

bool io_write_newline(IOHANDLE io);

 \
\
\
\
\
\
\
\

int io_close(IOHANDLE io);

 \
\
\
\
\
\
\
\

int io_flush(IOHANDLE io);

 \
\
\
\
\
\
\
\

int io_sync(IOHANDLE io);

 \
\
\
\
\
\
\
\

int io_error(IOHANDLE io);

 \
\
\
\
\
\
\
\

IOHANDLE io_stdin();

 \
\
\
\
\
\
\
\

IOHANDLE io_stdout();

 \
\
\
\
\
\
\
\

IOHANDLE io_stderr();

 \
\
\
\
\
\

IOHANDLE io_current_exe();

#endif
