#include "utils.hpp"

std::vector<std::string>	ws::ft_split(const std::string &s, std::string seperator)
{
	std::vector<std::string>	output;
	std::string::size_type		prev_pos = 0, pos = 0;

	while (s.substr(pos, seperator.length()) == seperator)
		++pos, ++prev_pos;
	while ((pos = s.find(seperator, pos)) != std::string::npos)
	{
		std::string	substring(s.substr(prev_pos, pos - prev_pos));
		output.push_back(substring);
		prev_pos = pos += seperator.size();
	}
	output.push_back(s.substr(prev_pos, pos - prev_pos));
	return output;
}
