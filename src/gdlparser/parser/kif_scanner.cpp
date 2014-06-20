/**
 * @file kif_scanner.cpp
 * @author Sumedh Ghaisas
 *
 * Implementation scanner class for Game Description Language.
 */
#include "kif_scanner.hpp"

#include "kif_driver.hpp"

using namespace gdlparser::parser::yy;
using namespace gdlparser::parser;

KIFParser::symbol_type KIFScanner::yylex()
{
    // if error has occured return END
    if(isError) return KIFParser::make_END(location_type());

    // if stream is NULL choose the first file to scan
    if(stream == NULL)
    {
        // if no file to scan
        if(files.size() == 0)
        {
            driver.Warn("No file provided...");
            return KIFParser::make_END(location_type());
        }
        // choose file file to scan
        else
        {
            stream = new std::ifstream(files[file_index].c_str());
            // if invalid file
            if(!stream->is_open())
            {
                driver.Error("Could not open file " + files[file_index]);
                isError = true;
                return KIFParser::make_END(location_type());
            }
        }
        // increment file index
        file_index++;
    }

    // collect value of the current token
    std::string currentTokenValue = "";

    // location of the current token
    location_type location;

    // if we already have next token to return
    if(state != HaveNextToken)
    {
        nextTokenValue = "";
        char c;
        while(true)
        {
            // get next character from stream
            c = stream->get();
            // check if the collected char if valid
            if(stream->good())
            {
                // if next line character then increment line number
                if(c == '\n')
                {
                    lineNo++;
                    charNo = 0;
                }
                // else increment character number
                else charNo++;

                // if next line occurs in comment mark that comment has ended
                if(c == '\n' && state == InComment) state = NoState;
                else if(c == '\n' && state == InCommentFromToken) state = InToken;
                // if we are in comment ignore the input
                else if(state == InComment || state == InCommentFromToken) continue;
                // if ';' change state to comment
                else if(c == ';' && state == InToken) state = InCommentFromToken;
                else if(c == ';') state = InComment;
                // ignore extra spaces, next lines and tabs outside token
                else if((c == '\t' || c == '\n' || c == ' ') && state == NoState) continue;
                // if space, next line or tab occurs in token mark the end of the token
                else if((c == ' ' || c == '\t' || c == '\n') && state == InToken)
                {
                    state = NoState;
                    break;
                }
                // mark clause command if '=' occurs after '<'
                else if(state == InClauseCommand && c == '=')
                {
                    state = NoState;
                    currentTokenValue += c;
                    break;
                }
                // if other than '=' comes after '<' mark it as a normal token
                else if(state == InClauseCommand)
                {
                    state = InToken;
                    currentTokenValue += c;
                }
                // if open or close bracket is encountered without token
                // return corresponding token
                else if((c == '(' || c == ')') && state == NoState)
                {
                    location = location_type(&files[file_index - 1], lineNo + 1, charNo);
                    currentTokenValue += c;
                    break;
                }
                // if open or close bracket is encountered while scanning token
                // return already scanned token and store corresponding bracket
                // to be returned next
                else if((c == '(' || c == ')') && state == InToken)
                {
                    state = HaveNextToken;
                    nextTokenValue += c;
                    break;
                }
                // if '<' mark it as a starting of clause command
                else if(c == '<' && state == NoState)
                {
                    location = location_type(&files[file_index - 1], lineNo + 1, charNo);
                    state = InClauseCommand;
                    currentTokenValue += c;
                }
                // for everything else append the character to current token value
                else
                {
                    if(state == NoState) location = location_type(&files[file_index - 1], lineNo + 1, charNo);
                    state = InToken;
                    currentTokenValue += c;
                }
            }
            // if character is invalid, stream no longer has any character left to return
            // initialize stream to next file
            else
            {
                location = location_type(&files[file_index - 1], lineNo + 1, charNo);
                location_type& loc = location;

                // if previous token is yet to be returned
                if(state == InToken)
                {
                    state = NoState;
                    break;
                }

                // check if we have any more files to scan
                if(file_index == files.size())
                {
                    // return end of input
                    return KIFParser::make_END(loc);
                }
                else
                {
                    // initialize stream to next file
                    stream->close();
                    stream->open(files[file_index].c_str());
                    if(!stream->is_open())
                    {
                        // invalid file
                        driver.Error("Could not open file " + files[file_index]);
                        isError = true;
                        return KIFParser::make_END(location_type());
                    }
                    file_index++;
                }
            }
        }
    }
    // if we already have a token to return
    else
    {
        location = location_type(&files[file_index - 1], lineNo + 1, charNo);
        currentTokenValue = nextTokenValue;
        nextTokenValue = "";
        state = NoState;
    }

    // location of the current token
    location_type& loc = location;

    // return appropriate type of token depending on token value
    if(currentTokenValue == "role") return KIFParser::make_role(currentTokenValue, loc);
    else if(currentTokenValue == "base") return KIFParser::make_base(currentTokenValue, loc);
    else if(currentTokenValue == "input") return KIFParser::make_input(currentTokenValue, loc);
    else if(currentTokenValue == "init") return KIFParser::make_init(currentTokenValue, loc);
    else if(currentTokenValue == "legal") return KIFParser::make_legal(currentTokenValue, loc);
    else if(currentTokenValue == "next") return KIFParser::make_next(currentTokenValue, loc);
    else if(currentTokenValue == "goal") return KIFParser::make_goal(currentTokenValue, loc);
    else if(currentTokenValue == "terminal") return KIFParser::make_terminal(currentTokenValue, loc);
    else if(currentTokenValue == "does") return KIFParser::make_does(currentTokenValue, loc);
    else if(currentTokenValue == "true") return KIFParser::make_ttrue(currentTokenValue, loc);
    else if(currentTokenValue == "not") return KIFParser::make_nnot(currentTokenValue, loc);
    else if(currentTokenValue == "or") return KIFParser::make_oor(currentTokenValue, loc);
    else if(currentTokenValue == "distinct") return KIFParser::make_distinct(currentTokenValue, loc);
    else if(currentTokenValue == "(") return KIFParser::make_Obracket(currentTokenValue, loc);
    else if(currentTokenValue == ")") return KIFParser::make_Cbracket(currentTokenValue, loc);
    else if(currentTokenValue == "<=") return KIFParser::make_clause_command(currentTokenValue, loc);
    else if(currentTokenValue[0] == '?') return KIFParser::make_var(currentTokenValue, loc);
    else
    {
        // return token num if all the characters in the token value are digits
        bool has_only_digits = true;
        for (size_t i = 0; i < currentTokenValue.length(); i++)
        {
            if (!isdigit(currentTokenValue[i]))
            {
                has_only_digits = false;
                break;
            }
        }

        if(has_only_digits) return KIFParser::make_num(currentTokenValue, loc);
        else return KIFParser::make_id(currentTokenValue, loc);
    }
}
