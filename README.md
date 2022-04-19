# ConsoleUtils
Simple library to enable a consistent CLI interface
The interface will be designed from a pattern with descriptions for all parameters provided to enable *--help* output

## Pattern definition:
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

## Notes:
- Options are position agnostic, can be switched around 
- Only parameters are order sensitive 
- *arg_type* string: cannot contain spaces unless surrounded in quotes 
- *arg_type* bool: accept values true/false, ignore case! 
- *arg_type* int/real: [+] restricts to positive values 
- *--help* and *-?* always implemented (!) prints pattern and all params in order, with descriptions 

