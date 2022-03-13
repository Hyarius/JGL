#include "jgl.h"

namespace jgl
{
	jgl::File				open_file(jgl::String path, jgl::File_mode mode)
	{
		jgl::File file;
		file.open(path.c_str(), static_cast<std::ios_base::openmode>(mode));
		if (file.fail() == true)
			THROW_EXCEPTION(jgl::Error_level::Error, 0, "Can't open file [" + path + "]");
		return (file);
	}

	jgl::String					get_str(jgl::File& file)
	{
		std::string line;

		if (file.eof())
			return ("");
		std::getline(file, line);
		if (line.size() == 0)
			return ("");
		if (line[strlen((char*)line.c_str()) - 1] == L'\n')
			line[strlen((char*)line.c_str()) - 1] = L'\0';
		return (jgl::String(line.c_str()));
	}

	jgl::Array<jgl::String>			get_strsplit(jgl::File& file, const jgl::String c, jgl::Int size)
	{
		jgl::Array<jgl::String> tab;
		jgl::String line;

		line = get_str(file);
		if (line.size() == 0)
		{
			if (size != -1)
				THROW_EXCEPTION(jgl::Error_level::Warning, 0, "Empty line in get_strsplit when espect a size");
			tab.resize(0);
			return (tab);
		}
		tab = line.split(c, false);
		if (size != -1 && tab.size() != (uint32_t)size)
			THROW_EXCEPTION(jgl::Error_level::Warning, 0, "Bad lenght in get_strsplit");
		return tab;
	}

	jgl::Array<jgl::String>		list_files(jgl::String path, jgl::String extension)
	{
		jgl::Array<jgl::String> files;
		jgl::String line;
		jgl::String test;
		dirent* dirent_ptr;
		DIR* dir = opendir(path.c_str());

		files.clear();
		if (dir == NULL)
			return (files);

		while ((dirent_ptr = readdir(dir)) != NULL)
		{
			jgl::String tmp_name = dirent_ptr->d_name;
			jgl::String tmp = path + tmp_name;
			jgl::File_type type = static_cast<jgl::File_type>(dirent_ptr->d_type);

			jgl::String path = "";
			jgl::String name = "";
			jgl::String file_extension = "";
			
			if (type == jgl::File_type::File)
			{
				jgl::Array<jgl::String> tmp2;
				tmp2 = tmp_name.split(".", true);
				name = tmp2[0];
				if (tmp2.size() >= 2)
					file_extension = "." + tmp2[1];
			}
			else if (type == jgl::File_type::Directory)
			{
				name = tmp_name;
			}

			if (extension == "*")
				files.push_back(tmp);
			else if ((type == jgl::File_type::File && file_extension == extension) ||
					 (type == jgl::File_type::Directory && name != "." && name != ".."))
				files.push_back(tmp);
		}
		closedir(dir);
		return files;
	}

	jgl::Bool				check_file_exist(jgl::String path)
	{
		struct stat buffer;
		return (stat(path.c_str(), &buffer) == 0);
	}

	void				copy_file(jgl::String src_path, jgl::String dest_path)
	{
		jgl::File source;
		jgl::File destination;

		source.open(src_path.c_str(), std::ios::binary);
		destination.open(dest_path.c_str(), std::ios::binary);

		source.seekg(0, std::ios::end);
		jgl::File::pos_type size = source.tellg();
		source.seekg(0);
		char* buffer = new char[static_cast<uint32_t>(size)];

		source.read(buffer, size);
		destination.write(buffer, size);
	}

	void				write_on_file(jgl::File& file, jgl::String text)
	{
		file << text << jgl::endl;
	}

	void				write_on_file(jgl::String path, jgl::String text)
	{
		jgl::File file;
		file.open(path.c_str(), std::ios::app);
		file << text << jgl::endl;
		file.close();
	}

	void				rewrite_on_file(jgl::String path, jgl::String text)
	{
		jgl::File file;
		file.open(path.c_str());
		file << text << jgl::endl;
		file.close();
	}

	jgl::String				read_file(jgl::String path)
	{
		jgl::File file = jgl::open_file(path, jgl::File_mode::in);
		jgl::String result;

		if (file.is_open() == false)
			THROW_EXCEPTION(jgl::Error_level::Error, 1, "Can't open file " + path);

		while (file.eof() == false)
		{
			result += jgl::get_str(file);
		}

		return (result);
	}

	void read_data(jgl::File& file, void* dest, jgl::Int size)
	{
		file.read(reinterpret_cast<char*>(dest), size);

		if (file.fail() == true)
		{
			char buffer[80];
			strerror_s<80>(buffer, errno);
			THROW_EXCEPTION(jgl::Error_level::Error, 1, jgl::String("Read error in file: ") + "[" + jgl::itoa(errno) + "] " + jgl::String(buffer));
		}
	}

	void seek_file(jgl::File& file, jgl::Int delta)
	{
		file.seekg(delta, file.beg);
		if (file.fail() == true)
		{
			char buffer[80];
			strerror_s<80>(buffer, errno);
			THROW_EXCEPTION(jgl::Error_level::Error, 1, jgl::String("Seek error in file:") + "[" + jgl::itoa(errno) + "] " + jgl::String(buffer));
		}
	}

	void seek_file(jgl::File& file, jgl::Int delta, jgl::File_pos pos)
	{
		file.seekg(delta, static_cast<std::ios_base::seekdir>(pos));
		if (file.fail() == true)
		{
			char buffer[80];
			strerror_s<80>(buffer, errno);
			THROW_EXCEPTION(jgl::Error_level::Error, 1, jgl::String("Seek error in file:") + "[" + jgl::itoa(errno) + "] " + jgl::String(buffer));
		}
	}
}