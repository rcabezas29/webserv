#include "autoindex.hpp"
#include <dirent.h>

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
		// if (*it != "." && *it != "..") // si que hay que mostrarlos
		paths.insert(std::make_pair(*it, (dir_path + "/" + *it)));
		++it;
	}

	return paths;
}

std::string create_file(std::map<std::string, std::string> dir_path)
{
	// <a href="url">link text</a>
	std::ofstream file;
	std::string file_path;

	file_path = "tmp/autoindex.html"; //add absolute path
    file.open(file_path);

	for (std::map<std::string, std::string>::const_iterator it_m = dir_path.begin(); it_m != dir_path.end(); ++it_m)
	{
		file << it_m->first<< std::endl;
	}
	file.close();
	//create a html autpindex canvas and insert all in the file
	return file_path;

}

std::string create_autoindex(std::string dir_path){
	
	std::map<std::string, std::string> paths;
	
	paths = get_dir_paths(dir_path);

	create_file(paths);
	return "Buenas";
}