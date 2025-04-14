#pragma once
#include "vt_device.h"

#include <memory>

namespace vt
{
	class VtImage 
	{
	public:
		VtImage(VtDevice& device);
		~VtImage();

		VtImage(const VtImage&) = delete;
		VtImage& operator=(const VtImage&) = delete;

		static std::unique_ptr<VtImage> CreateVtImage(VtDevice& device, const std::string& filepath);

		void CreateTextureImage(const std::string& filepath);
		void CreateTextureImageView();
		void CreateTextureSampler();

		VkDescriptorImageInfo GetDescriptorInfo();
	private:
		VtDevice& vtDevice;

		uint32_t mipLevels;
		VkImage textureImage;
		VkDeviceMemory textureImageMemory;
		VkDeviceSize textureImageDeviceSize;
		VkImageView textureImageView;
		VkSampler textureSampler;
	};
}