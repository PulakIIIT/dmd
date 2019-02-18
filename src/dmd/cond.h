
/* Compiler implementation of the D programming language
 * Copyright (C) 1999-2019 by The D Language Foundation, All Rights Reserved
 * written by Walter Bright
 * http://www.digitalmars.com
 * Distributed under the Boost Software License, Version 1.0.
 * http://www.boost.org/LICENSE_1_0.txt
 * https://github.com/dlang/dmd/blob/master/src/dmd/cond.h
 */

#pragma once

#include "ast_node.h"
#include "globals.h"
#include "visitor.h"

class Expression;
class Identifier;
class Module;
struct Scope;
class DebugCondition;
class ForeachStatement;
class ForeachRangeStatement;

int findCondition(Strings *ids, Identifier *ident);

enum Include
{
    INCLUDEnotComputed, /// not computed yet
    INCLUDEyes,         /// include the conditional code
    INCLUDEno           /// do not include the conditional code
};

class Condition : public ASTNode
{
public:
    Loc loc;
    Include inc;

    virtual Condition *syntaxCopy() = 0;
    virtual int include(Scope *sc) = 0;
    virtual DebugCondition *isDebugCondition() { return NULL; }
    void accept(Visitor *v) { v->visit(this); }
};

class StaticForeach
{
public:
    Loc loc;

    ForeachStatement *aggrfe;
    ForeachRangeStatement *rangefe;

    bool needExpansion;

    StaticForeach *syntaxCopy();
};

class DVCondition : public Condition
{
public:
    unsigned level;
    Identifier *ident;
    Module *mod;

    Condition *syntaxCopy();
    void accept(Visitor *v) { v->visit(this); }
};

class DebugCondition : public DVCondition
{
public:
    static void addGlobalIdent(const char *ident);

    int include(Scope *sc);
    DebugCondition *isDebugCondition() { return this; }
    void accept(Visitor *v) { v->visit(this); }
};

class VersionCondition : public DVCondition
{
public:
    static void addGlobalIdent(const char *ident);
    static void addPredefinedGlobalIdent(const char *ident);

    int include(Scope *sc);
    void accept(Visitor *v) { v->visit(this); }
};

class StaticIfCondition : public Condition
{
public:
    Expression *exp;
    int nest;         // limit circular dependencies

    Condition *syntaxCopy();
    int include(Scope *sc);
    void accept(Visitor *v) { v->visit(this); }
};
