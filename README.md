# ConsoleUtils
Simple library to enable a consistent CLI interface

## CLI parameter parsing:
The interface will be designed from a pattern with descriptions for all parameters provided to enable *--help* output

### Pattern definition:
- name Required parameter 
- [name] One optional parameter 
- name ... At least one parameter 
- [name ...] More than one optional parameters 
- [--name] named option 
- [--name <arg_type>] named option with value type arg_type 

>Names may not contain spaces! 
>arg_types are: string, [+]real, [+]int, bool 
>If there are multiple optional parameters they will be filled from left to right

Example Pattern: 

    [--name1] name2 [--name3 <+real>] [name4 ...] 
Can be called with:

    testUtil.exe --name1 --name3 4.0 name2_val name4_val1 name4_val2

>*--help/-?* is also always accepted!

### Notes:
- Options are position agnostic, can be switched around 
- Only parameters are order sensitive 
- *arg_type* string: cannot contain spaces unless surrounded in quotes 
- *arg_type* bool: accept values true/false, ignore case! 
- *arg_type* int/real: [+] restricts to positive values 
- *--help* and *-?* always implemented (!) prints pattern and all params in order, with descriptions 

## Interactive CLI:

Enables a simple single-threaded looping cli with custom definable commands.
Define a new command with the ConsoleCommand struct.

### Notes:
- The callback provided in the struct will be called on detection of the first space seperated part of the command pattern and will be passed the complete line that was entered and the preferred in- and output-streams.
- A command will always be called if the first word matches, regardless of the rest of the pattern.
- Registering a command with a different pattern but the same first word, will overwrite the previously registered command. 
- A "help" and an "exit" command are provided by default, they can be overwritten but this will break functionality (TODO fix)

## TODO:
- Implement Compile time parameter pattern parsing
- Make help and exit cli commands un overwriteable
- Add command aliasing to cli
- Add capability to distinguish multiple patterns for a command and call different callbacks depending

