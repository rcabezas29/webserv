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
		if (*it != "." )
			paths.insert(std::make_pair(*it, (dir_path + "/" + *it)));
		++it;
	}

	return paths;
}


std::string remove_first_path(std::string path){
	std::string out_path = "";
	std::string uncleaned_path = "";
	int position = 0;
	for (size_t n = 0; path[n] != '/' && n < path.size(); ++n){
		position++;
	}
	for (size_t n = position + 1; n < path.size(); n++){
		uncleaned_path = uncleaned_path + path[n];
	}
	for (size_t n = 0; n < uncleaned_path.size(); n++)
	{
		out_path = out_path + uncleaned_path[n];
		if (uncleaned_path[n] == '/')
		{
			for (; n < uncleaned_path.size() && uncleaned_path[n] == '/'; n++){}
			n--;
		}
	}

	return out_path;
}

std::string create_file(std::map<std::string, std::string> dir_path, std::string index_path)
{
	std::ofstream file_out;
	std::fstream template_auto;

	std::string file_path;
	file_path = "tmp/autoindex.html"; 
    
	file_out.open(file_path, std::ios::out);
	template_auto.open("tmp/auto_template.html", std::ifstream::in);
	
	std::string		line;

	while (std::getline(template_auto, line))
	{
		file_out << line << std::endl;
	}
	file_out << "<h1>Index of " + remove_first_path(index_path) + "</h1>" << std::endl;
	file_out << "<hr>" << std::endl;
	for (std::map<std::string, std::string>::const_iterator it_m = dir_path.begin(); it_m != dir_path.end(); ++it_m)
	{
		// std::cout << it_m->first << "--||--" << remove_first_path(it_m->second) << std::endl;
		file_out << "    <p style=\"line-height:5px;\"><a href=\"/" \
		+ remove_first_path(it_m->second) + "\">" + it_m->first \
		+ path_format(it_m->first) + "</a></p>" << std::endl;
	}
	file_out << "</body>" << std::endl;
	file_out << "</html>" << std::endl;

	file_out.close();
	return file_path;

}

std::string create_autoindex(std::string dir_path) {
	
	std::map<std::string, std::string> paths;
	
	paths = get_dir_paths(dir_path);

	return create_file(paths, dir_path);;
}