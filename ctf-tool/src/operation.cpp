#include <ctf-tool/operation.h>

#define CREATE_STRINGS(ops) #ops,

namespace ctf
{
    const char *AllOperations_to_string(AllOperations op)
    {
        const char *Op_Names[] = {ALL_OPS(CREATE_STRINGS)};
        return Op_Names[static_cast<int>(op)];
    }

    Operation::Operation(AllOperations type, const Input &input, const Key& key) : m_type(type), m_input(input), m_key(key) {}

    OperationStatus Operation::status() const
    {
        return m_status;
    }

    bool Operation::disallow_after(AllOperations type_prev) const { return false; }

    void Operation::setAssigned()
    {
        m_status = OperationStatus::Running;
    }
    void Operation::setCompleted()
    {
        m_status = OperationStatus::Finished;
    }

    AllOperations Operation::type() const
    {
        return m_type;
    }

    const Output &Operation::getOutput() const
    {
        return m_output;
    }
}