# Design principle
To avoid memory leaks, ensure allocated objects are freed within the same call stack. 
`exit` command can cause memory leaks since it leaves immediately without freeing elements on heap.
