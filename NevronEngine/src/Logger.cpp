#include "Logger.h"
#include "Utility.h"
#include <iostream>
#include "Time.h"

static std::ofstream logFile;

//Checks to see if the frame changes to put a divider between log calls on different frames
static int frame;
void LogS(const std::string& author, std::string format, ...)
{
	va_list vl;
	va_start(vl, format);

	std::string fullMsg = '(' + (author == "" ? "Log" : author) + " @ " + Time::getDateAndTime(Time::ONLY_TIME) + "): ";
	for (unsigned int i = 0; i < format.size(); i += 2)
	{
		std::string a;
		//Is a two wide substr of fmt
		if (format[i] == '%' && !(i > 0 && format[i - 1] == '\\')) //Format expected
			switch (format[(__int64)i + 1]) //checks next
			{
			case 'd': //Signed decimal integer
				fullMsg += std::to_string(va_arg(vl, signed int));
				break;

			case 'i': //Signed decimal integer
				fullMsg += std::to_string(va_arg(vl, signed int));
				break;

			case 'u': //Unsigned decimal integer
				fullMsg += std::to_string(va_arg(vl, unsigned int));
				break;
			case 'v':
				switch (format[(__int64)i + 2])
				{
				case '2':
					fullMsg += va_arg(vl, Vector2).str();
					break;
				case '3':
					fullMsg += va_arg(vl, Vector3).str();
					break;
				case '4':
					fullMsg += va_arg(vl, Vector4).str();
					break;
				case 'n':
					fullMsg += va_arg(vl, Vector).str();
					break;
				default:
					break;
				}
				i++; //Skipping vector size indicator
				break;
			case 'V':
				switch (format[(__int64)i + 2])
				{
				case '2':
					fullMsg += va_arg(vl, Vector2).str_d();
					break;
				case '3':
					fullMsg += va_arg(vl, Vector3).str_d();
					break;
				case '4':
					fullMsg += va_arg(vl, Vector4).str_d();
					break;
				default:
					fullMsg += va_arg(vl, Vector).str_d();
					break;
				}
				break;
			case 'm':
				switch (format[(__int64)i+2])
				{
				case 4:
					fullMsg += va_arg(vl, Matrix4).str();
				default:
					fullMsg += va_arg(vl, Matrix).str();
					break;
				}
				break;
			case 'M':
				switch (format[(__int64)i + 2])
				{
				case 4:
					fullMsg += va_arg(vl, Matrix4).str();
				default:
					fullMsg += va_arg(vl, Matrix).str();
					break;
				}
			case 'o': //Unsigned octal
				fullMsg += Math::ToOctal(va_arg(vl, signed int));
				break;

			case 'x': //Unsigned hexadecimal integer (lowercase)
				fullMsg += Math::ToHex(va_arg(vl, unsigned int), false);
				break;
			case 'X': //Unsigned hexadecimal integer (uppercase)
				fullMsg += Math::ToHex(va_arg(vl, unsigned int), true);
				break;
			case 'f': //Decimal doubleing point (lowercase)
				fullMsg += std::to_string(va_arg(vl, double));
				break;
			case 'F': //Decimal doubleing point (uppercase)
				fullMsg += std::to_string(va_arg(vl, double));
				break;
			case 'e':
			{char buffer[max_loglenght];
			snprintf(buffer, max_loglenght, "%e", va_arg(vl, double));
			fullMsg.append(buffer);
			break; }
			case 'E':
			{char buffer[max_loglenght];
			snprintf(buffer, max_loglenght, "%E", va_arg(vl, double));
			fullMsg.append(buffer);
			break; }
			case 'g':
			{	char buffer[max_loglenght];
			snprintf(buffer, max_loglenght, "%g", va_arg(vl, double));
			fullMsg.append(buffer);
			break; }
			case 'G':
			{	char buffer[max_loglenght];
			snprintf(buffer, max_loglenght, "%G", va_arg(vl, double));
			fullMsg.append(buffer);
			break; }
			case 'a':
			{	char buffer[max_loglenght];
			snprintf(buffer, max_loglenght, "%a", va_arg(vl, double));
			fullMsg.append(buffer);
			break; }
			case 'A':
			{	char buffer[max_loglenght];
			snprintf(buffer, max_loglenght, "%A", va_arg(vl, double));
			fullMsg.append(buffer);
			break; }
			case 'c': //Character
				fullMsg += va_arg(vl, char*);
				break;
			case 's': //String
				fullMsg += va_arg(vl, std::string);
				break;
			case 'p':
			{	char buffer[max_loglenght];
			snprintf(buffer, max_loglenght, "%p", va_arg(vl, void*));
			fullMsg.append(buffer);
			break; }
			case '%': //String
				fullMsg += "%";
				break;
			default:
				i--;
				break;
			}

		else
		{
			fullMsg += format[i];
			i--;
		}
	}

	if (!logFile.is_open())
	{
		Utility::GenerateFile(WORKDIR + "Logs\\" + Time::startDateAndTime + ".txt", "");
		logFile.open(WORKDIR + "Logs\\" + Time::startDateAndTime + ".txt");
	}

	if (frame != Time::frameCount)
	{
		frame = Time::frameCount;
		std::string divider(fullMsg.size(), '-');
		divider += "\n";
		fullMsg.insert(fullMsg.begin(), divider.begin(), divider.end());
	}
	fullMsg += '\n';
	printf(fullMsg.c_str());
	logFile.write(fullMsg.c_str(), fullMsg.size());
	va_end(vl);
}

void LogF(const std::string& author, const char* format, ...)
{


	if (!logFile.is_open())
	{
		Utility::GenerateFile(WORKDIR + "Logs\\" + Time::startDateAndTime + ".txt", "");
		logFile.open(WORKDIR + "Logs\\" + Time::startDateAndTime + ".txt");
	}

	va_list vl;
	va_start(vl, format);
	char buf[512];
	vsnprintf(buf, sizeof(buf), format, vl);
	va_end(vl);

	std::string fullMsg = '(' + (author == "" ? "Log" : author) + " @ " + Time::getDateAndTime(Time::ONLY_TIME) + "): " + buf + '\n';

	if (frame != Time::frameCount)
	{
		frame = Time::frameCount;
		std::string divider(fullMsg.size(), '-');
		divider += "\n";
		fullMsg.insert(fullMsg.begin(), divider.begin(), divider.end());
	}

	printf(fullMsg.c_str());
	logFile.write(fullMsg.c_str(), fullMsg.size());


	return;
}



