#pragma warning ( disable : 4996 )

#include <iostream>
#include <io.h>
#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h> 

#include "LogWriter.h"

std::unique_ptr<Logger> Logger::instance_;
std::once_flag Logger::onceFlag_;

Logger& Logger::GetInstance()
{
	std::call_once( onceFlag_, [] 
	{
		instance_.reset(new Logger);
	});

	return *instance_;
}

Logger::Logger() : maxFileSize_( 0 ), writeDay_( 0 )
{
}

Logger::~Logger()
{
	release();
}

bool Logger::Initialize( const char* outputDir, const char* backupOutputDir, const char* fileName, const int64_t maxFileSize )
{
	if( maxFileSize == 0 )
		return false;

	outDiretory_ = outputDir;
	backupOutDiretory_ = backupOutputDir;
	fileName_ = fileName;
	maxFileSize_ = maxFileSize;

	///< create directory
	struct _stat st;
	if( _stat( outDiretory_.c_str(), &st ) < 0 )
	{
		if( _mkdir( outDiretory_.c_str() ) < 0 )
		{
			char errroLogBuffer[1024] = { '\0', };
			_snprintf( errroLogBuffer, sizeof( errroLogBuffer ), "path : %s error msg[%s:%d]", outDiretory_.c_str(), strerror( errno ), errno );
			_writeErrorLog( errroLogBuffer );
		}
	}

	///< set new file path
	char newFilePath[FILE_NAME_SIZE];
	_snprintf( newFilePath, FILE_NAME_SIZE, "%s/%s.log", outDiretory_.c_str(), fileName_.c_str() );
	filePath_ = newFilePath;

	///< set write day
	time_t log_now = time( NULL );
	struct tm * tm_now = NULL;

#ifdef	__linux__
	tm tm_now_value;
	localtime_r( &log_now, &tm_now_value );
	tm_now = &tm_now_value;
#else	//__linux__		
	tm_now = localtime( &log_now );
#endif	//__linux__

	writeDay_ = static_cast<unsigned int>(tm_now->tm_mday);

	if( _access( filePath_.c_str(), 0 ) == 0 )
	{
		///< fileName already exists. do backup
		char backupDirectory[1024] = { '\0', };
		_snprintf( backupDirectory, sizeof( backupDirectory ), "%s/%s", outDiretory_.c_str(), backupOutDiretory_.c_str() );

		///< create directory
		struct _stat sta;
		if( _stat( backupDirectory, &sta ) < 0 )
		{
			if( _mkdir( backupDirectory ) < 0 )
				return false;
		}

		char backupFilePath[1024] = { '\0', };
		_snprintf( backupFilePath, sizeof( backupFilePath ), "%s/%s#%02d%02d%02d_%02d%02d%02d.log",
			backupDirectory, fileName_.c_str(), tm_now->tm_year % 100, tm_now->tm_mon + 1, tm_now->tm_mday, tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec );

		if( rename( filePath_.c_str(), backupFilePath ) < 0 )
		{
			char errroLogBuffer[1024] = { '\0', };
			_snprintf( errroLogBuffer, sizeof( errroLogBuffer ), "File(%s:%s) rename error msg[%s:%d]", filePath_.c_str(), backupFilePath, strerror( errno ), errno );
			_writeErrorLog( errroLogBuffer );
			exit( 1 );
		}
	}

	bool isCreateNewFile = _createNewFile();
	return isCreateNewFile;
}

void Logger::release()
{
	if( file_.is_open() == true )
	{
		file_.close();
	}
}

bool Logger::_createNewFile()
{
	release();

	file_.open( filePath_.c_str(), std::fstream::in | std::fstream::out | std::fstream::app );
	
	if( file_.is_open() == false )
	{
		char errroLogBuffer[1024] = { '\0', };
		_snprintf( errroLogBuffer, sizeof( errroLogBuffer ), "CAN'T OPEN OR CREATE THE FILE %s-%s\n", filePath_.c_str(), strerror( errno ) );
		_writeErrorLog( errroLogBuffer );
		return false;
	}

	if( firstAppendString_.empty() == false )
		Log( firstAppendString_.c_str() );

	return true;
}

bool Logger::changeNewFile( const bool isForceChangeFile )
{
	struct _stat st;
	int rst = _stat( filePath_.c_str(), &st );

	if( rst < 0 )
	{
		char errroLogBuffer[1024] = { '\0', };
		_snprintf( errroLogBuffer, sizeof( errroLogBuffer ), "File(%s) is not exist, errmsg[%s:%d]", filePath_.c_str(), strerror( errno ), errno );
		_writeErrorLog( errroLogBuffer );
		exit( 1 );
	}

#ifdef	__linux__
	///< S_IRUSR : owner has read permission
	if( (st.st_mode & S_IRUSR) != S_IRUSR )
	{
		char errroLogBuffer[1024] = { '\0', };
		_snprintf( errroLogBuffer, sizeof( errroLogBuffer ), "File(%s) has not read permission", filePath_.c_str() );
		_writeErrorLog( errroLogBuffer );
		exit( 1 );
		return false;
	}
	///< S_IWUSR : owner has write permission
	if( (st.st_mode & S_IWUSR) != S_IWUSR )
	{
		char errroLogBuffer[1024] = { '\0', };
		_snprintf( errroLogBuffer, sizeof( errroLogBuffer ), "File(%s) has not write permission", filePath_.c_str() );
		_writeErrorLog( errroLogBuffer );
		exit( 1 );
		return false;
	}
#endif	//__linux__

	///< set date
	time_t log_now = time( NULL );
	struct tm * tm_now = NULL;

#ifdef	__linux__
	tm tm_now_value;
	localtime_r( &log_now, &tm_now_value );
	tm_now = &tm_now_value;
#else	//__linux__
	tm_now = localtime( &log_now );
#endif	//__linux__

	bool isChangeFile = false;

	///< 날짜가 변경되면 새로운 파일 생성
	if( writeDay_ != static_cast<unsigned int>(tm_now->tm_mday) )
	{
		//파일이름을 '전날 23:59:59'으로 맞춰준다.
		writeDay_ = tm_now->tm_mday;

		//현재시간을 초로 변환
		const unsigned int HOUR_TO_SEC = 60 * 60;
		unsigned int nowTimeToSec = (tm_now->tm_hour*HOUR_TO_SEC) + (tm_now->tm_sec) + (tm_now->tm_min * 60 + 1);
		log_now -= nowTimeToSec;

#ifdef	__linux__
		localtime_r( &log_now, &tm_now_value );
		tm_now = &tm_now_value;
#else	//__linux__
		tm_now = localtime( &log_now );
#endif	//__linux__

		isChangeFile = true;
	}

	///< 정해진 크기 보다 더 많이 썼다면 backup
	if( maxFileSize_ < st.st_size )
		isChangeFile = true;

	if( (isChangeFile == false) && (isForceChangeFile == false) )
		return true;

	char backupDirectory[1024] = { '\0', };
	_snprintf( backupDirectory, sizeof( backupDirectory ), "%s/%s", outDiretory_.c_str(), backupOutDiretory_.c_str() );

	rst = _stat( backupDirectory, &st );
	if( rst < 0 )
	{
		if( errno != ENOENT )
		{
			char errroLogBuffer[1024] = { '\0', };
			_snprintf( errroLogBuffer, sizeof( errroLogBuffer ), "path : %s error msg[%s:%d]", backupDirectory, strerror( errno ), errno );
			_writeErrorLog( errroLogBuffer );
			return true;
		}

		if( _mkdir( backupDirectory ) < 0 )
		{
			char errroLogBuffer[1024] = { '\0', };
			_snprintf( errroLogBuffer, sizeof( errroLogBuffer ), "path : %s error msg[%s:%d]", backupDirectory, strerror( errno ), errno );
			_writeErrorLog( errroLogBuffer );
			return true;
		}
	}

	if( file_.is_open() == true )
	{
		file_.close();
	}

	char backupFilePath[1024] = { '\0', };
	_snprintf( backupFilePath, sizeof( backupFilePath ), "%s/%sB#%02d%02d%02d_%02d%02d%02d.log",
		backupDirectory, fileName_.c_str(), tm_now->tm_year % 100, tm_now->tm_mon + 1, tm_now->tm_mday, tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec );

	if( rename( filePath_.c_str(), backupFilePath ) < 0 )
	{
		char errroLogBuffer[1024] = { '\0', };
		_snprintf( errroLogBuffer, sizeof( errroLogBuffer ), "File(%s:%s) rename error msg[%s:%d]", filePath_.c_str(), backupFilePath, strerror( errno ), errno );
		_writeErrorLog( errroLogBuffer );
	}

	return _createNewFile();
}

void Logger::Log( const char* msg, ... )
{
	std::unique_lock<std::mutex> lock{ logLock_ };

	///< read arg
	int len = 0;
	va_list ap;
	char tmpbuf[BUFSIZE] = { 0, };
	char buf[BUFSIZE + 40] = { 0, };

	va_start( ap, msg );
	len = vsnprintf( tmpbuf, sizeof( tmpbuf ) - 1, msg, ap );
	va_end( ap );

	char nowBuffer[MAX_NOW_BUFFER_SIZE] = { '\0' };
	_getNowTime( nowBuffer );
	len = _snprintf( buf, BUFSIZE + 40, "%s%s\n", nowBuffer, tmpbuf );

	if( len < 0 )
		return;

	len = len * sizeof( char );

	//WRITE_TO_FILE:
	if( changeNewFile( false ) == false )
		return;

	if( file_.is_open() == false )
	{
		char errroLogBuffer[1024] = { '\0', };
		_snprintf( errroLogBuffer, sizeof( errroLogBuffer ), "File(%s) fileDescriptor error", filePath_.c_str() );
		_writeErrorLog( errroLogBuffer );
	}
	else
	{
		file_.write( buf, len );
		file_.flush();
		printf( buf );
	}
}

void Logger::setFirstAppend( const std::string& firstAppendString )
{
	firstAppendString_ = firstAppendString;
}

void Logger::_getNowTime( char* buffer )
{
	time_t tval = time(NULL);

	struct tm* t;
#ifdef	_WIN32
	t = localtime(&tval);
#else
	tm t_value;
	localtime_r(&tval, &t_value);
	t = &t_value;
#endif

	const size_t writedSize = _snprintf( buffer, MAX_NOW_BUFFER_SIZE, "%04d%02d%02d %02d%02d%02d : ", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

	buffer[writedSize] = '\0';
}

void Logger::_writeErrorLog( const char* msg )
{
	char errorFilePath[FILE_NAME_SIZE] = { '\0', };
	_snprintf( errorFilePath, FILE_NAME_SIZE, "%s/Error_LogWriter_%s.log", outDiretory_.c_str(), fileName_.c_str() );

	std::fstream errorLog;
	errorLog.open( errorFilePath, std::fstream::in | std::fstream::out );
	if( errorLog.is_open() == false )
	{
		fprintf( stderr, "CAN'T OPEN OR CREATE THE FILE %s-%s\n", errorFilePath, strerror( errno ) );
		return;
	}

	errorLog.write( msg, strlen(msg) );

	errorLog.close();
}


