#include "PhysicsActor.h"
#include "../Core/Engine.h"

/********** Base Physics actor class *********/

PhysicsActor::PhysicsActor(const Transform &transform)
{
	m_transform = transform;
}

void PhysicsActor::name(const std::string &name)
{
	m_name = name;
	m_actor->setName(m_name.c_str());
}

/********** Rigid Physics actor class *********/

PhysicsRigidActor::PhysicsRigidActor(const Transform &transform)
: PhysicsActor(transform)
{

}

void PhysicsRigidActor::add_geometry(const physx::PxGeometry& geom, const PhysicsMaterial &material, const physx::PxTransform &pose)
{
	physx::PxRigidActor* cast = static_cast<physx::PxRigidActor*>(m_actor);

	physx::PxShape* shape = cast->createShape(geom, *material.get(), pose);

	// Create a renderable mesh from the physx triangle mesh object
	physx::PxGeometryType::Enum type = shape->getGeometryType();

	// If the geometry mesh is not a primative type, we'll need to create an OpenGL mesh to render it
	if (type == physx::PxGeometryType::eTRIANGLEMESH)
	{

	}
	else if (type == physx::PxGeometryType::eCAPSULE)
	{
		std::vector<glm::vec3> vertices;
		std::vector<unsigned int> indicies;

		int res = 20;
		float radius_c = shape->getGeometry().capsule().radius;
		float height = shape->getGeometry().capsule().halfHeight * 2.0f;
		float pid2 = glm::pi<float>() / 2.0f;

		// Top cap
		for (int j = 0; j <= res / 4; j++)
		{
			for (int i = 0; i <= res; i++)
			{
				float theta = i * glm::two_pi<float>() / res;
				float phi = -pid2 + glm::pi<float>() * j / (res / 2); // PID2

				float x = radius_c * cos(phi) * cos(theta);
				float y = radius_c * cos(phi) * sin(theta);
				float z = (radius_c * sin(phi)) - (height / 2);

				vertices.emplace_back(x, y, z);
			}
		}

		// Bottom cap
		for (int j = res / 4; j <= res / 2; j++)
		{
			for (int i = 0; i <= res; i++)
			{
				float theta = i * glm::two_pi<float>() / res;
				float phi = -pid2 + glm::pi<float>() * j / (res / 2);

				float x = radius_c * cos(phi) * cos(theta);
				float y = radius_c * cos(phi) * sin(theta);
				float z = (radius_c * sin(phi)) + (height / 2);

				vertices.emplace_back(x, y, z);
			}
		}

		// Faces
		for (int j = 0; j <= res / 2; j++)
		{
			for (int i = 0; i < res; i++)
			{
				int i1 = j * (res + 1) + i;
				int i2 = j * (res + 1) + (i + 1);
				int i3 = (j + 1) * (res + 1) + (i + 1);
				int i4 = (j + 1) * (res + 1) + i;

				indicies.push_back(i1);
				indicies.push_back(i2);
				indicies.push_back(i3);
				indicies.push_back(i1);
				indicies.push_back(i3);
				indicies.push_back(i4);
			}
		}

		m_meshes[shape] = std::make_unique<Mesh>(vertices, indicies, Engine::instance().shader("shaders/physx_debug"));
	}

	/*if (type == physx::PxGeometryType::eTRIANGLEMESH)
	{
		physx::PxTriangleMesh *mesh = shape->getGeometry().triangleMesh().triangleMesh;

		std::vector<glm::vec3> vertices = physx_to_glm(mesh->getVertices(), mesh->getNbVertices());
		m_meshs.emplace_back(vertices, Engine::instance().shader("shaders/physx_debug"));
	}*/
}

Transform &PhysicsRigidActor::transform()
{
	Transform physx_t = physx::to_glm(get()->getGlobalPose());

	m_transform.position(physx_t.position());
	m_transform.orientation(physx_t.orientation());

	return m_transform;
}

void PhysicsRigidActor::transform(const Transform &t)
{
	m_transform = t;
	get()->setGlobalPose(glm::to_physx(t));
}

void PhysicsRigidActor::draw(const Camera &camera)
{
	unsigned int no_shapes = get()->getNbShapes();
	std::vector<physx::PxShape*> shapes(no_shapes);
	get()->getShapes(&shapes[0], no_shapes);

	Shader &debug_shader = Engine::instance().shader("shaders/physx_debug");
	debug_shader.use();
	debug_shader.setUniform("model", transform().modelMatrix());
	debug_shader.setUniform("view_proj", camera.view_projection());

	// Debug draw colour; blue = static object, green = awake dynamic object, orange = asleep dynamic object
	if (get()->getType() == physx::PxActorType::eRIGID_STATIC)
	{
		debug_shader.setUniform("color", glm::vec3(0.0f, 0.0f, 1.0f)); // Blue if static
	}
	else if (get()->getType() == physx::PxActorType::eRIGID_DYNAMIC)
	{
		if (get()->isRigidDynamic()->isSleeping())
		{
			debug_shader.setUniform("color", glm::vec3(1.0f, 0.4f, 0.0f)); // orange is sleeping
		}
		else
		{
			debug_shader.setUniform("color", glm::vec3(0.0f, 1.0f, 0.0f)); // green is awake
		}
	}


	for (unsigned int i = 0; i < no_shapes; ++i)
	{
		physx::PxGeometryType::Enum type = shapes[i]->getGeometryType();
		Transform t = physx::to_glm(shapes[i]->getLocalPose());

		if (type == physx::PxGeometryType::eBOX)
		{
			physx::PxBoxGeometry &box = shapes[i]->getGeometry().box();
			Mesh &mesh = Engine::instance().basic_mesh(MeshPrimiative::CUBE);

			// Scale mesh primative to match physx box geometry size
			t.scale(glm::vec3(box.halfExtents.x, box.halfExtents.y, box.halfExtents.z));
			debug_shader.setUniform("shape", t.modelMatrix());

			// Draw primative
			mesh.bind();
			mesh.draw();
			mesh.unbind();
		}
		else if (type == physx::PxGeometryType::ePLANE)
		{
			Mesh &mesh = Engine::instance().basic_mesh(MeshPrimiative::PLANE);

			// Scale plane
			t.scale(glm::vec3(1000.0f));
			t.rotateGlobal(90.0f, glm::vec3(0.0f, 0.0f, 1.0f)); // The model matrix converted from physx is at 90 degrees to the actual plane, due to the way physx stores transforms for planes.
			debug_shader.setUniform("shape", t.modelMatrix());

			// Draw primative
			mesh.bind();
			mesh.draw();
			mesh.unbind();
		}
		else if (type == physx::PxGeometryType::eSPHERE)
		{
			physx::PxSphereGeometry &sphere = shapes[i]->getGeometry().sphere();
			Mesh &mesh = Engine::instance().basic_mesh(MeshPrimiative::SPHERE);

			// Scale mesh primative to match physx box geometry size
			t.scale(glm::vec3(sphere.radius));
			debug_shader.setUniform("shape", t.modelMatrix());

			// Draw primative
			mesh.bind();
			mesh.draw();
			mesh.unbind();
		}
		else if (type == physx::PxGeometryType::eCAPSULE || type == physx::PxGeometryType::eCONVEXMESH)
		{
			physx::PxCapsuleGeometry &capsule = shapes[i]->getGeometry().capsule();
			Mesh &mesh = *m_meshes[shapes[i]];

			// Physx capsules are rotated 90 degrees too..
			t.rotateGlobal(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
			debug_shader.setUniform("shape", t.modelMatrix());

			// Draw primative
			mesh.bind();
			mesh.draw();
			mesh.unbind();
		}
	}
}

/********** Static Physics actor class *********/

PhysicsStaticActor::PhysicsStaticActor(const Transform &transform)
: PhysicsRigidActor(transform)
{
	m_actor = Engine::instance().physics().get_physx()->createRigidStatic(glm::to_physx(transform));
}

PhysicsStaticActor::PhysicsStaticActor(const physx::PxTransform &transform)
: PhysicsRigidActor(physx::to_glm(transform))
{
	m_actor = Engine::instance().physics().get_physx()->createRigidStatic(transform);
}

PhysicsStaticActor::PhysicsStaticActor(physx::PxRigidStatic* actor)
: PhysicsRigidActor(physx::to_glm(actor->getGlobalPose()))
{
	m_actor = actor;

	if (actor->getName() != nullptr)
	{
		m_name = actor->getName();
	}
}



/********** Dynamic Physics actor class *********/

PhysicsDynamicActor::PhysicsDynamicActor(const Transform &transform)
: PhysicsRigidActor(transform)
{
	m_actor = Engine::instance().physics().get_physx()->createRigidDynamic(glm::to_physx(transform));
}

PhysicsDynamicActor::PhysicsDynamicActor(const physx::PxTransform &transform)
: PhysicsRigidActor(physx::to_glm(transform))
{
	m_actor = Engine::instance().physics().get_physx()->createRigidDynamic(transform);
}

PhysicsDynamicActor::PhysicsDynamicActor(physx::PxRigidDynamic* actor)
: PhysicsRigidActor(physx::to_glm(actor->getGlobalPose()))
{
	m_actor = actor;

	if (actor->getName() != nullptr)
	{
		m_name = actor->getName();
	}
}

void PhysicsDynamicActor::make_kinematic(const bool val)
{
	static_cast<physx::PxRigidDynamic*>(m_actor)->setRigidBodyFlag(physx::PxRigidDynamicFlag::eKINEMATIC, val);
}

void PhysicsDynamicActor::set_density(const float &density)
{
	physx::PxRigidBodyExt::updateMassAndInertia(*get(), density);
}

void PhysicsDynamicActor::add_geometry(const physx::PxGeometry& geom, const PhysicsMaterial &material, const float &density, const physx::PxTransform &pose)
{
	PhysicsRigidActor::add_geometry(geom, material, pose);
	set_density(density);
}