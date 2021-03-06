#pragma once

#include <ctf-tool/operation.h>

namespace ctf {

class OpCls_FromHex : public Operation {
public:
    OpCls_FromHex(const Input& input, const Key& key = Key(), std::shared_ptr<Operation> previous_operation = nullptr);
    virtual ~OpCls_FromHex() {}

    virtual void run() override;
};

class OpCls_ToHex : public Operation {
public:
    OpCls_ToHex(const Input& input, const Key& key = Key(), std::shared_ptr<Operation> previous_operation = nullptr);
    virtual ~OpCls_ToHex() {}

    virtual void run() override;
};
}  // namespace ctf