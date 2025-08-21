# Design principle
To avoid memory leaks, ensure allocated objects are freed within the same call stack. 
`exit` command can cause memory leaks since it leaves immediately without freeing elements on heap.

# echo
The `echo` implementation is heavily sourced from the coreutils implementation of `echo`. It does **not** support options, however it does support processing escape sequences. 
Uses the `FALLTHROUGH` macro to annotate intentional fallthrough points. Use `-Wimplicit-fallthrough` to compile.

