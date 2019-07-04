#pragma once
#include <ostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

enum class logState
{
	compose, setAuthor, endMsg
};

enum logModifier
{
	none, author, lend, 
};

class Logger
{
public:
	Logger();
	void setState(logState state) { m_state = state; }
	void setAuthor(const std::string& author) { m_streams[author == "" ? "Log" : author] = std::ostringstream(); m_currentAuthor = author == "" ? "Log" : author; }

	std::ofstream& getLogFile() { return m_logFile; }
	logState getState() const { return m_state; };
	const std::string& getAuthor() { return m_currentAuthor; }
	std::unordered_map<std::string, std::ostringstream>& getStreamMap() { return m_streams; }
	std::ostringstream& getStream() { return m_streams[m_currentAuthor]; }

	void end();
private:

	std::ofstream m_logFile;
	//The incomplete message being gradually built
	std::unordered_map<std::string, std::ostringstream> m_streams; //Unfinished messages
	std::string m_currentAuthor; //Which stream is currently being written to
	logState m_state;
private:
};

Logger& operator<<(Logger& output, const std::string& msg);
Logger& operator<<(Logger& output, const logModifier& modifier);

extern Logger logger;