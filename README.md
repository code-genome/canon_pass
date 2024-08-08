# Canonicalization LLVM Pass

## How to build

To build the canonicalization pass,
```
make llvm_build
make
```

## How to run
Run the LLVM Pass using `opt` by 
```
./llvm_build/llvm-project-llvmorg-8.0.1/llvm/build/bin/opt  \
-load ./build/libcanonicalization-pass.so \
-canonicalization \
-canon-out "youroutputfile.jsonl" \
< "myinputfile.bc" >/dev/null
```

The following optional argument can be used:
- `-log-out "mylogfile.json"`: Create a log file containing metadata.
- `-canondebug`: Print debug information to stdout.

## How to test

To run test cases of the canonicalization LLVM pass using the locally built LLVM 8.0.1,
```
make test
```

To run the canonicalization LLVM pass on the input in `tests/global.ir`, 
```
make test_sample
```

The output in `tests/canonicalized.jsonl` looks something like:
```
{"code":"define i32 @_F(i32 %a1) {\nb1:\n  %v1 = load i32*, i32** @gv2, align 8\n  %v2 = load i32, i32* %v1, align 4\n  %v3 = add nsw i32 %v2, %a1\n  ret i32 %v3\n}","data":"@gv1 = global i32 123, align 4\n@gv2 = global i32* @gv1, align 8","extern":false,"name":"f0"}

{"code":"define i32 @_F(i32 %a1) {\nb1:\n  %v1 = load i32, i32* getelementptr inbounds ([3 x i32], [3 x i32]* @gv1, i64 0, i64 0), align 4\n  %v2 = add nsw i32 %v1, %a1\n  ret i32 %v2\n}","data":"@gv1 = global [3 x i32] [i32 1, i32 2, i32 3], align 4","extern":false,"name":"f1"}
```

## Log file format
Log files are in the following format:
```
{"bin_id":"test","dump_time_secs":273.949707705,"no_of_functions":5088,"prep_time_secs":3.1130904769999996}
```
