
/*
 * Copyright (C) hanchong liu
 */


#include "sae_file.h"

/*get file open way*/
static sae_int_t sae_file_get_sys_open_way(sae_int_t op)
{
    sae_int_t sys_way = 0;
    
#if (HAVE_WIN32)
    if (op & SAE_FILE_AUTO_CRE)
    {
        sys_way |= OPEN_ALWAYS;
    }
    else
    {
        sys_way |= OPEN_EXISTING;
    }
#else
    if (op & SAE_FILE_AUTO_CRE)
    {
        sys_way |=  O_CREAT;
    }
#endif
    
    return sys_way;
}

/*get file op*/
static sae_int_t sae_file_get_sys_op(sae_int_t op)
{
    sae_int_t sys_op = 0;
    
#if (HAVE_WIN32)
    if (op & SAE_FILE_READ)
    {
        sys_op |= GENERIC_READ;
    }
    if (op & SAE_FILE_WRITE)
    {
        sys_op |=  GENERIC_WRITE;
    }
#else
    if ((op & SAE_FILE_READ) &&
        (op & SAE_FILE_WRITE))
    {
        sys_op |= O_RDWR;
    }
    else if (op & SAE_FILE_WRITE)
    {
        sys_op |=  O_WRONLY;
    }
    else if (op & SAE_FILE_READ)
    {
        sys_op |= O_RDONLY;
    }
#endif
    
    return sys_op;
}

sae_file_t *sae_file_open(sae_cchar_t *path, sae_file_op_t op)
{
    sae_size_t len = 0;
    sae_int_t sys_op = 0;
    sae_int_t sys_way = 0;
#if (HAVE_WIN32)
    WCHAR *wpath = sae_null;
#endif
    
    /*alloc file*/
    sae_file_t *file = sae_alloc(sae_sizeof(sae_file_t));
    if (!file)
    {
        return file;
    }
    
    /*cp path*/
    len = sae_strlen(path);
    file->path = sae_alloc(len + 1);
    if (!file->path)
    {
        sae_alloc_free(file);
        return sae_null;
    }
    sae_cp_str(file->path, path, len);
    
    /*open file*/
#if (HAVE_WIN32)
    sys_op = sae_file_get_sys_op(op);
    sys_way = sae_file_get_sys_open_way(op);
    wpath = sae_char_to_utf8(path);
    file->fd = CreateFile(wpath, sys_op, 0, sae_null, sys_way, FILE_ATTRIBUTE_NORMAL, sae_null);
    sae_alloc_free(wpath);
    if (file->fd == INVALID_HANDLE_VALUE)
    {
        sae_alloc_free(file->path);
        sae_alloc_free(file);
        return sae_null;
    }
#else
    sys_op = sae_file_get_sys_op(op);
    sys_way = sae_file_get_sys_open_way(op);
    sys_op |= sys_way;
    mode_t mode = S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH;
    file->fd = open(path, sys_op, mode);
    if (file->fd == -1)
    {
        sae_alloc_free(file->path);
        sae_alloc_free(file);
        return sae_null;
    }
#endif
    
    file->valid = sae_true;
    
    return file;
}

sae_void_t sae_file_close(sae_file_t *file)
{
    file->valid = sae_false;
    
    sae_alloc_free(file->path);
    
    sae_file_fd_close(file->fd);
    
    sae_alloc_free(file);
}

sae_bool_t sae_file_fd_close(sae_file_fd_t fd)
{
#if (HAVE_WIN32)
    return CloseHandle(fd) ? sae_true : sae_false;
#else
    return (close(fd) == 0) ? sae_true : sae_false;
#endif
}

sae_ssize_t sae_file_read(sae_file_fd_t fd, sae_char_t *text, sae_size_t len)
{
#if (HAVE_WIN32)
    DWORD read_len = 0;
    ReadFile(fd, text, len, &read_len, sae_null);
#else
    sae_ssize_t read_len = 0;
    read_len = read(fd, text, len);
#endif

    return read_len;
}

sae_ssize_t sae_file_read_line(sae_file_fd_t fd, sae_char_t *text, sae_size_t len)
{
    sae_char_t *find = sae_null;
    sae_ssize_t read_len = 0;
    sae_ssize_t line_len = 0;
    
    /*clear mem*/
    sae_memzero(text, len);
    
    /*read len- 1 last \0*/
    read_len = sae_file_read(fd, text, len - 1);
    if (read_len < 1)
    {
        return read_len;
    }
    
    find = sae_strchr(text, '\n');
    if (find)
    {
        line_len = (find - text + 1);
        /*move point*/
#if (HAVE_WIN32)
        SetFilePointer(fd, (line_len - read_len), sae_null, FILE_CURRENT);
#else
        lseek(fd, (line_len - read_len), SEEK_CUR);
#endif
        
        /*\r\n judge*/
        if ((line_len > 1) &&
            (*(find - 1) == '\r'))
        {
            find = find - 1;
        }
        
        *find = sae_str_end;
    }
    
    return line_len;
}

sae_ssize_t sae_file_write(sae_file_fd_t fd, sae_cchar_t *text, sae_size_t len)
{
#if (HAVE_WIN32)
    DWORD write_len = 0;
    WriteFile(fd, text, len, &write_len, sae_null);
#else
    sae_ssize_t write_len = 0;
    write_len = write(fd, text, len);
#endif

    return write_len;
}

sae_bool_t sae_file_exist(sae_cchar_t *path)
{
    sae_int_t ret = -1;
    
#if (HAVE_WIN32)
    WIN32_FIND_DATA wfd;
    WCHAR *wpath = sae_char_to_utf8(path);
    HANDLE find = FindFirstFile(wpath, &wfd);
    sae_alloc_free(wpath);
    ret = (find != INVALID_HANDLE_VALUE) ? 0 : 1;
    if (find != INVALID_HANDLE_VALUE)
    {
        FindClose(find);
    }
#else
    ret = access(path, F_OK);
#endif
    
    return (ret == 0) ? sae_true : sae_false;
}
sae_bool_t sae_file_rm(sae_cchar_t *path)
{
    sae_int_t ret = -1;
    
#if (HAVE_WIN32)
    WCHAR *wpath = sae_char_to_utf8(path);
    ret = (DeleteFile(wpath) != 0) ? 0 : 1;
    sae_alloc_free(wpath);
#else
    ret = unlink(path);
#endif

    return (ret == 0) ? sae_true : sae_false;
}

sae_long_t sae_file_len(sae_file_fd_t fd)
{
    sae_long_t len = -1;
    
#if (HAVE_WIN32)
    len = GetFileSize(fd, sae_null);
#else
    struct stat stats;
    if (fstat(fd, &stats) == 0)
    {
        len = stats.st_size;
    }
#endif
    
    return len / sae_mb;
}

sae_void_t sae_file_sync(sae_file_fd_t fd)
{
#if (HAVE_WIN32)
    FlushFileBuffers(fd);
#else
    fsync(fd);
#endif
}

sae_void_t sae_file_seek_begin(sae_file_fd_t fd)
{
#if (HAVE_WIN32)
    SetFilePointer(fd, 0, sae_null, FILE_BEGIN);
#else
    lseek(fd, 0 ,SEEK_SET);
#endif
}

sae_bool_t sae_dir_exist(sae_cchar_t *path)
{
    sae_int_t ret = -1;
    
#if (HAVE_WIN32)
    WIN32_FIND_DATA wfd;
    WCHAR *wpath = sae_char_to_utf8(path);
    HANDLE find = FindFirstFile(wpath, &wfd);
    sae_alloc_free(wpath);
    ret = (find != INVALID_HANDLE_VALUE) ? 0 : 1;
    if (find != INVALID_HANDLE_VALUE)
    {
        FindClose(find);
    }
#else
    DIR *dir = opendir(path);
    ret = (dir != sae_null) ? 0 : 1;
    if (dir)
    {
        closedir(dir);
    }
#endif
    
    return (ret == 0) ? sae_true : sae_false;
}

sae_bool_t sae_dir_create(sae_cchar_t *path)
{
    sae_int_t ret = -1;
    
#if (HAVE_WIN32)
    WCHAR *wpath = sae_char_to_utf8(path);
    ret = (CreateDirectory(wpath, sae_null) == 0) ? 1 : 0;
    sae_alloc_free(wpath);
#else
    ret = (mkdir(path, S_IRUSR | S_IWUSR | S_IXUSR |
                 S_IROTH | S_IWOTH | S_IXOTH) == 0) ? 0 : 1;
#endif
    
    return (ret == 0) ? sae_true : sae_false;
}

