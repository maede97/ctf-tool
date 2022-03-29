#pragma once
#include <ctf-tool/input.h>
#include <ctf-tool/output.h>
#include <ctf-tool/key.h>

namespace ctf
{

// clang-format off
#define ALL_OPS(func) \
func(Base64Encode)    \
func(Base64Decode)    \
func(PassThrough)     \
func(FromHex)         \
func(ToHex)           \
func(XOR)

#define CREATE_ENUM(ops) Op_##ops,

    // clang-format on

    enum class AllOperations
    {
        ALL_OPS(CREATE_ENUM)
            Op_Count
    };

    const char *AllOperations_to_string(AllOperations op);

    enum class OperationStatus
    {
        NotStarted,
        Running,
        Finished
    };

    /**
     * @brief The Operation class is the base class for all operations.
     */
    class Operation
    {
    public:
        Operation(AllOperations type, const Input &input, const Key & = Key());
        virtual ~Operation() = default;
        virtual void run() = 0;

        virtual bool disallow_after(AllOperations type_prev) const;

        OperationStatus status() const;
        void setAssigned();
        void setCompleted();

        AllOperations type() const;

        const Output &getOutput() const;

    protected:
        Input m_input;
        Key m_key;
        Output m_output;
        OperationStatus m_status = OperationStatus::NotStarted;

        AllOperations m_type;
    };
}