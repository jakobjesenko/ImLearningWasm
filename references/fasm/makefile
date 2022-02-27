all: quicktest.elf hello.elf
quicktest.elf: quicktest.asm
	fasm quicktest.asm quicktest.elf
	chmod +x quicktest.elf
hello.elf: hello.asm
	fasm hello.asm hello.elf
	chmod +x hello.elf
clean:
	rm *.elf
