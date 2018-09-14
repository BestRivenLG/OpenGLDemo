#include "GLTools.h"
#include "GLShaderManager.h"
#include "GLFrustum.h"
#include "GLBatch.h"
#include "GLMatrixStack.h"
#include "GLGeometryTransform.h"
#include "StopWatch.h"


#include <GLUT/GLUT.h>


//注意顺序，定义变量的顺序  shaderManager、modelViewMatrix、projectionMatrix要放在viewFrustum之前，不然是一片空白。且transformPipeline要在torusBatch之前
GLShaderManager shaderManager;
// 模型视图矩阵
GLMatrixStack modelViewMatrix;
//设置投影矩阵
GLMatrixStack projectionMatrix;

GLFrustum viewFrustum;

//几何图形变换管道
GLGeometryTransform transformPipeline;

GLTriangleBatch torusBatch;

//**定义公转球的批处理（公转自转）**
GLTriangleBatch     sphereBatch;//球批处理

//**定义公转球的批处理（公转自转）**
GLTriangleBatch     sphereBatch2;//球批处理

GLBatch floorBatch;// 地板批处理

//角色帧 照相机角色帧
GLFrame cameraFrame;


void ChangeSize(int width, int height)
{
    if (height == 0) {
        height = 1;
    }

    glViewport(0, 0, width, height);

    //设置投影矩阵
    viewFrustum.SetPerspective(35.0, float(width)/float(height), 1.0f, 100.0f);
    //获取投影矩阵到projectionMatrix
    projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());

    //变换管道中来使用
    transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
    
}

void display()
{
    static CStopWatch rotTime;//要定义静态
    float yRot = rotTime.GetElapsedSeconds()*60.0;
    //甜甜圈颜色
    static GLfloat vColor[] = {1.0f,1.0f,0.0f,1.0f};
    static GLfloat vRed[] = {1.0f,0.0f,0.0f,1.0f};
    static GLfloat vGreen[] = {0.0f,1.0f,0.0f,1.0f};
    static GLfloat vBlue[] = {0.0f,0.0f,1.0f,1.0f};

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    modelViewMatrix.PushMatrix();
    //设置观察者的矩阵
    M3DMatrix44f mCamera;
    cameraFrame.GetCameraMatrix(mCamera);
    modelViewMatrix.PushMatrix(mCamera);

    modelViewMatrix.Translate(0.0f, 0.0f, -5.0f);

    shaderManager.UseStockShader(GLT_SHADER_FLAT,transformPipeline.GetModelViewProjectionMatrix(),vBlue);
    floorBatch.Draw();
    

    modelViewMatrix.PushMatrix();
    modelViewMatrix.Rotate(yRot*-0.5f, 1.0f, 0.0f, 0.0f);
    modelViewMatrix.Translate(0, -0.8f, 0);
    shaderManager.UseStockShader(GLT_SHADER_FLAT,transformPipeline.GetModelViewProjectionMatrix(),vGreen);
    sphereBatch2.Draw();
    modelViewMatrix.PopMatrix();
    
    modelViewMatrix.PushMatrix();
    modelViewMatrix.Rotate(yRot, 0.0f, 1.0f, 0.0f);
    modelViewMatrix.Translate(-0.8f, 0, 0);
    shaderManager.UseStockShader(GLT_SHADER_FLAT,transformPipeline.GetModelViewProjectionMatrix(),vColor);
    torusBatch.Draw();
    modelViewMatrix.PopMatrix();


    //先自转再偏移
    modelViewMatrix.Rotate(yRot*-2.0f, 0.0f, 0.0, 1.0f);
    modelViewMatrix.Translate(-0.8f, 0, 0);
 shaderManager.UseStockShader(GLT_SHADER_FLAT,transformPipeline.GetModelViewProjectionMatrix(),vRed);
    sphereBatch.Draw();

    modelViewMatrix.PopMatrix();

    modelViewMatrix.PopMatrix();
    

    glutSwapBuffers();
    glutPostRedisplay();
}

void setupRC()
{
    shaderManager.InitializeStockShaders();
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glClearColor(0, 0, 0, 1.0);
    //绘制甜甜圈
    gltMakeTorus(torusBatch, 0.4f, 0.15, 30, 30);
    
    //绘制球
    gltMakeSphere(sphereBatch, 0.1f, 26, 13);
    
    gltMakeSphere(sphereBatch2, 0.2f, 26, 13);

    //绘制地板
    floorBatch.Begin(GL_LINES, 324);
    //地板的宽度
    for (GLfloat i = -20.0f; i<=20.0f; i+=0.5f) {
        floorBatch.Vertex3f(i, -1.0f, 20.0f);
        floorBatch.Vertex3f(i, -1.0f, -20.0f);
        floorBatch.Vertex3f(20.0f, -1.0f, i);
        floorBatch.Vertex3f(-20.0f, -1.0f, i);
    }
    floorBatch.End();
}

void specialKey(int key, int x, int y){
    float linar = 0.1f;
    float angular = float(m3dDegToRad(5.0f));

    if (key == GLUT_KEY_UP) {
        cameraFrame.MoveForward(linar);
    }
    
    if (key == GLUT_KEY_DOWN) {
        cameraFrame.MoveForward(-linar);
    }
    
    if (key == GLUT_KEY_LEFT) {
        cameraFrame.RotateWorld(angular, 0, 1.0f, 0);
    }
    
    if (key == GLUT_KEY_RIGHT) {
        cameraFrame.RotateWorld(angular, 0, -1.0f, 0);
    }
}

int main(int argc,char *argv[]){
    gltSetWorkingDirectory(argv[0]);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("LGWindows");

    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(display);
    glutSpecialFunc(specialKey);
    GLenum status = glewInit();
    if (GLEW_OK != status) {
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(status));
        return 1;
    }
    setupRC();
    glutMainLoop();
    return 0;
}
