import os

class SettingsObject:
    def __init__(this, notes: list[str], values: list[tuple[str,str]], path: str):
        this.notes:  list[str]            = notes;
        this.values: list[tuple[str,str]] = values;
        this.path:   str                  = path;
    def create_value(this, name: str, value: str):
        for i in this.values:
            if i[0] == name:
                i[1] = value;
                return;
        this.values.append((name, value));
    def create_note(this, note: str):
        this.notes.append(note);
    def SaveData(this):
        ret: str = "LILSTATE\n";
        file: object = None;
        for i in this.notes:
            ret += f"-{i.replace(' ','%20')}\n";
        for i in this.values:
            ret += f"={i[0].replace(' ', '%20')},{i[1].replace(' ', '%20')}\n";
        file = open(this.path, "w");
        file.write(ret);
        file.close();
    
def parse_config(path: str) -> SettingsObject:
    file:      object               = open(os.path.expanduser(path), "r");
    processed: list[str]            = file.read().replace(' ', '').split('\n');
    notes:     list[str]            = [];
    tokens:    list[str]            = [];
    values:    list[tuple[str,str]] = [];
    file.close();
    if processed[0] != "LILSTATE":
        print("The given file is not a valid state file (missing mgnum 'LILSTATE')");
        exit(1);
    processed = processed[1:];
    for i in processed:
        if i == '': continue;
        if i[0] == '-':
            notes.append(i[1:].replace('%20', ' '));
        if i[0] == '=':
            tokens = i[1:].replace('%20', ' ').split(',');
            values.append((tokens[0], tokens[1]));
    return SettingsObject(notes, values, path);
1
def print_help():
    print(f"'help' or 'h':      open this menu");
    print(f"'new <path>'        create an empty settings at <path>");
    print(f"'-<content>':       create a note of content <content>");
    print(f"'=<name>,<value>':  create a value of name <name> and value <value>");
    print(f"'save':             save the current notes");
    print(f"'load <path>':      load the settings of the file at <path> "+
                              f"as the current settings");
    print(f"'show':             shows all notes with integer order");
    print(f"'show <min> <max>': shows notes netween min and max with integer order");
    print(f"'value':            shows all values with names, values, and integer order");
    print(f"'value <name>':     shows the value of value with name <name>");
    print(f"'exit':             exits the program");

def interface(orsettings: SettingsObject):
    inp:      str           = "";
    file:     object        = None;
    settings: SettingObject = orsettings;
    while True:
        inp = input("What to do? ['h' for help]> ");
        if inp == 'h' or inp == 'help':
            print_help();
        elif inp.split(' ')[0] == 'new':
            if input("save current settings [Y/n]? ").lower() != 'n':
                settings.SaveData();
            settings = SettingsObject([],[],inp.split(' ')[1]);
        elif inp[0] == '-':
            settings.create_note(inp[1:]);
        elif inp[0] == '=':
            settings.create_value(inp[1:].split(',')[0], inp[1:].split(',')[1]);
        elif inp == 'save':
            settings.SaveData();
        elif inp.split(' ')[0] == 'load':
            settings = parse_config(inp.split(' ')[1]);
        elif inp.split(' ')[0] == 'show':
            if len(inp.split(' ')) == 1:
                for idx, i in enumerate(settings.notes):
                    print(f"{idx}: {i}");
                continue;
            elif len(inp.split(' ')) == 3:
                try:
                    _ = int(inp.split(' ')[1]);
                except ValueError:
                    print(f"show minimum ({inp.split(' ')[1]}) NaN.");
                try:
                    _ = int(inp.split(' ')[2]);
                except ValueError:
                    print(f"show maximum ({inp.split(' ')[2]}) NaN.");
                for idx in range(int(inp.split(' ')[1]), int(inp.split(' ')[2])):
                    print(f"{idx}: {settings.notes[idx]}");
            else:
                print("incorrect number of arguments to show.");
        elif inp.split(' ')[0] == 'value':
            if len(inp.split(' ')) == 1:
                for idx, i in enumerate(settings.values):
                    print(f"{idx}: {i[0]} = {i[1]}");
                continue;
            elif len(inp.split(' ')) == 2:
                for i in settings.values:
                    if i[0] == inp.split(' ')[1]:
                        print(i[1]);
                        break;
            else:
                print("incorrect number of arguments to values.");
        elif inp == "exit":
            if input("are you sure [y/N]? ").lower() != 'y':
                continue
            if input("save [Y/n]? ").lower() != 'n':
                settings.SaveData();
            print("exiting...");
            exit(0);
        else:
            print(f"instruction {inp} does not exist\n");
            



def main():
    home: str = "";
    state_path: str = "";
    file: object = None;
    settings: SettingsObject = None;

    if os.name == 'nt':
        home = os.getenv("LOCALAPPDATA") + "\\.lilnotes"; 
        state_path = home + "\\state.lil";
    elif os.name == 'posix':
        home = os.getenv("HOME") + "/.lilnotes";
        state_path = home + "/state.lil";
    print(f"Lilnotes - a simple terminal notes app. [v1.0.py]");
    print(f"loading state file from {home}");
    if not os.path.exists(home):
        os.makedirs(home);
        print(f"{home} did not exist, creating it.\n");
    if not os.path.exists(state_path):
        print(f"{state_path} did not exist, creating it.\n");
        file = open(state_path, "w");
        file.write("LILSTATE\n");
        file.close();
        settings = SettingsObject([], [], state_path);
    else:
        print(f"state loaded, parsing state");
        settings = parse_config(state_path);
    interface(settings);

if __name__ == "__main__": main();
