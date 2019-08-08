#pragma once
#include <ostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include "../Math/Vector.h"
#include "../Math/Quaternion.h"
#include "../Math/Matrix.h"

enum class logState
{
	compose, setAuthor, endMsg
};

enum logModifier
{
	none, author, lend, 
};

/*class Logger
{
public:
	Logger();
	void setState(logState state) { m_state = state; }
	void setAuthor(const std::string& author) { m_keepAuthor = false;  m_streams[author == "" ? "Log" : author] = std::ostringstream(); m_currentAuthor = author == "" ? "Log" : author; }
	void keepAuthor(bool keep = true) { m_keepAuthor = keep; }

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
	bool m_keepAuthor; //Does not reset the author after lend
private:
};*/

/*Logger& operator<<(Logger& output, const std::string& msg);

Logger& operator<<(Logger& output, int msg);
Logger& operator<<(Logger& output, unsigned int msg);
Logger& operator<<(Logger& output, long msg);
Logger& operator<<(Logger& output, unsigned long msg);
Logger& operator<<(Logger& output, float msg);
Logger& operator<<(Logger& output, const Vector2& msg);
Logger& operator<<(Logger& output, const Vector3& msg);
Logger& operator<<(Logger& output, const Vector4& msg);
Logger& operator<<(Logger& output, const Quaternion& msg);
Logger& operator<<(Logger& output, const Vector& msg);
Logger& operator<<(Logger& output, const Matrix4& msg);
Logger& operator<<(Logger& output, const Matrix& msg);

Logger& operator<<(Logger& output, const logModifier& modifier);

//extern Logger logger;
//static Logger& Log = logger;

*/
/* va_arg example */
#include <stdio.h>      /* printf */
#include <stdarg.h>     /* va_list, va_start, va_arg, va_end */

#define max_loglenght 512

//extern void Logf(const char* format, ...);
extern void Logf(const std::string& author, const char* format, ...);