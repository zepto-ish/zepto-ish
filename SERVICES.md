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


`process`
---------

These service calls can be used to manipulate the host process.


### `exit`

```
void vprocess_exit(UB code);
```

Calls `exit(3)` with the given `code`.
