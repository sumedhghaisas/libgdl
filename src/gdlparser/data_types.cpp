/**
 * @file data_types.cpp
 * @author Sumedh Ghaisas
 *
 * Implementation of data types used by gdlparser and gdlreasoner.
 */
#include "data_types.hpp"

#include <stack>
#include <sstream>

using namespace gdlparser;

size_t Argument::copy_calls = 0;

Argument::Argument(const TokenValue& tok)
{
    // copy information from token
    if(tok.Type() == TokenValue::Relation) t = Relation;
    else if(tok.Type() == TokenValue::Function) t = Function;
    else t = Var;

    // assign argument command
    val = tok.Command();

    // get arguments from the token
    const std::vector<TokenValue>& args = tok.Arguments();

    // add them as arguments
    for(size_t i = 0;i < args.size();i++) AddArgument(args[i]);


}

Argument::Argument(const Argument& arg)
{
  copy_calls++;

    if(arg.IsVariable())
    {
        t = arg.t;
        val = arg.val;
        return;
    }

    // map to hold variable name versus assigned location mapping
    // this is important as all the occurrences of a variable in a clause
    // are assigned same object
    std::map<std::string, Argument*> v_map;

    t = arg.t;
    val = arg.val;

    // call recursively on arguments
    for(size_t i = 0;i < arg.args.size();i++)
        args.push_back(ConstructArgument(*arg.args[i], v_map));
}

Argument::Argument(const std::string& str)
{
    std::map<std::string, Argument*> v_map;

    if(str[0] != '(')
    {
        if(str[0] == '?') t = Argument::Var;
        else t = Argument::Function;
        val = str;
        return;
    }

    std::string cmd;
    StringVec args;
    if(!SeparateCommand(str, cmd, args))
    {
        std::cerr << "Unable to construct argument from " << str << std::endl;
        return;
    }

    val = cmd;
    t = Argument::Function;

    for(size_t i = 0;i < args.size();i++)
    {
        this->args.push_back(ConstructArgument(args[i], v_map));
    }
}

bool Argument::SeparateCommand (const std::string & input, std::string & cmd, std::vector <std::string> & args)
{

    if (input.size() < 3 || input[0] != '(' || input[input.length()-1] != ')')
    {
        std::cerr << "Input " << input << " is not surrounded by braces or not big enough" << std::endl;
        return false;
    }
    const std::string withoutBraces = input.substr (1, input.length() - 2);
    size_t sep = withoutBraces.find(' ');
    if (sep == withoutBraces.npos)
    {
        // no arguments in string
        cmd = withoutBraces;
        return true;
    }
    cmd = withoutBraces.substr (0, sep);

    sep++;
    int depth = 0;
    // parsing parameters
    std::ostringstream* o = new std::ostringstream();
    while (sep < withoutBraces.length())
    {
        char c = withoutBraces[sep];
        if (c == '(')
        {
            depth++;
            *o << c;
        }
        else if (c == ')')
        {
            depth--;
            *o << c;
        }
        else if ((c == ' ') && (depth == 0))
        {
            args.push_back (o->str());
            delete o;
            o = new std::ostringstream();
        }
        else *o << c;
        sep++;
    }
    if (depth != 0)
    {
        std::cerr << "Braces count mismatch in " << withoutBraces << std::endl;
        delete o;
        return false;
    }
    if (o->str() != "") args.push_back (o->str());
    delete o;
    return true;
}

Argument::~Argument()
{
    std::set<Argument*> v_set;
    // destroy arguments and clear arguments vector before deleting the object
    Destroy(v_set);
}

void Argument::Destroy(std::set<Argument*>& v_set)
{
    // if variable add it to deleted variable set
    if(t == Argument::Var && v_set.find(this) == v_set.end())
    {
        v_set.insert(this);
        return;
    }
    else if(t != Argument::Var)
    {
        for(size_t i = 0;i < args.size();i++)
        {
            // only delete argument which have not been deleted yet
            if(v_set.find(args[i]) == v_set.end())
            {
                args[i]->Destroy(v_set);
                delete args[i];
            }
        }
        args.clear();
    }
}

Argument* Argument::ConstructArgument(const Argument& arg, std::map<std::string, Argument*>& v_map)
{
    std::map<std::string, Argument*>::iterator it;
    if(arg.IsVariable() && (it = v_map.find(arg.val)) != v_map.end())
        return it->second;
    else if(arg.IsVariable())
    {
        Argument* out = new Argument();
        out->t = Argument::Var;
        out->val = arg.val;
        v_map[out->val] = out;
        return out;
    }

    Argument* out = new Argument();
    out->t = arg.t;
    out->val = arg.val;

    for(size_t i = 0;i < arg.args.size();i++)
        out->args.push_back(ConstructArgument(*arg.args[i], v_map));
    return out;
}

Argument* Argument::ConstructArgument(const TokenValue& tok, std::map<std::string, Argument*>& v_map)
{
    std::map<std::string, Argument*>::iterator it;
    if(tok.Type() == TokenValue::Var && (it = v_map.find(tok.Command())) != v_map.end())
        return it->second;
    else if(tok.Type() == TokenValue::Var)
    {
        Argument *out = new Argument(tok);
        v_map[tok.Command()] = out;
        return out;
    }

    Argument *out = new Argument();

    if(tok.Type() == TokenValue::Relation) out->t = Argument::Relation;
    else if(tok.Type() == TokenValue::Function) out->t = Argument::Function;
    else out->t = Argument::Var;

    out->val = tok.Command();

    const std::vector<TokenValue>& args = tok.Arguments();

    for(size_t i = 0;i < args.size();i++) out->args.push_back(ConstructArgument(args[i], v_map));

    return out;
}

Argument* Argument::ConstructArgument(const std::string& str, std::map<std::string, Argument*>& v_map)
{
    if(str[0] != '(')
    {
        std::map<std::string, Argument*>::iterator it;
        if(str[0] == '?' && (it = v_map.find(str)) != v_map.end()) return it->second;
        else
        {
            Argument* out = new Argument(str);
            if(str[0] == '?') v_map[str] = out;
            return out;
        }
    }

    Argument* out = new Argument();

    std::string cmd;
    StringVec args;
    if(!SeparateCommand(str, cmd, args))
    {
        std::cerr << "Unable to construct argument from " << str << std::endl;
        return NULL;
    }

    out->val = cmd;
    out->t = Argument::Function;

    for(size_t i = 0;i < args.size();i++)
    {
        out->args.push_back(ConstructArgument(args[i], v_map));
    }
    return out;
}

bool Argument::operator==(const Argument& arg) const
{
    if(val != arg.val) return false;
    if(args.size() != arg.args.size()) return false;

    for(size_t i = 0;i < args.size();i++)
        if(*args[i] != *arg.args[i]) return false;

    return true;
}

Argument& Argument::operator=(Argument arg)
{
    swap(*this, arg);
    return *this;
}

bool Argument::OrEquate(const Argument& arg)
{
    // for 'or', if arg matches any argument return true
    if(val == "or")
    {
        for(size_t i = 0;i < args.size();i++)
            if(args[i]->OrEquate(arg)) return true;
        return false;
    }

    if(val != arg.val) return false;
    if(args.size() != arg.args.size()) return false;

    for(size_t i = 0;i < args.size();i++)
        if(!args[i]->OrEquate(*arg.args[i])) return false;

    return true;
}

bool Argument::HasAsArgument(const Argument& arg) const
{
    if(val == "not") return false;
    else if(val == "or")
    {
        bool has = true;
        for(size_t i = 0;i < args.size();i++)
            if(!args[i]->HasAsArgument(arg))
            {
                has = false;
                break;
            }

        return has;
    }

    for(size_t i = 0;i < args.size();i++)
        if(*(args[i]) == arg) return true;

    return false;
}

bool Argument::IsGround() const
{
    std::stack<const Argument*> S;
    S.push(this);

    while(!S.empty())
    {
        const Argument* temp = S.top();
        S.pop();

        if(temp->IsVariable()) return false;
        else for(size_t i = 0;i < temp->args.size();i++)
            S.push(temp->args[i]);
    }
    return true;
}

bool Argument::IsEqualTo(const Argument& arg) const
{
    if(val != arg.val) return false;
    if(args.size() != arg.args.size()) return false;

    for(size_t i = 0;i < args.size();i++)
        if(args[i] != arg.args[i]) return false;

    return true;
}

Fact::Fact(const std::string& str) : isLocation(false)
{
    if(str.find("?") != std::string::npos)
    {
        std::cerr << "Unable to construct argument from " << str << std::endl;
        return;
    }

    arg = Argument(str);
}

Clause::Clause(const TokenValue& tok, const size_t id) : id(id), isLocation(false)
{
    const std::vector<TokenValue>& args = tok.Arguments();

    std::map<std::string, Argument*> v_map;

    head = Argument::ConstructArgument(args[0], v_map);

    for(size_t i = 1;i < args.size();i++) premisses.push_back(Argument::ConstructArgument(args[i], v_map));
}

Clause::Clause(const std::string& str) : isLocation(false)
{
    std::map<std::string, Argument*> v_map;

    std::string cmd;
    std::vector<std::string> args;
    if(!Argument::SeparateCommand(str, cmd, args) || cmd != "<=" || args.size() < 2)
    {
        std::cerr << "Unable to construct clause from " << str << std::endl;
        return;
    }

    head = Argument::ConstructArgument(args[0], v_map);

    for(size_t i = 1;i < args.size();i++)
        premisses.push_back(Argument::ConstructArgument(args[i], v_map));
}

Clause::Clause(const Clause& c)
{
    std::map<std::string, Argument*> v_map;

    id = c.id;

    isLocation = c.isLocation;
    loc = c.loc;

    head = Argument::ConstructArgument(*c.head, v_map);

    for(size_t i = 0;i < c.premisses.size();i++)
        premisses.push_back(Argument::ConstructArgument(*c.premisses[i], v_map));
}

Clause& Clause::operator=(const Clause& c)
{
    premisses.clear();

    std::map<std::string, Argument*> v_map;

    id = c.id;

    isLocation = c.isLocation;
    loc = c.loc;

    head = Argument::ConstructArgument(*c.head, v_map);

    for(size_t i = 0;i < c.premisses.size();i++)
        premisses.push_back(Argument::ConstructArgument(*c.premisses[i], v_map));

    return *this;
}

Clause::~Clause()
{
    // to hold the set of variables already deleted
    std::set<Argument*> v_set;

    // delete head if it is not NULL
    if(head != NULL)
    {
        head->Destroy(v_set);
        delete head;
    }

    for(size_t i = 0;i < premisses.size();i++)
    {
        premisses[i]->Destroy(v_set);
        delete premisses[i];
    }
}

bool Clause::IsGround()
{
    if(!(head->IsGround())) return false;

    for(size_t i = 0;i < premisses.size();i++)
        if(!(premisses[i]->IsGround())) return false;

    return true;
}

std::ostream& operator<<(std::ostream& o, const Argument::Type& t)
{
    if(t == Argument::Relation) o << "Relation";
    else if(t == Argument::Function) o << "Function";
    else o << "Variable";
    return o;
}

std::ostream& operator<<(std::ostream& o,const Argument& arg)
{
    if(arg.args.size() == 0)
    {
        o << arg.val;
        return o;
    }
    else o << "( " + arg.val;

    for(size_t i = 0;i < arg.args.size();i++) o << " " << *(arg.args[i]);
    o << " " << ")";
    return o;
}

std::ostream& operator<<(std::ostream& o, const Location& loc)
{
    o << loc.lineNo << " \"" << loc.filename << "\"";
    return o;
}

std::ostream& operator<<(std::ostream& o, const Fact& f)
{
    o << f.arg;
    return o;
}

std::ostream& operator<<(std::ostream& o, const Clause& clause)
{
    o << "(<= ";
    o << *clause.head << " ";
    for(size_t i = 0;i < clause.premisses.size();i++)
        o << *clause.premisses[i] << " ";
    o << ")";
    return o;
}
