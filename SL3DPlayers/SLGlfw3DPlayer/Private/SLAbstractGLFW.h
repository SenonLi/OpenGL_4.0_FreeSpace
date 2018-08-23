#ifndef __SLABSTRACT_GLFW__
#define __SLABSTRACT_GLFW__
#pragma once


class SLAbstractGLFW
{
public:
	SLAbstractGLFW();
	virtual ~SLAbstractGLFW();

	void ShowWidget();

protected:
	GLFWwindow* m_WidgetGLFW;
	GLint m_WidgetWidth, m_WidgetHeight;
	std::string m_WindowName;

	virtual void InitPaintContext() = 0;
	virtual void PaintGL();
	virtual void Finalize() = 0;

private:
	void InitGLFW();

};

#endif //__SLABSTRACT_GLFW__