#include "utils.hpp"

bool is_include(std::string line)
{
    boost::regex pat("#include.*");

    return boost::regex_match(line, pat, boost::match_default);
}