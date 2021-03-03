#ifndef BASM_H_
#define BASM_H_

#include "./sv.h"
#include "./arena.h"
#include "./bm.h"
#include "./expr.h"
#include "./statement.h"

#define BASM_BINDINGS_CAPACITY 1024
#define BASM_DEFERRED_OPERANDS_CAPACITY 1024
#define BASM_DEFERRED_ASSERTS_CAPACITY 1024
#define BASM_STRING_LENGTHS_CAPACITY 1024
#define BASM_MAX_INCLUDE_LEVEL 69
#define BASM_INCLUDE_PATHS_CAPACITY 1024

typedef enum {
    BINDING_CONST = 0,
    BINDING_LABEL,
    BINDING_NATIVE,
} Binding_Kind;

const char *binding_kind_as_cstr(Binding_Kind kind);

typedef enum {
    BINDING_UNEVALUATED = 0,
    BINDING_EVALUATING,
    BINDING_EVALUATED,
} Binding_Status;

typedef struct {
    Binding_Kind kind;
    String_View name;
    Word value;
    Expr expr;
    Binding_Status status;
    File_Location location;
} Binding;

typedef struct {
    Inst_Addr addr;
    Expr expr;
    File_Location location;
} Deferred_Operand;

typedef struct {
    Inst_Addr addr;
    uint64_t length;
} String_Length;

typedef struct {
    Expr expr;
    File_Location location;
} Deferred_Assert;

typedef struct {
    Binding bindings[BASM_BINDINGS_CAPACITY];
    size_t bindings_size;

    Deferred_Operand deferred_operands[BASM_DEFERRED_OPERANDS_CAPACITY];
    size_t deferred_operands_size;

    Deferred_Assert deferred_asserts[BASM_DEFERRED_ASSERTS_CAPACITY];
    size_t deferred_asserts_size;

    Inst program[BM_PROGRAM_CAPACITY];
    uint64_t program_size;
    Inst_Addr entry;
    bool has_entry;
    File_Location entry_location;
    String_View deferred_entry_binding_name;

    String_Length string_lengths[BASM_STRING_LENGTHS_CAPACITY];
    size_t string_lengths_size;

    uint8_t memory[BM_MEMORY_CAPACITY];
    size_t memory_size;
    size_t memory_capacity;

    Arena arena;

    size_t include_level;
    File_Location include_location;

    String_View include_paths[BASM_INCLUDE_PATHS_CAPACITY];
    size_t include_paths_size;
} Basm;

Binding *basm_resolve_binding(Basm *basm, String_View name);
void basm_bind_expr(Basm *basm, String_View name, Expr expr, Binding_Kind kind, File_Location location);
void basm_bind_value(Basm *basm, String_View name, Word value, Binding_Kind kind, File_Location location);
void basm_push_deferred_operand(Basm *basm, Inst_Addr addr, Expr expr, File_Location location);
void basm_save_to_file(Basm *basm, const char *output_file_path);
Word basm_push_string_to_memory(Basm *basm, String_View sv);
Word basm_push_byte_array_to_memory(Basm *basm, uint64_t size, uint8_t value);
bool basm_string_length_by_addr(Basm *basm, Inst_Addr addr, Word *length);
Word basm_expr_eval(Basm *basm, Expr expr, File_Location location);
Word basm_binding_eval(Basm *basm, Binding *binding, File_Location location);
void basm_push_include_path(Basm *basm, String_View path);
bool basm_resolve_include_file_path(Basm *basm,
                                    String_View file_path,
                                    String_View *resolved_path);

void basm_translate_block(Basm *basm, Block *block);
void basm_translate_bind_const(Basm *basm, Bind_Const bind_const, File_Location location);
void basm_translate_bind_label(Basm *basm, Bind_Label bind_label, File_Location location);
void basm_translate_bind_native(Basm *basm, Bind_Native bind_native, File_Location location);
void basm_translate_if(Basm *basm, If eef, File_Location location);
void basm_translate_include(Basm *basm, Include include, File_Location location);
void basm_translate_assert(Basm *basm, Assert azzert, File_Location location);
void basm_translate_error(Error error, File_Location location);
void basm_translate_entry(Basm *basm, Entry entry, File_Location location);
void basm_translate_emit_inst(Basm *basm, Emit_Inst emit_inst, File_Location location);
void basm_translate_statement(Basm *basm, Statement statement);
void basm_translate_source_file(Basm *basm, String_View input_file_path);

#endif // BASM_H_
