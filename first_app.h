#pragma once

#include "vt_window.h"
#include "vt_device.h"
#include "vt_game_object.h"
#include "vt_renderer.h"
#include "vt_descriptors.h"
#include "vt_image.h"

#include <memory>
#include <vector>

namespace vt {
	class FirstApp {
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		FirstApp();
		~FirstApp();

		FirstApp(const FirstApp&) = delete;
		FirstApp& operator=(const FirstApp&) = delete;

		void run();
	private:
		VtWindow vtWindow{ WIDTH,HEIGHT, "Vulkan Window" };
		VtDevice vtDevice{ vtWindow };
		VtRenderer vtRenderer{ vtWindow, vtDevice };

		std::unique_ptr<VtDescriptorPool> globalPool{};
		VtGameObject::Map gameObjects;
		std::unique_ptr<VtImage> globalTexture;

		void LoadGameObjects();
		void LoadTextures();
	};
}