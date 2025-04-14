#pragma once

#include "vt_camera.h"
#include "vt_game_object.h"

#include <vulkan/vulkan.h>

namespace vt {

#define MAX_LIGHTS 10

	struct PointLight 
	{
		glm::vec4 position{};
		glm::vec4 color{};
	};

	struct GlobalUbo 
	{
		glm::mat4 projection{ 1.f };
		glm::mat4 view{ 1.f };
		glm::mat4 inverseView{ 1.f };
		glm::vec4 ambientLightColor{ 1.f, 1.f, 1.f, .02f };
		PointLight pointLights[MAX_LIGHTS];
		int numLights;
	};

	struct FrameInfo
	{
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		VtCamera& camera;
		VkDescriptorSet globalDescriptorSets;
		VtGameObject::Map& gameObjects;
	};
}