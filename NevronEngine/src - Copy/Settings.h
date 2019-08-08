#pragma once
#include <string>

enum DisplayMode
{
	WINDOWED, FULLSCREEN, BORDERLESS
};

class Settings
{
public:

	~Settings();
	static Settings* get();
	bool Load(const std::string& filename = "Settings.cnf");
	bool Save(const std::string& filepath = "");

	//setters:
	void setResolution(int width, int height);
	void setScreenWidth(int width);
	void setScreenHeight(int height);

	void setScreenFar(float distance);
	void setScreenNear(float distance);
	void setDisplayMode(DisplayMode displayMode);
	void EnableVSync(bool enable);
	void EnableMSAA(bool enable);
	void setMSAASample(int sampleQuantity);
	void setFOV(float FOV);
	void setMouseSensitivity(float sensitivity);

	//getters:
	int getScreenWidth() { return m_screenWidth; };
	int getScreenHeight() { return m_screenHeight; };
	float getAspect() { return (float)getScreenWidth() / getScreenHeight(); };


	float getScreenFar() { return m_screenFar; };
	float getScreenNear() { return m_screenNear; };
	DisplayMode getDisplayMode() { return m_displayMode; };
	bool getVSyncEnabled() { return m_VSync; };
	bool getMSAAEnabled() { return m_MSAA; };

	float getFOV() { return m_FOV; };
	float getMouseSensitivity() { return m_mouseSensitivity; };

private:
	Settings();
	static Settings* m_instance;
private: //SettingsVariables

	std::string m_filepath;

	unsigned int m_screenWidth;
	unsigned int m_screenHeight;
	bool m_useNativeResoultion;

	float m_screenNear;
	float m_screenFar;
	DisplayMode m_displayMode;
	bool m_VSync;
	bool m_MSAA;
	unsigned int m_MSAASamples;

	float m_FOV; //Field of view in radians.
	float m_mouseSensitivity;

};



