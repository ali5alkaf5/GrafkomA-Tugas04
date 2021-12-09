#include "Demo.h"



Demo::Demo() {

}


Demo::~Demo() {
}



void Demo::Init() {
	BuildShaders();
	BuildDepthMap();

	BuildBeam(-4, 2, -4, 6.5, 1, 6.5, "wood.png", "wood_specular.png", VAO, VBO, EBO, texture, stexture);
	BuildBeam(1, -0.5, 1, 0.5, 2.5, 0.5, "wood_leg.png", "wood_leg_specular.png", VAOL1, VBOL1, EBOL1, textureL1, stextureL1);
	BuildBeam(-3, -0.5, -3, 0.5, 2.5, 0.5, "wood_leg.png", "wood_leg_specular.png", VAOL2, VBOL2, EBOL2, textureL2, stextureL2);
	BuildBeam(-3, -0.5, 1, 0.5, 2.5, 0.5, "wood_leg.png", "wood_leg_specular.png", VAOL3, VBOL3, EBOL2, textureL3, stextureL3);
	BuildBeam(1, -0.5, -3, 0.5, 2.5, 0.5, "wood_leg.png", "wood_leg_specular.png", VAOL4, VBOL4, EBOL4, textureL4, stextureL4);

	BuildTexturedPlane();
	InitCamera();
	InitObject();
}

void Demo::BuildBeam(float xOffset, float yOffset, float zOffset, float xLength, float yLength, float zLength, char* texturePath, char* specularPath, GLuint& VAO, GLuint VBO, GLuint EBO, GLuint& texture, GLuint& stexture) {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image(specularPath, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &stexture);
	glBindTexture(GL_TEXTURE_2D, stexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	image = SOIL_load_image(texturePath, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// front
		xOffset, yOffset, zOffset, 0, 0, 0, 1, 0, // 1
		xOffset + xLength, yOffset, zOffset, 0,1, 0, 1, 0, // 2
		xOffset + xLength, yOffset + yLength, zOffset, 1,1, 0, 1, 0, // 3
		xOffset, yOffset + yLength, zOffset, 1,0, 0, 1, 0, // 4
		// back
		xOffset + xLength, yOffset, zOffset + zLength, 0, 0, 0, 1, 0, // 5
		xOffset, yOffset, zOffset + zLength, 0,1, 0, 1, 0, // 6
		xOffset, yOffset + yLength, zOffset + zLength, 1,1, 0, 1, 0, // 7
		xOffset + xLength, yOffset + yLength, zOffset + zLength, 1,0, 0, 1, 0, // 8
		// left
		xOffset, yOffset, zOffset, 0,0, 0, 1, 0, // 9
		xOffset, yOffset, zOffset + zLength, 0,1, 0, 1, 0, // 10
		xOffset, yOffset + yLength, zOffset + zLength, 1,1, 0, 1, 0, // 11
		xOffset, yOffset + yLength, zOffset, 1,0, 0, 1, 0, // 12
		// right
		xLength + xOffset, yOffset + yLength, zOffset, 0, 0, 0, 1, 0, // 13
		xLength + xOffset, yOffset + yLength, zOffset + zLength, 0, 1, 0, 1, 0, // 14
		xLength + xOffset, yOffset, zOffset + zLength, 1, 1, 0, 1, 0, // 15
		xLength + xOffset, yOffset, zOffset, 1, 0, 0, 1, 0, // 16
		// top
		xOffset, yOffset + yLength, zOffset, 0, 0, 0, 1, 0, // 17
		xLength + xOffset, yOffset + yLength, zOffset, 0, 1, 0, 1, 0, // 18
		xLength + xOffset, yOffset + yLength, zOffset + zLength, 1, 1, 0, 1, 0, // 19
		xOffset, yOffset + yLength, zOffset + zLength, 1, 0, 0, 1, 0, // 20
		// bottom
		xOffset, yOffset, zOffset, 0, 0, 0, 1, 0, // 21
		xLength + xOffset, yOffset, zOffset, 0, 1, 0, 1, 0, // 22
		xLength + xOffset, yOffset, zOffset + zLength, 1, 1, 0, 1, 0, // 23
		xOffset, yOffset, zOffset + zLength, 1, 0, 0, 1, 0, // 24
	};
	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,	 // back
		8,  9,  10, 8,  10, 11,  // right
		12, 13, 14, 12, 14, 15,  // left
		16, 17, 18, 16, 18, 19,  // upper
		20, 21, 22, 20, 22, 23   // bottom
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DeInit() {
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glDeleteVertexArrays(1, &VAOL1);
	glDeleteBuffers(1, &VBOL1);
	glDeleteBuffers(1, &EBOL1);

	glDeleteVertexArrays(1, &VAOL2);
	glDeleteBuffers(1, &VBOL2);
	glDeleteBuffers(1, &EBOL2);

	glDeleteVertexArrays(1, &VAOL3);
	glDeleteBuffers(1, &VBOL3);
	glDeleteBuffers(1, &EBOL3);

	glDeleteVertexArrays(1, &VAOL4);
	glDeleteBuffers(1, &VBOL4);
	glDeleteBuffers(1, &EBOL4);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &planeVBO);
	glDeleteBuffers(1, &planeEBO);
	glDeleteBuffers(1, &depthMapFBO);
}

void Demo::InitCamera()
{
	posCamX = 10.0f;
	posCamY = 3.0f;
	posCamZ = 0.0f;
	viewCamX = 0.0f;
	viewCamY = 1.0f;
	viewCamZ = 0.0f;
	upCamX = 0.0f;
	upCamY = 1.0f;
	upCamZ = 0.0f;
	CAMERA_SPEED = 0.001f;
	fovy = 45.0f;
	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Demo::InitObject() {
	objX = 0.0f;
	objY = 0.0f;
	objZ = 0.0f;
	viewObjX = 0.0f;
	viewObjY = 0.5f;
	viewObjZ = -0.5f;
	MOVE_SPEED = 0.02f;
}

void Demo::MoveCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	// forward positive cameraspeed and backward negative -cameraspeed.
	posCamX = posCamX + x * speed;
	posCamZ = posCamZ + z * speed;
	viewCamX = viewCamX + x * speed;
	viewCamZ = viewCamZ + z * speed;
}

void Demo::StrafeCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	float orthoX = -z;
	float orthoZ = x;

	// left positive cameraspeed and right negative -cameraspeed.
	posCamX = posCamX + orthoX * speed;
	posCamZ = posCamZ + orthoZ * speed;
	viewCamX = viewCamX + orthoX * speed;
	viewCamZ = viewCamZ + orthoZ * speed;
}

void Demo::RotateCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	viewCamZ = (float)(posCamZ + glm::sin(speed) * x + glm::cos(speed) * z);
	viewCamX = (float)(posCamX + glm::cos(speed) * x - glm::sin(speed) * z);
}

void Demo::MoveObject(float speed)
{
	float x = viewObjX - objX;
	float z = viewObjZ - objZ;
	objX = objX + x * speed;
	objZ = objZ + z * speed;
	viewObjX = viewObjX + x * speed;
	viewObjZ = viewObjZ + z * speed;
}

void Demo::StrafeObject(float speed)
{
	float x = viewObjX - objX;
	float z = viewObjZ - objZ;
	float orthoX = -z;
	float orthoZ = x;

	objX = objX + orthoX * speed;
	objZ = objZ + orthoZ * speed;
	viewObjX = viewObjX + orthoX * speed;
	viewObjZ = viewObjZ + orthoZ * speed;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Demo::ProcessInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		if (fovy < 90) {
			fovy += 0.0001f;
		}
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (fovy > 0) {
			fovy -= 0.0001f;
		}
	}

	// Object Movement
	if (glfwGetKey(window, GLFW_KEY_UP)) {
		MoveObject(MOVE_SPEED);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN)) {
		MoveObject(-MOVE_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT)) {
		StrafeObject(-MOVE_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
		StrafeObject(MOVE_SPEED);
	}

	// update camera movement 
	// -------------
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		MoveCamera(CAMERA_SPEED);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		MoveCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		StrafeCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		StrafeCamera(CAMERA_SPEED);
	}

	// update camera rotation
	// ----------------------
	double mouseX, mouseY;
	double midX = screenWidth / 2;
	double midY = screenHeight / 2;
	float angleY = 0.0f;
	float angleZ = 0.0f;

	// Get mouse position
	glfwGetCursorPos(window, &mouseX, &mouseY);
	if ((mouseX == midX) && (mouseY == midY)) {
		return;
	}

	// Set mouse position
	glfwSetCursorPos(window, midX, midY);

	// Get the direction from the mouse cursor, set a resonable maneuvering speed
	angleY = (float)((midX - mouseX)) / 1000;
	angleZ = (float)((midY - mouseY)) / 1000;

	// The higher the value is the faster the camera looks around.
	viewCamY += angleZ * 2;

	// limit the rotation around the x-axis
	if ((viewCamY - posCamY) > 8) {
		viewCamY = posCamY + 8;
	}
	if ((viewCamY - posCamY) < -8) {
		viewCamY = posCamY - 8;
	}
	RotateCamera(-angleY);
}

void Demo::Update(double deltaTime) {
}

void Demo::Render() {
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	
	// Step 1 Render depth of scene to texture
	// ----------------------------------------
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane = 1.0f, far_plane = 7.5f;
	lightProjection = glm::ortho(-5.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;
	// render scene from light's point of view
	UseShader(this->depthmapShader);
	glUniformMatrix4fv(glGetUniformLocation(this->depthmapShader, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
	glViewport(0, 0, this->SHADOW_WIDTH, this->SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	DrawBeam(this->depthmapShader, texture, stexture, VAO, GL_FALSE);
	DrawBeam(this->depthmapShader, textureL1, stextureL1, VAOL1, GL_FALSE);
	DrawBeam(this->depthmapShader, textureL2, stextureL2, VAOL2, GL_FALSE);
	DrawBeam(this->depthmapShader, textureL3, stextureL3, VAOL3, GL_FALSE);
	DrawBeam(this->depthmapShader, textureL4, stextureL4, VAOL4, GL_FALSE);
	DrawTexturedPlane(this->depthmapShader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Step 2 Render scene normally using generated depth map
	// ------------------------------------------------------
	glViewport(0, 0, this->screenWidth, this->screenHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Pass perspective projection matrix
	UseShader(this->shadowmapShader);
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(this->shadowmapShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	// LookAt camera (position, target/direction, up)
	glm::vec3 cameraPos = glm::vec3(posCamX, posCamY, posCamZ);
	glm::vec3 cameraFront = glm::vec3(viewCamX, viewCamY, viewCamZ);
	glm::mat4 view = glm::lookAt(cameraPos, cameraFront, glm::vec3(upCamX, upCamY, upCamZ));
	GLint viewLoc = glGetUniformLocation(this->shadowmapShader, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	// Blend Function
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Setting Light Attributes
	glUniformMatrix4fv(glGetUniformLocation(this->shadowmapShader, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "lightPos"), -2.0f, 4.0f, -1.0f);

	GLint viewPosLoc = glGetUniformLocation(this->shadowmapShader, "viewPos");
	glUniform3f(viewPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);

	glUniform3f(glGetUniformLocation(this->shadowmapShader, "dirLight.direction"), -1.0f, -1.0f, -1.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "dirLight.ambient"), 0.1f, 0.1f, 0.1f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "dirLight.diffuse"), 0.2f, 0.2f, 0.2f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "dirLight.specular"), 0.1f, 0.1f, 0.1f);
	// point light 1
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[0].position"), 0.0f, 3.0f, 0.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[0].ambient"), 1.0f, 0.0f, 1.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[0].diffuse"), 0.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[0].specular"), 1.0f, 0.0f, 0.0f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "pointLights[0].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "pointLights[0].linear"), 0.09f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "pointLights[0].quadratic"), 0.032f);
	// point light 2
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[1].position"), -2.0f, 3.0f, 0.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[1].ambient"), 0.0f, 1.0f, 0.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[1].diffuse"), 0.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[1].specular"), 0.0f, 1.0f, 0.0f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "pointLights[1].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "pointLights[1].linear"), 0.09f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "pointLights[1].quadratic"), 0.032f);
	// point light 3
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[2].position"), 2.0f, 3.0f, 0.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[2].ambient"), 0.0f, 0.0f, 1.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[2].diffuse"), 0.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[2].specular"), 0.0f, 0.0f, 1.0f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "pointLights[2].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "pointLights[2].linear"), 0.09f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "pointLights[2].quadratic"), 0.032f);
	// point light 4
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[3].position"), 0.0f, 3.0f, 2.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[3].ambient"), 0.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[3].diffuse"), 0.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "pointLights[3].specular"), 0.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "pointLights[3].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "pointLights[3].linear"), 0.09f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "pointLights[3].quadratic"), 0.032f);
	// spotLight
	glUniform3fv(glGetUniformLocation(this->shadowmapShader, "spotLight.position"), 1, &cameraPos[0]);
	glUniform3fv(glGetUniformLocation(this->shadowmapShader, "spotLight.direction"), 1, &cameraFront[0]);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "spotLight.ambient"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "spotLight.diffuse"), 0.5f, 0.5f, 0.5f);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "spotLight.specular"), 1.0f, 0.0f, 1.0f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "spotLight.constant"), 1.0f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "spotLight.linear"), 0.09f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "spotLight.quadratic"), 0.032f);
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
	glUniform1f(glGetUniformLocation(this->shadowmapShader, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));


	// Configure Shaders
	glUniform1i(glGetUniformLocation(this->shadowmapShader, "diffuseTexture"), 0);
	glUniform1i(glGetUniformLocation(this->shadowmapShader, "shadowMap"), 1);

	// Render floor
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, plane_texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	DrawTexturedPlane(this->shadowmapShader);
	
	DrawBeam(this->shadowmapShader, texture, stexture, VAO, GL_TRUE);
	DrawBeam(this->shadowmapShader, textureL1, stextureL1, VAOL1, GL_TRUE);
	DrawBeam(this->shadowmapShader, textureL2, stextureL2, VAOL2, GL_TRUE);
	DrawBeam(this->shadowmapShader, textureL3, stextureL3, VAOL3, GL_TRUE);
	DrawBeam(this->shadowmapShader, textureL4, stextureL4, VAOL4, GL_TRUE);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void Demo::BuildTexturedPlane()
{
	// Load and create a texture 
	glGenTextures(1, &plane_texture);
	glBindTexture(GL_TEXTURE_2D, plane_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("marble_diffusemap.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &stexture2);
	glBindTexture(GL_TEXTURE_2D, stexture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	image = SOIL_load_image("marble_specularmap.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		// bottom
		-50.0, -0.5, -50.0,  0,  0, 0.0f,  1.0f,  0.0f,
		50.0, -0.5, -50.0, 50,  0, 0.0f,  1.0f,  0.0f,
		50.0, -0.5,  50.0, 50, 50, 0.0f,  1.0f,  0.0f,
		-50.0, -0.5,  50.0,  0, 50, 0.0f,  1.0f,  0.0f,
	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2 };

	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glGenBuffers(1, &planeEBO);

	glBindVertexArray(planeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Normal attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO
}

void Demo::DrawTexturedPlane(GLuint shader)
{
	UseShader(shader);
	glBindVertexArray(planeVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildDepthMap() {
	// configure depth map FBO
	// -----------------------
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, this->SHADOW_WIDTH, this->SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Demo::DrawBeam(GLuint& shader, GLuint& texture, GLuint& stexture, GLuint& VAO, GLboolean useTexture)
{
	UseShader(shader);

	if (useTexture) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(this->shadowmapShader, "material.diffuse"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, stexture);
		glUniform1i(glGetUniformLocation(this->shadowmapShader, "material.specular"), 1);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, depthMap);
	}

	GLint shininessMatLoc = glGetUniformLocation(this->shadowmapShader, "material.shininess");
	glUniform1f(shininessMatLoc, 0.4f);
	glBindVertexArray(VAO);

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(objX, objY, objZ));

	GLint modelLoc = glGetUniformLocation(this->shadowmapShader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}


void Demo::BuildShaders()
{
	// build and compile our shader program
	// ------------------------------------
	shadowmapShader = BuildShader("shadowMapping.vert", "shadowMapping.frag", nullptr);
	depthmapShader = BuildShader("depthMap.vert", "depthMap.frag", nullptr);
}


int main(int argc, char** argv) {
	RenderEngine &app = Demo();
	app.Start("Shadow Mapping Demo", 800, 600, false, false);
}