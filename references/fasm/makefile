all: quicktest.elf hello.elf printimmediate.elf
quicktest.elf: quicktest.asm
	fasm quicktest.asm quicktest.elf
	chmod +x quicktest.elf
hello.elf: hello.asm
	fasm hello.asm hello.elf
	chmod +x hello.elf
printimmediate.elf: printimmediate.asm
	fasm printimmediate.asm printimmediate.elf
	chmod +x printimmediate.elf

clean:
	rm *.elf
