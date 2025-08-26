# Design principle
To avoid memory leaks, ensure allocated objects are freed within the same call stack. 
`exit` command can cause memory leaks since it leaves immediately without freeing elements on heap.

# echo
The `echo` implementation is heavily sourced from the coreutils implementation of `echo`. It does **not** support options, however it does support processing escape sequences. 
Uses the `FALLTHROUGH` macro to annotate intentional fallthrough points. Use `-Wimplicit-fallthrough` to compile.

# Function pointers
Using function pointers makes it easy to quickly execute on the corresponding function for a command. Avoids unnecessary interpretation logic to find the function to execute in child process.

# Spawning a child process
As per traditional shell design, each command is offloaded to a separate child process. After the child process completes execution or terminates, the control flow returns back to the shell process. Doing so prevents shell from crashing and allows multiple concurrent processes to be chained with piping (later). 

# Future feature needs
Need a hash map implementation to **quickly** lookup commands or determine if a command is valid. Can provide major performance benefits if the tokenization can short-circuit for an unrecognized command.
