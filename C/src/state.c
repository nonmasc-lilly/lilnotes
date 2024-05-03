#include "inc/main.h"

settings_data parse_settings(const char *path, const char *data) {
    settings_data ret;
    unsigned int i, blen;
    char *buf, *buf2;
    value_data *working_value;
    if(memcmp(data, "LILSTATE\n", strlen("LILSTATE\n"))) {
        memset(&ret, 0, sizeof(settings_data));
        return ret;
    }
    ret.path = malloc(strlen(path)+1);
    strcpy(ret.path, path);
    ret.notes = malloc(1);
    ret.notes_length = 0;
    ret.values = malloc(1);
    ret.values_length = 0;
    for(i=strlen("LILSTATE"); data[i]; i++) {
        switch(data[i]) {
        case '\n':
        case '\r':
        case '\t':
        case ' ':
            continue;
        case '-':
            buf = malloc(1);
            blen = 0;
            i++;
            while(data[i] != '\n' && data[i] != '\t' &&
                    data[i] != '\r' && data[i] != ' ') {
                buf = realloc(buf, ++blen + 1);
                buf[blen-1] = data[i];
                i++;
            }
            buf[blen] = 0;
            buf2 = calloc(1,blen+1);
            string_replace(buf2, buf, "%20", " ");
            free(buf);
            ret.notes = realloc(ret.notes, ++ret.notes_length * sizeof(char*));
            ret.notes[ret.notes_length-1] = calloc(1,strlen(buf2)+1);
            strcpy(ret.notes[ret.notes_length-1], buf2);
            free(buf2);
            break;
        case '=':
            buf = malloc(1);
            blen = 0;
            ret.values = realloc(ret.values, ++ret.values_length *
                sizeof(value_data));
            working_value = &(ret.values[ret.values_length-1]);
            i++;
            while(data[i] != ',') {
                buf = realloc(buf, ++blen + 1);
                buf[blen-1] = data[i];
                i++;
            }
            i++;
            buf[blen] = 0;
            working_value->name = calloc(1,blen+1);
            string_replace(working_value->name, buf, "%20", " ");
            free(buf);
            buf = malloc(1);
            blen = 0;
            while(data[i] != '\n' && data[i] != '\t' &&
                    data[i] != '\r' && data[i] != ' ') {
                buf = realloc(buf, ++blen + 1);
                buf[blen-1] = data[i];
                i++;
            }
            buf[blen] = 0;
            working_value->value = malloc(blen+1);
            string_replace(working_value->value, buf, "%20", " ");
            free(buf);
            break;
        }
    }
    return ret;
}

void save_settings(const settings_data *state) {
    FILE *fp;
    const char *template;
    char *content, *buf, *buf2, *buf3;
    unsigned int i;
    content = malloc(strlen("LILSTATE\n")+1);
    strcpy(content, "LILSTATE\n");
    for(i=0; i<state->notes_length; i++) {
        template = "-%s\n";
        buf2 = malloc(strlen(state->notes[i])+string_count(state->notes[i], " ")*3
                + 1);
        string_replace(buf2, state->notes[i], " ", "%20");
        buf = malloc(strlen(template)+strlen(buf2)+1);
        sprintf(buf, template, buf2);
        content = realloc(content, strlen(content)+strlen(buf)+1);
        strcat(content, buf);
        free(buf);
        free(buf2);
    }
    for(i=0; i<state->values_length; i++) {
        template = "=%s,%s\n";
        buf2 = calloc(1,strlen(state->values[i].name) +
            string_count(state->values[i].name, " ")*2 + 1);
        string_replace(buf2, state->values[i].name, " ", "%20");
        buf3 = calloc(1,strlen(state->values[i].value) +
            string_count(state->values[i].value, " ")*2 + 1);
        string_replace(buf3, state->values[i].value, " ", "%20");
        buf = malloc(strlen(template)+strlen(buf2)+strlen(buf3) + 1);
        sprintf(buf, template, buf2, buf3);
        content = realloc(content, strlen(content)+strlen(buf)+1);
        strcat(content, buf);
        free(buf);
    }
    fp = fopen(state->path, "w");
    fwrite(content, 1, strlen(content), fp);
    fclose(fp);
}

void settings_free(settings_data *state) {
    unsigned int i;
    for(i=0; i < state->notes_length; i++)
        free(state->notes[i]);
    free(state->notes);
    for(i=0; i < state->values_length; i++) {
        free(state->values[i].name);
        free(state->values[i].value);
    }
    free(state->values);
    free(state->path);
    state->notes = NULL;
    state->values = NULL;
    state->path = NULL;
}
