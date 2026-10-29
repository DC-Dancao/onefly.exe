#ifndef BASE_AIO_H
#define BASE_AIO_H

#include "types.h"

 \
\
\
\
\
\
\
\

ASYNCIO *aio_new(IOHANDLE io);

 \
\
\
\
\
\
\

void aio_lock(ASYNCIO *aio);

 \
\
\
\
\
\
\

void aio_unlock(ASYNCIO *aio);

 \
\
\
\
\
\
\
\

void aio_write(ASYNCIO *aio, const void *buffer, unsigned size);

 \
\
\
\
\
\

void aio_write_newline(ASYNCIO *aio);

 \
\
\
\
\
\
\
\
\

void aio_write_unlocked(ASYNCIO *aio, const void *buffer, unsigned size);

 \
\
\
\
\
\
\

void aio_write_newline_unlocked(ASYNCIO *aio);

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

int aio_error(ASYNCIO *aio);

 \
\
\
\
\
\

void aio_close(ASYNCIO *aio);

 \
\
\
\
\
\

void aio_wait(ASYNCIO *aio);

 \
\
\
\
\
\

void aio_free(ASYNCIO *aio);

#endif
