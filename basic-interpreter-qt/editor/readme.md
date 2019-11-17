# Documentation for Lab3: An Editor with List
The entrance for the program is `ed.cc` in which calls `ed.run()`.

The Editor class implements the function `run()`.

## Front End
Function `run()` runs continuously to receive input from user. `cmd` stores the command that the user inputted. The program exits when `cmd` is "Q". Otherwise, function `dispatchCmd` is called to sort out the proper action accordingly.
Exceptions in `dispachCmd` is handled.

## Command Dispatch
### Preparations
- A `stringstream` object is created with `cmd` as its parameter.
- `type` is a string to extract the command(list, save, etc.) later.
- `line` is an integer, which may or may not be used, standing for the line number to operate.

### Command Pretreatment in `Editor`
In this part, the program tryies to match the four action according to `cmd`. Once an action is matched, the action is carried out and the function would be returned. "Bad/Unknown command\n" would be threw if none of the actions is matched.

First, the function tries to match the action "list". Two functions are declared and defined to implement the case-insensitive string match.
The two parameter of function `charCompareI` are both converted to upper case and then compared. Function `stringCompareI` utilize `charCompareI` to convert every char in referenced string to upper case and then compare them. Together with the comparison of the length of the two string. The case-insensitive comparison is implemented. After that,`cmdList` is called to continue the job.

Second, the function tries to match the "delete" action. If `cmd` begins with `d` and an integer following, the command is matched. `cmdDelete` is called to continue the job. The line number is passed through the parameter.

Third, the function tries to match the "save" action. If `cmd` begins with "save " (s-a-v-e-space) and a filename following, the command is matched. "No filename specified\n" would be threw if no filename is specified. The filename is appended character by character. After that, `cmdSave` is called to continue the job.

Finally, the function tries to match the "input" action. `ss` reads an integer  and saves it to `line`. If it's not eof and `line` is an actual and meaningful number, the program would build the statement string character by character. The program presumes that the content after the integer and a single space is the statement to input. After that, `cmdInput` is called to continue the job. The first parameter is the line number. The second parameter is the statement string.

As mentioned before, `cmdList`, `cmdDelete`, `cmdSave` and `cmdInput` are called to give the task to `ListBuffer`.

In `cmdList`  , `showLines`  is called  by  buffer.

In `cmdDelete`  , `deleteLine`  is called  by  buffer.

In `cmdSave`  , `writeToFile`  is called  by  buffer.

In `cmdInput`  , `insertLine`  is called  by  buffer.

### Command Processing in `ListBuffer`
Struct `node` is defined in ListBuffer.
Here's its member:
- `line:int` stores the line number of the statement
- `statement:string` stores the statement string
- `next:node*` stores the pointer to next node
- `node()` default constructor: create a default node object
- `node(int line, const string &statement)`, `node (int line, const string &text, node *next)` create an object with parameters as its members.

#### Insert
The `insert` function inserts statements in ascending order. When a statement is to be inserted, its `line` is compared, so that it can be inserted properly. If the line already exists, its statement would be renewed.

#### Delete
The function traverse the linked list from the first node. If `line` matches, the node is deleted.

#### List
The function traverse the linked list from the first node while printing every statement in that node.

#### Save to a file
Traverse the linked list. Put every statement in a `fileOutputStream` object. Write them to the specified file.