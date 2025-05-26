#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "ir.h"
#include "optimizer.h"
#include "codegen_llvm.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <source_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *source = fopen(argv[1], "r");
    if (!source) {
        perror("Failed to open source file");
        return EXIT_FAILURE;
    }

    parser_init(source);

    ASTList *program = parse_program();
    if (!program) {
        fprintf(stderr, "Parsing failed\n");
        fclose(source);
        return EXIT_FAILURE;
    }

    IRList ir;
    ir_list_init(&ir);
    ir_generate_program(&ir, program);

    // Optional optimizer
    ir_optimize(&ir);

    // Print IR (for debugging)
    ir_print(&ir);

    // Emit LLVM IR to file
    FILE *ll = fopen("output.ll", "w");
    if (!ll) {
        perror("Failed to open output.ll for writing");
        return EXIT_FAILURE;
    }
    codegen_llvm_emit(&ir, ll);
    fclose(ll);
    printf("LLVM IR emitted to output.ll\n");

    // Cleanup
    ir_free(&ir);
    ast_list_free(program);
    fclose(source);
    return EXIT_SUCCESS;
}
