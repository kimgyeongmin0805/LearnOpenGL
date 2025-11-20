# Creating a Window
### 라이브러리 설치 방법
* GLFW: [설치방법](https://learnopengl.com/Getting-started/Creating-a-window), [다운로드](https://www.glfw.org/download.html)
* GLAD: [설치방법](https://learnopengl.com/Getting-started/Creating-a-window), [다운로드](https://glad.dav1d.de/)

### 요약
> **GLFW 초기화**

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

* OpenGL 3.3 Core Profile 설정
* GLFW 시작 및 설정
  
> **Window 생성 + OpenGL Context 설정**

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

* 800x600 크기 윈도우 생성
* 실패 시 종료
* 생성된 윈도우에 OpenGL context 활성화

> **GLAD 로딩**

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

* OpenGL 함수 주소를 런타임에 불러오기
* GLAD 초기화 실패 시 종료

> **Viewport 설정**

	void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
		glViewPort(0, 0, width, height);
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

> **입력 처리**

	void processInput(GLFWwindow* window) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWinodwShouldClose(window, true);
		}
	}

* ESC 입력 시 창을 닫도록 플래그 설정

> **Render Loop**

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

* 창을 닫을 때까지 반복
* 매 프레임 배경색 설정 + 화면 클리어
* 버퍼 스왑
* 이벤트 처리