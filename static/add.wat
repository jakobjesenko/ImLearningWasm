(module
    (import "imports" "log2" (func $log (param i32)))
    
    (func $add (param $a i32) (param $b i32) (result i32)
        local.get $a
        local.get $b
        i32.add
    )
    (export "add" (func $add))

    (func (export "run")
        i32.const 11
        i32.const 22
        call $add
        call $log
    )
)