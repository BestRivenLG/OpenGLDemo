#include "GLTools.h"
#include "GLFrustum.h"
#include "StopWatch.h"

#include <GLUT/GLUT.h>
//三角形批次类，绘制图形
GLBatch triangleBatch;
//固定着色管理器
GLShaderManager shaderManager;

//视景体-投影矩阵通过它来设置
GLFrustum viewFrustum;
GLTriangleBatch torusBatch;

GLfloat vVert[] = {
    0.5,0.5,0,
    -0.5,0.5,0,
    0,-0.5,0
};
void display(){
    /*绘制三角形
    glClear(GL_COLOR_BUFFER_BIT);
    GLfloat vColor[] = {0.0f, 1.0f, 1.0f, 1.0f};
    shaderManager.UseStockShader(GLT_SHADER_IDENTITY,vColor);
    triangleBatch.Draw();
    glutSwapBuffers();
     */
    
    //建立一个基于时间变化的动画
    static CStopWatch rotTimer;
    
    float yRot = rotTimer.GetElapsedSeconds() * 60.0f;
    
    M3DMatrix44f mTranslate,mRotate,mModelView,mModelViewProjection;
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //将圆球像Z轴负方向移动2.5个单位长度
    m3dTranslationMatrix44(mTranslate, 0.0f, 0.0f, -2.5f);
    
    //旋转
    m3dRotationMatrix44(mRotate, m3dDegToRad(yRot), 0.0f, 1.0f, 0.0f);
    
    //将平移和旋转的矩阵进行叉乘,产生一个新的矩阵mModelView
    m3dMatrixMultiply44(mModelView, mTranslate, mRotate);
    
    //模型视图矩阵 和 投影矩阵
    //将投影矩阵 与 模型视图矩阵进行叉乘 ,将变化最终结果通过矩阵叉乘的方式应用到mModelViewProjection中来
    m3dMatrixMultiply44(mModelViewProjection,  viewFrustum.GetProjectionMatrix(),mModelView);
    
    GLfloat vBlack[] = {1.0f,1.0f,0.0f,1.0f};

    //平面着色器来渲染图像
    shaderManager.UseStockShader(GLT_SHADER_FLAT,mModelViewProjection,vBlack);
    triangleBatch.Draw();

    torusBatch.Draw();
    glutSwapBuffers();
    glutPostRedisplay();
}

void setupRC(){
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    shaderManager.InitializeStockShaders();
    /*绘制三角形
    triangleBatch.Begin(GL_LINE_LOOP, 3);
    triangleBatch.CopyVertexData3f(vVert);
    triangleBatch.End();
    */
    
    //柱体
//    gltMakeCylinder(torusBatch, 0.3f, 0.5f, 1 , 3, 1);
    //碟
//    gltMakeDisk(torusBatch, 0.3f, 0.5f, 10, 20);
    //正方体
//    gltMakeCube(triangleBatch, 0.5f);
    //球体
    gltMakeSphere(torusBatch, 0.4f, 10, 20);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPointSize(5.0f);

}

// 设置视口和投影矩阵
void ChangeSize(int width, int height){
    if (height==0) {
        height=1;
    }
    glViewport(0, 0, width, height);
    //设置投影矩阵
    viewFrustum.SetPerspective(35.0f, float(width)/float(height), 1.0f, 1000.0f);
}

int main(int argc, char * argv[])
{
    gltSetWorkingDirectory(argv[0]);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH|GLUT_STENCIL);
    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutCreateWindow("LGWindows");
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(display);
    GLenum status = glewInit();
    if (GLEW_OK != status) {
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(status));
        return 1;
    }
    setupRC();
    glutMainLoop();
    return 0;
}
