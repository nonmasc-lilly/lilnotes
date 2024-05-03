#include <stdio.h>
#include "inc/main.h"

int main(int argc, char **argv) {
    FILE *fp;
    char *settings_string;
    unsigned int file_length;
    FILEPATH settings_path;
    settings_data state;
    printf("Lilnotes - a simple notes app\n");
    printf("loading state file from %s\n", HOME_DIRECTORY);
    if(!does_directory_exist(HOME_DIRECTORY)) {
        printf("%s did not exist, creating it...\n", HOME_DIRECTORY);
        create_directory(HOME_DIRECTORY);
        printf("done.\n");
    }
    append_paths(&settings_path, HOME_DIRECTORY, "state.lil");
    if(!(fp = fopen(settings_path, "r"))) {
        printf("%s did not exist, creating it...\n", settings_path);
        fp = fopen(settings_path, "w");
        if(!fp) {
            printf("error: %s couldnt be accessed\n", settings_path);
        }
        fwrite("LILSTATE\n", 1, strlen("LILSTATE\n"), fp);
        fclose(fp);
        fp = fopen(settings_path, "r");
        printf("done.\n");
    }
    file_length = (fseek(fp, 0, SEEK_END), ftell(fp));
    fseek(fp, 0, SEEK_SET);
    settings_string = malloc(file_length);
    fread(settings_string, 1, file_length, fp);
    printf("State loaded, parsing state...\n");
    state = parse_settings(settings_path, settings_string);
    printf("ready...\n");

    interface(&state);

    free(settings_string);
    FREE_FILEPATH(settings_path);
    FREE_HOME_DIRECTORY();
    return 0;
}
