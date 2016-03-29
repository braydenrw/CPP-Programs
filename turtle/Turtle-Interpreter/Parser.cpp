#include "Parser.h"
#include <sstream>
#include <stdexcept>

void Parser::match(Token tok) {
    if (tok != lookahead_) {
        std::stringstream ss;
        ss << "Unexpected token '" << tokenToString(lookahead_) << "', ";
        ss << "Expecting '" << tokenToString(tok) << "'";
        throw std::runtime_error(ss.str());
    }
    lookahead_ = scanner_.nextToken(attribute_, lineno_);
}

void Parser::parse() {
    lookahead_ = scanner_.nextToken(attribute_, lineno_);
    try {
        prog();
    } catch(const std::exception& error) {
        std::stringstream ss;
        ss << lineno_ << ": " << error.what();
        throw std::runtime_error(ss.str());
    }
}

void Parser::prog() {
    stmt_seq();
    match(Token::EOT);
}

void Parser::stmt_seq() {
    while (lookahead_ != Token::EOT) {
        Stmt *s = stmt();
        AST_.push_back(s);
    }
}

Stmt *Parser::stmt() {
    if (lookahead_ == Token::IDENT)
        return assign();
    else if (lookahead_ == Token::IF)
        return if_stmt();
    else if (lookahead_ == Token::WHILE)
        return while_stmt();
    else
        return action();
}

Stmt *Parser::assign() {
    std::string name = attribute_.s;
    match(Token::IDENT);
    match(Token::ASSIGN);
    Expr *e = expr();
    return new AssignStmt(name, e);
}

Stmt *Parser::block() {
    std::vector<Stmt*> stmts;
    do {
        Stmt *s = stmt();
        stmts.push_back(s);
    } while(lookahead_ != Token::FI && lookahead_ != Token::OD && 
        lookahead_ != Token::ELSE && lookahead_ != Token::ELSIF);
    return new BlockStmt(stmts);
}

Stmt *Parser::while_stmt() {
    match(Token::WHILE);
    Expr *cond = bool_();
    match(Token::DO);
    Stmt *body = block();
    match(Token::OD);
    return new WhileStmt(cond, body);
}

Stmt *Parser::elsePart() {
    switch(lookahead_) {
        case Token::ELSIF: {
            match(Token::ELSIF);
            Expr *cond = bool_();
            match(Token::THEN);
            Stmt *body = block();
            Stmt *else_body = elsePart();
            return new IfStmt(cond, body, else_body);
        }
        case Token::ELSE: {
            match(Token::ELSE);
            Stmt *body = block();
            match(Token::FI);
            return body;
        }
        case Token::FI: match(Token::FI); return NULL;
        default: throw std::runtime_error("Expecting factor!");
    }
}

Stmt *Parser::if_stmt() {
    match(Token::IF);
    Expr *cond = bool_();
    match(Token::THEN);
    Stmt *body = block();
    Stmt *else_body = elsePart();
    return new IfStmt(cond, body, else_body);
}

Stmt *Parser::action() {
    switch(lookahead_) {
        case Token::HOME:    match(Token::HOME); return new HomeStmt();
        case Token::PENUP:   match(Token::PENUP); return new PenUpStmt();
        case Token::PENDOWN: match(Token::PENDOWN); return new PenDownStmt();
        case Token::FORWARD: match(Token::FORWARD); return new ForwardStmt(expr());
        case Token::LEFT:    match(Token::LEFT); return new LeftStmt(expr());
        case Token::RIGHT:   match(Token::RIGHT); return new RightStmt(expr());
        case Token::PUSHSTATE:
            match(Token::PUSHSTATE); return new PushStateStmt();
        case Token::POPSTATE:
            match(Token::POPSTATE); return new PopStateStmt();
        default:
            throw std::runtime_error("Expecting turtle action statement!");
    }
}

Expr *Parser::expr() {
    Expr *e = term();
    while (lookahead_ == Token::PLUS ||
           lookahead_ == Token::MINUS) {
        const auto op = lookahead_;
        match(lookahead_);
        Expr *t = term();
        if (op == Token::PLUS)
            e = new AddExpr(e, t);
        else
            e = new SubExpr(e, t);
    }
    return e;
}

Expr *Parser::term() {
    Expr *e = factor();
    while (lookahead_ == Token::MULT ||
           lookahead_ == Token::DIV) {
        Token op = lookahead_;
        match(lookahead_);
        Expr *t = factor();
        if (op == Token::MULT)
            e = new MulExpr(e, t);
        else
            e = new DivExpr(e, t);
    }
    return e;
}

Expr *Parser::factor() {
    switch(lookahead_) {
        case Token::PLUS:   match(Token::PLUS); return factor();
        case Token::MINUS:  match(Token::MINUS); return new NegExpr(factor());
        case Token::LPAREN:
        {
            match(Token::LPAREN);
            Expr *e = expr();
            match(Token::RPAREN);
            return e;
        }
        case Token::IDENT:
        {
            const std::string name = attribute_.s;
            match(Token::IDENT);
            return new VarExpr(name);
        }
        case Token::REAL:
        {
            const float val = attribute_.f;
            match(Token::REAL);
            return new ConstExpr(val);
        }
        default:
            throw std::runtime_error("Expecting factor!");
    }
}

Expr *Parser::bool_() {
    Expr *t = bool_term();
    while (lookahead_ == Token::OR) {
        match(Token::OR);
        t = new BoolTerm(t, bool_term());
    }
    return t;
}

Expr *Parser::bool_term() {
    Expr *e = bool_factor();
    while (lookahead_ == Token::AND) {
        match(Token::AND);
        e = new BoolFactor(e, bool_factor());
    }
    return e;
}

Expr *Parser::bool_factor() {
    switch(lookahead_) {
        case Token::NOT: match(Token::NOT); return bool_factor();
        case Token::LPAREN: {
            match(Token::LPAREN);
            Expr *e = bool_();
            match(Token::RPAREN);
            return e;
        }
        default: return cmp();
    }
}

Expr *Parser::cmp() {
    Expr *e = expr();
    switch(lookahead_) {
        case Token::NE: match(Token::NE); return new CmpNEExpr(e, expr());
        case Token::LT: match(Token::LT); return new CmpLTExpr(e, expr());
        case Token::LE: match(Token::LE); return new CmpLEExpr(e, expr());
        case Token::GT: match(Token::GT); return new CmpGTExpr(e, expr());
        case Token::GE: match(Token::GE); return new CmpGEExpr(e, expr());
        case Token::EQ: match(Token::EQ); return new CmpEQExpr(e, expr());
        default: throw std::runtime_error("Expecting Expresion!");
    }
}
