#include "Logger.h"
#include "Utility.h"
#include <iostream>
#include "Time.h"

Logger logger = Logger::Logger();

Logger& operator<<(Logger& output, const std::string& msg)
{
	if (output.getState() == logState::setAuthor)
	{
		output.setAuthor(msg);
		output.setState(logState::compose);
		return output;
	}
	else if (output.getState() == logState::endMsg)
		output.end();
	else
		output.getStream() << msg;

	return output;
}

Logger& operator<<(Logger& output, int msg)
{
	if (output.getState() == logState::setAuthor)
	{
		output.setAuthor(std::to_string(msg));
		output.setState(logState::compose);
		return output;
	}
	else if (output.getState() == logState::endMsg)
		output.end();
	else
		output.getStream() << std::to_string(msg);

	return output;
}

Logger& operator<<(Logger& output, unsigned int msg)
{
	if (output.getState() == logState::setAuthor)
	{
		output.setAuthor(std::to_string(msg));
		output.setState(logState::compose);
		return output;
	}
	else if (output.getState() == logState::endMsg)
		output.end();
	else
		output.getStream() << std::to_string(msg);

	return output;
}

Logger& operator<<(Logger& output, long msg)
{
	if (output.getState() == logState::setAuthor)
	{
		output.setAuthor(std::to_string(msg));
		output.setState(logState::compose);
		return output;
	}
	else if (output.getState() == logState::endMsg)
		output.end();
	else
		output.getStream() << std::to_string(msg);

	return output;
}

Logger& operator<<(Logger& output, unsigned long msg)
{
	if (output.getState() == logState::setAuthor)
	{
		output.setAuthor(std::to_string(msg));
		output.setState(logState::compose);
		return output;
	}
	else if (output.getState() == logState::endMsg)
		output.end();
	else
		output.getStream() << std::to_string(msg);

	return output;
}

Logger& operator<<(Logger& output, float msg)
{
	if (output.getState() == logState::setAuthor)
	{
		output.setAuthor(std::to_string(msg));
		output.setState(logState::compose);
		return output;
	}
	else if (output.getState() == logState::endMsg)
		output.end();
	else
		output.getStream() << std::to_string(msg);

	return output;
}

Logger& operator<<(Logger& output, const Vector2& msg)
{
	if (output.getState() == logState::setAuthor)
	{
		output.setAuthor(msg.str());
		output.setState(logState::compose);
		return output;
	}
	else if (output.getState() == logState::endMsg)
		output.end();
	else
		output.getStream() << msg.str();

	return output;
}

Logger& operator<<(Logger& output, const Vector3& msg)
{
	if (output.getState() == logState::setAuthor)
	{
		output.setAuthor(msg.str());
		output.setState(logState::compose);
		return output;
	}
	else if (output.getState() == logState::endMsg)
		output.end();
	else
		output.getStream() << msg.str();

	return output;
}

Logger& operator<<(Logger& output, const Vector4& msg)
{
	if (output.getState() == logState::setAuthor)
	{
		output.setAuthor(msg.str());
		output.setState(logState::compose);
		return output;
	}
	else if (output.getState() == logState::endMsg)
		output.end();
	else
		output.getStream() << msg.str();

	return output;
}

Logger& operator<<(Logger& output, const Quaternion& msg)
{
	if (output.getState() == logState::setAuthor)
	{
		output.setAuthor(msg.str());
		output.setState(logState::compose);
		return output;
	}
	else if (output.getState() == logState::endMsg)
		output.end();
	else
		output.getStream() << msg.str();

	return output;
}

Logger& operator<<(Logger& output, const Vector& msg)
{
	if (output.getState() == logState::setAuthor)
	{
		output.setAuthor(msg.str());
		output.setState(logState::compose);
		return output;
	}
	else if (output.getState() == logState::endMsg)
		output.end();
	else
		output.getStream() << msg.str();

	return output;
}

Logger& operator<<(Logger& output, const Matrix4& msg)
{
	if (output.getState() == logState::setAuthor)
	{
		output.setAuthor(msg.str());
		output.setState(logState::compose);
		return output;
	}
	else if (output.getState() == logState::endMsg)
		output.end();
	else
		output.getStream() << "\n" << msg.str();

	return output;
}

Logger& operator<<(Logger& output, const Matrix& msg)
{
	if (output.getState() == logState::setAuthor)
	{
		output.setAuthor(msg.str());
		output.setState(logState::compose);
		return output;
	}
	else if (output.getState() == logState::endMsg)
		output.end();
	else
		output.getStream() << "\n" << msg.str();

	return output;
}

Logger& operator<<(Logger& output, const logModifier& modifier)
{
	if (modifier == author)
	{
		output.setState(logState::setAuthor);
	}
	else if (modifier == lend)
	{
		output.setState(logState::endMsg);
		output << "";
	}
	return output;
}

Logger::Logger() : m_logFile(std::ofstream()), m_streams(), m_currentAuthor("Log"), m_state(logState::compose)
{
}

void Logger::end()
{
	std::ostringstream& stream = getStream();

	if (stream.str().size() == 0) //Empty message
		return;

	std::ostringstream fullMsg;
	fullMsg << "(" << getAuthor() << " @ " << Time::getDateAndTime(Time::ONLY_TIME) << "): " << stream.str();

	//Retrieves, and opens logFile if neccesary
	std::ofstream& logFile = getLogFile();
	if (!logFile.is_open())
		logFile.open(WORKDIR + "Logs\\" + Time::startDateAndTime + ".txt", std::ios::app);

	logFile << fullMsg.str() << std::endl;
	std::cout << fullMsg.str() << std::endl;

	//Clears the composed message and sets logger back to compose state
	stream.clear();
	getStreamMap().erase(getAuthor());
	setState(logState::compose);

	if (!m_keepAuthor) //Doesn't reset author if true
		setAuthor("");
}
