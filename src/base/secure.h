 
 

#ifndef BASE_SECURE_H
#define BASE_SECURE_H

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

void generate_password(char *buffer, unsigned length, const unsigned short *random, unsigned random_length);

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

void secure_random_password(char *buffer, unsigned length, unsigned pw_length);

 \
\
\
\
\
\
\

void secure_random_fill(void *bytes, unsigned length);

 \
\
\
\
\
\
\
\
\

int secure_rand_below(int below);

#endif
