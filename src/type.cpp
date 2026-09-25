#include "type.h"

TypeTable::TypeTable() {
    table[TypeKind::I32] = std::make_unique<PrimitiveType>(TypeKind::I32);
    table[TypeKind::I64] = std::make_unique<PrimitiveType>(TypeKind::I64);
    table[TypeKind::U32] = std::make_unique<PrimitiveType>(TypeKind::U32);
    table[TypeKind::U64] = std::make_unique<PrimitiveType>(TypeKind::U64);
    table[TypeKind::F32] = std::make_unique<PrimitiveType>(TypeKind::F32);
    table[TypeKind::F64] = std::make_unique<PrimitiveType>(TypeKind::F64);
    table[TypeKind::BOOL] = std::make_unique<PrimitiveType>(TypeKind::BOOL);
    table[TypeKind::VOID] = std::make_unique<PrimitiveType>(TypeKind::VOID);
    table[TypeKind::UNKNOWN] = std::make_unique<PrimitiveType>(TypeKind::UNKNOWN);
}

Type* TypeTable::getType(TypeKind kind) { 
    auto it = table.find(kind);
    if (it != table.end()) return it->second.get();
    return table[TypeKind::UNKNOWN].get(); 
}

ArrayType* TypeTable::getArrayType(Type* elem, size_t size) {
    return new ArrayType(elem, size);
}