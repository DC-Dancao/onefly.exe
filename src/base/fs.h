 
 

#ifndef BASE_FS_H
#define BASE_FS_H

#include "types.h"

#include <cstddef> 
#include <ctime> 

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
\
\

[[nodiscard]] int fs_makedir(const char *path);

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

[[nodiscard]] int fs_makedir_rec_for(const char *path);

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

[[nodiscard]] int fs_removedir(const char *path);

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

void fs_listdir(const char *dir, FS_LISTDIR_CALLBACK cb, int type, void *user);

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

void fs_listdir_fileinfo(const char *dir, FS_LISTDIR_CALLBACK_FILEINFO cb, int type, void *user);

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

[[nodiscard]] int fs_chdir(const char *path);

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

[[nodiscard]] char *fs_getcwd(char *buffer, int buffer_size);

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
\

[[nodiscard]] int fs_storage_path(const char *appname, char *path, int max);

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

[[nodiscard]] int fs_executable_path(char *buffer, int buffer_size);

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

[[nodiscard]] int fs_is_file(const char *path);

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

[[nodiscard]] int fs_is_dir(const char *path);

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

[[nodiscard]] int fs_is_relative_path(const char *path);

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

[[nodiscard]] const char *fs_filename(const char *path);

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

void fs_split_file_extension(const char *filename, char *name, size_t name_size, char *extension = nullptr, size_t extension_size = 0);

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

[[nodiscard]] int fs_parent_dir(char *path);

 \
\
\
\
\
\
\
\
\

void fs_normalize_path(char *path);

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

[[nodiscard]] int fs_remove(const char *filename);

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

[[nodiscard]] int fs_rename(const char *oldname, const char *newname);

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

[[nodiscard]] int fs_file_time(const char *name, time_t *created, time_t *modified);

#endif
