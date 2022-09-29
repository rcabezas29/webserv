#include "autoindex.hpp"
#include "util.h"


std::string path_format(std::string path)
{
	for (size_t n = 0; n < path.size(); n++){
		if (path[n] == '.' && path != "..")
			return "";
	}
	return "/";
}

std::map<std::string, std::string> get_dir_paths(std::string dir_path)
{

	std::map<std::string, std::string> paths;

	DIR *dir; struct dirent *diread;
    std::vector<std::string> files;


    if ((dir = opendir(dir_path.c_str())) != nullptr) 
    {
        while ((diread = readdir(dir)) != nullptr) 
        {
            files.push_back(diread->d_name);
        }
        closedir (dir);
    } 
    else 
    {
        std::cout <<  "ERROR FILE PATH AUTOINDEX" << std::endl;
    }

    std::vector<std::string>::iterator it = files.begin();
	while (it != files.end())
	{
		if (*it != "." ) // si que hay que mostrarlos
			paths.insert(std::make_pair(*it, (dir_path + "/" + *it)));
		++it;
	}

	return paths;
}

std::string remove_first_path(std::string path){
	std::string out_path = "";
	int position = 0;
	for (size_t n = 0; path[n] != '/' && n < path.size(); ++n){
		position++;
	}
	for (size_t n = position + 1; n < path.size(); n++){
		out_path = out_path + path[n];
	}
	return out_path;
}

std::string create_file(std::map<std::string, std::string> dir_path)
{
	// <a href="url">link text</a>
	// </body>
	// </html>
	std::ofstream file_out;
	std::fstream template_auto;

	std::string file_path;
	file_path = "tmp/autoindex.html"; //add absolute path
    
	file_out.open(file_path, std::ios::out);
	template_auto.open("tmp/auto_template.html", std::ifstream::in);
	
	std::string		line;

	while (std::getline(template_auto, line))
	{
		file_out << line << std::endl;
	}
	for (std::map<std::string, std::string>::const_iterator it_m = dir_path.begin(); it_m != dir_path.end(); ++it_m)
	{
		file_out << "    <p><a href=\"/" + remove_first_path(it_m->second) + "\">" + it_m->first + path_format(it_m->first) + "</a></p>" << std::endl;
	}
	file_out << "</body>" << std::endl;
	file_out << "</html>" << std::endl;

	// std::cout << "Si que cierra" << std::endl;
	file_out.close();
	return file_path;

}

std::string create_autoindex(std::string dir_path) {
	
	std::map<std::string, std::string> paths;
	
	paths = get_dir_paths(dir_path);

	return create_file(paths);;
}