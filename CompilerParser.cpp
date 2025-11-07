#include "CompilerParser.h"

using std::string;

CompilerParser::CompilerParser(std::list<Token*> tokens)
    : tokens(tokens) {
    current_it = this->tokens.begin();
}

void CompilerParser::next() {
    if (current_it != tokens.end()) {
        ++current_it;
    }
}

Token* CompilerParser::current() {
    if (current_it == tokens.end()) return nullptr;
    return *current_it;
}

bool CompilerParser::have(std::string expectedType, std::string expectedValue) {
    Token* tok = current();
    if (!tok) return false;
    if (tok->getType() != expectedType) return false;
    if (!expectedValue.empty() && tok->getValue() != expectedValue) return false;
    return true;
}

Token* CompilerParser::mustBe(std::string expectedType, std::string expectedValue) {
    if (have(expectedType, expectedValue)) {
        Token* tok = current();
        next();
        return tok;
    }
    throw ParseException();
}

bool CompilerParser::isReservedKeyword(const std::string& v) {
    return v == "class"      || v == "constructor" || v == "function" ||
           v == "method"     || v == "field"       || v == "static"   ||
           v == "var"        || v == "int"         || v == "char"     ||
           v == "boolean"    || v == "void"        || v == "true"     ||
           v == "false"      || v == "null"        || v == "this"     ||
           v == "let"        || v == "do"          || v == "if"       ||
           v == "else"       || v == "while"       || v == "return";
}

bool CompilerParser::isOpSymbol(const std::string& v) {
    return v == "+" || v == "-" || v == "*" || v == "/" ||
           v == "&" || v == "|" || v == "<" || v == ">" || v == "=";
}

Token* CompilerParser::peekNext() {
    auto it = current_it;
    if (it == tokens.end()) return nullptr;
    ++it;
    if (it == tokens.end()) return nullptr;
    return *it;
}

Token* CompilerParser::mustBeIdentifier() {
    if (have("identifier", "")) {
        return mustBe("identifier", "");
    } else if (have("keyword", "") &&
               !isReservedKeyword(current()->getValue())) {
        Token* tok = current();
        next();
        return tok;
    } else {
        throw ParseException();
    }
}

Token* CompilerParser::parseType() {
    if (have("keyword", "int") ||
        have("keyword", "char") ||
        have("keyword", "boolean")) {
        return mustBe("keyword", "");
    } else {
        return mustBeIdentifier();
    }
}

ParseTree* CompilerParser::compileProgram() {
    if (have("keyword", "class")) {
        return compileClass();
    }
    throw ParseException();
}

ParseTree* CompilerParser::compileClass() {
    ParseTree* node = new ParseTree("class", "");
    node->addChild(mustBe("keyword", "class"));
    node->addChild(mustBeIdentifier());
    node->addChild(mustBe("symbol", "{"));

    while (have("keyword", "static") || have("keyword", "field")) {
        node->addChild(compileClassVarDec());
    }

    while (have("keyword", "constructor") ||
           have("keyword", "function") ||
           have("keyword", "method")) {
        node->addChild(compileSubroutine());
    }

    node->addChild(mustBe("symbol", "}"));
    return node;
}

ParseTree* CompilerParser::compileClassVarDec() {
    ParseTree* node = new ParseTree("classVarDec", "");
    node->addChild(mustBe("keyword", ""));
    node->addChild(parseType());
    node->addChild(mustBeIdentifier());

    while (have("symbol", ",")) {
        node->addChild(mustBe("symbol", ","));
        node->addChild(mustBeIdentifier());
    }

    node->addChild(mustBe("symbol", ";"));
    return node;
}

ParseTree* CompilerParser::compileSubroutine() {
    ParseTree* node = new ParseTree("subroutine", "");
    node->addChild(mustBe("keyword", ""));

    if (have("keyword", "void")) {
        node->addChild(mustBe("keyword", "void"));
    } else {
        node->addChild(parseType());
    }

    node->addChild(mustBeIdentifier());
    node->addChild(mustBe("symbol", "("));
    node->addChild(compileParameterList());
    node->addChild(mustBe("symbol", ")"));
    node->addChild(compileSubroutineBody());
    return node;
}

ParseTree* CompilerParser::compileParameterList() {
    ParseTree* node = new ParseTree("parameterList", "");

    if (!have("symbol", ")")) {
        node->addChild(parseType());
        node->addChild(mustBeIdentifier());

        while (have("symbol", ",")) {
            node->addChild(mustBe("symbol", ","));
            node->addChild(parseType());
            node->addChild(mustBeIdentifier());
        }
    }

    return node;
}

ParseTree* CompilerParser::compileSubroutineBody() {
    ParseTree* node = new ParseTree("subroutineBody", "");
    node->addChild(mustBe("symbol", "{"));

    while (have("keyword", "var")) {
        node->addChild(compileVarDec());
    }

    node->addChild(compileStatements());
    node->addChild(mustBe("symbol", "}"));
    return node;
}

ParseTree* CompilerParser::compileVarDec() {
    ParseTree* node = new ParseTree("varDec", "");
    node->addChild(mustBe("keyword", "var"));
    node->addChild(parseType());
    node->addChild(mustBeIdentifier());

    while (have("symbol", ",")) {
        node->addChild(mustBe("symbol", ","));
        node->addChild(mustBeIdentifier());
    }

    node->addChild(mustBe("symbol", ";"));
    return node;
}

ParseTree* CompilerParser::compileStatements() {
    ParseTree* node = new ParseTree("statements", "");

    while (true) {
        if (have("keyword", "let")) {
            node->addChild(compileLet());
        } else if (have("keyword", "if")) {
            node->addChild(compileIf());
        } else if (have("keyword", "while")) {
            node->addChild(compileWhile());
        } else if (have("keyword", "do")) {
            node->addChild(compileDo());
        } else if (have("keyword", "return")) {
            node->addChild(compileReturn());
        } else {
            break;
        }
    }

    return node;
}

ParseTree* CompilerParser::compileLet() {
    ParseTree* node = new ParseTree("letStatement", "");
    node->addChild(mustBe("keyword", "let"));
    node->addChild(mustBeIdentifier());

    if (have("symbol", "[")) {
        node->addChild(mustBe("symbol", "["));
        node->addChild(compileExpression());
        node->addChild(mustBe("symbol", "]"));
    }

    node->addChild(mustBe("symbol", "="));
    node->addChild(compileExpression());
    node->addChild(mustBe("symbol", ";"));
    return node;
}

ParseTree* CompilerParser::compileIf() {
    ParseTree* node = new ParseTree("ifStatement", "");
    node->addChild(mustBe("keyword", "if"));
    node->addChild(mustBe("symbol", "("));
    node->addChild(compileExpression());
    node->addChild(mustBe("symbol", ")"));
    node->addChild(mustBe("symbol", "{"));
    node->addChild(compileStatements());
    node->addChild(mustBe("symbol", "}"));

    if (have("keyword", "else")) {
        node->addChild(mustBe("keyword", "else"));
        node->addChild(mustBe("symbol", "{"));
        node->addChild(compileStatements());
        node->addChild(mustBe("symbol", "}"));
    }

    return node;
}

ParseTree* CompilerParser::compileWhile() {
    ParseTree* node = new ParseTree("whileStatement", "");
    node->addChild(mustBe("keyword", "while"));
    node->addChild(mustBe("symbol", "("));
    node->addChild(compileExpression());
    node->addChild(mustBe("symbol", ")"));
    node->addChild(mustBe("symbol", "{"));
    node->addChild(compileStatements());
    node->addChild(mustBe("symbol", "}"));
    return node;
}

ParseTree* CompilerParser::compileDo() {
    ParseTree* node = new ParseTree("doStatement", "");
    node->addChild(mustBe("keyword", "do"));
    node->addChild(compileSubroutineCall());
    node->addChild(mustBe("symbol", ";"));
    return node;
}

ParseTree* CompilerParser::compileReturn() {
    ParseTree* node = new ParseTree("returnStatement", "");
    node->addChild(mustBe("keyword", "return"));
    if (!have("symbol", ";")) {
        node->addChild(compileExpression());
    }
    node->addChild(mustBe("symbol", ";"));
    return node;
}

ParseTree* CompilerParser::compileExpression() {
    ParseTree* node = new ParseTree("expression", "");
    node->addChild(compileTerm());

    while (have("symbol", "") && isOpSymbol(current()->getValue())) {
        node->addChild(mustBe("symbol", ""));
        node->addChild(compileTerm());
    }

    return node;
}

ParseTree* CompilerParser::compileTerm() {
    ParseTree* node = new ParseTree("term", "");
    Token* tok = current();
    if (!tok) throw ParseException();

    string type  = tok->getType();
    string value = tok->getValue();

    if (type == "integerConstant") {
        node->addChild(mustBe("integerConstant", ""));
    } else if (type == "stringConstant") {
        node->addChild(mustBe("stringConstant", ""));
    } else if (type == "keyword" &&
               (value == "true" || value == "false" ||
                value == "null" || value == "this")) {
        node->addChild(mustBe("keyword", ""));
    } else if (type == "identifier" ||
               (type == "keyword" && !isReservedKeyword(value))) {
        Token* nextTok = peekNext();

        if (nextTok &&
            nextTok->getType() == "symbol" &&
            nextTok->getValue() == "[") {
            node->addChild(mustBeIdentifier());
            node->addChild(mustBe("symbol", "["));
            node->addChild(compileExpression());
            node->addChild(mustBe("symbol", "]"));
        } else if (nextTok &&
                   nextTok->getType() == "symbol" &&
                  (nextTok->getValue() == "(" ||
                   nextTok->getValue() == ".")) {
            node->addChild(compileSubroutineCall());
        } else {
            node->addChild(mustBeIdentifier());
        }
    } else if (type == "symbol" && value == "(") {
        node->addChild(mustBe("symbol", "("));
        node->addChild(compileExpression());
        node->addChild(mustBe("symbol", ")"));
    } else if (type == "symbol" && (value == "-" || value == "~")) {
        node->addChild(mustBe("symbol", ""));
        node->addChild(compileTerm());
    } else {
        throw ParseException();
    }

    return node;
}

ParseTree* CompilerParser::compileExpressionList() {
    ParseTree* node = new ParseTree("expressionList", "");

    if (!have("symbol", ")")) {
        node->addChild(compileExpression());
        while (have("symbol", ",")) {
            node->addChild(mustBe("symbol", ","));
            node->addChild(compileExpression());
        }
    }

    return node;
}

ParseTree* CompilerParser::compileSubroutineCall() {
    ParseTree* node = new ParseTree("subroutineCall", "");
    node->addChild(mustBeIdentifier());

    if (have("symbol", ".")) {
        node->addChild(mustBe("symbol", "."));
        node->addChild(mustBeIdentifier());
    }

    node->addChild(mustBe("symbol", "("));
    node->addChild(compileExpressionList());
    node->addChild(mustBe("symbol", ")"));
    return node;
}

const char* ParseException::what(){
    return "An Exception occurred while parsing!";
}
