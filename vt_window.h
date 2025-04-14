#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace vt {
	class VtWindow{
	public:
		VtWindow(int w, int h, std::string name);
		~VtWindow();

		// 禁止拷贝和赋值操作
		VtWindow(const VtWindow&) = delete;
		VtWindow& operator=(const VtWindow&) = delete;
		/*
		* 可以使用以下方法转移所有权。移动操作会将资源从 other 转移到当前对象，并置空 other 的资源指针，避免重复释放
		* 
		* VtWindow(VtWindow&& other) noexcept;            // 移动构造函数
		* VtWindow& operator=(VtWindow&& other) noexcept; // 移动赋值运算符
		* 
		* 也可以使用unique_ptr，会自动处理资源释放，且默认禁止拷贝，但需要自定义删除器:
		* struct GLFWwindowDeleter {
			void operator()(GLFWwindow* window) const {
			glfwDestroyWindow(window);
			}
		  };
		  std::unique_ptr<GLFWwindow, GLFWwindowDeleter> window;
		*/

		bool ShouldClose() { return glfwWindowShouldClose(window); }
		VkExtent2D GetExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }
		bool WasWindowResized() { return framebufferResized; }
		void ResetWindowResizedFlag() { framebufferResized = false; }
		GLFWwindow *GetWindow() { return window; }

		void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	private:
		GLFWwindow* window;
		std::string windowName;

		int width;
		int height;
		bool framebufferResized = false;

		void InitWindow();
		static void FramebufferResizeCallback(GLFWwindow* window, int width, int height);
	};
}