# GET_NEXT_LINE

## Overview

**get_next_line** is a low-level C utility that reads and returns a single line from a file descriptor on each call.
This implementation is designed with a strong emphasis on **predictable performance, explicit memory ownership, and correctness under extreme constraints**, including very small `BUFFER_SIZE` values.

Instead of relying on repeated string concatenation or reallocation, the project uses a **chunk-based buffering strategy** to ensure linear copy behavior and stable performance, even on large files without newline characters.

This project complies fully with the 42 School specification, including the **bonus requirement for multiple file descriptors**.

---

## Project Structure

```bash
.
├── get_next_line.c					# Core logic and control flow
├── get_next_line_utils.c			# Helper functions and memory utilities
├── get_next_line.h					# Public interface and type definitions
├── get_next_line_bonus.c			# Core logic and control flow for multiple FDs
├── get_next_line_utils_bonus.c		# Helper functions and memory utilities
├── get_next_line_bonus.h			# Public interface and type definitions
```

The implementation is intentionally compact and modular:

* Public API is limited to `get_next_line`
* Internal helpers are `static` where appropriate
* Clear separation between control flow, memory management, and data processing

---

## Implemented Features

### Line-Oriented Reading

* Reads from a file descriptor until a full line is available
* Returns lines **including the terminating `\n`**, except at EOF without newline
* Handles arbitrary line lengths

### Chunk-Based Buffering

* Input is stored as a linked list of read chunks
* Each chunk owns exactly one allocation from `read()`
* No repeated copying of previously read data

### Cache Persistence

* Uses a `static` cache to preserve unread data across function calls
* Bonus version supports **up to 1024 file descriptors simultaneously**

### Robust Memory Handling

* Every allocation has a single, well-defined owner
* All partial reads and error paths clean up correctly
* Fully Valgrind-clean under stress tests

---

## Design Decisions

### Why Not `strjoin` or Reallocation?

Repeated reallocation and copying leads to **quadratic copy behavior** when `BUFFER_SIZE` is small (e.g. `BUFFER_SIZE = 1`).
In such cases, early bytes are copied many times, significantly degrading performance.

This implementation avoids that entirely.

### Chunked Linked List Strategy

* Each `read()` call produces one chunk
* Chunks are linked without copying existing data
* Data is copied **exactly once** when extracting the final line

This guarantees:

* Stable performance regardless of `BUFFER_SIZE`
* Predictable memory behavior
* No reliance on `strlen` or full-buffer rescans

### Explicit Ownership Model

* Read buffers are owned by list nodes
* Extracted lines own their own memory
* Cache updates transfer ownership cleanly

This model avoids hidden side effects and makes reasoning about memory straightforward.

---

## Performance Rationale

### Copy Cost Matters More Than Big-O Labels

In line-based input systems like `get_next_line`, performance is dominated not by asymptotic notation alone, but by **how many times each byte is copied**.

A common approach—reallocating and concatenating buffers on every `read()`—can appear linear at a glance, but in practice leads to **quadratic copy behavior** when `BUFFER_SIZE` is small.

For example, with `BUFFER_SIZE = 1` and a line of length *n*:

* The first byte is copied *n* times
* The second byte is copied *n-1* times
* …
* Total copies ≈ *n(n+1)/2*

This behavior is independent of `ft_strlen` usage or allocation optimizations; it is an inherent property of repeated reallocation and full-buffer copying.

### Chunk-Based Buffering Strategy

This implementation avoids repeated copying entirely by storing each `read()` result as an independent chunk in a linked list:

* Each byte is written **once** when read
* Each byte is copied **once** when extracting the final line
* No previously read data is ever recopied during accumulation

As a result, total copy cost is strictly proportional to the line length, regardless of `BUFFER_SIZE`.

### BUFFER_SIZE Independence

Because accumulation does not rely on growing contiguous buffers, performance remains stable even under adversarial conditions such as:

* `BUFFER_SIZE = 1`
* Very large files without newline characters
* Lines that span many read calls

This makes the implementation predictable and resilient across all valid configurations.

### Practical Outcome

* Linear copy behavior in all cases
* No pathological slowdowns under small buffer sizes
* Memory usage grows only with unread data
* Suitable for streaming and large-input scenarios

This design prioritizes **worst-case correctness and stability**, rather than relying on average-case assumptions.

---

## Usage

### Compilation

Compile with your preferred flags and define `BUFFER_SIZE`:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c
```

### Example

```c
int fd = open("file.txt", O_RDONLY);
char *line;

while ((line = get_next_line(fd)))
{
    printf("%s", line);
    free(line);
}
close(fd);
```

### Bonus (Multiple File Descriptors)

The bonus version supports parallel reads:

```c
static t_gnode *cache[1024];
```

Each file descriptor maintains an independent read state.

---

## Key Technical Skills

* Low-level I/O with `read`
* Manual memory management in C
* Static state management across function calls
* Linked-list–based buffering strategies
* Performance analysis under constrained environments

---
