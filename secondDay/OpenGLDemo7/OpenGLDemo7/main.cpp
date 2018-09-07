//
//  main.cpp
//  OpenGLDemo7
//
//  Created by hzx on 2018/9/7.
//  Copyright © 2018年 hzx. All rights reserved.
//
#include "GLShaderManager.h"
#include "GLFrustum.h"
#include "GLFrame.h"
#include "GLMatrixStack.h"
#include "GLGeometryTransform.h"
#include "GLTools.h"
#include <GLUT/GLUT.h>

GLShaderManager shaderManager;
GLBatch triangleBatch;

//使用GLFrustum类来设置透视投影
GLFrustum viewFrustum;

//设置角色帧，作为相机
GLFrame viewFrame;
GLMatrixStack modelViewMatix;

GLTriangleBatch torusBatch;
GLGeometryTransform transformPipeline;
GLMatrixStack projectionMatrix;

bool iCull = false;
bool iDepth = false;

void ChangeSize(int w, int h)
{
    //防止h变为0
    if(h == 0)
        h = 1;
    
    //设置视口窗口尺寸
    glViewport(0, 0, w, h);
    
    //setPerspective函数的参数是一个从顶点方向看去的视场角度（用角度值表示）
    // 设置透视模式，初始化其透视矩阵
    viewFrustum.SetPerspective(35.0f, float(w)/float(h), 1.0f, 100.0f);
    
    //把透视矩阵加载到透视矩阵对阵中
    //viewFrustum.GetProjectionMatrix() 获取投影矩阵
    //projectionMatrix.LoadMatrix(矩阵) 把矩阵加载到projectionMatrix中来
    projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());

    // 初始化渲染管线
    transformPipeline.SetMatrixStacks(modelViewMatix, projectionMatrix);
}


void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (iCull) {
        glEnable(GL_CULL_FACE);
        //2.指定模型
        glFrontFace(GL_CCW);
        //3.执行剔除
        glCullFace(GL_BACK);
    }else {
        glDisable(GL_CULL_FACE);
    }
    
    if (iDepth) {
        glEnable(GL_DEPTH_TEST);
    }else {
        glDisable(GL_DEPTH_TEST);
    }

    GLfloat vRed[] = {0.0f, 1.0f, 1.0f, 1.0f};

    modelViewMatix.PushMatrix(viewFrame);
    //默认光源着色器
    //参数1:GLT_SHADER_DEFAULT_LIGHT
    //参数2:模型视图矩阵
    //参数3:投影矩阵
    //参数4:颜色
    
    //transformPipeline 管理堆栈,模型视图堆栈\投影矩阵堆栈
    shaderManager.UseStockShader(GLT_SHADER_DEFAULT_LIGHT,transformPipeline.GetModelViewMatrix(),transformPipeline.GetProjectionMatrix(),vRed);

    //绘制
    torusBatch.Draw();
    modelViewMatix.PopMatrix();

//    shaderManager.UseStockShader(GLT_SHADER_FLAT,vRed);
//    triangleBatch.Draw();
    
    glutSwapBuffers();
}

void setupRC(){
    glClearColor(0.7, 0.7, 0.7, 1.0);
    shaderManager.InitializeStockShaders();
    gltMakeTorus(torusBatch, 1.0f, 0.3f, 52, 26);
    //设置点的大小
    glPointSize(4.0f);
    viewFrame.MoveForward(7.0f);
    
//    GLfloat vVerts[] = {
//        0.5,0.5,0,
//        -0.5,0.5,0,
//        0,-0.5,0
//    };
//    triangleBatch.Begin(GL_TRIANGLES, 3);
//    triangleBatch.CopyVertexData3f(vVerts);
//    triangleBatch.End();
}

void keyBoardChange(int key, int x, int y){
    if (GLUT_KEY_UP == key) {
        printf("上");
        
        viewFrame.RotateWorld(m3dDegToRad(-5), 1.0, 0, 0);
    }
    
    if (GLUT_KEY_DOWN == key) {
        printf("下");
        viewFrame.RotateWorld(m3dDegToRad(5), 1.0, 0, 0);

    }
    
    if (GLUT_KEY_RIGHT == key) {
        printf("右");
        viewFrame.RotateWorld(m3dDegToRad(5), 0, 1.0, 0);

    }
    
    if (GLUT_KEY_LEFT == key) {
        printf("左");
        viewFrame.RotateWorld(m3dDegToRad(-5), 0.0, 1.0, 0);

    }
    //重新刷新window
    glutPostRedisplay();

}
//右键菜单栏选项 点击右击菜单中的选项,则会调用到此处
void menuList(int i){
    printf("右击");
    if (i==1) {//正背面剔除
        iCull = !iCull;
        if (iCull) {
            glutChangeToMenuEntry(i, "正背面不剔除", 1);
        }else {
            glutChangeToMenuEntry(i, "正背面剔除", 1);
        }
    }
    if (i==2) {//深度测试
        iDepth = !iDepth;
        if (iDepth) {
            glutChangeToMenuEntry(i, "深度测试关闭", 2);
        }else {
            glutChangeToMenuEntry(i, "深度测试", 2);
        }
    }
    
    if (i==3) {
        glPolygonMode(GL_FRONT, GL_FILL);
    }
    
    if (i==4) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    
    if (i==5) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    }
    //重新刷新window
    glutPostRedisplay();

}

int main(int argc, char* argv[]){
    
    gltSetWorkingDirectory(argv[0]);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutCreateWindow("LGWindows");
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(display);
    glutSpecialFunc(keyBoardChange);
    glutCreateMenu(menuList);
    glutAddMenuEntry("正背面剔除", 1);
    glutAddMenuEntry("深度测试", 2);
    glutAddMenuEntry("Set Fill Mode", 3);
    glutAddMenuEntry("Set line Mode", 4);
    glutAddMenuEntry("Set Point Mode", 5);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    GLenum status = glewInit();
    if (GLEW_OK != status) {
        printf("GLEW Error:%s\n",glewGetErrorString(status));
        return 1;
    }
    setupRC();
    glutMainLoop();
    return 0;
}
