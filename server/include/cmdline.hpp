#ifndef __CMDLINE_HPP__
#define __CMDLINE_HPP__
#include <string>
#include <memory>
#include <vector>

namespace ZDC
{

    // class CmdNamedArgTemplate {
    //     friend class CmdLine;
    // public:
    //     // A vector of aliases that the named argument can be declared as, requires at least one argument.
    //     std::vector<std::string> vectAliases;
    //     // A helpful description of the named argument.
    //     std::string strDescription;
    //     // True if the named argument should have a value.
    //     bool bHasValue;
    //     // True if the named argument is a singleton and can only have one value.
    //     bool bSingleton;

    // protected:
    //     std::vector<std::shared_ptr<CmdNamedArg>> vectValues;

    // public:
    //     // A constant reference to the value('s) of the template.
    //     inline const std::vector<std::shared_ptr<CmdNamedArg>>& values() const { return vectValues; };

    // public:
    //     CmdNamedArgTemplate(const std::vector<std::string>& vectAliases, const char* strDescription, bool bIsFlag=false) :
    //         vectAliases(vectAliases), strDescription(strDescription), bHasValue(!bIsFlag)
    //         { assert(vectAliases.size() >= 1 && "CmdNamedArgTemplate: requires at least one alias per template!"); };
    //     ~CmdNamedArgTemplate() = default;
    // };

    // class CmdNamedArg {
    // public:
    //     // A pointer to the template for this named argument.
    //     std::weak_ptr<CmdNamedArgTemplate> ptrTemplate;
    //     // A pointer to the alias used to declare this named argument.
    //     const char* strAlias;
    //     // A pointer to the value of this named argument.
    //     const char* strValue;
    // };

    class CmdLine {
    private:
        // The number of arguments in this command line.
        const int argc;
        // A pointer to the array of command line arguments.
        const char** const argv;

    // private:
    //     // A vector of named argument templates.
    //     std::vector<std::shared_ptr<CmdNamedArgTemplate>> vectNamedArgTemplates;
    //     // A vector of named arguments.
    //     std::vector<std::shared_ptr<CmdNamedArg>> vectNamedArgs;
    //     // A vector of indexed arguments.
    //     std::vector<const char*> vectIndexedArgs;

    public:
        explicit CmdLine(int argc, const char** argv);

    public:
        // The path to this executable, or nullptr if the command line is corrupted/invalid.
        inline const char* executable() const { return argv[0]; };
        // Gets the argument at a specific index, will return nullptr if invalid index or command line.
        inline const char* arg(int index) const { return (index < argc ? argv[index] : nullptr); };
        // // A constant referance to the named argument templates.
        // inline const std::vector<std::shared_ptr<CmdNamedArgTemplate>>& namedTemplates() const { return vectNamedArgTemplates; };
        // // A constant referance to the named arguments.
        // inline const std::vector<std::shared_ptr<CmdNamedArg>>& named() const { return vectNamedArgs; };
        // // A constant referance to the indexed arguments.
        // inline const std::vector<const char*>& indexed() const { return vectIndexedArgs; };
    };

} // namespace ZDC
#endif//__CMDLINE_HPP__ GUARD