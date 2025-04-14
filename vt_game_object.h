#pragma once

#include "vt_model.h"

#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include <unordered_map>

namespace vt {

	struct TransformComponent 
	{
		glm::vec3 translation{};
		glm::vec3 scale{ 1.f, 1.f, 1.f};
		glm::vec3 rotation{};

		// Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
		// Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
		// https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
		glm::mat4 mat4();

		glm::mat3 normalMatrix();
	};

	struct PointLightComponent
	{
		float lightIntensity = 1.f;
	};

	class VtGameObject
	{
	public:
		using id_t = unsigned int;
		using Map = std::unordered_map<id_t, VtGameObject>;

		static VtGameObject CreateGameObject()
		{
			static id_t currentId = 0;
			return VtGameObject{ currentId++ };
		}

		static VtGameObject MakePointLight(float intensity = 10.f, float radius = 0.1f, glm::vec3 color = glm::vec3(1.f));

		VtGameObject(const VtGameObject&) = delete;
		VtGameObject& operator=(const VtGameObject&) = delete;
		VtGameObject(VtGameObject&&) = default;
		VtGameObject& operator=(VtGameObject&&) = default;

		const id_t GetId() { return id; }

		glm::vec3 color{};
		TransformComponent transform{};

		// optional
		std::shared_ptr<VtModel> model;
		std::unique_ptr<PointLightComponent> pointLight = nullptr;

	private:
		VtGameObject(id_t objId) : id{objId} {}
		id_t id;
	};
}