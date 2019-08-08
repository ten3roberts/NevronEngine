#include "Settings.h"
#include "Utility.h"

#include <fstream>
#include <iostream>

using namespace Utility;

Settings* Settings::m_instance = nullptr;

Settings::Settings()
	: m_screenWidth(640), m_screenHeight(480), m_useNativeResoultion(false), m_screenNear(0.01f), m_screenFar(1000.0f),
	m_displayMode(WINDOWED), m_VSync(false), m_MSAA(false), m_MSAASamples(1), m_FOV(1/*radians*/), m_mouseSensitivity(1)
{
}

Settings::~Settings()
{
}

bool Settings::Load(const std::string& filename)
{
	m_filepath = FindFile(strLead(filename, ".cnf"));
	std::ifstream SettingsFile(m_filepath);

	if (SettingsFile.is_open())
	{
		LogS("Settings", "Loading Settingsfile: %s", ShortenPath(m_filepath));
		while (SettingsFile.good())
		{
			std::string currentLine;
			std::getline(SettingsFile, currentLine);
			std::string keyWord = currentLine.substr(0, currentLine.find_first_of(' '));
			std::string value = currentLine.substr(currentLine.find_first_of(' ') + 1);


			if (keyWord == "resolution")
			{
					m_screenWidth = num<unsigned int>(value.substr(0, value.find_first_of(',')));
					m_screenHeight = num<unsigned int>(value.substr(value.find_first_of(',') + 1));
					LogS("Settings", "Resoulution: %u, %u", m_screenWidth, m_screenHeight);;
			}
			if (keyWord == "useNativeResoultion")
			{
				m_useNativeResoultion = parseBool(value);
				LogS("Settings", ("UseNativeResolution: " + FormatBool(m_screenFar)));
			}
			else if (keyWord == "screenFar")
			{
				m_screenFar = num(value);
				LogS("Settings", "ScreenFar: %f", m_screenFar);
			}
			else if (keyWord == "screenNear")
			{
				m_screenNear = num(value);
				LogS("Settings", "ScreenNear: %f", m_screenNear);
			}

			else if (keyWord == "displayMode")
			{
				m_displayMode = value == "WINDOWED" ? WINDOWED : value == "FULLSCREEN" ? FULLSCREEN : value == "BORDERLESS" ? BORDERLESS : WINDOWED;

				LogS("Settings", "DisplayMode: %d", m_displayMode);
			}

			else if (keyWord == "VSync")
			{
				m_VSync = parseBool(value);
				LogS("Settings", ("Vsync: " + FormatBool(m_VSync)));
			}
			else if (keyWord == "MSAA")
			{
				m_MSAA = parseBool(value);
				LogS("Settings", ("MSAA: " + FormatBool(m_MSAA)));
			}
			else if (keyWord == "MSAASamples")
			{
				m_MSAASamples = num<unsigned int>(value);
				LogS("Settings", "MSAASamples: %u", m_MSAASamples);
			}
			else if (keyWord == "FOV")
			{

				m_FOV = num(value);
				LogS("Settings", "FOV: %f", m_FOV);
			}
			else if (keyWord == "sensitivity")
			{
				m_mouseSensitivity = num(value);
				LogS("Settings", "Sensitivity: %f", m_mouseSensitivity);
			}
		}

		SettingsFile.close();
	}
	else
	{
		LogS("Settings", "Could not locate Settingsfile. Generating default Settings");
		m_filepath = ASSETS + "Settings.cnf";
		Save(m_filepath);
		return false;
	}
	return true;
}

bool Settings::Save(const std::string& filepath)
{
	if (filepath != "") //Saving under a different path
		m_filepath = filepath;

	std::ofstream SettingsFile(m_filepath);
	if (SettingsFile.is_open())
	{
		SettingsFile << "resolution " << m_screenWidth << "," << m_screenHeight << std::endl;
		SettingsFile << "useNativeResoultion " << m_useNativeResoultion << std::endl;
		SettingsFile << "screenFar " << m_screenFar << std::endl;
		SettingsFile << "screenNear " << m_screenNear << std::endl;
		std::string displayModeOut = WINDOWED ? "WINDOWED" : m_displayMode == FULLSCREEN ? "FULLSCREEN" : m_displayMode == BORDERLESS ? "BORDERLESS" : "WINDOWED";
		SettingsFile << "displayMode " << displayModeOut << std::endl;
		SettingsFile << "VSync " << m_VSync << std::endl;
		SettingsFile << "MSAA " << m_MSAA << std::endl;
		SettingsFile << "MSAASamples " << m_MSAASamples << std::endl;
		SettingsFile << "FOV " << m_FOV << std::endl;
		SettingsFile << "sensitivity " << m_mouseSensitivity << std::endl;

		SettingsFile.close();
	}
	return true;
}

//getters
void Settings::setResolution(int width, int height)
{
	m_screenWidth = width;
	m_screenHeight = height;
}

void Settings::setScreenWidth(int width)
{
	m_screenWidth = width;
}

void Settings::setScreenHeight(int height)
{
	m_screenHeight = height;
}


void Settings::setScreenFar(float distance)
{
	m_screenFar = distance;
}

void Settings::setScreenNear(float distance)
{
	m_screenNear = distance;
}

void Settings::setDisplayMode(DisplayMode displayMode)
{
	this->m_displayMode = displayMode;
}

void Settings::EnableVSync(bool enable)
{
	m_VSync = enable;
}

void Settings::EnableMSAA(bool enable)
{
	m_MSAA = enable;
}

void Settings::setMSAASample(int sampleQuantity)
{
	m_MSAASamples = sampleQuantity;
}

void Settings::setFOV(float FOV)
{
	this->m_FOV = FOV;
}

void Settings::setMouseSensitivity(float sensitivity)
{
	m_mouseSensitivity = sensitivity;
}



Settings* Settings::get()
{
	if (m_instance == nullptr)
	{
		m_instance = new Settings();
	}

	return m_instance;
}


