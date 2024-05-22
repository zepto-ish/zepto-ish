Additional service calls
========================

Note, while the API prefixes these service calls with `v`, as the spec requires, the `v` prefix is dropped outside of the signatures for brevity.


`console`
---------

These service calls can be used to output to the console.

The meaning of `stdout` and `stderr` follows the convention, but it should not be assumed it will necessarily map to the actual host process' `stdout` and `stderr` in the future.


### `stdoutf`

```
void vconsole_stdoutf(char* fmt, ...);
```

Prints the given `fmt` string to `stdout`, following `printf` format conventions.


### `stderrf`

```
void vconsole_stderrf(char* fmt, ...);
```

Prints the given `fmt` string to `stderr`, following `printf` format conventions.

### `reghooks`

```
ER vconsole_reghooks(FP before, FP after);
```

Registers functions to be called before and after `vconsole` output.

This is intended to be used to do "housekeeping" before showing things on the terminal.
For example, removing a prompt in `before` and putting back the prompt in `after`.

 - Returns `E_OK` when functions have been registered correctly.
 - Returns `E_OBJ` when called after functions have been registered.


`process`
---------

These service calls can be used to manipulate the host process.


### `exit`

```
void vprocess_exit(UB code);
```

Calls `exit(3)` with the given `code`.
