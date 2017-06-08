#include "Graphics.h"
#include "InputManager.h"
#include "CameraControl.h"

int Graphics::init()
{
	window = nullptr;

	/* Initialize the library */
	if (!glfwInit()) return -1;

	glfwWindowHint(GLFW_SAMPLES, 32);

	// window resolution
	/*const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	width_window = mode->width * 0.8f;
	height_window = mode->height * 0.8f;*/

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width_window, height_window, "Break Bricks 3D ! by. 2013111995 LuciZ::Zeikar", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}


	// input
	// initialize the singleton
	InputManager &hexmap = InputManager::getInstance(); 

	//The glfw callback is set up as follows:   
	// specifying the static callback method, which internally forwards it to the instance method
	glfwSetScrollCallback(window, &InputManager::scrollCallback);
	glfwSetMouseButtonCallback(window, &InputManager::mouseButtonCallback);
	glfwSetCursorPosCallback(window, &InputManager::cursorPositionCallback);
	glfwSetKeyCallback(window, &InputManager::keyCallback);


	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}


	//glClearColor(1, 1, 1, 1); // while background
	glClearColor(0.4f, 0.4f, 0.4f, 1.0f);


	printf("%s\n", glGetString(GL_VERSION));

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	const float aspect = (float)width / (float)height;
	glViewport(0, 0, width, height);
	glOrtho(-1.2*aspect, 1.2*aspect, -1.2, 1.2, -100.0, 100.0);
	gluLookAt(0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0, 1, 0);//camera takes care of it

	const float zNear = 0.001, zFar = 100.0, fov = 45.0;			// UI
	gl_world.camera_.Resize(width, height, fov, zNear, zFar);
	gl_world.camera_.UpdateDolly(-90.0f);
	gl_world.camera_.StartMousePan(70, 30);
	gl_world.camera_.ProcessMouseMotion(20, 5);
	gl_world.camera_.EndMousePan(20, 5);
	gl_world.initShaders();
	
	// depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	glDisable(GL_COLOR_MATERIAL);

	glLoadIdentity();
}

int Graphics::mainLoop()
{
	/* Swap front and back buffers */
	glfwSwapBuffers(window);

	/* Render here */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gl_world.camera_.ContinueRotation();

	glm::mat4 vp = gl_world.camera_.GetWorldViewMatrix();


	// drawing axes
	glUseProgram(0);

	glLoadMatrixf(&vp[0][0]);

	glLineWidth(2);
	//gl_world.drawAxes();
	

	// drawing objects
	glUseProgram(gl_world.shaders_.program_id_);

	gl_world.shaders_.sendUniform(vp, "mvp");


	/* Swap front and back buffers */
	//glfwSwapBuffers(window);

	/* Poll for and process events */
	glfwPollEvents();

	std::this_thread::sleep_for(std::chrono::milliseconds(1000 / FPS));

	return !glfwWindowShouldClose(window);
}

void Graphics::release()
{
	glfwTerminate();
}

GL2_World& Graphics::getGL_World()
{
	return gl_world;
}

GL2_Light& Graphics::getLight()
{
	return light;
}