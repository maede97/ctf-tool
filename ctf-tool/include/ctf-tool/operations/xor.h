#pragma once

#include <ctf-tool/operation.h>

namespace ctf {
class OpCls_XOR : public Operation {
public:
    OpCls_XOR(const Input& input, const Key& key = Key());
    virtual ~OpCls_XOR() {}

    virtual void run() override;

    bool disallow_after(AllOperations type_prev) const override;
};
}  // namespace ctf