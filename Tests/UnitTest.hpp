//
// Created by David Puente rodriguez on 09/08/2022.
//

#ifndef WEBSERV_UNITTEST_H
#define WEBSERV_UNITTEST_H

#include "../libraries/utils.hpp"
#include <sys/stat.h>

#define GREEN = "\x1b[32m"
#define RED = "\x1b[31m"
#define YELLOW = "\x1b[33m"
#define RESET = "\x1b[0m"

namespace ft
{
	class UnitTest
	{
		protected:
			int		total_count;
			int		listen_fd;
			int		socked_fd;

		public:

			void file_exists(char * name) {
				struct stat buffer;
				int exists;
				exists = stat(name.c_str(), &buffer);
				if (exists == 0)
					return (true);
				else
					return (false);
			}

			UnitTest (char * config_file)
			{
				if (file_exists(config_file))
					std::cout << "exists" << std::endl;
				else
					std::cout << config_array[n] << std::endl;
			}

			UnitTest (char ** config_array)
			{
				for (int n = 0; n < len(config_array); n++)
				{
					if (file_exists(config_array[n]))
						std::cout << "exists" << std::endl;
					else
						std::cout << config_array[n] << std::endl;
				}
			}

			virtual ~UnitTest(void){}

			/* ############# */
			/* # OPERATORS # */
			/* ############# */

//			UnitTest & operator=(const UnitTest & tmp)
//			{
//
//			}
//
//			char * format_output(void)
//			{
//
//			}
	};
}


#endif //WEBSERV_UNITTEST_H
