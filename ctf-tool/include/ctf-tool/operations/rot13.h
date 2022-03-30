#pragma once

#include <ctf-tool/operation.h>

namespace ctf {
class OpCls_Rot13 : public Operation {
public:
    OpCls_Rot13(const Input& input, const Key& key = Key(), std::shared_ptr<Operation> previous_operation = nullptr);
    virtual ~OpCls_Rot13() {}

    virtual void run() override;

    bool disallow_after(AllOperations type_prev) const override;
};
}  // namespace ctf