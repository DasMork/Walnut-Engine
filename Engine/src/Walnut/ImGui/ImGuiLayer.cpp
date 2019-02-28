#include "wnpch.h"
#include "ImGuiLayer.h"

#include "Walnut/KeyCodes.h"
#include "imgui.h"
#include "Walnut/Application.h"
#include "Walnut/ImGui/ImGuiLayer.h"
#include "Walnut\Platform\OpenGL\ImGuiOpenGLRenderer.h"

//TODO REDO!
#include <GLFW\glfw3.h>
#include <glad\glad.h>

Walnut::ImGuiLayer::ImGuiLayer()
	:Layer("ImGuiLayer")
{
}

Walnut::ImGuiLayer::~ImGuiLayer()
{
}

void Walnut::ImGuiLayer::OnAttach()
{
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO();
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

	// TODO Redo!
	//Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
	io.KeyMap[ImGuiKey_Tab] = WN_KEY_TAB;
	io.KeyMap[ImGuiKey_LeftArrow] = WN_KEY_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = WN_KEY_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = WN_KEY_UP;
	io.KeyMap[ImGuiKey_DownArrow] = WN_KEY_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = WN_KEY_PAGE_UP;
	io.KeyMap[ImGuiKey_PageDown] = WN_KEY_PAGE_DOWN;
	io.KeyMap[ImGuiKey_Home] = WN_KEY_HOME;
	io.KeyMap[ImGuiKey_End] = WN_KEY_END;
	io.KeyMap[ImGuiKey_Insert] = WN_KEY_INSERT;
	io.KeyMap[ImGuiKey_Delete] = WN_KEY_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = WN_KEY_BACKSPACE;
	io.KeyMap[ImGuiKey_Space] = WN_KEY_SPACE;
	io.KeyMap[ImGuiKey_Enter] = WN_KEY_ENTER;
	io.KeyMap[ImGuiKey_Escape] = WN_KEY_ESCAPE;
	io.KeyMap[ImGuiKey_A] = WN_KEY_A;
	io.KeyMap[ImGuiKey_C] = WN_KEY_C;
	io.KeyMap[ImGuiKey_V] = WN_KEY_V;
	io.KeyMap[ImGuiKey_X] = WN_KEY_X;
	io.KeyMap[ImGuiKey_Y] = WN_KEY_Y;
	io.KeyMap[ImGuiKey_Z] = WN_KEY_Z;

	io.SetClipboardTextFn = [](void* userdata, const char* text)
	{
		glfwSetClipboardString((GLFWwindow*)userdata, text);
	};
	io.GetClipboardTextFn = [](void* userdata)
	{
		return glfwGetClipboardString((GLFWwindow*)userdata);
	};

	ImGui_ImplOpenGL3_Init("#version 410");
}

void Walnut::ImGuiLayer::OnDetach()
{
}

void Walnut::ImGuiLayer::OnUpdate()
{
	ImGuiIO& io = ImGui::GetIO();

	Application& app = Application::Get();
	io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());
	float time = (float)glfwGetTime();
	io.DeltaTime = mTime > 0.0f ? (time - mTime) : (1.0f / 60.0f);
	mTime = time;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();


	static bool show = true;
	ImGui::ShowDemoWindow(&show);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Walnut::ImGuiLayer::OnEvent(Event & event)
{
	EventDispatcher dispatcher(event);

	dispatcher.Dispatch<MouseButtonPressedEvent>(WN_BIND_EVENT_FN(Walnut::ImGuiLayer::OnMouseButtonPressed));
	dispatcher.Dispatch<MouseButtonReleasedEvent>(WN_BIND_EVENT_FN(Walnut::ImGuiLayer::OnMouseButtonReleased));
	dispatcher.Dispatch<MouseMovedEvent>(WN_BIND_EVENT_FN(Walnut::ImGuiLayer::OnMouseMoved));
	dispatcher.Dispatch<MouseScrolledEvent>(WN_BIND_EVENT_FN(Walnut::ImGuiLayer::OnMouseScrolled));

	dispatcher.Dispatch<KeyPressedEvent>(WN_BIND_EVENT_FN(Walnut::ImGuiLayer::OnKeyPressed));
	dispatcher.Dispatch<KeyReleasedEvent>(WN_BIND_EVENT_FN(Walnut::ImGuiLayer::OnKeyReleased));
	dispatcher.Dispatch<KeyTypedEvent>(WN_BIND_EVENT_FN(Walnut::ImGuiLayer::OnKeyTyped));

	dispatcher.Dispatch<WindowResizeEvent>(WN_BIND_EVENT_FN(Walnut::ImGuiLayer::OnWindowResize));
}

bool Walnut::ImGuiLayer::OnMouseButtonPressed(MouseButtonPressedEvent & event)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[event.GetMouseButton()] = true;

	return false;
}

bool Walnut::ImGuiLayer::OnMouseButtonReleased(MouseButtonReleasedEvent & event)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[event.GetMouseButton()] = false;

	return false;
}

bool Walnut::ImGuiLayer::OnMouseMoved(MouseMovedEvent & event)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MousePos = ImVec2(event.GetX(), event.GetY());

	return false;
}

bool Walnut::ImGuiLayer::OnMouseScrolled(MouseScrolledEvent & event)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseWheelH += event.GetXOffset();
	io.MouseWheel += event.GetYOffset();

	return false;
}

bool Walnut::ImGuiLayer::OnKeyPressed(KeyPressedEvent & event)
{
	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[event.GetKeyCode()] = true;

	io.KeyCtrl = io.KeysDown[WN_KEY_LEFT_CONTROL] || io.KeysDown[WN_KEY_RIGHT_CONTROL];
	io.KeyShift = io.KeysDown[WN_KEY_LEFT_SHIFT] || io.KeysDown[WN_KEY_RIGHT_SHIFT];
	io.KeyAlt = io.KeysDown[WN_KEY_LEFT_ALT] || io.KeysDown[WN_KEY_RIGHT_ALT];
	io.KeySuper = io.KeysDown[WN_KEY_LEFT_SUPER] || io.KeysDown[WN_KEY_RIGHT_SUPER];
	return false;
}

bool Walnut::ImGuiLayer::OnKeyReleased(KeyReleasedEvent & event)
{
	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[event.GetKeyCode()] = false;

	return false;
}

bool Walnut::ImGuiLayer::OnKeyTyped(KeyTypedEvent & event)
{
	ImGuiIO& io = ImGui::GetIO();
	int keyCode = event.GetKeyCode();

	if (keyCode > 0 && keyCode < 0x10000)
		io.AddInputCharacter((unsigned short)keyCode);

	return false;
}

bool Walnut::ImGuiLayer::OnWindowResize(WindowResizeEvent & event)
{
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(event.GetWidth(), event.GetHeight());
	io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
	glViewport(0, 0, event.GetWidth(), event.GetHeight());
	return false;
}
