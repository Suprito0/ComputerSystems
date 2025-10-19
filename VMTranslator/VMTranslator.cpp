#include <string>
#include <sstream>
#include <iostream>

#include "VMTranslator.h"

using namespace std;

static int label_count = 0;
static string current_function = "GLOBAL";

/* Helper: generate unique label */
static string unique_label(const string &base){
    stringstream ss;
    ss << base << "_" << label_count++;
    return ss.str();
}

/* Small helpers that return common stuff */
static string push_D_to_stack(){
    string s;
    s += "@SP\n";
    s += "A=M\n";
    s += "M=D\n";
    s += "@SP\n";
    s += "M=M+1\n";
    return s;
}

static string pop_stack_to_D(){
    string s;
    s += "@SP\n";
    s += "M=M-1\n";
    s += "A=M\n";
    s += "D=M\n";
    return s;
}

static string pop_stack_to_addr_in_R13(){
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

VMTranslator::VMTranslator() {
}

VMTranslator::~VMTranslator() {
}

string VMTranslator::vm_push(string segment, int offset){
    string out;
    if(segment == "constant"){
        out += "@" + to_string(offset) + "\n";
        out += "D=A\n";
        out += push_D_to_stack();
    } else if(segment == "local" || segment == "argument" || segment == "this" || segment == "that"){
        string base;
        if(segment == "local") base = "LCL";
        else if(segment == "argument") base = "ARG";
        else if(segment == "this") base = "THIS";
        else base = "THAT";
        out += "@" + to_string(offset) + "\n";
        out += "D=A\n";
        out += "@" + base + "\n";
        out += "A=M+D\n";
        out += "D=M\n";
        out += push_D_to_stack();
    } else if(segment == "temp"){
        int addr = 5 + offset;
        out += "@" + to_string(addr) + "\n";
        out += "D=M\n";
        out += push_D_to_stack();
    } else if(segment == "pointer"){
        int addr = 3 + offset;
        out += "@" + to_string(addr) + "\n";
        out += "D=M\n";
        out += push_D_to_stack();
    } else if(segment == "static"){
        string sym = current_function + "." + to_string(offset);
        out += "@" + sym + "\n";
        out += "D=M\n";
        out += push_D_to_stack();
    } else {
        out += "// push unknown segment " + segment + "\n";
    }
    return out;
}

string VMTranslator::vm_pop(string segment, int offset){
    string out;
    if(segment == "local" || segment == "argument" || segment == "this" || segment == "that"){
        string base;
        if(segment == "local") base = "LCL";
        else if(segment == "argument") base = "ARG";
        else if(segment == "this") base = "THIS";
        else base = "THAT";
        out += "@" + to_string(offset) + "\n";
        out += "D=A\n";
        out += "@" + base + "\n";
        out += "D=M+D\n";
        out += "@R13\n";
        out += "M=D\n";
        out += pop_stack_to_addr_in_R13();
    } else if(segment == "temp"){
        int addr = 5 + offset;
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

string VMTranslator::vm_add(){
    string out;
    out += pop_stack_to_D();     
    out += "@SP\n";
    out += "M=M-1\n";
    out += "A=M\n";
    out += "M=M+D\n";            
    out += "@SP\n";
    out += "M=M+1\n";
    return out;
}

string VMTranslator::vm_sub(){
    string out;
    out += pop_stack_to_D();     
    out += "@SP\n";
    out += "M=M-1\n";
    out += "A=M\n";
    out += "M=M-D\n";            
    out += "@SP\n";
    out += "M=M+1\n";
    return out;
}

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

static string compare_template(const string &jump_cond){
    string lblTrue = unique_label("CMP_TRUE");
    string lblEnd  = unique_label("CMP_END");
    string out;
    out += pop_stack_to_D();  
    out += "M=M-1\n";
    out += "A=M\n";
    out += "D=M-D\n";         
    out += "@" + lblTrue + "\n";
    out += "D;" + jump_cond + "\n";
    out += "@SP\n";
    out += "A=M\n";
    out += "M=0\n";
    out += "@" + lblEnd + "\n";
    out += "0;JMP\n";
    out += "(" + lblTrue + ")\n";
    out += "@SP\n";
    out += "A=M\n";
    out += "M=-1\n";
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
    out += pop_stack_to_D();
    out += "@" + scoped + "\n";
    out += "D;JNE\n";
    return out;
}

string VMTranslator::vm_function(string function_name, int n_vars){
    current_function = function_name;
    string out;
    out += "(" + function_name + ")\n";
    for(int i=0;i<n_vars;i++){
        out += "@0\n";
        out += "D=A\n";
        out += push_D_to_stack();
    }
    return out;
}

string VMTranslator::vm_call(string function_name, int n_args){
    string out;
    string return_label = unique_label(function_name + "$ret");
    out += "@" + return_label + "\n";
    out += "D=A\n";
    out += push_D_to_stack();
    out += "@LCL\n";
    out += "D=M\n";
    out += push_D_to_stack();
    out += "@ARG\n";
    out += "D=M\n";
    out += push_D_to_stack();
    out += "@THIS\n";
    out += "D=M\n";
    out += push_D_to_stack();
    out += "@THAT\n";
    out += "D=M\n";
    out += push_D_to_stack();
    out += "@SP\n";
    out += "D=M\n";
    out += "@" + to_string(n_args + 5) + "\n";
    out += "D=D-A\n";
    out += "@ARG\n";
    out += "M=D\n";
    out += "@SP\n";
    out += "D=M\n";
    out += "@LCL\n";
    out += "M=D\n";
    out += "@" + function_name + "\n";
    out += "0;JMP\n";
    out += "(" + return_label + ")\n";
    return out;
}


string VMTranslator::vm_return(){
    string out;
    out += "@LCL\n";
    out += "D=M\n";
    out += "@R13\n";
    out += "M=D\n";
    out += "@5\n";
    out += "A=D-A\n";   
    out += "D=M\n";
    out += "@R14\n";
    out += "M=D\n";
    out += pop_stack_to_D();
    out += "@ARG\n";
    out += "A=M\n";
    out += "M=D\n";
    out += "@ARG\n";
    out += "D=M+1\n";
    out += "@SP\n";
    out += "M=D\n";
    out += "@R13\n";
    out += "AM=M-1\n";   
    out += "D=M\n";
    out += "@THAT\n";
    out += "M=D\n";
    out += "@R13\n";
    out += "AM=M-1\n";
    out += "D=M\n";
    out += "@THIS\n";
    out += "M=D\n";
    out += "@R13\n";
    out += "AM=M-1\n";
    out += "D=M\n";
    out += "@ARG\n";
    out += "M=D\n";
    out += "@R13\n";
    out += "AM=M-1\n";
    out += "D=M\n";
    out += "@LCL\n";
    out += "M=D\n";
    out += "@R14\n";
    out += "A=M\n";
    out += "0;JMP\n";
    return out;
}
