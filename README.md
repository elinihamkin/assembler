# Assembler

An assembler written in the c language that receives a list of names of files with the extension 'as'. These files are the assembly files that the assembler converts to a low level machine code.
The assembler warns of errors in the assembly code and creates output files in case everything was fine.
## General idea
The assember convert the assmbly file to a list of rows, that evrey row is a list of the words in the row. the assembler works by the the two pass idea. After evrey pass of the assember there is a pass of error checker that go thrue the list and print evrey error that was found by the assembler pass, if there are no errors the assembler continue to the next pass or creates the output files.
##
The assembly is written in ANSI c.
