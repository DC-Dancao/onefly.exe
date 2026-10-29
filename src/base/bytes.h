 
 

#ifndef BASE_BYTES_H
#define BASE_BYTES_H

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

void swap_endian(void *data, unsigned elem_size, unsigned num);

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

unsigned bytes_be_to_uint(const unsigned char *bytes);

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

void uint_to_bytes_be(unsigned char *bytes, unsigned value);

#endif
