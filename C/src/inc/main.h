#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#endif

#define HOME_DIRECTORY get_home_config_directory(false)
#define FREE_HOME_DIRECTORY() get_home_config_directory(true)
#define FREE_FILEPATH(filepath) free(filepath)
#define NCMP(a, b) (!strcmp((a), (b)))
#define IFNCMP(a, b) if(NCMP((a), (b)))
#define ELNCMP(a, b) else IFNCMP((a), (b))
#define NIFNCMP(a, b) if(!NCMP((a), (b)))
#define NELNCMP(a, b) else NIFNCMP((a), (b))
#define NMCMP(a, b, len) (!memcmp((a), (b), (len)))
#define IFNMCMP(a, b, len) if(NMCMP((a), (b), (len)))
#define ELNMCMP(a, b, len) else IFNMCMP((a), (b), (len))
#define NIFNMCMP(a, b, len) if(!NMCMP((a), (b), (len)))
#define NELNMCMP(a, b, len) else NIFNMCMP((a), (b), (len))
typedef char *FILEPATH;
typedef char *MSTRING;

void append_paths(FILEPATH *ret, const char *dir, const char *file);
void create_directory(const char *path);
bool does_directory_exist(const char *path);
const char *get_home_config_directory(bool should_free);

typedef struct {
    char *name;
    char *value;
} value_data;
typedef struct {
    char *path;
    char **notes;
    value_data *values;
    unsigned int notes_length, values_length;
} settings_data;

void string_replace(char *dest, const char *src, const char *to, const char *with);

void settings_free(settings_data *state);
settings_data parse_settings(const char *path, const char *data);
void save_settings(const settings_data *state);

void help();
MSTRING get_input();
void interface(settings_data *state);

