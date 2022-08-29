(module
    (import "wasi" "putn" (func $putn (param i32)))
    
    (func $add (param $a i32) (param $b i32) (result i32)
        local.get $a
        local.get $b
        i32.add
    )
    (export "add" (func $add))

    (func (export "main")
        i32.const 11
        i32.const 22
        call $add
        call $putn
    )
)