#include "cgi.hpp"
#include "stdlib.h"
#include "limits.h"
#include <sstream>
#include <cstdlib>
#include <fcntl.h>

extern char **environ;

ws::cgi::cgi(std::pair<std::string, std::string> conf, server_config serv, std::string file, request req) : _file(file), _program(conf.second)
{
	// this->_cmv.content_length = file.length();
	this->_cmv.gateway_interface = "CGI/1.1";
	this->_cmv.path_info = file;
	this->_cmv.query_string = "";
	this->_cmv.remote_addr = req.get_host();
	this->_cmv.request_method = req.get_start_line().method;
	this->_cmv.script_name = conf.second;
	this->_cmv.server_name = serv.server_name;
	this->_cmv.server_port = serv.listen;
	this->_cmv.server_protocol = req.get_start_line().http_version;
	this->_cmv.server_software = "webserv";
}

ws::cgi::~cgi(void) {}

char	**ws::cgi::fusion_env_with_vars(char **vars) const
{
	char	**ret = new char*[ws::matrix_length(vars) + ws::matrix_length(environ) + 1];
	int		i = 0;

	for (int j = 0; environ[j]; ++j)
		ret[i++] = strdup(environ[j]);

	for (int j = 0; vars[j]; ++j)
		ret[i++] = strdup(vars[j]);
	ret[i] = NULL;

	delete[] vars;
	return ret;
}

char	**ws::cgi::set_vars_into_env(void) const
{
	std::string	str;
	char		**env = new char*[12];
	char		tmp[FILENAME_MAX];

	getcwd(tmp, FILENAME_MAX);

	env[0] = strdup("CONTENT_TYPE=text");
	env[1] = strdup("GATEWAY_INTERFACE=CGI/1.1");
	str = "PATH_INFO=" + std::string(tmp) + "/"  + this->_file;
	env[2] = strdup(str.c_str());
	str = "REMOTE_ADDR=" + this->_cmv.remote_addr;
	env[3] = strdup(str.c_str());
	str = "REQUEST_METHOD=" + this->_cmv.request_method;
	env[4] = strdup(str.c_str());
	str = "SCRIPT_FILENAME=" + std::string(tmp) + "/" + this->_cmv.script_name;
	env[5] = strdup(str.c_str());
	str = "SERVER_NAME=" + this->_cmv.server_name;
	env[6] = strdup(str.c_str());
	str = "SERVER_PORT=" + std::to_string(this->_cmv.server_port);
	env[7] = strdup(str.c_str());
	str = "SERVER_PROTOCOL=" + this->_cmv.server_protocol;
	env[8] = strdup(str.c_str());
	str = "SERVER_SOFTWARE=" + this->_cmv.server_software;
	env[9] = strdup(str.c_str());
	env[10] = strdup("REDIRECT_STATUS=200");
	env[11] = NULL;
	return env;
}

ws::response	ws::cgi::response_from_cgi(char *cgi_text)
{
	ws::response	res;
	std::string		text(cgi_text);
	std::vector<std::string>	splitted_text = ws::ft_split(text, "\r\n");
	std::map<std::string, std::string>	headers;

	std::vector<std::string>::iterator it = splitted_text.begin();
	while (*it != "")
	{
		std::vector<std::string>	split = ws::ft_split(*it, ": ");
		headers[split[0]] = split[1];
		++it;
	}
	res.set_headers(headers);
	std::string	body;
	while (it != splitted_text.end())
	{
		body += *it;
		++it;
	}
	res.set_body(body);
	res.set_status_line((status_line){"HTTP/1.1", "OK", 200});
	return res;
}

std::string	ws::cgi::create_response(void)
{
	char		**env = set_vars_into_env();
	char		tmp[FILENAME_MAX];
	char		aux[4096];
	int			pid;
	int			pipe_fd[2];
	response	res;

	bzero(aux, sizeof(aux));

	pipe(pipe_fd);

	env = fusion_env_with_vars(env);

	getcwd(tmp, FILENAME_MAX);

	std::string	pwd(tmp);
	std::string	abs_path = pwd + "/" + this->_program;
	this->_file = pwd + "/" + this->_file;

	for (int i = 0; env[i]; ++i)
		std::cout << " -> " << env[i] << std::endl;

	if ((pid = fork()) == -1)
		perror("fork error");
	else if (pid == 0)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[0]);
		close(pipe_fd[1]);


		// 							Failing when incluiding META VARIABLES ----->
		if (execle(abs_path.c_str(), abs_path.c_str(), this->_file.c_str(), NULL, environ) < 0)
			perror("execle");
	}
	close(pipe_fd[1]);
	read(pipe_fd[0], aux, sizeof(aux));

	res = response_from_cgi(aux);

	return res.response_to_text();
}
