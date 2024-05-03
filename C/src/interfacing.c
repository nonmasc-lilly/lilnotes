#include "inc/main.h"

void help() {
    printf("'help' or 'h':      open this menu\n");
    printf("'new <path>':       create an empty state file at <path>\n");
    printf("'-<content>':       create a bite if content <content>\n");
    printf("'=<name>,<value>':  create a value of name <name> and value <value>\n");
    printf("'save':             save the current notes\n");
    printf("'load <path>':      load the state of the file at <path> as the"
                               "current state\n");
    printf("'show':             shows all notes with integer order\n");
    printf("'show <min> <max>': shows notes between <min> and <max> with integer"
                               "order\n");
    printf("'value':            show all value's names, values, and integer"
                               "orders\n");
    printf("'value <name>':     show the value of value with name <name>\n");
    printf("'exit':             exits the program\n");
}

void exit_program(const settings_data *state) {
    char *input;
    printf("are you sure [y/N]? ");
    fflush(stdout);
    input = get_input();
    NIFNCMP(input, "Y")
    NIFNCMP(input, "y")
        return;
    printf("save [Y/n]? ");
    fflush(stdout);
    free(input);
    input = get_input();
    if(!NCMP(input, "n") && !NCMP(input, "N")) {
        save_settings(state);
    }
    printf("exiting...\n");
    free(input);
    exit(0);
}

MSTRING get_input() {
    char *ret;
    unsigned int len, slen;
    ret = malloc(1024);
    slen = 0;
    len = 1024;
    while(1) {
        ret[slen] = getc(stdin);
        if(ret[slen] == EOF) continue;
        if(ret[slen] == '\n') {
            ret[slen] = 0;
            break;
        }
        slen++;
        if(slen > len-1) {
            len += 1024;
            ret = realloc(ret, len);
        }
    }
    return ret;
}

void interface(settings_data *initial_state) {
    char *input, *working_input;
    char *buf, *eptr;
    unsigned int blen, i, min, max;
    FILE *fp;
    settings_data *state;
    value_data *working_value;
    state = initial_state;
    while(1) {
        printf("What to do? ['h' for help]> ");
        fflush(stdout);
        input = get_input();
        IFNCMP(input, "h")    help();
        ELNCMP(input, "help") help();
        ELNMCMP(input, "new", strlen("new")) {
            NIFNMCMP(input, "new ", strlen("new ")) {
                printf("error: new requires a <path> argument\n");
                continue;
            }
            if(!(*(input + strlen("new ")))) {
                printf("error: missing <path> value\n");
                continue;
            }
            fp = fopen(input + strlen("new "), "w");
            if(!fp) {
                printf("error: could not open path %s\n", input + strlen("new "));
                continue;
            }
            fwrite("LILSTATE\n", 1, strlen("LILSTATE\n"), fp);
            fclose(fp);
        } ELNMCMP(input, "-", 1) {
            working_input = input+1;
            state->notes = realloc(state->notes, ++state->notes_length *
                sizeof(char*));
            state->notes[state->notes_length-1] = malloc(strlen(working_input)+1);
            strcpy(state->notes[state->notes_length-1], working_input);
        } ELNMCMP(input, "=", 1) {
            working_input = input+1;
            if(string_count(working_input, ",") > 1) {
                printf("error: too many commas in value, there must be 1 to "
                       "delimit value name, and value value\n");
                continue;
            } else if(string_count(working_input, ",") < 1) {
                printf("error: no value assigned to %s\n", working_input);
                continue;
            }
            state->values = realloc(state->values, ++state->values_length *
                sizeof(char *));
            working_value = &state->values[state->values_length - 1];
            buf = malloc(1);
            blen = 0;
            for(i=0; working_input[i] != ','; i++) {
                buf = realloc(buf, ++blen + 1);
                buf[blen-1] = working_input[i];
            }
            buf[blen] = 0;
            i++;
            working_value->name = malloc(string_count(buf, " ")*2 + strlen(buf) + 1);
            strcpy(working_value->name, buf);
            free(buf);
            buf = malloc(1);
            blen = 0;
            for(; working_input[i]; i++) {
                buf = realloc(buf, ++blen + 1);
                buf[blen-1] = working_input[i];
            }
            buf[blen] = 0;
            working_value->value = malloc(string_count(buf, " ")*2 + strlen(buf) + 1);
            strcpy(working_value->value, buf);
            free(buf);
            printf("%s=%s\n", working_value->name, working_value->value);
        }
        ELNCMP(input, "save") save_settings(state);
        ELNMCMP(input, "load", strlen("load")) {
            NIFNMCMP(input, "load ", strlen("load ")) {
                printf("error: load expects a path argument\n");
                continue;
            }
            if(!(*(input + strlen("load ")))) {
                printf("error: missing path data\n");
                continue;
            }
            fp = fopen(input + strlen("load "), "r");
            if(!fp) {
                printf("error: could not open path %s", input + strlen("load "));
                continue;
            }
            blen = (fseek(fp, 0, SEEK_END), ftell(fp));
            fseek(fp, 0, SEEK_SET);
            buf = malloc(blen);
            fread(buf, 1, blen, fp);
            fclose(fp);
            if(state != initial_state) {
                settings_free(state);
                free(state);
            }
            state = malloc(sizeof(settings_data));
            *state = parse_settings(input+strlen("load "), buf);
            free(buf);
        } ELNCMP(input, "show") {
            for(i=0; i<state->notes_length; i++)
                printf("%d: %s\n", i, state->notes[i]);
        } ELNMCMP(input, "show ", strlen("show ")) {
            working_input = input + strlen("show ");
            buf = malloc(1);
            blen = 0;
            for(i=0; working_input[i] != ' ' && working_input[i]; i++) {
                buf = realloc(buf, ++blen + 1);
                buf[blen-1] = working_input[i];
            }
            i++;
            buf[blen] = 0;
            if(!working_input[i]) {
                free(buf);
                printf("error: show requires a min and a max argument\n");
                continue;
            }
            min = strtol(buf, &eptr, 0);
            if(!(*buf) || *eptr) {
                free(buf);
                printf("error: show min argument is not a number\n");
                continue;
            }
            free(buf);
            buf = malloc(1);
            blen = 0;
            for(;working_input[i]; i++) {
                buf = realloc(buf, ++blen + 1);
                buf[blen-1] = working_input[i];
            }
            i++;
            buf[blen] = 0;
            max = strtol(buf, &eptr, 0);
            if(!(*buf) || *eptr) {
                free(buf);
                printf("error: show max argument is not a number\n");
                continue;
            }
            free(buf);
            for(i=min; i<max; i++) {
                if(i > state->notes_length-1) break;
                printf("%d: %s\n", i, state->notes[i]);
            }
        } ELNCMP(input, "value") {
            for(i=0; i<state->values_length; i++) {
                printf("%d: %s = %s\n", i, state->values[i].name,
                    state->values[i].value);
            }
        } ELNMCMP(input, "value", strlen("value")) {
            NIFNMCMP(input, "value ", strlen("value ")) {
                printf("error: missing value name argument\n");
                continue;
            }
            if(!(*(input+strlen("value ")))) {
                printf("error: missing value name argument\n");
                continue;
            }
            working_input = input + strlen("value ");
            for(i = 0; i < state->values_length; i++) {
                IFNCMP(working_input, state->values[i].name) {
                    printf("%d: %s = %s\n", i, state->values[i].name,
                        state->values[i].value);
                    break;
                }
            }
            if(i == state->values_length) {
                printf("error: %s does is not a real value\n", working_input);
                continue;
            }
        }
        ELNCMP(input, "exit") exit_program(state);
        free(input);
    }
}
