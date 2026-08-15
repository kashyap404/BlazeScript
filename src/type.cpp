#include "type.h"

TypeTable::TypeTable() {
    table[TypeKind::I32] = std::make_unique<Type>(TypeKind::I32);
    table[TypeKind::I64] = std::make_unique<Type>(TypeKind::I64);
    table[TypeKind::U32] = std::make_unique<Type>(TypeKind::U32);
    table[TypeKind::U64] = std::make_unique<Type>(TypeKind::U64);
    table[TypeKind::F32] = std::make_unique<Type>(TypeKind::F32);
    table[TypeKind::F64] = std::make_unique<Type>(TypeKind::F64);
    table[TypeKind::BOOL] = std::make_unique<Type>(TypeKind::BOOL);
    table[TypeKind::VOID] = std::make_unique<Type>(TypeKind::VOID);
    table[TypeKind::UNKNOWN] = std::make_unique<Type>(TypeKind::UNKNOWN);
}

Type* TypeTable::getType(TypeKind kind) { return table[kind].get(); }