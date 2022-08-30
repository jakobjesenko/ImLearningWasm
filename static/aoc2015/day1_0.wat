(module
    (import "env" "mem" (memory 1))
    (import "wasi" "mmapfile" (func $open (param i32 i32 i32 i32)))
    (import "wasi" "putn" (func $putn (param i32)))

    (data (i32.const 0) "aoc2015/day1_0input.txt") ;; length 23

    (func (export "main")
        i32.const 0
        i32.const 23
        i32.const 24 ;; return count
        i32.const 28 ;; done flag
        call $open
        (loop $waiting
        i32.const 28
        i32.load
        i32.const 28
        i32.load
        call $putn
        i32.const 0
        i32.ne
        br_if $waiting
        )
        i32.const 24
        i32.load
        call $putn
    )
)