# Assembler

An assembler is written in the c language that receives a list of names of files with the extension 'as'. These files are the assembly files that the assembler converts to a low-level machine code.
The assembler warns of errors in the assembly code and creates output files in case everything was fine.
## General idea
The assembler converts the assembly file to a list of rows, that every row is a list of the words in the row. the assembler works by the two-pass idea. After every pass of the assembler, there is a pass of error checker that goes through the list and prints every error that was found by the assembler pass, if there are no errors the assembler continues to the next pass or creates the output files.
##
The assembly is written in ANSI c.
