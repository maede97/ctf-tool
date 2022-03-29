#pragma once

#include <ctf-tool/operation.h>

namespace ctf
{
    class OpCls_Base64Encode : public Operation
    {
    public:
        OpCls_Base64Encode(const Input &input);
        virtual ~OpCls_Base64Encode() {}

        virtual void run() override;
        bool disallow_after(AllOperations type_prev) const override;
    };

    class OpCls_Base64Decode : public Operation
    {
    public:
        OpCls_Base64Decode(const Input &input);
        virtual ~OpCls_Base64Decode() {}

        virtual void run() override;
        bool disallow_after(AllOperations type_prev) const override;
    };
}