# xv6 File Access Logger

This modification adds file access logging functionality to the xv6 operating system. Whenever any process opens a file via the `open()` system call, the kernel automatically logs the process ID and filename.

## Features

- **Automatic Logging**: All file opens are logged automatically - no manual instrumentation needed
- **Circular Buffer**: Maintains up to 100 log entries in a circular buffer
- **Thread-Safe**: Uses spinlocks to ensure safe concurrent access
- **Simple Interface**: Easy-to-use `showlogs` command to view logs

## Implementation Details

### Files Modified/Created

1. **types.h** - Added `filelog` structure definition
2. **defs.h** - Added function declarations for logging system
3. **filelog.c** - Core logging implementation (NEW FILE)
4. **sysfile.c** - Modified `sys_open()` to add logging
5. **syscall.h** - Added `SYS_getlogs` system call number
6. **syscall.c** - Added `sys_getlogs` to system call table
7. **usys.S** - Added `getlogs` system call entry point
8. **user.h** - Added `getlogs` function declaration and `filelog` struct
9. **main.c** - Added `filelog_init()` call during kernel initialization
10. **showlogs.c** - User program to display logs (NEW FILE)
11. **Makefile** - Added `filelog.o` and `_showlogs` to build targets

### Data Structures

```c
struct filelog {
  int pid;                    // Process ID
  char filename[256];         // Filename (with null terminator)
};
```

### System Calls

- **getlogs(struct filelog* logs, int max_entries)**: Copies log entries to user space
  - Returns number of entries copied
  - Returns -1 on error

### Kernel Functions

- **filelog_init()**: Initializes the logging system
- **filelog_add(int pid, char* filename)**: Adds a log entry
- **filelog_get(struct filelog* logs, int max_entries)**: Retrieves log entries

## Usage

### Building

```bash
make clean && make
```

### Running

```bash
make qemu
```

### Testing

In the xv6 shell, run various commands that open files:

```bash
$ cat README
$ ls
$ echo hello > test.txt
$ cat test.txt
$ grep hello test.txt
```

### Viewing Logs

```bash
$ showlogs
```

Example output:
```
File Access Log (5 entries):
PID	Filename
---	--------
2	README
2	.
2	test.txt
2	test.txt
2	test.txt
```

## How It Works

1. **Automatic Logging**: The `sys_open()` function in `sysfile.c` calls `filelog_add()` after successfully opening a file
2. **Circular Buffer**: The kernel maintains a circular buffer of 100 log entries
3. **Thread Safety**: All logging operations are protected by a spinlock
4. **User Access**: The `getlogs()` system call allows user programs to retrieve log entries
5. **Display**: The `showlogs` program uses `getlogs()` to display all logged file accesses

## Design Decisions

- **Simple Structure**: Used a basic struct with PID and filename for simplicity
- **Circular Buffer**: Prevents unbounded memory growth while preserving recent history
- **Thread-Safe**: Uses spinlocks for safe concurrent access
- **No Performance Impact**: Minimal overhead - only logs on successful file opens
- **Clean Interface**: Single system call for retrieving logs

## Limitations

- Maximum 100 log entries (configurable via `MAX_LOG_ENTRIES`)
- Filenames truncated to 255 characters
- No timestamp information (could be added)
- No filtering capabilities (could be added)

## Testing

The implementation has been designed to be simple and robust. Test with various commands that open files:

- File reading: `cat`, `grep`, `wc`
- File writing: `echo >`, `>` redirection
- Directory listing: `ls`
- File creation: `touch`, `mkdir`

All file opens will be automatically logged and visible via `showlogs`.
