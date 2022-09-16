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

bool						ws::is_error_code(short code) {
	if (code == 400 || code == 403 || code == 404)
		return true;
	else
		return false;
}

std::string					ws::generate_reason_phrase(short code) {
	if (code == 200)
		return "OK";
	else if (code == 400)
		return "Bad Request";
	else if (code == 403)
		return "Forbidden";
	else // 404
		return "Not Found";
}

std::string					ws::file_to_text(std::fstream &file) {
	std::string	line;
	std::string	text;

	while (std::getline(file, line))
		text += line + "\n";
	return text;
}

bool						ws::check_if_dir(std::string path) { return (opendir(path.c_str())); }

int							ws::matrix_length(char **m) {
	int	i = 0;
	while (m[i])
		i++;
	return i;
}