#pragma once

#include "jgl_includes.h"
#include "jgl_basic_types.h"

namespace jgl
{
	/*
		Open a file with custom [mode], at path [path] and return it
	*/
	jgl::File open_file(jgl::String path, jgl::File_mode mode);

	/*
		Read one line of the file [file] (until a \\n is found)
	*/
	jgl::String get_str(jgl::File& file);

	/*
		Read the whole content of the file at path [path] and return it as one string
	*/
	jgl::String read_file(jgl::String path);

	/*
		Read [size] number of bytes and place it inside [dest]
	*/
	void read_data(jgl::File& file, void* dest, jgl::Int size);

	/*
		Replace the file pointer to the desired [delta] pos from the beggining of the file
	*/
	void seek_file(jgl::File& file, jgl::Int delta);

	/*
		Replace the file pointer to the desired [delta] pos from [pos] position of the file
	*/
	void seek_file(jgl::File& file, jgl::Int delta, jgl::File_pos pos);

	/*
		Read a line of the file and split it with delimiter [c]
		Optional 
		jgl::Int size - the number of substring expected - -1 for no expectation
	*/
	jgl::Array<jgl::String> get_strsplit(jgl::File& file, const jgl::String c, jgl::Int size = -1);

	/*
		List every file contain in [path] location.
		Optional 
		jgl::String extension - if specified, will only give you the file with the correct extension
	*/
	jgl::Array<jgl::String> list_files(jgl::String path, jgl::String extension);

	/*
		Check if the desired file exist. Return true if yes, false if not
	*/
	jgl::Bool check_file_exist(jgl::String path);

	/*
		Copy the file at [src_path] to a file at [dest_path]
	*/
	void copy_file(jgl::String src_path, jgl::String dest_path);

	/*
		Write in file [file] the string [text]
	*/
	void write_on_file(jgl::File& file, jgl::String text);

	/*
		Write in file at [path] the string [text]
	*/
	void write_on_file(jgl::String path, jgl::String text);

	/*
		Write the string [text] after clearing the content of the file at [path]	
	*/
	void rewrite_on_file(jgl::String path, jgl::String text);
}

