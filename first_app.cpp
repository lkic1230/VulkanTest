#include "first_app.h"

#include "vt_buffer.h"
#include "vt_camera.h"
#include "simple_render_system.h"
#include "point_light_system.h"
#include "keyboard_movement_controller.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>
#include <chrono>

namespace vt {

	FirstApp::FirstApp()
	{
		globalPool = VtDescriptorPool::Builder(vtDevice)
			.setMaxSets(VtSwapChain::MAX_FRAMES_IN_FLIGHT)
			.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VtSwapChain::MAX_FRAMES_IN_FLIGHT)
			.addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VtSwapChain::MAX_FRAMES_IN_FLIGHT)
			.build();
		LoadGameObjects();
		LoadTextures();
	}

	FirstApp::~FirstApp()
	{
		
	}

	void FirstApp::run()
	{
		std::vector<std::unique_ptr<VtBuffer>> uboBuffers(VtSwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < uboBuffers.size(); i++)
		{
			uboBuffers[i] = std::make_unique<VtBuffer>(
				vtDevice,
				sizeof(GlobalUbo),
				1,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
			);
			uboBuffers[i]->map();
		}

		auto globalSetLayout = VtDescriptorSetLayout::Builder(vtDevice)
			.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
			.addBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
			.build();

		std::vector<VkDescriptorSet> globalDescriptorSets(VtSwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < globalDescriptorSets.size(); i++)
		{
			auto bufferInfo = uboBuffers[i]->descriptorInfo();
			auto imageInfo = globalTexture->GetDescriptorInfo();
			VtDescriptorWriter(*globalSetLayout, *globalPool)
				.writeBuffer(0, &bufferInfo)
				.writeImage(1, &imageInfo)
				.build(globalDescriptorSets[i]);
		}

		SimpleRenderSystem simpleRenderSystem(vtDevice, vtRenderer.GetSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout());
		PointLightSystem pointLightSystem(vtDevice, vtRenderer.GetSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout());
		VtCamera camera{};

		auto viewObject = VtGameObject::CreateGameObject();
		viewObject.transform.translation.z = -2.0f; // 相机跟踪点坐标
		KeyboardMovementController cameraController{};

		auto currentTime = std::chrono::high_resolution_clock::now();

		while (!vtWindow.ShouldClose()) 
		{
			glfwPollEvents();

			auto newTime = std::chrono::high_resolution_clock::now();
			float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
			currentTime = newTime;

			cameraController.MoveInPlaneXZ(vtWindow.GetWindow(), frameTime, viewObject);
			camera.SetViewYXZ(viewObject.transform.translation, viewObject.transform.rotation);

			float aspect = vtRenderer.GetAspectRatio();
			//camera.SetOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
			camera.SetPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 1000.f);
			
			if (auto commandBuffer = vtRenderer.BeginFrame())
			{
				int frameIndex = vtRenderer.GetFrameIndex();
				FrameInfo frameInfo{
					frameIndex,
					frameTime,
					commandBuffer,
					camera,
					globalDescriptorSets[frameIndex],
					gameObjects
				};

				// update
				GlobalUbo ubo{};
				ubo.projection = camera.GetProjection();
				ubo.view = camera.GetView();
				ubo.inverseView = camera.GetInverseView();
				pointLightSystem.Update(frameInfo, ubo);
				uboBuffers[frameIndex]->writeToBuffer(&ubo);
				uboBuffers[frameIndex]->flush();

				// render
				vtRenderer.BeginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.RenderGameObjects(frameInfo);
				pointLightSystem.Render(frameInfo);
				vtRenderer.EndSwapChainRenderPass(commandBuffer);
				vtRenderer.EndFrame();
			}
		}

		// 阻塞CPU直到所有GPU操作完成
		vkDeviceWaitIdle(vtDevice.device());
	}

	void FirstApp::LoadGameObjects()
	{
		std::shared_ptr<VtModel> vtModel = VtModel::CreateModelFromFile(vtDevice, "D:\\Vulkan Sample\\VulkanTest\\models\\smooth_vase.obj");
		auto smoothVase = VtGameObject::CreateGameObject();
		smoothVase.model = vtModel;
		smoothVase.transform.translation = { .0f, .25f, 0.f };
		smoothVase.transform.scale = glm::vec3(2.f, 1.f, 2.f);
		gameObjects.emplace(smoothVase.GetId(), std::move(smoothVase));

		vtModel = VtModel::CreateModelFromFile(vtDevice, "D:\\Vulkan Sample\\VulkanTest\\models\\quad.obj");
		auto floor = VtGameObject::CreateGameObject();
		floor.model = vtModel;
		floor.transform.translation = { .0f, .25f, 0.f };
		floor.transform.scale = glm::vec3(5.f, 1.f, 5.f);
		gameObjects.emplace(floor.GetId(), std::move(floor));

		std::vector<glm::vec3> lightColors{
			{1.f, .1f, .1f},
			{.1f, .1f, 1.f},
			{.1f, 1.f, .1f},
			{1.f, 1.f, .1f},
			{.1f, 1.f, 1.f},
			{1.f, 1.f, 1.f}  //
		};

		for (int i = 0; i < lightColors.size(); i++)
		{
			auto pointLight = VtGameObject::MakePointLight(.2f);
			pointLight.color = lightColors[i];
			auto rotateLight = glm::rotate(
				glm::mat4(1.f),
				(i * glm::two_pi<float>()) / lightColors.size(),
				{ 0.f, -1.f, 0.f });
			pointLight.transform.translation = glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, 1.f));
			gameObjects.emplace(pointLight.GetId(), std::move(pointLight));
		}
	}

	void FirstApp::LoadTextures()
	{
		globalTexture = VtImage::CreateVtImage(vtDevice, "D:\\Vulkan Sample\\VulkanTest\\textures\\texture.jpg");
	}
}