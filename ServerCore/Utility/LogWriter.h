#pragma once

#include <string>
#include <mutex>
#include <fstream>

enum ENUM_LOG_CONFIG
{
	FILE_NAME_SIZE = 256,
	BUFSIZE = 1024 * 2,
	MAX_LOG_FILE_SIZE = 1024 * 20000,
	MAX_NOW_BUFFER_SIZE = 100,
};

class Logger
{
private:

	static std::unique_ptr<Logger>	instance_;
	static std::once_flag			onceFlag_;
	
	std::mutex			logLock_;
	std::fstream		file_;

	std::string			filePath_;
	std::string			outDiretory_;
	std::string			backupOutDiretory_;
	std::string			fileName_;
	int64_t				maxFileSize_;

	unsigned int		writeDay_;

	std::string			firstAppendString_;

public:
	Logger();
	~Logger();

	static Logger& GetInstance();

	bool Initialize( const char* outputDir, const char* backupOutputDir, const char* fileName, const int64_t maxFileSize = MAX_LOG_FILE_SIZE );
	void Log(const char* msg, ...);	
	
	void release();
	void setFirstAppend(const std::string& firstAppendString);	
	bool changeNewFile(const bool isForceChangeFile);

private:

	void _getNowTime( char* buffer );
	bool _createNewFile();
	void _writeErrorLog(const char* msg);
};

