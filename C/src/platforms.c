#include "inc/main.h"

void append_paths(FILEPATH *ret, const char *dir, const char *file) {
    #ifdef _WIN32
        *ret = calloc(1,MAX_PATH);
        strcat(*ret, dir);
        strcat(*ret, "\\");
        strcat(*ret, file);
    #else
        *ret = calloc(1, 4096);
        strcat(*ret, dir);
        strcat(*ret, "/");
        strcat(*ret, file);
    #endif
}

void create_directory(const char *path) {
    #ifdef _WIN32
        CreateDirectoryA(path, NULL);
    #else
        mkdir(path, 0700);
    #endif
}

bool does_directory_exist(const char *path) {
    #ifdef _WIN32
        DWORD attrib;
        attrib = GetFileAttributes(path);
        return (attrib != INVALID_FILE_ATTRIBUTES) &&
               (attrib & FILE_ATTRIBUTE_DIRECTORY);
    #else
        struct stat sb;
        return !stat(path, &sb) && S_ISDIR(sb.st_mode);
    #endif
}

const char *get_home_config_directory(bool should_free) {
    static char *path = NULL;
    if(should_free) {
        free(path);
        path = NULL;
    } else if(!path) {
    #ifdef _WIN32
        path = calloc(1,MAX_PATH);
        SHGetFolderPathA(0, CSIDL_LOCAL_APPDATA, NULL, path);
        strcat(path, "\\.lilnotes");
    #else
        path = calloc(1,4096);
        strcat(path, getenv("HOME"));
        strcat(path, "/.lilnotes");
    #endif
    }
    return path;
}
