#pragma once

#include "vt_camera.h"
#include "vt_pipeline.h"
#include "vt_device.h"
#include "vt_game_object.h"
#include "vt_frame_info.h"

#include <memory>
#include <vector>

namespace vt {
	class SimpleRenderSystem {
	public:
		SimpleRenderSystem(VtDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

		void RenderGameObjects(FrameInfo &frameInfo);
	private:
		VtDevice& vtDevice;

		std::unique_ptr<VtPipeline> vtPipeline;
		VkPipelineLayout pipelineLayout;

		void CreatePipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void CreatePipeline(VkRenderPass renderPass);
	};
}