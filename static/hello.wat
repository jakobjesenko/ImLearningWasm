(module
    (import "wasi" "puts" (func $puts (param i32 i32)))
    (import "env" "mem" (memory 1))

    (data (i32.const 0) "Hello, World!")

    (func $hello
        i32.const 0
        i32.const 13
        call $puts
    )
    (func (export "main")
        call $hello
    )
)