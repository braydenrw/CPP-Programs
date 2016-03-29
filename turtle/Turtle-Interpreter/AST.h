#ifndef AST_H
#define AST_H

#include <string>
#include <iostream>
#include <vector>
#include <iomanip>
#include "Env.h"

//
// Abstract base class for all expressions.
//
class Expr {
public:
    virtual ~Expr() {}
    virtual float eval(Env& env) const = 0;
};

//
// Abstract base class for all statements.
//
class Stmt {
public:
    virtual ~Stmt() {};
    virtual void execute(Env& env) = 0;
};

//
// AST's expressions and statements go here.
//
class AssignStmt : public Stmt {
protected:
    const std::string _name;  //l-value
    Expr *_expr; // r-value
public:
    AssignStmt(const std::string& n, Expr *e) : _name{n}, _expr{e} {}
    virtual void execute(Env& env) {
        env.put(_name, _expr->eval(env));
    }
    ~AssignStmt() {delete _expr;}
};

class WhileStmt : public Stmt {
protected:
    Expr *_expr;
    Stmt *_stmt;
public:
    WhileStmt(Expr *cond, Stmt *body) : _expr{cond}, _stmt{body} {}
    virtual void execute(Env &env) {
        while(_expr->eval(env)) {
            _stmt->execute(env);
        }
    }
    ~WhileStmt() {delete _expr; delete _stmt;}
};

class IfStmt : public Stmt {
protected:
    Expr *_cond;
    Stmt *_body;
    Stmt *_else_body;
public:
    IfStmt(Expr *cond, Stmt *body, Stmt *else_body) :
            _cond{cond}, _body{body}, _else_body{else_body} {}
    virtual void execute(Env &env) {
        if(_cond->eval(env)) {
            _body->execute(env);
        } else if(_else_body != NULL) {
            _else_body->execute(env);
        }
    }
    ~IfStmt() {delete _cond; delete _body; delete _else_body;}
};

class BlockStmt : public Stmt {
protected:
    std::vector<Stmt*> _stmts;
    Stmt *_s;
public:
    BlockStmt(std::vector<Stmt*> stmts) : _stmts{stmts} {}
    virtual void execute(Env &env) {
        std::vector<Stmt*>::iterator itr;
        for(itr = _stmts.begin(); itr != _stmts.end(); ++itr) {
            _s = *itr;
            _s->execute(env);
        }
    }
    ~BlockStmt() {delete _s;}
};

class HomeStmt : public Stmt {
public:
    virtual void execute(Env& env) {
        std::cout << "H" << std::endl;
    }
};

class PenUpStmt : public Stmt {
public:
    virtual void execute(Env& env) {
        std::cout << "U" << std::endl;
    }
};

class PenDownStmt : public Stmt {
public:
    virtual void execute(Env& env) {
        std::cout << "D" << std::endl;
    }
};

class PushStateStmt : public Stmt {
public:
    virtual void execute(Env& env) {
        std::cout << "[" << std::endl;
    }
};

class PopStateStmt : public Stmt {
public:
    virtual void execute(Env& env) {
        std::cout << "]" << std::endl;
    }
};

class ForwardStmt : public Stmt {
protected:
    Expr *_dist;
public:
    ForwardStmt(Expr *e) : _dist{e} {}
    virtual void execute(Env& env) {
        const float d = _dist->eval(env);
        std::cout << "M " << d << std::endl;
    }
    ~ForwardStmt() {delete _dist;}
};

class RightStmt : public Stmt {
protected:
    Expr *_angle;
public:
    RightStmt(Expr *e) : _angle{e} {}
    virtual void execute(Env& env) {
        const float a = _angle->eval(env);
        std::cout << "R " << -a << std::endl;
    }
    ~RightStmt() {delete _angle;}
};

class LeftStmt : public Stmt {
protected:
    Expr *_angle;
public:
    LeftStmt(Expr *e) : _angle{e} {}
    virtual void execute(Env& env) {
        const float a = _angle->eval(env);
        std::cout << "R " << a << std::endl;
    }
    ~LeftStmt() {delete _angle;}
};

class VarExpr : public Expr {
protected:
    const std::string _name;
public:
    VarExpr(const std::string& n) : _name{n} {}
    virtual float eval(Env& env) const {
        return env.get(_name);
    }
};

class ConstExpr : public Expr {
protected:
    const float _val;
public:
    ConstExpr(float v) : _val{v} {}
    virtual float eval(Env& env) const {
        return _val;
    }
};

class UnaryExpr : public Expr {
protected:
    Expr *_expr;
public:
    UnaryExpr(Expr *e) : _expr{e} {}
    ~UnaryExpr() {delete _expr;}
};

class NegExpr : public UnaryExpr {
public:
    NegExpr(Expr *e) : UnaryExpr(e) {}
    virtual float eval(Env& env) const {
        return -_expr->eval(env);
    }
};

class BinaryExpr : public Expr {
protected:
    Expr *_left, *_right;
public:
    BinaryExpr(Expr *l, Expr *r) : _left{l}, _right{r} {}
    ~BinaryExpr() {delete _left; delete _right;}
};

class AddExpr : public BinaryExpr {
public:
    AddExpr(Expr *l, Expr *r) : BinaryExpr(l,r) {}
    virtual float eval(Env& env) const {
        return _left->eval(env) + _right->eval(env);
    }
};

class SubExpr : public BinaryExpr {
public:
    SubExpr(Expr *l, Expr *r) : BinaryExpr(l,r) {}
    virtual float eval(Env& env) const {
        return _left->eval(env) - _right->eval(env);
    }
};

class MulExpr : public BinaryExpr {
public:
    MulExpr(Expr *l, Expr *r) : BinaryExpr(l,r) {}
    virtual float eval(Env& env) const {
        return _left->eval(env) * _right->eval(env);
    }
};

class DivExpr : public BinaryExpr {
public:
    DivExpr(Expr *l, Expr *r) : BinaryExpr(l,r) {}
    virtual float eval(Env& env) const {
        return _left->eval(env) / _right->eval(env);
    }
};

class BoolTerm : public BinaryExpr {
public:
    BoolTerm(Expr *l, Expr *r) : BinaryExpr(l,r) {}
    virtual float eval(Env &env) const {
        return _left->eval(env) || _right->eval(env);
    }
};

class BoolFactor : public BinaryExpr {
public:
    BoolFactor(Expr *l, Expr *r) : BinaryExpr(l,r) {}
    virtual float eval(Env &env) const {
        return _left->eval(env) && _right->eval(env);
    }
};

class CmpNEExpr : public BinaryExpr {
public:
    CmpNEExpr(Expr *l, Expr *r) : BinaryExpr(l,r) {}
    virtual float eval(Env &env) const {
        return _left->eval(env) != _right->eval(env);
    }
};

class CmpLTExpr : public BinaryExpr {
public:
    CmpLTExpr(Expr *l, Expr *r) : BinaryExpr(l,r) {}
    virtual float eval(Env &env) const {
        return _left->eval(env) < _right->eval(env);
    }
};

class CmpLEExpr : public BinaryExpr {
public:
    CmpLEExpr(Expr *l, Expr *r) : BinaryExpr(l,r) {}
    virtual float eval(Env &env) const {
        return _left->eval(env) <= _right->eval(env);
    }
};

class CmpGTExpr : public BinaryExpr {
public:
    CmpGTExpr(Expr *l, Expr *r) : BinaryExpr(l,r) {}
    virtual float eval(Env &env) const {
        return _left->eval(env) > _right->eval(env);
    }
};

class CmpGEExpr : public BinaryExpr {
public:
    CmpGEExpr(Expr *l, Expr *r) : BinaryExpr(l,r) {}
    virtual float eval(Env &env) const {
        return _left->eval(env) >= _right->eval(env);
    }
};

class CmpEQExpr : public BinaryExpr {
public:
    CmpEQExpr(Expr *l, Expr *r) : BinaryExpr(l,r) {}
    virtual float eval(Env &env) const {
        return _left->eval(env) == _right->eval(env);
    }
};

#endif //AST_H
