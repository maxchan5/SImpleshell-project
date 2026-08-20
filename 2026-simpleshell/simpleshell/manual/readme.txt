
User Manual

Built in Commands
cd - changes the current directory. If no argument is given then the current directory is printed 
clr - clears the screen
dir - lists the contents of the directory
environ - lists all the environment strings
echo - prints the arguments after echo followed by a new line
help - displays this user manual
pause - pauses the shell until "Enter" is pressed
quit - exits the shell

External commands not built into the shell are executed using the system

I/O redirection
input redirection - "<" redirects input from a file
eg. command < inputfile, stdin is read from the input file
output redirection(overwrite) - ">" writes output to a file and overwrites if theres existing content
eg. command > outputfile, stdout is written to the outputfile, overwriting any existing content
output redirection(append) - ">>" appends output to a file
eg. command >> outputfile, stdout is appended to the outputfile, being added onto any exisitng content

background execution - "&" executes command in the background. the shell does not wait and the user is still able to enter commands
eg. command &, the shell will reappear instantly and the user will be able to type in more commands while the previous command runs

Environment Variables
shell - stores the full path of the shell executable
parent - set for child processes to indicate the parent shell path
PWD - stores the current directory