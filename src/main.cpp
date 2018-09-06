#include <iostream>

#include "Rendering/Display.h"
#include "Rendering/Shader.h"
#include "Rendering/Camera.h"
#include "Core/Object.h"
#include "Core/Time.h"
#include "Core/Input.h"
#include "Rendering/Light.h"

#include "Physics/PhysicsScene.h"
#include "Physics/PhysicsActor.h"
#include "Physics/PhysicsMaterial.h"
#include "Physics/PhysicsCharacter.h"
#include "Physics/PhysicsJoint.h"

#include "BasicActors.h"

#include "Core/Engine.h"

const glm::vec3 g_gravity(0.0f, -9.81f, 0.0f);

bool g_running = true;
float g_accumulator = 0.0f;
RenderMode g_rendering = RenderMode::NORMAL;

int main(int argc, char** argv)
{
	// References to engine subsystems
	Time &time = Engine::instance().time();
	Input &input = Engine::instance().input();
	Display &display = Engine::instance().display("Physics Assignment", 1280, 720);
	Physics &physics = Engine::instance().physics();

	// Shader
	Shader &textured_shader = Engine::instance().shader("shaders/textured");
	Shader &crosshair_shader = Engine::instance().shader("shaders/crosshair");

	// Crosshair texture
	std::vector<glm::vec3> crosshair_verts = { glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0f, -1.0f, 0.0f) };
	std::vector<unsigned int> crosshair_indicies = { 0, 1, 3, 3, 1, 2 };
	Texture2D crosshair_texture("textures/crosshair6.png", false, false, 0);
	Mesh crosshair_mesh(crosshair_verts, crosshair_indicies, crosshair_shader);

	// Physics scene
	MyScene scene;

	// Physics materials
	//PhysicsMaterial default_material(0.2f, 0.15f, 0.0f); // prop surfing
	PhysicsMaterial default_material(0.6f, 0.5f, 0.0f);
	PhysicsMaterial bouncy_material(0.2f, 0.15f, 0.5f);
	PhysicsMaterial frictionless_material(0.0f, 0.0f, 0.02f);
	PhysicsMaterial highfriction_material(0.6f, 0.5f, 0.0f);

	// Player
	PhysicsCharacter player = scene.create_character(1.6f, 0.8f, glm::vec3(0.0f, 2.0f, 4.0f), default_material, 25.0f);
	Camera camera(70.0f, 0.01f, 1000.0f, display.aspect_ratio(), Transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	camera.transform().parent(player.transform());

	NStaticObject &player_attachment = Engine::instance().create_object<NStaticObject>(Transform());
	scene.add_object(player_attachment);	

	// Create objects
	NStaticObject &ground = Engine::instance().create_object<NStaticObject>(physx::to_glm(physx::PxTransformFromPlaneEquation(physx::PxPlane(physx::PxVec3(0.0f, 1.0f, 0.0f), 0.0f))));
	ground.actor().add_geometry(physx::PxPlaneGeometry(), default_material);
	scene.add_object(ground);
	NNonPhysicsObject &ground_render = Engine::instance().create_object<NNonPhysicsObject>();
	ground_render.add_model("models/Environment/Ground.obj", textured_shader);

	Crate1 &crate1 = Engine::instance().create_object<Crate1>(glm::vec3(-3.0f, 2.45f, -8.0f), 1.0f, textured_shader, default_material);
	Crate1 &crate2 = Engine::instance().create_object<Crate1>(glm::vec3(3.0f, 2.45f, -8.0f), 1.0f, textured_shader, default_material);
	//Crate2 &crate3 = Engine::instance().create_object<Crate2>(glm::vec3(-3.0f, 4.45f, -8.0f), 1.0f, textured_shader, default_material);
	//Crate2 &crate4 = Engine::instance().create_object<Crate2>(glm::vec3(3.0f, 4.45f, -8.0f), 1.0f, textured_shader, default_material);

	scene.add_object(crate1);
	scene.add_object(crate2);
	//scene.add_object(crate3);
	//scene.add_object(crate4);

	Barrel &barrel1 = Engine::instance().create_object<Barrel>(glm::vec3(-3.0f, 1.0f, -4.0f), 1.0f, textured_shader, default_material);
	scene.add_object(barrel1);

	// Bricks
	for (unsigned int i = 0; i < 20; ++i)
	{
		for (unsigned int j = 0; j < 5; ++j)
		{
			Brick &brick = Engine::instance().create_object<Brick>(glm::vec3(5.0f + (j * -2.0f), (i * 0.29f) + 0.15f, -2.0f), 2.0f, textured_shader, default_material);
			scene.add_object(brick);
			brick.actor().get()->putToSleep();
		}
	}

	// Space between the spirals
	int a = 8; int b = 8;

	for (int i = 0; i < 100; i++)
	{
		float angle = 0.05f * i;
		float x = (a + b * angle) * cos(angle);
		float y = (a + b * angle) * sin(angle);

		float scale_factor = ((float)i / (float)100) * 0.5f;


		Domino &domino = Engine::instance().create_object<Domino>(glm::vec3(x, 1.0f + (2.0f * scale_factor), y), 25.0f - glm::degrees(angle), 0.5f + scale_factor, textured_shader, default_material);
		scene.add_object(domino);
	}

	// Seesaw
	NStaticObject &base = Engine::instance().create_object<NStaticObject>(Transform(glm::vec3(0.0f, 0.68f, -8.0f), glm::vec3(0.0f, 90.0f, 0.0f)));
	base.add_model("models/Props/Drum/drum.obj", textured_shader, Transform(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.5f, 2.0f, 2.0f)));
	base.actor().add_geometry(physx::PxCapsuleGeometry(0.68f, 0.4f), default_material);

	NDynamicObject &board = Engine::instance().create_object<NDynamicObject>(Transform(glm::vec3(0.0f, 1.0f, -8.0f), glm::vec3(0.0f, 90.0f, 0.0f)));
	board.add_model("models/Environment/platform.obj", textured_shader, Transform(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f, 0.5f, 4.0f)));
	board.actor().add_geometry(physx::PxBoxGeometry(physx::PxVec3(1.0f, 0.05f, 4.0f)), default_material, 10.0f);

	HingeJoint board_joint(base.actor(), Transform(glm::vec3(0.0f, 0.68f, 0.0f)), board.actor(), Transform(glm::vec3(0.0f, -0.05f, 0.0f)));

	scene.add_object(base);
	scene.add_object(board);

	// Bouncy trampoline
	NStaticObject &leg1 = Engine::instance().create_object<NStaticObject>(Transform(glm::vec3(-10.0f, 0.0f, -2.0f)));
	leg1.add_model("models/Props/Trampoline/leg.obj", textured_shader);
	leg1.actor().add_geometry(physx::PxBoxGeometry(physx::PxBoxGeometry(physx::PxVec3(0.125f, 1.5f, 0.125f))), default_material, physx::PxTransform(physx::PxVec3(0.0f, 1.5f, 0.0f)));

	NStaticObject &leg2 = Engine::instance().create_object<NStaticObject>(Transform(glm::vec3(-6.0f, 0.0f, -2.0f)));
	leg2.add_model("models/Props/Trampoline/leg.obj", textured_shader);
	leg2.actor().add_geometry(physx::PxBoxGeometry(physx::PxBoxGeometry(physx::PxVec3(0.125f, 1.5f, 0.125f))), default_material, physx::PxTransform(physx::PxVec3(0.0f, 1.5f, 0.0f)));

	NStaticObject &leg3 = Engine::instance().create_object<NStaticObject>(Transform(glm::vec3(-10.0f, 0.0f, 2.0f)));
	leg3.add_model("models/Props/Trampoline/leg.obj", textured_shader);
	leg3.actor().add_geometry(physx::PxBoxGeometry(physx::PxBoxGeometry(physx::PxVec3(0.125f, 1.5f, 0.125f))), default_material, physx::PxTransform(physx::PxVec3(0.0f, 1.5f, 0.0f)));

	NStaticObject &leg4 = Engine::instance().create_object<NStaticObject>(Transform(glm::vec3(-6.0f, 0.0f, 2.0f)));
	leg4.add_model("models/Props/Trampoline/leg.obj", textured_shader);
	leg4.actor().add_geometry(physx::PxBoxGeometry(physx::PxBoxGeometry(physx::PxVec3(0.125f, 1.5f, 0.125f))), default_material, physx::PxTransform(physx::PxVec3(0.0f, 1.5f, 0.0f)));

	NDynamicObject &trampoline = Engine::instance().create_object<NDynamicObject>(Transform(glm::vec3(-8.0f, 4.0f, 0.0f)));
	trampoline.add_model("models/Props/Trampoline/surface.obj", textured_shader);
	trampoline.actor().add_geometry(physx::PxBoxGeometry(physx::PxVec3(1.5f, 0.01f, 1.5f)), default_material, 100.0);

	DistanceJoint joint1(leg1.actor(), Transform(glm::vec3(0.0f, 3.0f, 0.0f)), trampoline.actor(), Transform(glm::vec3(-1.5f, 0.0f, -1.5f)), true, 0.5f, 0.5f, true, 500.0f, 50.0f);
	DistanceJoint joint2(leg2.actor(), Transform(glm::vec3(0.0f, 3.0f, 0.0f)), trampoline.actor(), Transform(glm::vec3(1.5f, 0.0f, -1.5f)), true, 0.5f, 0.5f, true, 500.0f, 50.0f);
	DistanceJoint joint3(leg3.actor(), Transform(glm::vec3(0.0f, 3.0f, 0.0f)), trampoline.actor(), Transform(glm::vec3(-1.5f, 0.0f, 1.5f)), true, 0.5f, 0.5f, true, 500.0f, 50.0f);
	DistanceJoint joint4(leg4.actor(), Transform(glm::vec3(0.0f, 3.0f, 0.0f)), trampoline.actor(), Transform(glm::vec3(1.5f, 0.0f, 1.5f)), true, 0.5f, 0.5f, true, 500.0f, 50.0f);

	scene.add_object(leg1);
	scene.add_object(leg2);
	scene.add_object(leg3);
	scene.add_object(leg4);
	scene.add_object(trampoline);

	input.keyboard().bind_button(SDL_SCANCODE_F1, "normal_mode");
	input.keyboard().bind_button(SDL_SCANCODE_F2, "debug_mode");
	input.keyboard().bind_button(SDL_SCANCODE_F3, "both_mode");

	input.keyboard().bind_button(SDL_SCANCODE_1, "object1");
	input.keyboard().bind_button(SDL_SCANCODE_2, "object2");
	input.keyboard().bind_button(SDL_SCANCODE_3, "object3");
	input.keyboard().bind_button(SDL_SCANCODE_4, "object4");
	input.keyboard().bind_button(SDL_SCANCODE_5, "object5");
	input.keyboard().bind_button(SDL_SCANCODE_6, "object6");

	input.keyboard().bind_button(SDL_SCANCODE_LCTRL, "crouch");
	input.keyboard().bind_button(SDL_SCANCODE_RCTRL, "crouch");

	// Grav gun
	bool can_grab = true;
	physx::PxRigidDynamic *tracking = nullptr;
	DistanceJoint* grabbed_joint = nullptr;
	bool dropped = false;
	float pull = 0.0f;
	float old_lin = 0.0f;
	float old_ang = 0.0f;
	float old_mass = 0.0f;

	float current_size = 1.6f;

	// Main loop
	while (g_running)
	{
		time.start();

		// Update input
		input.update();

		// Handle quit input
		if (input.event("quit") || input.keyboard().button_down("back"))
		{
			g_running = false;
		}

		// Visualisation mode
		if (input.keyboard().button_down("normal_mode"))
		{
			g_rendering = RenderMode::NORMAL;
		}

		if (input.keyboard().button_down("debug_mode"))
		{
			g_rendering = RenderMode::DEBUG;
		}

		if (input.keyboard().button_down("both_mode"))
		{
			g_rendering = RenderMode::BOTH;
		}

		// Handle player input
		glm::vec3 direction = glm::vec3(0.0f);

		direction += camera.transform().zaxis() * input.keyboard().axis("moveY");
		direction += camera.transform().xaxis() * input.keyboard().axis("moveX");
		direction.y = 0.0f;

		if (direction != glm::vec3())
		{
			direction = glm::normalize(direction);
			float speed = (input.keyboard().button("sprint") ? 5.0f : 2.5f);

			direction = direction * speed * time.delta_time();
		}

		// Move player
		player.move(direction, glm::quat(), time.delta_time());

		// Camera rotation
		camera.look(input.mouse().axis("mouseY") * time.delta_time(), input.mouse().axis("mouseX") * time.delta_time());

		// Move player attachment point with the camera
		Transform pose(camera.transform().global_position() + (camera.transform().zaxis() * -1.0f) + glm::vec3(0.0f, 0.5f, 0.0f), camera.transform().orientation());
		player_attachment.actor().get()->setGlobalPose(glm::to_physx(pose));

		/// TODO: FACTORY!
		// Object spawning
		if (input.keyboard().button_down("object1"))
		{
			Crate1 &spawned_crate = Engine::instance().create_object<Crate1>(player_attachment.transform().global_position() + (camera.transform().zaxis() * -2.0f), 0.5f, textured_shader, default_material);
			scene.add_object(spawned_crate);
		}
		else if (input.keyboard().button_down("object2"))
		{
			Crate2 &spawned_crate = Engine::instance().create_object<Crate2>(player_attachment.transform().global_position() + (camera.transform().zaxis() * -2.0f), 0.5f, textured_shader, default_material);
			scene.add_object(spawned_crate);
		}
		else if (input.keyboard().button_down("object3"))
		{
			Barrel &spawned_barrel = Engine::instance().create_object< Barrel >(player_attachment.transform().global_position() + (camera.transform().zaxis() * -2.0f), 1.0f, textured_shader, default_material);
			scene.add_object(spawned_barrel);
		}
		else if (input.keyboard().button_down("object4"))
		{
			Domino &spawned_domino = Engine::instance().create_object< Domino >(player_attachment.transform().global_position() + (camera.transform().zaxis() * -2.0f), 0.0f, 1.8f, textured_shader, default_material);
			scene.add_object(spawned_domino);
		}
		else if (input.keyboard().button_down("object5"))
		{
			Brick &spawned_brick = Engine::instance().create_object< Brick >(player_attachment.transform().global_position() + (camera.transform().zaxis() * -2.0f), .25f, textured_shader, default_material);
			scene.add_object(spawned_brick);
		}

		if (input.keyboard().button("crouch") && current_size > 0.8f)
		{
			current_size -= time.delta_time() * 7.5f;

			if (current_size < 0.8f)
				current_size = 0.8f;

			player.get()->resize(current_size);
		}

		if (!input.keyboard().button("crouch") && current_size <= 1.6f)
		{
			current_size += time.delta_time() * 7.5f;

			if (current_size > 1.6f)
				current_size = 1.6f;

			player.get()->resize(current_size);
		}

		// Crouching
		


		// Jumping
		if (input.keyboard().button_down("jump"))
		{
			player.jump();
		}

		// Gravity gun
		// Dropped after grabbing
		if (input.mouse().button_down("aim") && grabbed_joint != nullptr)
		{
			// Grab projectile
			physx::PxRigidDynamic* projectile = grabbed_joint->actor2().get()->isRigidDynamic();

			// Release joint
			grabbed_joint->get()->release();
			delete grabbed_joint;
			grabbed_joint = nullptr;

			// Add force
			if (projectile != nullptr)
			{
				// Reset dampening
				projectile->setLinearDamping(old_lin);
				projectile->setAngularDamping(old_ang);
				projectile->setMass(old_mass);
			}

			dropped = true;
		}

		if (input.mouse().button_up("aim") && dropped)
		{
			dropped = false;
		}

		// Starting to pull an object towards us? Lock onto it.
		if (input.mouse().button("aim") && grabbed_joint == nullptr && tracking == nullptr && !dropped)
		{
			physx::PxRaycastBuffer hit;

			glm::vec3 origin = camera.transform().global_position() + (-camera.transform().zaxis() * 1.0f);

			if (scene.raycast(origin, -camera.transform().zaxis(), 50.0f, hit))
			{
				physx::PxRigidDynamic *body = hit.block.actor->isRigidDynamic();

				if (body != nullptr && !(body->getRigidBodyFlags() & physx::PxRigidBodyFlag::eKINEMATIC) && body->getNbConstraints() <= 0)
				{
					tracking = body;
					tracking->setRigidDynamicFlag(physx::PxRigidDynamicFlag::eKINEMATIC, true);
				}
			}
		}

		// Pulling object towards us
		if (input.mouse().button("aim") && tracking != nullptr)
		{
			glm::vec3 direction = camera.transform().global_position() - physx::to_glm(tracking->getGlobalPose().p);
			float distance = glm::length(direction);
			float max_size = tracking->getWorldBounds().getDimensions().magnitude();

			if (distance > max_size + 1.0f)
			{
				// Pull towards (kinematic makes this easier!!)
				pull += 0.02f;

				Transform t = physx::to_glm(tracking->getGlobalPose());
				t.position(t.position() + direction * pull);
				tracking->setKinematicTarget(glm::to_physx(t));
			}
			else
			{
				// Lock on
				tracking->setRigidDynamicFlag(physx::PxRigidDynamicFlag::eKINEMATIC, false);

				tracking->setLinearVelocity(physx::PxVec3(0.0f, 0.0f, 0.0f));
				tracking->setAngularVelocity(physx::PxVec3(0.0f, 0.0f, 0.0f));

				old_lin = tracking->getLinearDamping();
				old_ang = tracking->getAngularDamping();
				old_mass = tracking->getMass();

				tracking->setLinearDamping(5.0f);
				tracking->setAngularDamping(0.2f);
				tracking->setMass(100.0f);

				std::cout << "Grabbed object of mass " << old_mass << std::endl;

				grabbed_joint = new DistanceJoint(player_attachment.actor(), Transform(glm::vec3(0.0f, 0.0f, -max_size)), PhysicsDynamicActor(tracking), Transform(glm::vec3(0.0f, 0.0f, 0.0f)), true, 0.0f, 0.0f, true, 2000.0f, 0.0f);

				pull = 0.0f;
				tracking = nullptr;
			}
		}

		// Dropped it before grabbing it
		if (input.mouse().button_up("aim") && tracking != nullptr)
		{
			tracking->setRigidDynamicFlag(physx::PxRigidDynamicFlag::eKINEMATIC, false);

			tracking->setLinearVelocity(physx::PxVec3(0.0f, 0.0f, 0.0f));
			tracking->setAngularVelocity(physx::PxVec3(0.0f, 0.0f, 0.0f));

			tracking = nullptr;
			pull = 0.0f;
		}

		if (grabbed_joint != nullptr)
		{
			physx::PxTransform p = grabbed_joint->actor2().get()->isRigidDynamic()->getGlobalPose();
			glm::quat orientation = physx::to_glm(p.q);

			if (orientation != camera.transform().orientation())
			{
				glm::quat r = glm::angleAxis(glm::radians(input.mouse().axis("mouseX") * time.delta_time()), glm::vec3(0.0f, 1.0f, 0.0f)) * orientation;

				p.q = glm::to_physx(r);
				p.q.normalize();

				grabbed_joint->actor2().get()->isRigidDynamic()->setGlobalPose(p);
			}

			grabbed_joint->actor2().get()->isRigidDynamic()->wakeUp();
		}

		// Launch		
		if (input.mouse().button_down("fire") && grabbed_joint != nullptr)
		{
			// Grab projectile
			physx::PxRigidDynamic* projectile = grabbed_joint->actor2().get()->isRigidDynamic();

			// Release joint
			grabbed_joint->get()->release();
			delete grabbed_joint;
			grabbed_joint = nullptr;

			// Add force
			if (projectile != nullptr)
			{
				// Reset dampening
				projectile->setLinearDamping(old_lin);
				projectile->setAngularDamping(old_ang);
				projectile->setMass(old_mass);

				projectile->addForce(glm::to_physx(-camera.transform().zaxis() * 50.0f * old_mass), physx::PxForceMode::eIMPULSE);
			}
		}

		// Shoot without grabbing
		if (input.mouse().button_down("fire") && grabbed_joint == nullptr)
		{
			physx::PxRaycastBuffer hit;

			glm::vec3 origin = camera.transform().global_position() + (-camera.transform().zaxis() * 1.0f);

			if (scene.raycast(origin, -camera.transform().zaxis(), 50.0f, hit))
			{
				physx::PxRigidDynamic *body = hit.block.actor->isRigidDynamic();
				physx::PxRigidStatic *static_body = hit.block.actor->isRigidStatic();

				if (body != nullptr && !(body->getRigidBodyFlags() & physx::PxRigidBodyFlag::eKINEMATIC))
				{
					physx::PxRigidBodyExt::addForceAtPos(*body, glm::to_physx(-camera.transform().zaxis() * 50.0f * body->getMass()), hit.block.position, physx::PxForceMode::eIMPULSE);
				}
			}
		}

		// Clear display
		display.clear();

		// Update objects
		Engine::instance().update_objects();

		// Render objects
		Engine::instance().render_objects(g_rendering, camera);

		float d_width = (float)display.width();
		float d_height = (float)display.height();

		// Render crosshair
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		crosshair_shader.use();
		crosshair_texture.use(0);
		crosshair_shader.setUniform("crosshairTexture", 0);
		crosshair_shader.setUniform("view", glm::ortho(-d_width, d_width, -d_height, d_height));
		crosshair_shader.setUniform("size", glm::scale(glm::vec3(64.0f)));
		crosshair_mesh.bind();
		crosshair_mesh.draw();
		crosshair_mesh.unbind();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		// Swap double buffer
		display.swap();

		g_accumulator += time.delta_time();

		while (g_accumulator > 0.01f)
		{
			// Simulate physX
			scene.simulate(0.01f);
			g_accumulator -= 0.01f;
		}

		time.end();
	}

	//NewRenderedPhysicsStaticObject &new_object = Engine::instance().create_object<NewRenderedPhysicsStaticObject>(Transform(glm::vec3(0.0f, 0.0f, 0.0f)), "models/Props/Drum/oildrum.obj", textured_shader);
	//new_object.actor().add_geometry(physx::PxBoxGeometry(glm::to_physx(glm::vec3(0.1f, 0.08f, 0.2f))), one_material);


	// Shaders
	/*Shader textured("shaders/textured");

	//Engine::instance().create<NewRenderedPhysicsStaticObject>(Transform(), "models/Props/Drum/drum.obj", textured);

	// Lights
	DirectionalLight sun(glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.05f, glm::vec3(0.0f, 0.0f, -1.0f));

	// Physics scene
	MyScene scene;

	// Physics materials
	PhysicsMaterial default_material(0.5f, 0.5f, 0.0f);
	PhysicsMaterial player_material(1.0f, 1.0f, 0.0f);

	PhysicsMaterial box1_material(0.2f, 0.15f, 0.02f);
	PhysicsMaterial box2_material(0.2f, 0.15f, 0.02f);
	PhysicsMaterial box3_material(0.2f, 0.15f, 0.02f);

	// Physics actors
	Plane plane(glm::vec3(0.0f, 1.0f, 0.0f), default_material);

	//PhysicsCharacterDynamic player_dynamic(Transform(glm::vec3(0.0f, 0.0f, 4.0f)), 1.6f, 0.8f, 10.0f, player_material);
	//PhysicsStaticObject player_attachment("models/Environment/Crates/Crate_wooden.obj", textured, Transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.1f)));
	//player_attachment.actor().add_geometry(physx::PxBoxGeometry(glm::to_physx(glm::vec3(0.01f))), default_material, 1.0f);
	
	PhysicsStaticActor player_attachment(Transform(glm::vec3(0.0f, 5.0f, 0.0f)));

	plane.name("Ground");

	scene.add_actor(plane);
	//scene.add_actor(player_dynamic);
	scene.add_actor(player_attachment);

	//DistanceJoint player_joint(player_dynamic, Transform(glm::vec3(0.0f, 0.0f, -5.0f)), player_attachment.actor(), Transform(), 0.0f, 0.0f);

	// PhysX character controller
	PhysicsCharacter player = scene.create_character(1.6f, 0.8f, glm::vec3(0.0f, 2.0f, 4.0f), default_material, 25.0f);

	std::cout << "Mass: " << player.get()->getActor()->getMass() << std::endl;

	// Input devices
	Input input;

	// Set mouse sensitivity
	input.mouse().sensitivity(20.0f);

	// Camera
	Camera camera(70.0f, 0.01f, 1000.0f, display.aspect_ratio(), Transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

	camera.transform().parent(player.transform());

	// Objects
	PhysicsDynamicObject crate1("models/Environment/Crates/Crate_wooden.obj", textured, Transform(glm::vec3(-3.0f, 10.0f, -5.0f), glm::vec3(0.0f), glm::vec3(0.5f)));
	PhysicsDynamicObject crate2("models/Environment/Crates/Crate_mossy.obj", textured, Transform(glm::vec3(0.0f, 10.0f, -5.0f), glm::vec3(0.0f), glm::vec3(1.0f)));
	PhysicsDynamicObject crate3("models/Environment/Crates/Crate_mossy.obj", textured, Transform(glm::vec3(3.0f, 10.0f, -5.0f), glm::vec3(0.0f), glm::vec3(1.0f)));

	PhysicsDynamicObject brick1("models/Props/Brick/lego_brick.obj", textured, Transform(glm::vec3(7.0f, 8.0f, -2.0f), glm::vec3(0.0f), glm::vec3(0.1f)));
	PhysicsDynamicObject brick2("models/Props/Brick/lego_brick.obj", textured, Transform(glm::vec3(7.0f, 8.5f, -2.0f), glm::vec3(0.0f), glm::vec3(0.1f)));
	PhysicsDynamicObject brick3("models/Props/Brick/lego_brick.obj", textured, Transform(glm::vec3(7.0f, 9.0f, -2.0f), glm::vec3(0.0f), glm::vec3(0.1f)));
	PhysicsDynamicObject brick4("models/Props/Brick/lego_brick.obj", textured, Transform(glm::vec3(7.0f, 9.5f, -2.0f), glm::vec3(0.0f), glm::vec3(0.1f)));
	PhysicsDynamicObject brick5("models/Props/Brick/lego_brick.obj", textured, Transform(glm::vec3(7.0f, 10.0f, -2.0f), glm::vec3(0.0f), glm::vec3(0.1f)));
	PhysicsDynamicObject brick6("models/Props/Brick/lego_brick.obj", textured, Transform(glm::vec3(7.0f, 10.5f, -2.0f), glm::vec3(0.0f), glm::vec3(0.1f)));
	PhysicsDynamicObject brick7("models/Props/Brick/lego_brick.obj", textured, Transform(glm::vec3(7.0f, 7.5f, -2.0f), glm::vec3(0.0f), glm::vec3(0.1f)));
	PhysicsDynamicObject brick8("models/Props/Brick/lego_brick.obj", textured, Transform(glm::vec3(7.0f, 7.0f, -2.0f), glm::vec3(0.0f), glm::vec3(0.1f)));
	PhysicsDynamicObject brick9("models/Props/Brick/lego_brick.obj", textured, Transform(glm::vec3(7.0f, 6.5f, -2.0f), glm::vec3(0.0f), glm::vec3(0.1f)));
	PhysicsDynamicObject brick10("models/Props/Brick/lego_brick.obj", textured, Transform(glm::vec3(7.0f, 6.0f, -2.0f), glm::vec3(0.0f), glm::vec3(0.1f)));


	brick1.actor().add_geometry(physx::PxBoxGeometry(glm::to_physx(glm::vec3(0.1f, 0.08f, 0.2f))), box1_material, 1175.0f); //1,175
	brick2.actor().add_geometry(physx::PxBoxGeometry(glm::to_physx(glm::vec3(0.1f, 0.08f, 0.2f))), box1_material, 1175.0f);
	brick3.actor().add_geometry(physx::PxBoxGeometry(glm::to_physx(glm::vec3(0.1f, 0.08f, 0.2f))), box1_material, 1175.0f);
	brick4.actor().add_geometry(physx::PxBoxGeometry(glm::to_physx(glm::vec3(0.1f, 0.08f, 0.2f))), box1_material, 1175.0f);
	brick5.actor().add_geometry(physx::PxBoxGeometry(glm::to_physx(glm::vec3(0.1f, 0.08f, 0.2f))), box1_material, 1175.0f);
	brick6.actor().add_geometry(physx::PxBoxGeometry(glm::to_physx(glm::vec3(0.1f, 0.08f, 0.2f))), box1_material, 1175.0f);
	brick7.actor().add_geometry(physx::PxBoxGeometry(glm::to_physx(glm::vec3(0.1f, 0.08f, 0.2f))), box1_material, 1175.0f);
	brick8.actor().add_geometry(physx::PxBoxGeometry(glm::to_physx(glm::vec3(0.1f, 0.08f, 0.2f))), box1_material, 1175.0f);
	brick9.actor().add_geometry(physx::PxBoxGeometry(glm::to_physx(glm::vec3(0.1f, 0.08f, 0.2f))), box1_material, 1175.0f);
	brick10.actor().add_geometry(physx::PxBoxGeometry(glm::to_physx(glm::vec3(0.1f, 0.08f, 0.2f))), box1_material, 1175.0f);

	scene.add_object(brick1);
	scene.add_object(brick2);
	scene.add_object(brick3);
	scene.add_object(brick4);
	scene.add_object(brick5);
	scene.add_object(brick6);
	scene.add_object(brick7);
	scene.add_object(brick8);
	scene.add_object(brick9);
	scene.add_object(brick10);

	crate1.actor().name("Crate1");
	crate2.actor().name("Crate2");
	crate3.actor().name("Crate3");

	crate1.actor().add_geometry(physx::PxBoxGeometry(glm::to_physx(glm::vec3(0.5f))), box1_material, 10.0f);
	crate2.actor().add_geometry(physx::PxBoxGeometry(glm::to_physx(glm::vec3(1.0f))), box2_material, 10.0f);
	crate3.actor().add_geometry(physx::PxBoxGeometry(glm::to_physx(glm::vec3(1.0f))), box3_material, 10.0f);

	scene.add_object(crate1);
	scene.add_object(crate2);
	scene.add_object(crate3);

	PhysicsStaticObject portal_button("models/Props/Portal/portalbutton.obj", textured, Transform(glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f), glm::vec3(0.01f)));
	//PhysicsDynamicObject platform("models/Environment/platform.obj", textured, Transform(glm::vec3(-10.0f, 1.0f, -2.0f), glm::vec3(0.0f), glm::vec3(2.0f, 1.0f, 2.0f)));

	portal_button.actor().add_geometry(physx::PxBoxGeometry(physx::PxVec3(0.3f, 0.1f, 0.3f)), default_material, physx::PxTransform(physx::PxVec3(0.0f, 0.05f, 0.0f)));
	//platform.actor().add_geometry(physx::PxBoxGeometry(physx::PxVec3(2.0f, 0.1f, 2.0f)), default_material, 10.0f, physx::PxTransform(physx::PxVec3(0.0f, 0.0f, 0.0f)));
	//platform.actor().make_kinematic(true);

	scene.add_object(portal_button);
	//scene.add_object(platform);

	NonPhysicsObject portal_gun("models/Props/Portal Gun.obj", textured, Transform(glm::vec3(0.25f, -0.15f, -0.5f), glm::vec3(0.0f), glm::vec3(0.4f)));

	portal_gun.transform().parent(camera.transform());

	PhysicsStaticObject ground("models/Environment/Ground.obj", textured, Transform(glm::vec3(0.0f, 0.0f, 0.0f)));


	// Seesaw
	PhysicsStaticActor seesaw(Transform(glm::vec3(10.0f, 0.0f, 2.0f)));
	PhysicsDynamicObject barrel("models/Props/Drum/drum.obj", textured, Transform(glm::vec3(10.0f, 0.5f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	barrel.actor().add_geometry(physx::PxCapsuleGeometry(0.4f, 0.25f), default_material, 10.0f, glm::to_physx(Transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f))));
	PhysicsDynamicObject plank("models/Environment/platform.obj", textured, Transform(glm::vec3(10.0f, 1.0f, 2.5f), glm::vec3(0.0f), glm::vec3(1.0f, 0.5f, 4.0f)));
	plank.actor().add_geometry(physx::PxBoxGeometry(physx::PxVec3(1.0f, 0.05f, 4.0f)), default_material, 10.0f, physx::PxTransform(physx::PxVec3(0.0f, 0.0f, 0.0f)));
	FixedJoint seesaw_barrel(seesaw, Transform(), barrel.actor(), Transform(glm::vec3(0.0f, -0.4f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	HingeJoint barrel_plank(barrel.actor(), Transform(glm::vec3(0.0f, 0.4f, 0.0)), plank.actor(), Transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	scene.add_actor(seesaw);
	scene.add_object(barrel);
	scene.add_object(plank);

	// Moving platforms
	MovingPlatform platform1(glm::vec3(-10.0f, 1.0f, -2.0f), textured, glm::vec3(0.0f, 0.0f, 1.0f), 5.0f, 0.5f, 0.0f, default_material);
	MovingPlatform platform2(glm::vec3(-16.0f, 1.0f, -2.0f), textured, glm::vec3(0.0f, 0.0f, 1.0f), 5.0f, 0.5f, 1.0f, default_material);
	MovingPlatform platform3(glm::vec3(-22.0f, 1.0f, -2.0f), textured, glm::vec3(0.0f, 0.0f, 1.0f), 5.0f, 0.5f, 2.0f, default_material);
	MovingPlatform platform4(glm::vec3(-28.0f, 1.0f, -2.0f), textured, glm::vec3(0.0f, 0.0f, 1.0f), 5.0f, 0.5f, 3.0f, default_material);
	scene.add_object(platform1);
	scene.add_object(platform2);
	scene.add_object(platform3);
	scene.add_object(platform4);

	// Jumping
	float jump_velocity = 0.0f;
	bool can_jump = true;
	glm::vec3 movement_final = glm::vec3();

	// Grav gun
	bool can_grab = true;
	physx::PxRigidDynamic *tracking = nullptr;
	DistanceJoint* grabbed_joint = nullptr;
	bool dropped = false;
	float pull = 0.0f;
	float old_lin = 0.0f;
	float old_ang = 0.0f;
	float old_mass = 0.0f;

	time.update();

	float accumulator = 0.0f;

	// new testing ---------------------
	NewRenderedPhysicsStaticObject new_static_object(Transform(glm::vec3(0.0f, 4.0f, 0.0f)), "models/Props/Drum/oildrum.obj", textured);


	// end new testing -----------------



	// Main loop
	while (g_running)
	{
		time.start();

		// Update input
		input.update();

		// Handle quit input
		if (input.event("quit") || input.keyboard().button_down("back"))
		{
			g_running = false;
		}

		display.clear();

		// Camera movement
		if (can_jump)
		{
			movement_final = g_gravity * time.delta_time();
		}

		glm::vec3 direction = glm::vec3(0.0f);
		glm::vec3 target_vel = glm::vec3(0.0f);
		
		direction += camera.transform().zaxis() * input.keyboard().axis("moveY");
		direction += camera.transform().xaxis() * input.keyboard().axis("moveX");
		direction.y = 0.0f;

		if (direction != glm::vec3())
		{
			direction = glm::normalize(direction);
			float speed = (input.keyboard().button("sprint") ? 5.0f : 2.5f);

			target_vel = direction * speed;

			direction = direction * speed * time.delta_time();
			movement_final += direction;
		}

		//direction.y = player.jump_movement(time.delta_time());

		player.move(direction, glm::quat(), time.delta_time());// , glm::quat(), time.delta_time());

		camera.look(input.mouse().axis("mouseY") * time.delta_time(), input.mouse().axis("mouseX") * time.delta_time());

		Transform pose(camera.transform().global_position() + (camera.transform().zaxis() * -1.0f), camera.transform().orientation());
		player_attachment.get()->setGlobalPose(glm::to_physx(pose));

		if (input.keyboard().button_down("jump"))
		{
			player.jump();
		}

		//std::cout << glm::to_string(player.transform().position()) << std::endl;

		// Dropped after grabbing
		if (input.mouse().button_down("aim") && grabbed_joint != nullptr)
		{
			// Grab projectile
			physx::PxRigidDynamic* projectile = grabbed_joint->actor2().get()->isRigidDynamic();

			// Release joint
			grabbed_joint->get()->release();
			delete grabbed_joint;
			grabbed_joint = nullptr;

			// Add force
			if (projectile != nullptr)
			{
				// Reset dampening
				projectile->setLinearDamping(old_lin);
				projectile->setAngularDamping(old_ang);
				projectile->setMass(old_mass);
			}

			dropped = true;
		}

		if (input.mouse().button_up("aim") && dropped)
		{
			dropped = false;
		}

		// Starting to pull an object towards us? Lock onto it.
		if (input.mouse().button("aim") && grabbed_joint == nullptr && tracking == nullptr && !dropped)
		{
			physx::PxRaycastBuffer hit;

			glm::vec3 origin = camera.transform().global_position() + (-camera.transform().zaxis() * 1.0f);

			if (scene.raycast(origin, -camera.transform().zaxis(), 25.0f, hit))
			{
				physx::PxRigidDynamic *body = hit.block.actor->isRigidDynamic();

				if (body != nullptr && !(body->getRigidBodyFlags() & physx::PxRigidBodyFlag::eKINEMATIC) && body->getNbConstraints() <= 0)
				{
					tracking = body;
					tracking->setRigidDynamicFlag(physx::PxRigidDynamicFlag::eKINEMATIC, true);
				}
			}
		}

		// Pulling object towards us
		if (input.mouse().button("aim") && tracking != nullptr)
		{
			glm::vec3 direction = camera.transform().global_position() - physx::to_glm(tracking->getGlobalPose().p);
			float distance = glm::length(direction);
			float max_size = tracking->getWorldBounds().getDimensions().magnitude();

			if (distance > max_size + 1.0f)
			{
				// Pull towards (kinematic makes this easier!!)
				//pull += 0.004f;
				pull += 0.02f;

				Transform t = physx::to_glm(tracking->getGlobalPose());
				t.position(t.position() + direction * pull);
				tracking->setKinematicTarget(glm::to_physx(t));
			}
			else
			{
				// Lock on
				tracking->setRigidDynamicFlag(physx::PxRigidDynamicFlag::eKINEMATIC, false);

				tracking->setLinearVelocity(physx::PxVec3(0.0f, 0.0f, 0.0f));
				tracking->setAngularVelocity(physx::PxVec3(0.0f, 0.0f, 0.0f));

				old_lin = tracking->getLinearDamping();
				old_ang = tracking->getAngularDamping();
				old_mass = tracking->getMass();

				std::cout << "Grabbed object of mass " << old_mass << std::endl;

				grabbed_joint = new DistanceJoint(player_attachment, Transform(glm::vec3(0.0f, 0.0f, -max_size)), PhysicsDynamicActor(tracking), Transform(glm::vec3(0.0f, 0.0f, 0.0f)), 0.0f, 0.0f);

				pull = 0.0f;
				tracking = nullptr;
			}
		}

		// Dropped it before grabbing it
		if (input.mouse().button_up("aim") && tracking != nullptr)
		{
			tracking->setRigidDynamicFlag(physx::PxRigidDynamicFlag::eKINEMATIC, false);

			tracking->setLinearVelocity(physx::PxVec3(0.0f, 0.0f, 0.0f));
			tracking->setAngularVelocity(physx::PxVec3(0.0f, 0.0f, 0.0f));

			tracking = nullptr;
			pull = 0.0f;
		}

		if (grabbed_joint != nullptr)
		{
			// Check for break
			if (grabbed_joint->get()->getConstraintFlags() & physx::PxConstraintFlag::eBROKEN)
			{
				// Reset dampening
				grabbed_joint->actor2().get()->isRigidDynamic()->setLinearDamping(old_lin);
				grabbed_joint->actor2().get()->isRigidDynamic()->setAngularDamping(old_ang);
				grabbed_joint->actor2().get()->isRigidDynamic()->setMass(old_mass);

				// Release joint
				grabbed_joint->get()->release();
				delete grabbed_joint;
				grabbed_joint = nullptr;
			}
			else
			{
				physx::PxTransform p = grabbed_joint->actor2().get()->isRigidDynamic()->getGlobalPose();
				glm::quat orientation = physx::to_glm(p.q);

				if (orientation != camera.transform().orientation())
				{
					glm::quat r = glm::angleAxis(glm::radians(input.mouse().axis("mouseX") * time.delta_time()), glm::vec3(0.0f, 1.0f, 0.0f)) * orientation;

					p.q = glm::to_physx(r);
					p.q.normalize();

					grabbed_joint->actor2().get()->isRigidDynamic()->setGlobalPose(p);
				}

				grabbed_joint->actor2().get()->isRigidDynamic()->wakeUp();

			}
		}

		// Launch		
		if (input.mouse().button_down("fire") && grabbed_joint != nullptr)
		{
			// Grab projectile
			physx::PxRigidDynamic* projectile = grabbed_joint->actor2().get()->isRigidDynamic();

			// Release joint
			grabbed_joint->get()->release();
			delete grabbed_joint;
			grabbed_joint = nullptr;

			// Add force
			if (projectile != nullptr)
			{
				// Reset dampening
				projectile->setLinearDamping(old_lin);
				projectile->setAngularDamping(old_ang);
				projectile->setMass(old_mass);

				projectile->addForce(glm::to_physx(-camera.transform().zaxis() * 5000.0f), physx::PxForceMode::eIMPULSE);
			}
		}

		// Shoot without grabbing
		if (input.mouse().button_down("fire") && grabbed_joint == nullptr)
		{
			physx::PxRaycastBuffer hit;

			glm::vec3 origin = camera.transform().global_position() + (-camera.transform().zaxis() * 1.0f);

			if (scene.raycast(origin, -camera.transform().zaxis(), 25.0f, hit))
			{
				std::cout << "ray hit." << std::endl;

				physx::PxRigidDynamic *body = hit.block.actor->isRigidDynamic();
				physx::PxRigidStatic *static_body = hit.block.actor->isRigidStatic();

				if (body != nullptr && !(body->getRigidBodyFlags() & physx::PxRigidBodyFlag::eKINEMATIC))
				{
					//hit.block.position

					physx::PxRigidBodyExt::addForceAtPos(*body, glm::to_physx(-camera.transform().zaxis() * 5000.0f), hit.block.position, physx::PxForceMode::eIMPULSE);

					//body->addForce(glm::to_physx(-camera.transform().zaxis() * 50.0f), physx::PxForceMode::eVELOCITY_CHANGE);
				}
				else if (body != nullptr && (body->getRigidBodyFlags() & physx::PxRigidBodyFlag::eKINEMATIC))
				{
					//std::cout << "Shit kinematic body" << std::endl;
					//player_dynamic.move(camera.transform().zaxis() * 50.0f);
					//player_dynamic.get()->addForce(glm::to_physx(camera.transform().zaxis() * 50.0f), physx::PxForceMode::eVELOCITY_CHANGE);
				}
				else if (static_body != nullptr)
				{
					//std::cout << "Shot static body" << std::endl;
					//player_dynamic.move(camera.transform().zaxis() * 50.0f);
					//player_dynamic.get()->addForce(glm::to_physx(camera.transform().zaxis() * 50.0f), physx::PxForceMode::eVELOCITY_CHANGE);
				}
			}
		}

		// Moving platform
		/*glm::vec3 movement = glm::slerp(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(4.0f, 0.0f, 0.0f), platform_progress);
		platform_progress += (platform_direction * 0.5f) * time.delta_time();

		Transform t = platform.transform();
		t.position(platform_start + movement);
		platform.actor().get()->setKinematicTarget(glm::to_physx(t));

		if (platform_progress > 1.0f || platform_progress < -1.0f)
			platform_direction = -platform_direction;*/

		// Update moving platforms
		/*platform1.update(time.delta_time());
		platform2.update(time.delta_time());
		platform3.update(time.delta_time());
		platform4.update(time.delta_time());

		ground.draw(camera.projection() * camera.view());
		crate1.draw(camera.projection() * camera.view());
		crate2.draw(camera.projection() * camera.view());
		crate3.draw(camera.projection() * camera.view());
		//portal_gun.draw(camera.projection() * camera.view());
		portal_button.draw(camera.projection() * camera.view());
		//platform.draw(camera.projection() * camera.view());
		barrel.draw(camera.projection() * camera.view());
		plank.draw(camera.projection() * camera.view());


		brick1.draw(camera.projection() * camera.view());
		brick2.draw(camera.projection() * camera.view());
		brick3.draw(camera.projection() * camera.view());
		brick4.draw(camera.projection() * camera.view());
		brick5.draw(camera.projection() * camera.view());
		brick6.draw(camera.projection() * camera.view());
		brick7.draw(camera.projection() * camera.view());
		brick8.draw(camera.projection() * camera.view());
		brick9.draw(camera.projection() * camera.view());
		brick10.draw(camera.projection() * camera.view());

		platform1.draw(camera.projection() * camera.view());
		platform2.draw(camera.projection() * camera.view());
		platform3.draw(camera.projection() * camera.view());
		platform4.draw(camera.projection() * camera.view());
		
		// Call update function on objects
		Engine::instance().update_objects();

		// Render objects
		Engine::instance().render_objects(RenderMode::DEBUG, camera);

		display.swap();

		//const physx::PxRenderBuffer& rb = scene.get()->getRenderBuffer();

		//for (physx::PxU32 i = 0; i < rb.getNbLines(); i++)
		//{
		//	const physx::PxDebugLine& line = rb.getLines()[i];
		//
		//	
		//}

		time.end();

		accumulator += time.delta_time();

		while (accumulator > 0.01f)
		{
			// Simulate physX
			scene.simulate(0.01f);
			accumulator -= 0.01f;
		}
	}*/

	return 0;
}