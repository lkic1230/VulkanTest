#pragma once

#include "vt_camera.h"
#include "vt_pipeline.h"
#include "vt_device.h"
#include "vt_game_object.h"
#include "vt_frame_info.h"

#include <memory>
#include <vector>

namespace vt {
	class PointLightSystem {
	public:
		PointLightSystem(VtDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		~PointLightSystem();

		PointLightSystem(const PointLightSystem&) = delete;
		PointLightSystem& operator=(const PointLightSystem&) = delete;

		void Update(FrameInfo& frameInfo, GlobalUbo& ubo);
		void Render(FrameInfo& frameInfo);
	private:
		VtDevice& vtDevice;

		std::unique_ptr<VtPipeline> vtPipeline;
		VkPipelineLayout pipelineLayout;

		void CreatePipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void CreatePipeline(VkRenderPass renderPass);
	};
}