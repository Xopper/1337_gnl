# Memory Leak Analysis for get_next_line

## Overview
This document analyzes the memory management in the linked-list based implementation of get_next_line to ensure there are no memory leaks.

## Data Structures

### t_fd_list
```c
typedef struct s_fd_list {
    int fd;              // File descriptor (no allocation)
    char *rest;          // Dynamically allocated string
    struct s_fd_list *next;  // Pointer to next node
} t_fd_list;
```

## Memory Allocation Points

### 1. ft_find_or_create()
**Allocations:**
- `new_node = malloc(sizeof(t_fd_list))` at line 27
- `new_node->rest = ft_strnew(0)` at line 31

**Leak Prevention:**
- If ft_strnew() fails, new_node is freed (line 34) ✓
- Node is added to linked list, will be freed in ft_remove_node() ✓

### 2. ft_read_buffer()
**Allocations:**
- `buffer = malloc(BUFF_SIZE + 1)` at line 48
- `tmp = ft_strjoin(node->rest, buffer)` at line 61

**Leak Prevention:**
- buffer is ALWAYS freed before return (lines 57, 64, 70) ✓
- tmp NULL check added (line 62) - frees buffer on failure ✓
- Old node->rest freed before assigning tmp (line 67) ✓

### 3. ft_extract_line()
**Allocations:**
- `*line = ft_strsub(node->rest, 0, i)` at line 82
- `new_rest = ft_strdup(node->rest + i + 1)` at line 91

**Leak Prevention:**
- If ft_strsub() fails, returns error immediately (line 84) ✓
- If ft_strdup() fails, frees *line before returning error (line 94) ✓
- Old node->rest freed before assigning new_rest (line 97) ✓
- **Caller responsibility**: Must free *line after use

### 4. ft_remove_node()
**Deallocations:**
- Frees node->rest if not NULL (line 108)
- Frees node itself (line 109)

**Ensures:**
- Complete cleanup when fd reaches EOF ✓
- Removes node from linked list properly ✓

## Critical Paths Analysis

### Path 1: Normal Line Reading
1. Node exists or created → memory tracked in list ✓
2. Buffer allocated → freed before return ✓
3. Strings joined → old freed, new assigned ✓
4. Line extracted → caller must free ✓
5. Rest updated → old freed, new assigned ✓

**Result:** No leaks ✓

### Path 2: End of File
1. ft_extract_line() returns 0
2. ft_remove_node() called (line 132)
3. node->rest freed ✓
4. node freed ✓
5. Node removed from list ✓

**Result:** No leaks ✓

### Path 3: Error Handling
**ft_find_or_create() fails:**
- If malloc fails: returns NULL, nothing allocated ✓
- If ft_strnew fails: node freed ✓

**ft_read_buffer() fails:**
- If malloc fails: returns error immediately ✓
- If read fails: buffer freed ✓
- If ft_strjoin fails: buffer freed, node->rest preserved ✓

**ft_extract_line() fails:**
- If ft_strsub fails: returns error ✓
- If ft_strdup fails: *line freed ✓

**Main function on error:**
- Calls ft_remove_node() if result <= 0 (line 132) ✓

**Result:** All error paths clean up properly ✓

## libft Functions Verification

### ft_strnew()
- Allocates size + 1 bytes
- Returns NULL on failure
- **Safe** ✓

### ft_strjoin()
- Allocates new string
- Returns NULL on failure (if s1 or s2 NULL, or malloc fails)
- Does NOT free input strings
- **Safe** ✓

### ft_strsub()
- Allocates new string
- Returns NULL on failure
- Does NOT free input string
- **Safe** ✓

### ft_strdup()
- Allocates new string
- Returns NULL on failure
- Does NOT free input string
- **Safe** ✓

## Potential Issues Found and Fixed

### ✅ Issue 1: ft_strjoin() NULL check
**Problem:** If ft_strjoin() returned NULL, we would free node->rest and assign NULL without checking.

**Fix:** Added NULL check at line 62-66:
```c
if (!tmp)
{
    free(buffer);
    return (-1);
}
```

### ✅ Issue 2: ft_strdup() NULL check
**Problem:** If ft_strdup() returned NULL, we would lose the allocated *line.

**Fix:** Added NULL check at line 92-96:
```c
if (!new_rest)
{
    free(*line);
    return (-1);
}
```

## Caller Responsibilities

The caller of get_next_line() MUST:
1. Free the returned line after use
2. Continue calling until return value is 0 or -1 for proper cleanup

## Example Usage (No Leaks)

```c
int fd = open("file.txt", O_RDONLY);
char *line;
int ret;

while ((ret = get_next_line(fd, &line)) > 0)
{
    // Use line
    free(line);  // MUST FREE
}
// When ret == 0 or -1, internal memory is cleaned up
close(fd);
```

## Conclusion

✅ **No memory leaks detected**
✅ **All allocations have corresponding frees**
✅ **Error paths properly clean up**
✅ **Linked list nodes properly managed**

The implementation is memory-safe when used correctly.
