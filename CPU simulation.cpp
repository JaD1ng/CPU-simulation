#include <iostream>
#include <vector>

using namespace std;

// 定义指令类型
enum class Opcode {
    ADD,
    SUB,
    MOV,
    JMP
};

// 定义 CPU 控制器类
class ControlUnit {
public:
    ControlUnit() : pc_(0) {}

    // 加载指令到指令寄存器
    void LoadInstruction(vector<int> instruction) {
        instruction_ = instruction;
        pc_ = 0;
    }

    // 读取当前指令
    Opcode ReadOpcode() {
        int opcode = instruction_[pc_] >> 4;
        return static_cast<Opcode>(opcode);
    }

    // 读取操作数
    int ReadOperand() {
        int operand = instruction_[pc_] & 0xf;
        pc_++;
        return operand;
    }

private:
    vector<int> instruction_; // 指令寄存器
    int pc_; // 程序计数器
};

// 定义 CPU 运算器类
class ALU {
public:
    // 加法运算
    int Add(int x, int y) {
        return x + y;
    }

    // 减法运算
    int Sub(int x, int y) {
        return x - y;
    }

    // 移动数据
    int Mov(int x) {
        return x;
    }
};

// 定义 CPU 寄存器类
class Register {
public:
    // 读取寄存器的值
    int Read() {
        return value_;
    }

    // 写入寄存器的值
    void Write(int value) {
        value_ = value;
    }

private:
    int value_; // 寄存器的值
};

// 定义 CPU 类
class CPU {
public:
    CPU() : alu_(), control_unit_(), accumulator_(), operand_() {}

    // 执行程序
    void Run(vector<vector<int>> program) {
        program_ = program;
        for (auto instruction : program_) {
            control_unit_.LoadInstruction(instruction);
            ExecuteInstruction();
        }
    }

    // 获取累加器的值
    int GetAccumulatorValue() {
        return accumulator_.Read();
    }

private:
    ALU alu_; // 运算器
    ControlUnit control_unit_; // 控制器
    Register accumulator_; // 累加器
    int operand_; // 操作数
    vector<vector<int>> program_; // 程序

    // 执行指令
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
            // 直接跳转到指定位置
            control_unit_.LoadInstruction(program_[operand_]);
            break;
        default:
            break;
        }
    }
};

// 主函数
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