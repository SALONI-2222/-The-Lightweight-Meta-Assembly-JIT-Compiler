@echo off
gcc -o compiler main.c parser.c lexer.c ir.c optimizer.c codegen_llvm.c -Wall
if %errorlevel% neq 0 (
    echo Build failed.
) else (
    echo Build succeeded.
    echo Running compiler on input.txt...
    .\compiler input.txt
)