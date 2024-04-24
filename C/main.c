#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#define PLATFORM_WINDOWS 1
#define PLATFORM_LINUX   0
#define MAXPATH MAX_PATH
#else
#define PLATFORM_WINDOWS 0
#define PLATFORM_LINUX   1
#define MAXPATH          8192
#endif

typedef unsigned int COUNT;

typedef struct {
    char *name, *value;
} value_data;
value_data value_create(const char *name, const char *value) {
    value_data ret;
    ret.name  = name  ? malloc(strlen(name) + 1) : calloc(1,1);
    ret.value = value ? malloc(strlen(value)+ 1) : calloc(1,1);
    if(name)  strcpy(ret.name,   name);
    if(value) strcpy(ret.value, value);
    return ret;
}
void value_destroy(value_data *value) {
    free(value->name);
    free(value->value);
    value->name  = NULL;
    value->value = NULL;
}

typedef struct {
    char **notes;
    value_data *values;
    const char *path;
    COUNT notes_length, values_length;
} settings_data;
settings_data settings_create(char **notes, COUNT notes_length,
        value_data *values, COUNT values_length, const char *path) {
    settings_data ret;
    COUNT i;
    if(notes_length > 0 && notes) {
        ret.notes = malloc(sizeof(char*)*notes_length);
        for(i=0; i<notes_length; i++) {
            ret.notes[i] = malloc(strlen(notes[i])+1);
            strcpy(ret.notes[i], notes[i]);
        }
    } else {
        ret.notes = malloc(1);
        ret.notes_length = 0;
    }
    if(values_length > 0 && values) {
        ret.values = malloc(sizeof(value_data)*values_length);
        for(i=0; i<values_length; i++) {
            ret.values[i] = value_create(values[i].name, values[i].value);
        }
    } else {
        ret.notes = malloc(1);
        ret.notes_length = 0;
    }
    ret.path = path;
    return ret;
}
void settings_destroy(settings_data *settings) {
    COUNT i;
    for(i=0; i<settings->notes_length; i++)
        free(settings->notes[i]);
    free(settings->notes);
    settings->notes = NULL;
    setting->notes_length = 0;
    for(i=0; i<settings->values_length; i++)
        value_destroy(&settings->values[i]);
    free(settings->values);
    settings->values = NULL;
    settings->values_length = 0;
    path = NULL;
}
void settings_add_note(settings_data *settings, const char *note) {
    ++settings->notes_length;
    settings->notes = realloc(settings->notes, sizeof(char*)*settings->notes_length);
    settings->notes[settings->notes_length-1] = malloc(strlen(note)+1);
    strcpy(settings->notes[settings->notes_length-1], note);
}
void settings_add_value(settings_data *settings, const char *name, const char *value) {
    ++settings->values_length;
    settings->values = realloc(settings->values, sizeof(value_data)*settings->values_length);
    settings->values[settings->values_length-1] = value_create(name, value);
}


static const char *home_path(bool free) {
    static char *home_path = NULL;
    if(free) {
        free(home_path);
        home_path = NULL;
        return NULL;
    }
    if(!home_path) {
        #ifdef _WIN32
            home_path = malloc(MAX_PATH);
            if(!SUCCEEDED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, home_path))) {
                printf("Could not get LOCAL APPDATA (windows machine)\n");
                free(home_path);
                exit(1);
            }
        #else
            if(!getenv("HOME")) {
                printf("Could not get $HOME (linux machine)\n");
                exit(1);
            }
            home_path = malloc(strlen(getenv("HOME"))+1);
            strcpy(home_path, getenv("HOME"));
        #endif
    }
    return home_path;
}

void path(const char *path_a, const char *path_b, char *ret) {
    strcpy(ret, path_a);
    #ifdef _WIN32
        strcat(ret, "\\");
    #else
        strcat(ret, "/");
    #endif
    strcat(ret, path_b);
}



int main(int argc, char **argv) {
    char *settings_path, *file_content;
    FILE *fp;
    settings_path = malloc(MAXPATH);
    path(home_path(false), ".lilnotes", settings_path);



    free(settings_path);
    home_path(true);
}
