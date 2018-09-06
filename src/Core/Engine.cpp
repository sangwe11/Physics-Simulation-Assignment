#include "Engine.h"

#include <exception>

Engine &Engine::instance()
{
	static Engine instance;

	return instance;
}

Engine::Engine()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		throw std::exception("SDL init failed.");
	}
}

Engine::~Engine()
{
	SDL_Quit();
}

void Engine::generate_meshes()
{
	// Preload simple mesh shapes
	std::vector<unsigned int> indicies;
	std::vector<glm::vec3> vertices;

	// Primative for rendering a single axis (the X axis), this can be rotated to draw other axis
	indicies = { 0, 1, 0 };
	vertices.clear();

	vertices.emplace_back(0.0f, 0.0f, 0.0f);
	vertices.emplace_back(1.0f, 0.0f, 0.0f);

	m_meshes[MeshPrimiative::AXIS] = std::make_unique<Mesh>(vertices, indicies, shader("shaders/physx_debug"));

	// Basic 1 x 1 x 1 cube mesh
	indicies = { 0, 2, 3, 3, 1, 3, 2, 3, 4, 4, 3, 5, 4, 5, 6, 6, 5, 7, 6, 7, 0, 0, 7, 1, 1, 7, 3, 3, 7, 5, 6, 0, 4, 4, 0, 2 };
	vertices.clear();

	vertices.emplace_back(-1, -1, 1);
	vertices.emplace_back(1, -1, 1);
	vertices.emplace_back(-1, 1, 1);
	vertices.emplace_back(1, 1, 1);
	vertices.emplace_back(-1, 1, -1);
	vertices.emplace_back(1, 1, -1);
	vertices.emplace_back(-1, -1, -1);
	vertices.emplace_back(1, -1, -1);

	m_meshes[MeshPrimiative::CUBE] = std::make_unique<Mesh>(vertices, indicies, shader("shaders/physx_debug"));

	// Basic sphere mesh, resolution of 10 and a radius of 1.0f
	vertices.clear();

	int res = 10;
	float radius = 1.0f;
	float x1, y1, x2, y2, z1, z2;
	float inc1, inc2, inc3, inc4, radius1, radius2;

	for (int w = 0; w < res; w++)
	{
		for (int h = (-res / 2); h < (res / 2); h++)
		{
			inc1 = (w / (float)res) * 2 * glm::pi<float>();
			inc2 = ((w + 1) / (float)res) * 2 * glm::pi<float>();

			inc3 = (h / (float)res) * glm::pi<float>();
			inc4 = ((h + 1) / (float)res) * glm::pi<float>();

			x1 = sin(inc1);
			y1 = cos(inc1);
			x2 = sin(inc2);
			y2 = cos(inc2);

			radius1 = radius * cos(inc3);
			radius2 = radius  * cos(inc4);

			z1 = radius * sin(inc3);
			z2 = radius * sin(inc4);

			vertices.push_back(glm::vec3(radius1 * x1, z1, radius1 * y1));
			vertices.push_back(glm::vec3(radius1 * x2, z1, radius1 * y2));
			vertices.push_back(glm::vec3(radius2 * x2, z2, radius2 * y2));
			vertices.push_back(glm::vec3(radius1 * x1, z1, radius1 * y1));
			vertices.push_back(glm::vec3(radius2 * x2, z2, radius2 * y2));
			vertices.push_back(glm::vec3(radius2 * x1, z2, radius2 * y1));
		}

	}

	m_meshes[MeshPrimiative::SPHERE] = std::make_unique<Mesh>(vertices, shader("shaders/physx_debug"));

	// Basic 1 x 1 plane mesh
	indicies = { 0, 1, 3, 3, 1, 2 };
	vertices.clear();

	vertices.emplace_back(-1.0f, 0.0f, -1.0f);
	vertices.emplace_back(-1.0f, 0.0f, 1.0f);
	vertices.emplace_back(1.0f, 0.0f, 1.0f);
	vertices.emplace_back(1.0f, 0.0f, -1.0f);

	m_meshes[MeshPrimiative::PLANE] = std::make_unique<Mesh>(vertices, indicies, shader("shaders/physx_debug"));
}

Time &Engine::time()
{
	return m_time;
}

Input &Engine::input()
{
	return m_input;
}

Physics &Engine::physics()
{
	return m_physics;
}

Display &Engine::display(const std::string &title, const unsigned int &width, const unsigned int &height)
{
	m_displays.push_back(std::make_unique<Display>(width, height, title));

	return *m_displays[m_displays.size() - 1];
}

Display &Engine::display(const unsigned int &id)
{
	if (id > m_displays.size() - 1)
	{
		throw std::runtime_error("Display " + std::to_string(id) + " does not exist");
	}

	return *m_displays[id];
}

Shader &Engine::shader(const std::string &name)
{
	if (m_shaders.find(name) == m_shaders.end())
	{
		m_shaders[name] = std::make_unique<Shader>(name);
	}

	return *m_shaders[name];
}

Model &Engine::load_model(const std::string &model_name)
{
	if (m_models.find(model_name) == m_models.end())
	{
		throw std::exception("Model has not been loaded.");
	}

	return *m_models[model_name];
}

Model &Engine::load_model(const std::string &name, const Shader &shader)
{
	// Model name is a combination of the shader name and the model name.
	// This allows for flexibility of using the same model with multiple different shaders
	std::string model_name = shader.name() + name;

	if (m_models.find(model_name) == m_models.end())
	{
		m_models[model_name] = std::make_unique<Model>(name, shader);
	}

	return *m_models[model_name];
}

Mesh &Engine::basic_mesh(const MeshPrimiative &primative)
{
	if (m_meshes.size() <= 0)
	{
		generate_meshes();
	}

	return *m_meshes[primative];
}

void Engine::update_objects()
{
	for (std::unique_ptr<NObject> &object : m_objects)
	{
		//object->update(m_time.delta_time());
	}
}

void Engine::render_objects(const RenderMode& mode, const Camera &camera)
{
	if (mode == RenderMode::NORMAL || mode == RenderMode::BOTH)
	{
		for (std::unique_ptr<NObject> &object : m_objects)
		{
			object->draw(camera);
		}
	}

	if (mode == RenderMode::DEBUG || mode == RenderMode::BOTH)
	{
		glDisable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// Draw object "collison" meshes
		for (std::unique_ptr<NObject> &object : m_objects)
		{
			object->debug_draw(camera);
		}

		// Draw object axis
		glLineWidth(2.0f);

		for (std::unique_ptr<NObject> &object : m_objects)
		{
			object->axis_draw(camera, 1.0f);
		}

		glLineWidth(1.0f);

		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}