## In-Memory File Indexer

# What it does
Implements an in-memory search engine for indexing and retrieving files based on keywords utilizing custom C data structures including a multi-way retrieval tree (Trie), doubly linked lists, and priority queues (Max-Heap). The program reads operational commands to manage file metadata, update keyword associations, and execute relevance-based searches.

# The core architecture features:
- System structure: Manages a main doubly linked list for storing file metadata (ID, relevance score, and local keyword list) alongside a Trie for global keyword indexing.
- Search logic: Utilizes a Trie for fast O(L) keyword lookups and a dynamically resizing Max-Heap to filter and extract the most relevant files during Top-K queries.
- Memory safety: Explicit memory management routines recursively traverse and free all dynamically allocated Trie nodes, linked list nodes, heap arrays, and strings to prevent memory leaks during file deletions and upon termination.

# Core Operations & Logic
- ADD: Parses input data (handling string manipulation via strtok), allocates a new file node in the doubly linked list, and inserts each keyword into the Trie, linking terminal nodes back to the file record.
- DEL: Identifies the file, iterates through its keywords to remove references from the Trie (recursively pruning and freeing orphaned nodes), and safely unlinks and frees the file from the main list.
- FIND: Traverses the Trie character by character for a specific keyword. If a terminal node is reached, it retrieves all associated files, sorts them lexicographically by ID, and outputs the results.
- TOPK: Locates the keyword in the Trie, gathers all referenced files, and inserts them into a custom Max-Heap. Extracts and outputs the top k files based on their relevance score, using lexicographical order as a tie-breaker.

# Usage
A Makefile is provided for automated compilation.
To compile the project with -Wall, -Wextra, and -g flags:
```
make build
```
Execute the binary:
```
./search_index
```
Note: The program reads input operations (e.g., ADD, FIND, TOPK) from standard input and writes execution results to standard output. You can easily redirect input from a local file using ./search_index < input.txt.
To remove the compiled binary and output files:
```
make clean
```
Requirements:
- GCC (GNU Compiler Collection)
- GNU Make
