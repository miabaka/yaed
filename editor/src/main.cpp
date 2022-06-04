#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <GL/gl3w.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "EditorApplication.hpp"

int main() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(1280, 720, "Ayaya! Ayaya!", nullptr, nullptr);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	gl3wInit();

	ImGui::CreateContext();

	{
		ImGuiIO &io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	}

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");

	{
		EditorApplication editor;

		try { editor.openWorld("data/packs/kids/0.thp"); } catch (const std::exception &) {}
		try { editor.openWorld("data/packs/kids/1.thp"); } catch (const std::exception &) {}
		try { editor.openWorld("data/packs/kids/2.thp"); } catch (const std::exception &) {}
		try { editor.openWorld("data/packs/kids/3.thp"); } catch (const std::exception &) {}
		try { editor.openWorld("data/packs/standard/0.thp"); } catch (const std::exception &) {}
		try { editor.openWorld("data/packs/standard/1.thp"); } catch (const std::exception &) {}
		try { editor.openWorld("data/packs/standard/2.thp"); } catch (const std::exception &) {}
		try { editor.openWorld("data/packs/standard/3.thp"); } catch (const std::exception &) {}

		while (true) {
			glfwPollEvents();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGui::DockSpaceOverViewport();

			if (!editor.update(glfwWindowShouldClose(window)))
				break;

			ImGui::Render();

			glClear(GL_COLOR_BUFFER_BIT);

			editor.render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			{
				GLFWwindow *previousContext = glfwGetCurrentContext();

				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();

				glfwMakeContextCurrent(previousContext);
			}

			glfwSwapBuffers(window);
		}
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();

	ImGui::DestroyContext();

	glfwTerminate();

	return 0;
}