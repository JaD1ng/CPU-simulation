#include <iostream>
#include <vector>

using namespace std;

// ����ָ������
enum class Opcode {
    ADD,
    SUB,
    MOV,
    JMP
};

// ���� CPU ��������
class ControlUnit {
public:
    ControlUnit() : pc_(0) {}

    // ����ָ�ָ��Ĵ���
    void LoadInstruction(vector<int> instruction) {
        instruction_ = instruction;
        pc_ = 0;
    }

    // ��ȡ��ǰָ��
    Opcode ReadOpcode() {
        int opcode = instruction_[pc_] >> 4;
        return static_cast<Opcode>(opcode);
    }

    // ��ȡ������
    int ReadOperand() {
        int operand = instruction_[pc_] & 0xf;
        pc_++;
        return operand;
    }

private:
    vector<int> instruction_; // ָ��Ĵ���
    int pc_; // ���������
};

// ���� CPU ��������
class ALU {
public:
    // �ӷ�����
    int Add(int x, int y) {
        return x + y;
    }

    // ��������
    int Sub(int x, int y) {
        return x - y;
    }

    // �ƶ�����
    int Mov(int x) {
        return x;
    }
};

// ���� CPU �Ĵ�����
class Register {
public:
    // ��ȡ�Ĵ�����ֵ
    int Read() {
        return value_;
    }

    // д��Ĵ�����ֵ
    void Write(int value) {
        value_ = value;
    }

private:
    int value_; // �Ĵ�����ֵ
};

// ���� CPU ��
class CPU {
public:
    CPU() : alu_(), control_unit_(), accumulator_(), operand_() {}

    // ִ�г���
    void Run(vector<vector<int>> program) {
        program_ = program;
        for (auto instruction : program_) {
            control_unit_.LoadInstruction(instruction);
            ExecuteInstruction();
        }
    }

    // ��ȡ�ۼ�����ֵ
    int GetAccumulatorValue() {
        return accumulator_.Read();
    }

private:
    ALU alu_; // ������
    ControlUnit control_unit_; // ������
    Register accumulator_; // �ۼ���
    int operand_; // ������
    vector<vector<int>> program_; // ����

    // ִ��ָ��
    void ExecuteInstruction() {
        Opcode opcode = control_unit_.ReadOpcode();
        operand_ = control_unit_.ReadOperand();

        switch (opcode) {
        case Opcode::ADD:
            accumulator_.Write(alu_.Add(accumulator_.Read(), operand_));
            break;
        case Opcode::SUB:
            accumulator_.Write(alu_.Sub(accumulator_.Read(), operand_));
            break;
        case Opcode::MOV:
            accumulator_.Write(alu_.Mov(operand_));
            break;
        case Opcode::JMP:
            // ֱ����ת��ָ��λ��
            control_unit_.LoadInstruction(program_[operand_]);
            break;
        default:
            break;
        }
    }
};

// ������
int main() {
    vector<vector<int>> program = {
    {0x10, 0x05}, // ADD 5
    {0x20, 0x03}, // MOV 3
    {0x10, 0x02}, // ADD 2
    {0x30, 0x00}, // JMP 0
    };

    CPU cpu;
    cpu.Run(program);
    cout << "Result: " << cpu.GetAccumulatorValue() << endl;

    return 0;
}