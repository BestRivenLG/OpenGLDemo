#include "GLTools.h"
#include <GLUT/GLUT.h>


void display(){
    glClearColor(0.8f, 0.8f, 0.8f, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glClearColor(0.6f, 0.6f, 0.6f, 1.0);
    glScissor(100, 100, 600, 400);
    //3.开启裁剪
    glEnable(GL_SCISSOR_TEST);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glClearColor(0.4f, 0.4f, 0.4f, 1.0);
    glScissor(200, 200, 400, 200);
    //3.开启裁剪
    glEnable(GL_SCISSOR_TEST);
    glClear(GL_COLOR_BUFFER_BIT);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0);
    glScissor(300, 250, 200, 100);
    //3.开启裁剪
    glEnable(GL_SCISSOR_TEST);
    glClear(GL_COLOR_BUFFER_BIT);
    
    //关闭裁剪测试
    glDisable(GL_SCISSOR_TEST);

    //强制执行缓存区
    glutSwapBuffers();
}

int main(int argc,char * argv[]){
    gltSetWorkingDirectory(argv[0]);
    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutCreateWindow("LGWindows");
    glutDisplayFunc(display);
    GLenum status = glewInit();
    if (GLEW_OK!=status) {
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(status));
        return 1;
    }
    glutMainLoop();
    return 0;
}

