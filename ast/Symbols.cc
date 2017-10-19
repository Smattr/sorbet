#include "Symbols.h"
#include "Context.h"
#include <sstream>

namespace ruby_typer {
namespace ast {

bool SymbolRef::operator==(const SymbolRef &rhs) const {
    return _id == rhs._id;
}

bool SymbolRef::operator!=(const SymbolRef &rhs) const {
    return !(rhs == *this);
}

bool SymbolRef::isPrimitive() const {
    Error::notImplemented();
}
bool SymbolInfo::isConstructor(ContextBase &ctx) const {
    return this->name._id == 1;
}
SymbolInfo &SymbolRef::info(ContextBase &ctx, bool allowNone) const {
    Error::check(_id < ctx.symbols.size());
    if (!allowNone)
        Error::check(this->exists());

    return ctx.symbols[this->_id];
}

bool SymbolRef::isSynthetic() const {
    return this->_id <= ContextBase::defn_last_synthetic_sym()._id;
}

void printTabs(std::ostringstream &to, int count) {
    int i = 0;
    while (i < count) {
        to << "  ";
        i++;
    }
}

std::string SymbolRef::toString(ContextBase &ctx, int tabs) const {
    std::ostringstream os;
    auto myInfo = info(ctx, true);
    auto name = myInfo.name.toString(ctx);
    auto members = myInfo.members;
    printTabs(os, tabs);
    os << name << std::endl;
    std::vector<std::string> children;
    for (auto pair : members) {
        auto info = pair.second.info(ctx);
        std::string type = "unknown";
        if (info.isClass()) { type = "class"; }
        else if (info.isArray()) { type = "array"; }
        else if (info.isField()) { type = "field"; }
        else if (info.isMethod()) { type = "method"; }

        children.push_back(type + pair.second.toString(ctx, tabs + 1));
    }
    std::sort(children.begin(), children.end());
    for (auto row : children) {
        os << row;
    }
    return os.str();
}

} // namespace ast
} // namespace ruby_typer
