#include "token.h"
#include "token_types.h"
#include <sstream>

std::string Token::toString() const {
    std::ostringstream out;
    out << "[line " << line_ << ", col " << column_ << "] " << tokenTypeToString(type_) << "  "
        << lexeme_ << " -> ";

    if (std::holds_alternative<int>(literal_)) {
        out << std::get<int>(literal_);
    } else if (std::holds_alternative<double>(literal_)) {
        out << std::get<double>(literal_);
    } else if (std::holds_alternative<std::string>(literal_)) {
        out << std::get<std::string>(literal_);
    } else {
        out << "null";
    }

    return out.str();
}