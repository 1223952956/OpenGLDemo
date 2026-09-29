#pragma once
#include <string>
#include <memory>
#include "nlohmann/json.hpp"


class Scene;
class Piece;
class PieceComponent;
class SceneComponent;
class PrimitiveComponent;
class ModelComponent;

namespace Math
{
	class Vec3;
	class Quat;
	class Transform;
}

class SceneSerializer
{
public:

	static bool Serialize(Scene* scene, const std::string& filepath);
	static std::unique_ptr<Scene> Deserialize(const std::string& filepath, std::string& error);

private:
	static nlohmann::json SerializeVec3(const Math::Vec3& vec);
	static Math::Vec3 DeserializeVec3(const nlohmann::json& json);
	static nlohmann::json SerializeQuat(const Math::Quat& quat);
	static Math::Quat DeserializeQuat(const nlohmann::json& json);
	static nlohmann::json SerializeTransform(const Math::Transform& transform);
	static Math::Transform DeserializeTransform(const nlohmann::json& json);
	static std::string GetComponentType(const PieceComponent* component);
	static nlohmann::json SerializeComponent(const PieceComponent* component);

	static void DeserializeModelComponent(const nlohmann::json& json, ModelComponent& component);
	static void DeserializePrimitiveComponent(const nlohmann::json& json, PrimitiveComponent& component);
	static void DeserializeSceneComponent(const nlohmann::json& json, SceneComponent& component);
	static void DeserializeComponent(const nlohmann::json& json, Piece& piece);

	static nlohmann::json SerializePiece(Piece* piece);
	static uint64_t DeserializeUUID(const nlohmann::json& json);
	static void DeserializePiece(const nlohmann::json& json, Scene* scene);

};

