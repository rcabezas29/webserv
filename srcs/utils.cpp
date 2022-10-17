#include "utils.hpp"

std::vector<std::string>	ws::ft_split(const std::string &s, std::string seperator)
{
	std::vector<std::string>	output;
	std::string::size_type		prev_pos = 0, pos = 0;

	while (s.substr(pos, seperator.length()) == seperator)
		pos += seperator.length(), prev_pos += seperator.length();
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

std::string					ws::generate_reason_phrase(short code)
{
	if (code == 400)
		return "Bad Request";
	else if (code == 403)
		return "Forbidden";
	else if (code == 404)
		return "Not Found";
	else if (code == 405)
		return "Method Not Allowed";
	else if (code == 413)
		return "Payload Too Large";
	else if (code == 429)
		return "Method Not Allowed";
	else if (code == 501)
		return "Not Implemented";
	else // 500
		return "Internal Server Error";

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

bool ws::map_value_exists(std::map<std::string, std::string> m, std::string first_value, std::string second_value) 
{
	std::map<std::string, std::string>::iterator val = m.find(first_value);
	if (val != m.end())
	{
		if (std::strncmp(val->second.c_str(), second_value.c_str(), second_value.size()) == 0)
			return true;
		else
			return false;
	}
	return false;
}

void	ws::remove_fd_from_pollfd(std::vector<struct pollfd> *pfds, int fd)
{
	for (std::vector<struct pollfd>::iterator it = pfds->begin(); it != pfds->end(); ++it)
	{
		if (it->fd == fd)
		{
			pfds->erase(it);
			return ;
		}
	}
}
