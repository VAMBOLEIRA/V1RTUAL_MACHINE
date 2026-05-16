#include "CU.hpp"
#include <iostream>
#include "lexycal_analyzer.hpp"

CU::CU() : status("idle") {}

CU::CU(std::string theStatus) : status(theStatus) {}

Instruccion CU::fetch(Program& theProgram)
{
    // MAR ← PC
    reg.setMAR(reg.getPC());
    // MBR ← Memoria[MAR]
    Instruccion inst = theProgram.getInstruction(reg.getMAR());
    reg.setMBR(inst.getCode());  // guardamos el código en MBR
    // IR ← MBR
    reg.setIR(reg.getMBR());
    // PC++
    //reg.incrementPC();
    
    return inst;
}

int CU::decode(const Instruccion& theInstruction)
{
    LexicalAnalyzer lexer;
    std::string name = theInstruction.getName();
    int code = lexer.getInstructionCode(name);
    
    if (code == -1) {
        std::cout << "Error: Instrucción '" << name << "' no reconocida por el analizador léxico" << std::endl;
    }

	return code;
}

void CU::execute(int theCode, int operand1, int operand2)
{
    int result;
	
    
    switch (theCode)
    {
        case 50:  // START
            std::cout << "=== Inicio del programa ===" << std::endl;
            status = "running";
            reg.reset();  // Inicializar registros
            break;
            
        case 51:  // STOP
            std::cout << "=== Fin de ejecucion del programa ===" << std::endl;
            status = "stopped";
            reg.display();  // Mostrar estado final de registros
            break;
            
        case 80:  // ADD
            // Cargar operandos en registros de propósito general
            reg.setAL(operand1);
            reg.setBL(operand2);
            // Ejecutar en ALU
            result = alu.add(reg.getAL(), reg.getBL());
            // Guardar resultado en AC
            reg.setAC(result);
            std::cout << "ADD: " << operand1 << " + " << operand2 
                      << " = " << result << " [AC=" << reg.getAC() << "]" << std::endl;
            break;
            
        case 81:  // SUB
            reg.setAL(operand1);
            reg.setBL(operand2);
            result = alu.sub(reg.getAL(), reg.getBL());
            reg.setAC(result);
            std::cout << "SUB: " << operand1 << " - " << operand2 
                      << " = " << result << " [AC=" << reg.getAC() << "]" << std::endl;
            break;
            
        case 82:  // MUL
            reg.setAL(operand1);
            reg.setBL(operand2);
            result = alu.mul(reg.getAL(), reg.getBL());
            reg.setAC(result);
            std::cout << "MUL: " << operand1 << " * " << operand2 
                      << " = " << result << " [AC=" << reg.getAC() << "]" << std::endl;
            break;
            
        case 83:  // DIV
            reg.setAL(operand1);
            reg.setBL(operand2);
            result = alu.div(reg.getAL(), reg.getBL());
            reg.setAC(result);
            std::cout << "DIV: " << operand1 << " / " << operand2 
                      << " = " << result << " [AC=" << reg.getAC() << "]" << std::endl;
            break;
            
        case 90:  // MOV reg, valor
            reg.setAL(operand1);  // Mover a AL
            std::cout << "MOV: AL = " << operand1 << std::endl;
            break;
            
        case 91:  // STO addr
            reg.setMAR(operand2);  // Dirección en MAR
            reg.setMBR(operand1);  // Valor en MBR
            std::cout << "STO: memoria[" << reg.getMAR() << "] = " 
                      << reg.getMBR() << std::endl;
            break;
            
        default:
            std::cout << "Error: Instruccion desconocida (codigo: " 
                      << theCode << ")" << std::endl;
            break;
    }
}

void CU::run(Program& theProgram)
{
    status = "running";
    reg.reset();
    reg.setPC(0);
    
    while (status == "running" && reg.getPC() < theProgram.getSize())
    {
        std::cout << "\n[Ciclo " << reg.getPC() + 1 << "]" << std::endl;
        
        int pc_actual = reg.getPC();
        
        // FETCH
        Instruccion inst = fetch(theProgram);
        std::cout << "Fetch: " << inst.getName() 
                  << " [PC=" << pc_actual << " -> IR=" << reg.getIR() << "]" << std::endl;
        
        // DECODE
        int code = decode(inst);
        std::cout << "Decode: codigo " << code << std::endl;
        
        // Obtener operandos
        int op1 = theProgram.getOperand1(pc_actual);
        int op2 = theProgram.getOperand2(pc_actual);
        
        // EXECUTE
        std::cout << "Execute: ";
        execute(code, op1, op2);
        
        // Incrementar PC DESPUÉS de ejecutar
        reg.incrementPC();
    }
    
    std::cout << "\nPrograma finalizado." << std::endl;
}

void CU::displayRegistros() const {
    reg.display();
}
