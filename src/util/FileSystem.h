#pragma once

#include <stdio.h>
#include <string>
#include <sys/types.h>
#include <dirent.h>

enum FilesystemEntityType {
	TYPE_DIRECTORY,
	TYPE_FILE
};

class FilesystemEntity {
	public:
	FilesystemEntityType type;
	std::string path;
	std::string name;
	std::string parentPath;
	
	FilesystemEntity(FilesystemEntityType t, std::string path) : type(t), path(path) {
		size_t found = path.find_last_of('/');
		name = path.substr(found+1);
		parentPath = path.substr(0, found);
	};

	bool exists() {
		return stat(path.c_str(), NULL) == 0;
	}
};

class File : public FilesystemEntity {
	FILE *file = nullptr;
	
	public:
	File(std::string path) : FilesystemEntity(TYPE_FILE, path) {};
	
	bool open(const char *mode) {
		file = fopen(path.c_str(), mode);
		return file != nullptr;
	}

	FILE *getHandle() {
		return file;
	}
	
	size_t read(void *ptr, size_t size) {
		if (!file) return 0;
		
		return fread(ptr, size, 1, file);
	}
	
	size_t write(const void *ptr, size_t size) {
		if (!file) return 0;
		
		return fwrite(ptr, size, 1, file);
	}
	
	long seek(long offset, int whence) {
		if (!file) return 0;
		
		long initial = ftell(file);
		fseek(file, offset, whence);
		return ftell(file)-initial;
	}
	
	bool close() {
		if (!file) return false;
		
		return fclose(file) == 0;
	}
};

class Directory : public FilesystemEntity {
	DIR *dir = nullptr;
	
	public:
	Directory(std::string path) : FilesystemEntity(TYPE_DIRECTORY, path) {};
	
	bool open() {
		dir = opendir(path.c_str());
		return dir != nullptr;
	}
	
	FilesystemEntity *read() {
		struct dirent *entry = readdir(dir);
		
		if (!entry) {
			return nullptr;
		}
		
		//TODO: Need code to properly create perfect paths
		std::string rpath = path+"/"+std::string(entry->d_name);
		
		if (entry->d_type == DT_DIR) {
			return new Directory(rpath);
		} else {
			return new File(rpath);
		}
	}
	
	bool close() {
		return closedir(dir) == 0;
	}
};
