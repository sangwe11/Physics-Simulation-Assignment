#ifndef _ENGINE_H
#define _ENGINE_H

#include <memory>

#include "../Config.h"
#include "Time.h"
#include "../Rendering/Display.h"
#include "Input.h"
#include "../Physics/Physics.h"
#include "Object.h"
#include "../Rendering/Camera.h"
#include "../Rendering/Shader.h"

enum RenderMode
{
	NORMAL,
	DEBUG,
	BOTH
};

enum MeshPrimiative
{
	AXIS,
	CUBE,
	SPHERE,
	PLANE,
};

class Engine
{
public:
	Engine(Engine const &) = delete;
	void operator=(Engine const &) = delete;

	static Engine& instance();

	Time &time();
	Input &input();	
	Display &display(const unsigned int &id);
	Physics &physics();
	//Audio &audio();

	Display &display(const std::string &title, const unsigned int &width, const unsigned int &height);

	Shader &shader(const std::string &name);
	Model &load_model(const std::string &name, const Shader &shader);
	Model &load_model(const std::string &name);

	Mesh &basic_mesh(const MeshPrimiative &primative);
	
	template <typename T, typename ... Args>
	T &create_object(Args &&... args);

	void update_objects();
	void render_objects(const RenderMode& mode, const Camera &camera);

private:
	Engine();
	~Engine();

	void generate_meshes();

	Time m_time;
	Input m_input;
	Physics m_physics;
	//Audio m_audio;

	std::vector<std::unique_ptr<Display>> m_displays;
	std::map<std::string, std::unique_ptr<Shader>> m_shaders;

	std::vector<std::unique_ptr<NObject>> m_objects;

	float m_accumulator = 0.0f;

	std::map<std::string, std::unique_ptr<Model>> m_models;
	std::map<MeshPrimiative, std::unique_ptr<Mesh>> m_meshes;
};

template <typename T, typename ... Args>
T &Engine::create_object(Args &&... args)
{
	static_assert(std::is_base_of<NObject, T>::value, "T must be derived from NObject.");

	m_objects.push_back(std::make_unique<T>(std::forward<Args>(args) ...));

	return *dynamic_cast<T *>(m_objects[m_objects.size() - 1].get());
}

#endif