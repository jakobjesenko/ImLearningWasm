(module
    (import "imports" "memory" (memory 1))
    (import "imports" "log" (func $log (param i32 i32)))

    (data (i32.const 0) "Hello, World!")

    (func (export "hello")
        i32.const 0
        i32.const 13
        call $log
    )
)