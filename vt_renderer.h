#pragma once

#include "vt_window.h"
#include "vt_device.h"
#include "vt_swap_chain.h"

#include <memory>
#include <vector>
#include <cassert>

namespace vt {
	class VtRenderer {
	public:
		VtRenderer(VtWindow& window, VtDevice& device);
		~VtRenderer();

		VtRenderer(const VtRenderer&) = delete;
		VtRenderer& operator=(const VtRenderer&) = delete;

		VkRenderPass GetSwapChainRenderPass() const { return vtSwapChain->getRenderPass(); }
		float GetAspectRatio() const { return vtSwapChain->extentAspectRatio(); }
		bool isFrameInProgess() const { return isFrameStarted; }

		VkCommandBuffer GetCurrentCommandBuffer() const
		{
			assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
			return commandBuffers[currentFrameIndex];
		}

		int GetFrameIndex() const
		{
			assert(isFrameStarted && "Cannot get frame index when frame not in progress");
			return currentFrameIndex;
		}

		VkCommandBuffer BeginFrame();
		void EndFrame();
		void BeginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void EndSwapChainRenderPass(VkCommandBuffer commandBuffer);

	private:
		VtWindow& vtWindow;
		VtDevice& vtDevice;
		std::unique_ptr<VtSwapChain> vtSwapChain;
		std::vector<VkCommandBuffer> commandBuffers;

		uint32_t currentImageIndex;
		int currentFrameIndex;
		bool isFrameStarted{false};

		void CreateCommandBuffers();
		void FreeCommandBuffers();
		void RecreateSwapChain();
	};
}