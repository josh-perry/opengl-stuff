#pragma once
#include <string>
#include <map>
#include <iostream>
#include <sstream>

#include <chrono>
#include <ctime>
#include <iomanip>

enum class LogLevel
{
	ALL = 0,
	TRACE = 1,
	DEBUG = 2,
	INFO = 3,
	WARN = 4,
	ERROR = 5,
	FATAL = 6
};

std::map<LogLevel, std::string> log_level_strings =
{
	{LogLevel::DEBUG, "DEBUG"},
	{LogLevel::TRACE, "TRACE"},
	{LogLevel::INFO,  "INFO"},
	{LogLevel::WARN,  "WARN"},
	{LogLevel::ERROR, "ERROR"},
	{LogLevel::FATAL, "FATAL"}
};

std::ofstream log_file;

LogLevel min_log_level = LogLevel::DEBUG;

void set_min_log_level(LogLevel log_level)
{
	min_log_level = log_level;
}

std::string get_log_line(std::string line, LogLevel log_level)
{
	auto n = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(n);
	std::tm buf;
	localtime_s(&buf, &in_time_t);

	std::stringstream ss;
	ss << log_level_strings[log_level] << "\t"
	   << std::put_time(&buf, "%Y-%m-%d %X") << "\t"
	   << line << std::endl;

	return ss.str();
}

void log_to_file(std::string line)
{
	if (!log_file.is_open())
	{
		log_file.open("log.txt", std::ios::app);
	}

	log_file << line;
}

void log_line(std::string line, LogLevel log_level, bool to_file = false)
{
	if (log_level < min_log_level)
	{
		return;
	}

	std::string full_line = get_log_line(line, log_level);

	if (log_level == LogLevel::ERROR || log_level == LogLevel::FATAL)
	{
		std::cerr << full_line;
	}

	std::cout << full_line;

	if (to_file)
	{
		log_to_file(full_line);
	}
}