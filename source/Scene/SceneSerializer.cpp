#include "SceneSerializer.h"
#include "Scene.h"
#include <fstream>
#include <iostream>
#include "spdlog/spdlog.h"
#include "Components/ModelComponent.h"
#include "Math/GlmInterop.h"

using JSON = nlohmann::json;

bool SceneSerializer::Serialize(Scene* scene, const std::string& filepath)
{
	JSON jsonScene;

	jsonScene["version"] = 1;
	jsonScene["name"] = "ChessScene";
	jsonScene["environment"] = {
		{"hdrPath", scene->Environment->SourcePath},
	};
	jsonScene["camera"] = {
		{"position", {scene->MainCamera->Pos.x, scene->MainCamera->Pos.y, scene->MainCamera->Pos.z}},
		{"fov", scene->MainCamera->GetFoV()},
	};

	jsonScene["lights"] = JSON::array();

	for (auto& dirLight : scene->DirectionalLights)
	{
		jsonScene["lights"].push_back({
			{"type", "directional"},
			{"direction", SerializeVec3(dirLight.GetDirection())},
			{"color", SerializeVec3(dirLight.GetColor())},
			{"intensity", dirLight.GetIntensity()},
			});
	}

	for (auto& pointLight : scene->PointLights)
	{
		jsonScene["lights"].push_back({
			{"type", "point"},
			{"position", SerializeVec3(pointLight.GetPosition())},
			{"color", SerializeVec3(pointLight.GetColor())},
			{"range", pointLight.GetRange()},
			{"intensity", pointLight.GetIntensity()},
			});
	}

	for (auto& spotLight : scene->SpotLights)
	{
		jsonScene["lights"].push_back({
			{"type", "spot"},
			{"position", SerializeVec3(spotLight.GetPosition())},
			{"direction", SerializeVec3(spotLight.GetDirection())},
			{"color", SerializeVec3(spotLight.GetColor())},
			{"innerCos", spotLight.GetInnerCos()},
			{"outerCos", spotLight.GetOuterCos()},
			{"range", spotLight.GetRange()},
			{"intensity", spotLight.GetIntensity()},
			});
	}

	jsonScene["pieces"] = JSON::array();

	for (auto& piece : scene->Pieces)
	{
		jsonScene["pieces"].push_back(SerializePiece(piece.get()));
	}

	std::ofstream output(filepath);
	output << jsonScene.dump(4);

	spdlog::info("Scene serialized to '{}'", filepath);

	return true;
}

std::unique_ptr<Scene> SceneSerializer::Deserialize(const std::string& filepath, std::string& error)
{
	std::ifstream input(filepath);
	if (!input.is_open())
	{
		error = "Failed to open file: " + filepath;
		return nullptr;
	}

	try
	{
		JSON jsonScene;
		input >> jsonScene;

		if (jsonScene.at("version").get<int>() != 1)
		{
			error = "Unsupported scene version";
			return nullptr;
		}

		auto scene = std::make_unique<Scene>();

		scene->EnvironmentPath = jsonScene.at("environment").at("hdrPath").get<std::string>();

		scene->MainCamera = std::make_unique<Camera>();
		scene->MainCamera->Pos = Math::ToGlm(DeserializeVec3(jsonScene.at("camera").at("position")));
		scene->MainCamera->SetFoV(jsonScene.at("camera").at("fov").get<float>());

		for (const auto& lightJson : jsonScene.at("lights"))
		{
			std::string type = lightJson.at("type").get<std::string>();
			if (type == "directional")
			{
				Math::Vec3 direction = DeserializeVec3(lightJson.at("direction"));
				Math::Vec3 color = DeserializeVec3(lightJson.at("color"));
				float intensity = lightJson.at("intensity").get<float>();
				scene->DirectionalLights.emplace_back(direction, color, intensity);
			}
			else if (type == "point")
			{
				Math::Vec3 position = DeserializeVec3(lightJson.at("position"));
				Math::Vec3 color = DeserializeVec3(lightJson.at("color"));
				float range = lightJson.at("range").get<float>();
				float intensity = lightJson.at("intensity").get<float>();
				scene->PointLights.emplace_back(position, color, range, intensity);
			}
			else if (type == "spot")
			{
				Math::Vec3 position = DeserializeVec3(lightJson.at("position"));
				Math::Vec3 direction = DeserializeVec3(lightJson.at("direction"));
				Math::Vec3 color = DeserializeVec3(lightJson.at("color"));
				float innerCos = lightJson.at("innerCos").get<float>();
				float outerCos = lightJson.at("outerCos").get<float>();
				float range = lightJson.at("range").get<float>();
				float intensity = lightJson.at("intensity").get<float>();
				scene->SpotLights.emplace_back(position, direction, color, innerCos, outerCos, range, intensity);
			}
			else
			{
				spdlog::warn("Unknown light type: {}", type);
			}
		}

		for (const auto& pieceJson : jsonScene.at("pieces"))
		{
			DeserializePiece(pieceJson, scene.get());
		}

		return scene;
	}
	catch (const std::exception& e)
	{
		error = e.what();
		return nullptr;
	}
}

JSON SceneSerializer::SerializeVec3(const Math::Vec3& vec)
{
	return JSON::array({ vec.x, vec.y, vec.z });
}

Math::Vec3 SceneSerializer::DeserializeVec3(const JSON& json)
{
	if (!json.is_array() || json.size() != 3)
		throw std::runtime_error("Expected Vec3 array");

	return Math::Vec3(
		json.at(0).get<float>(),
		json.at(1).get<float>(),
		json.at(2).get<float>()
	);
}

JSON SceneSerializer::SerializeQuat(const Math::Quat& quat)
{
	return JSON::array({ quat.x, quat.y, quat.z, quat.w });
}

Math::Quat SceneSerializer::DeserializeQuat(const JSON& json)
{
	if (!json.is_array() || json.size() != 4)
		throw std::runtime_error("Expected Quat array");

	return Math::Quat(
		json.at(3).get<float>(),
		json.at(0).get<float>(),	
		json.at(1).get<float>(),
		json.at(2).get<float>()
	);
}

JSON SceneSerializer::SerializeTransform(const Math::Transform& transform)
{
	return {
		{
			"position", SerializeVec3(transform.Position)
		},
		{
			"rotation", SerializeQuat(transform.Rotation.GetQuaternion())
		},
		{
			"scale", SerializeVec3(transform.Scale)
		}
	};
}

Math::Transform SceneSerializer::DeserializeTransform(const JSON& json)
{
	Math::Transform transform;

	transform.Position = DeserializeVec3(json.at("position"));
	transform.Rotation = Math::Quat(DeserializeQuat(json.at("rotation")));
	transform.Scale = DeserializeVec3(json.at("scale"));

	return transform;
}

std::string SceneSerializer::GetComponentType(const PieceComponent* component)
{
	if (dynamic_cast<const ModelComponent*>(component))
	{
		return "ModelComponent";
	}
	if (dynamic_cast<const PrimitiveComponent*>(component))
	{
		return "PrimitiveComponent";
	}
	if (dynamic_cast<const SceneComponent*>(component))
	{
		return "SceneComponent";
	}


	return "PieceComponent";
}

JSON SceneSerializer::SerializeComponent(const PieceComponent* component)
{
	JSON result = {
		{"id", component->GetID().ToString()},
		{"type", GetComponentType(component)},
	};

	if (const SceneComponent* sceneComp = dynamic_cast<const SceneComponent*>(component))
	{
		result["transform"] = SerializeTransform(sceneComp->GetLocalTransform());
		const SceneComponent* parent = sceneComp->GetParent();

		if (parent)
		{
			result["parent"] = parent->GetID().ToString();
		}
		else
		{
			result["parent"] = nullptr;
		}
	}

	if (const PrimitiveComponent* primComp = dynamic_cast<const PrimitiveComponent*>(component))
	{
		result["visible"] = primComp->IsVisible();
		result["castShadow"] = primComp->IsCastShadow();
	}

	if (const ModelComponent* modelComp = dynamic_cast<const ModelComponent*>(component))
	{
		result["modelPath"] = modelComp->GetModel()->GetSourcePath();
	}

	return result;
}

void SceneSerializer::DeserializeModelComponent(const JSON& json, ModelComponent& component)
{
	const std::string modelPath = json.at("modelPath").get<std::string>();
	auto model = std::make_shared<Model>(modelPath.c_str());
	component.SetMesh(model);

	DeserializePrimitiveComponent(json, component);
}

void SceneSerializer::DeserializePrimitiveComponent(const JSON& json, PrimitiveComponent& component)
{
	const bool visible = json.at("visible").get<bool>();
	const bool castShadow = json.at("castShadow").get<bool>();

	component.SetVisible(visible);
	component.SetCastShadow(castShadow);

	DeserializeSceneComponent(json, component);
}

void SceneSerializer::DeserializeSceneComponent(const JSON& json, SceneComponent& component)
{
	const Math::Transform transform = DeserializeTransform(json.at("transform"));
	component.SetLocalTransform(transform);

	//TODO : Handle parent-child relationships after all pieces are deserialized
}

void SceneSerializer::DeserializeComponent(const JSON& json, Piece& piece)
{
	const UUID componentID(DeserializeUUID(json.at("id")));

	const std::string type = json.at("type").get<std::string>();

	if (type == "ModelComponent")
	{
		auto& modelComp = piece.AddComponent<ModelComponent>(componentID);
		DeserializeModelComponent(json, modelComp);
	}
	else if (type == "PrimitiveComponent")
	{
		auto& primComp = piece.AddComponent<PrimitiveComponent>(componentID);
		DeserializePrimitiveComponent(json, primComp);
	}
	else if (type == "SceneComponent")
	{
		auto& sceneComp = piece.AddComponent<SceneComponent>(componentID);
		DeserializeSceneComponent(json, sceneComp);
	}
	else if (type == "PieceComponent")
	{
		auto& pieceComp = piece.AddComponent<PieceComponent>(componentID);
	}
	else
	{
		spdlog::warn("Unknown component type: {}", type);
	}
}

JSON SceneSerializer::SerializePiece(Piece* piece)
{
	JSON result = {
		{"id", piece->GetID().ToString()},
		{"rootComponent", piece->GetRootComponent()->GetID().ToString()},
		{"components", JSON::array()},
	};

	for (const auto* component : piece->GetComponents<PieceComponent>())
	{
		result["components"].push_back(SerializeComponent(component));
	}

	return result;
}

uint64_t SceneSerializer::DeserializeUUID(const nlohmann::json& json)
{
	const std::string& uuidStr = json.get<std::string>();
	const uint64_t value = std::stoull(uuidStr);

	if (value == 0)
	{
		throw std::runtime_error("Invalid UUID string: " + uuidStr);
	}

	return value;
}

void SceneSerializer::DeserializePiece(const nlohmann::json& json, Scene* scene)
{
	const UUID pieceID(DeserializeUUID(json.at("id")));
	Piece& piece = scene->CreatePiece(pieceID);

	for (const JSON& compJson : json.at("components"))
	{
		DeserializeComponent(compJson, piece);
	}

	const UUID rootCompID(DeserializeUUID(json.at("rootComponent")));

	for (auto* component : piece.GetComponents<PieceComponent>())
	{
		if (component->GetID() == rootCompID)
		{
			piece.SetRootComponent(component);
			break;
		}
	}
}


