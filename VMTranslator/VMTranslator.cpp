#include <string>
#include <sstream>
#include <iostream>

#include "VMTranslator.h"

using namespace std;

/*
  Implementation notes:
  - Uses a file-scoped label counter to create unique labels for eq/gt/lt and call return addresses.
  - Maintains a currentFunction string to scope labels and static variables (static variables named as functionName.i).
  - temp segment base: 5
  - pointer segment: 3 (pointer 0 -> THIS (3), pointer 1 -> THAT (4))
  - static variables: named functionName.i (so they are unique per VM "file" / function).
*/

static int label_count = 0;
static string current_function = "GLOBAL";

/* Helper: generate unique label */
static string unique_label(const string &base){
    stringstream ss;
    ss << base << "_" << label_count++;
    return ss.str();
}

/* Small helpers that return common assembly fragments */
static string push_D_to_stack(){
    // *SP = D; SP++
    string s;
    s += "@SP\n";
    s += "A=M\n";
    s += "M=D\n";
    s += "@SP\n";
    s += "M=M+1\n";
    return s;
}

static string pop_stack_to_D(){
    // SP--; D = *SP
    string s;
    s += "@SP\n";
    s += "M=M-1\n";
    s += "A=M\n";
    s += "D=M\n";
    return s;
}

static string pop_stack_to_addr_in_R13(){
    // Pop top into D, and store it into the address stored in R13:
    // SP--; D=*SP; @R13 A=M; M=D
    string s;
    s += "@SP\n";
    s += "M=M-1\n";
    s += "A=M\n";
    s += "D=M\n";
    s += "@R13\n";
    s += "A=M\n";
    s += "M=D\n";
    return s;
}

/** VMTranslator constructor */
VMTranslator::VMTranslator() {
    // nothing to initialize here (static state is file-scoped)
}

/** VMTranslator destructor */
VMTranslator::~VMTranslator() {
    // nothing to free
}

/** push segment offset */
string VMTranslator::vm_push(string segment, int offset){
    string out;
    if(segment == "constant"){
        // D = offset
        out += "@" + to_string(offset) + "\n";
        out += "D=A\n";
        out += push_D_to_stack();
    } else if(segment == "local" || segment == "argument" || segment == "this" || segment == "that"){
        string base;
        if(segment == "local") base = "LCL";
        else if(segment == "argument") base = "ARG";
        else if(segment == "this") base = "THIS";
        else base = "THAT";
        // D = *(base + offset)
        out += "@" + to_string(offset) + "\n";
        out += "D=A\n";
        out += "@" + base + "\n";
        out += "A=M+D\n";
        out += "D=M\n";
        out += push_D_to_stack();
    } else if(segment == "temp"){
        // temp base 5
        int addr = 5 + offset;
        out += "@" + to_string(addr) + "\n";
        out += "D=M\n";
        out += push_D_to_stack();
    } else if(segment == "pointer"){
        // pointer 0 -> THIS (3), pointer 1 -> THAT (4)
        int addr = 3 + offset;
        out += "@" + to_string(addr) + "\n";
        out += "D=M\n";
        out += push_D_to_stack();
    } else if(segment == "static"){
        // use current_function.offset as symbol name
        string sym = current_function + "." + to_string(offset);
        out += "@" + sym + "\n";
        out += "D=M\n";
        out += push_D_to_stack();
    } else {
        // Unknown segment: generate no-op comment
        out += "// push unknown segment " + segment + "\n";
    }
    return out;
}

/** pop segment offset */
string VMTranslator::vm_pop(string segment, int offset){
    string out;
    if(segment == "local" || segment == "argument" || segment == "this" || segment == "that"){
        string base;
        if(segment == "local") base = "LCL";
        else if(segment == "argument") base = "ARG";
        else if(segment == "this") base = "THIS";
        else base = "THAT";
        // addr = base + offset -> store in R13; pop stack -> *R13 = popped
        out += "@" + to_string(offset) + "\n";
        out += "D=A\n";
        out += "@" + base + "\n";
        out += "D=M+D\n";
        out += "@R13\n";
        out += "M=D\n";
        out += pop_stack_to_addr_in_R13();
    } else if(segment == "temp"){
        int addr = 5 + offset;
        // pop stack then store in fixed address
        out += pop_stack_to_D();
        out += "@" + to_string(addr) + "\n";
        out += "M=D\n";
    } else if(segment == "pointer"){
        int addr = 3 + offset;
        out += pop_stack_to_D();
        out += "@" + to_string(addr) + "\n";
        out += "M=D\n";
    } else if(segment == "static"){
        string sym = current_function + "." + to_string(offset);
        out += pop_stack_to_D();
        out += "@" + sym + "\n";
        out += "M=D\n";
    } else {
        out += "// pop unknown segment " + segment + "\n";
    }
    return out;
}

/** Arithmetic and logical operations */

/** add: pop y, pop x, push x+y */
string VMTranslator::vm_add(){
    string out;
    out += pop_stack_to_D();      // D = y
    out += "@SP\n";
    out += "M=M-1\n";
    out += "A=M\n";
    out += "M=M+D\n";             // x = x + y
    out += "@SP\n";
    out += "M=M+1\n";
    return out;
}

/** sub: pop y, pop x, push x-y */
string VMTranslator::vm_sub(){
    string out;
    out += pop_stack_to_D();      // D = y
    out += "@SP\n";
    out += "M=M-1\n";
    out += "A=M\n";
    out += "M=M-D\n";             // x = x - y
    out += "@SP\n";
    out += "M=M+1\n";
    return out;
}

/** neg: pop x, push -x */
string VMTranslator::vm_neg(){
    string out;
    out += "@SP\n";
    out += "M=M-1\n";
    out += "A=M\n";
    out += "D=M\n";
    out += "D=-D\n";
    out += push_D_to_stack();
    return out;
}

/* Comparison helpers (eq, gt, lt) produce -1 for true, 0 for false */
static string compare_template(const string &jump_cond){
    // Pop y into D; pop x into M; compute x-y; if x-y {jump_cond} set -1 else 0
    string lblTrue = unique_label("CMP_TRUE");
    string lblEnd  = unique_label("CMP_END");
    string out;
    out += pop_stack_to_D();  // D=y
    out += "@SP\n";
    out += "M=M-1\n";
    out += "A=M\n";
    out += "D=M-D\n";         // D = x - y
    out += "@" + lblTrue + "\n";
    out += "D;" + jump_cond + "\n";
    // false
    out += "@SP\n";
    out += "A=M\n";
    out += "M=0\n";
    out += "@" + lblEnd + "\n";
    out += "0;JMP\n";
    // true
    out += "(" + lblTrue + ")\n";
    out += "@SP\n";
    out += "A=M\n";
    out += "M=-1\n";
    // end
    out += "(" + lblEnd + ")\n";
    out += "@SP\n";
    out += "M=M+1\n";
    return out;
}

string VMTranslator::vm_eq(){
    return compare_template("JEQ");
}

string VMTranslator::vm_gt(){
    return compare_template("JGT");
}

string VMTranslator::vm_lt(){
    return compare_template("JLT");
}

string VMTranslator::vm_and(){
    // pop y into D; pop x into M; push x & y
    string out;
    out += pop_stack_to_D();
    out += "@SP\n";
    out += "M=M-1\n";
    out += "A=M\n";
    out += "M=M&D\n";
    out += "@SP\n";
    out += "M=M+1\n";
    return out;
}

string VMTranslator::vm_or(){
    string out;
    out += pop_stack_to_D();
    out += "@SP\n";
    out += "M=M-1\n";
    out += "A=M\n";
    out += "M=M|D\n";
    out += "@SP\n";
    out += "M=M+1\n";
    return out;
}

string VMTranslator::vm_not(){
    string out;
    out += "@SP\n";
    out += "M=M-1\n";
    out += "A=M\n";
    out += "D=M\n";
    out += "D=!D\n";
    out += push_D_to_stack();
    return out;
}

/** Program flow: label, goto, if-goto
    Labels are scoped by current_function: functionName$label
*/
string VMTranslator::vm_label(string label){
    string scoped = current_function + "$" + label;
    string out;
    out += "(" + scoped + ")\n";
    return out;
}

string VMTranslator::vm_goto(string label){
    string scoped = current_function + "$" + label;
    string out;
    out += "@" + scoped + "\n";
    out += "0;JMP\n";
    return out;
}

string VMTranslator::vm_if(string label){
    string scoped = current_function + "$" + label;
    string out;
    // pop top into D, if D != 0 goto scoped
    out += pop_stack_to_D();
    out += "@" + scoped + "\n";
    out += "D;JNE\n";
    return out;
}

/** function f nvars: declare label (f) and push nvars zeros onto stack */
string VMTranslator::vm_function(string function_name, int n_vars){
    current_function = function_name;
    string out;
    out += "(" + function_name + ")\n";
    // push n_vars zeros
    for(int i=0;i<n_vars;i++){
        out += "@0\n";
        out += "D=A\n";
        out += push_D_to_stack();
    }
    return out;
}

/** call function_name n_args
    - push return-address
    - push LCL, ARG, THIS, THAT
    - ARG = SP - n_args - 5
    - LCL = SP
    - goto function_name
    - (return-address)
*/
string VMTranslator::vm_call(string function_name, int n_args){
    string out;
    string return_label = unique_label(function_name + "$ret");
    // push return-address
    out += "@" + return_label + "\n";
    out += "D=A\n";
    out += push_D_to_stack();
    // push LCL
    out += "@LCL\n";
    out += "D=M\n";
    out += push_D_to_stack();
    // push ARG
    out += "@ARG\n";
    out += "D=M\n";
    out += push_D_to_stack();
    // push THIS
    out += "@THIS\n";
    out += "D=M\n";
    out += push_D_to_stack();
    // push THAT
    out += "@THAT\n";
    out += "D=M\n";
    out += push_D_to_stack();
    // ARG = SP - n_args - 5
    out += "@SP\n";
    out += "D=M\n";
    out += "@" + to_string(n_args + 5) + "\n";
    out += "D=D-A\n";
    out += "@ARG\n";
    out += "M=D\n";
    // LCL = SP
    out += "@SP\n";
    out += "D=M\n";
    out += "@LCL\n";
    out += "M=D\n";
    // goto function_name
    out += "@" + function_name + "\n";
    out += "0;JMP\n";
    // return-address label
    out += "(" + return_label + ")\n";
    return out;
}

/** return
   FRAME = LCL (store in R13)
   RET = *(FRAME-5) (store in R14)
   *ARG = pop()
   SP = ARG+1
   THAT = *(FRAME-1)
   THIS = *(FRAME-2)
   ARG  = *(FRAME-3)
   LCL  = *(FRAME-4)
   goto RET
*/
string VMTranslator::vm_return(){
    string out;
    // FRAME = LCL -> R13
    out += "@LCL\n";
    out += "D=M\n";
    out += "@R13\n";
    out += "M=D\n";
    // RET = *(FRAME - 5) -> R14
    out += "@5\n";
    out += "A=D-A\n";   // A = FRAME - 5
    out += "D=M\n";
    out += "@R14\n";
    out += "M=D\n";
    // *ARG = pop()
    out += pop_stack_to_D();
    out += "@ARG\n";
    out += "A=M\n";
    out += "M=D\n";
    // SP = ARG + 1
    out += "@ARG\n";
    out += "D=M+1\n";
    out += "@SP\n";
    out += "M=D\n";
    // restore THAT = *(FRAME - 1)
    out += "@R13\n";
    out += "AM=M-1\n";   // R13 = FRAME-1 ; A=R13
    out += "D=M\n";
    out += "@THAT\n";
    out += "M=D\n";
    // restore THIS = *(FRAME - 2)
    out += "@R13\n";
    out += "AM=M-1\n";
    out += "D=M\n";
    out += "@THIS\n";
    out += "M=D\n";
    // restore ARG = *(FRAME - 3)
    out += "@R13\n";
    out += "AM=M-1\n";
    out += "D=M\n";
    out += "@ARG\n";
    out += "M=D\n";
    // restore LCL = *(FRAME - 4)
    out += "@R13\n";
    out += "AM=M-1\n";
    out += "D=M\n";
    out += "@LCL\n";
    out += "M=D\n";
    // goto RET (R14)
    out += "@R14\n";
    out += "A=M\n";
    out += "0;JMP\n";
    return out;
}
