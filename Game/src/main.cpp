#include "Application/Application.h"
#include "EngineLoop/EngineLoop.h"
#include "Services/Window/WindowService.h"
#include "Services/Graphics/GraphicsService.h"
#include "Services/Graphics/Renderers/Direct2DRenderer/Direct2DRenderer.h"

class MyApplication final : public Engine::Application {
public:
	MyApplication() {}

protected:
	void Startup() override {
		m_mainWindow = Engine::Services::WindowService::Instance().MakeWindow();
		m_mainWindow.SetTitle("Main Window");
		m_mainWindow.Resize(800, 600);
		m_mainWindow.Show();

		m_mainRenderer = Engine::Services::GraphicsService::Instance().CreateRenderer2D<Engine::Direct2DRenderer>();
		m_mainRenderer.DoSomething();
	}

	void FixedUpdate() override {

	}

	void VariableUpdate() override {
		m_mainWindow.Update();
		if (m_mainWindow.HasClosed()) {
			Engine::EngineLoop::Instance().Exit();
		}
	}

	void Shutdown() override {
		m_mainWindow.Destroy();
	}

private:
	Engine::WindowProxy m_mainWindow;
	Engine::Renderer2DProxy<Engine::Direct2DRenderer> m_mainRenderer;
};

int main() {
	MyApplication application;
	Engine::EngineLoop::Instance().Run(&application);

	return 0;
}
