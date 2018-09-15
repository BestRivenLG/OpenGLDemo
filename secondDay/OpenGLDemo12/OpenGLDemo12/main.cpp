#include "GLTools.h"
#include "GLMatrixStack.h"

#include "GLShaderManager.h"
#include "GLGeometryTransform.h"
#include "GLFrame.h"
#include "GLFrustum.h"
#include <GLUT/GLUT.h>
GLShaderManager        shaderManager;
//管道，用来管理投影视图矩阵堆栈和模型视图矩阵堆栈的
GLGeometryTransform    transformPipeline;
//球
GLTriangleBatch     sphereBatch;
//环
GLTriangleBatch     torusBatch;
//圆柱
GLTriangleBatch     cylinderBatch;
//锥
GLTriangleBatch     coneBatch;
//磁盘
GLTriangleBatch     diskBatch;

//观察者位置
GLFrame                cameraFrame;

//模型视图矩阵堆栈
GLMatrixStack        modelViewMatrix;

//投影视图矩阵堆栈
GLMatrixStack        projectionMatrix;
//视景体，用来构造投影矩阵
GLFrustum            viewFrustum;
//世界坐标位置
GLFrame             objectFrame;

GLfloat vColor[] = {1.0,0,0,1.0},vYellow[] = {1.0,1.0,1.0,1.0};
//空格的标记
int nStep = 0;

void DrawWireFramedBatch(GLTriangleBatch* pBatch)
{
    shaderManager.UseStockShader(GLT_SHADER_FLAT,transformPipeline.GetModelViewProjectionMatrix(),vColor);
    pBatch->Draw();
    
    //2.黑色轮廓
    glPolygonOffset(-0.1f, -0.1f);
    
    //3.开启混合抗锯齿,对线
    glEnable(GL_LINE_SMOOTH);
    
    //4.开启混合
    glEnable(GL_BLEND);
    
    //5.颜色混合
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    //6.
    glEnable(GL_POLYGON_OFFSET_LINE);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    glLineWidth(2.5f);
    
    //使用平面着色器绘制黑色边框线条
    shaderManager.UseStockShader(GLT_SHADER_FLAT,transformPipeline.GetModelViewProjectionMatrix(),vYellow);
    
    pBatch->Draw();
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_POLYGON_OFFSET_LINE);
    glLineWidth(1.0f);
    glDisable(GL_BLEND);
    glDisable(GL_LINE_SMOOTH);
    
}

//点击空格，切换渲染图形
void KeyPressFunc(unsigned char key, int x, int y)
{
    if(key == 32)//空格
    {
        nStep++;
        
        if(nStep > 4)
            nStep = 0;
    }
    
    switch (nStep) {
        case 0:
            glutSetWindowTitle("Sphere");
            break;
        case 1:
            glutSetWindowTitle("Torus");

            break;
        case 2:
            glutSetWindowTitle("Cylinder");

            break;
        case 3:
            glutSetWindowTitle("Cone");

            break;
        case 4:
            glutSetWindowTitle("Disk");

            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void RenderScene(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    modelViewMatrix.PushMatrix();
    M3DMatrix44f camera;
    cameraFrame.GetCameraMatrix(camera);
//    modelViewMatrix.MultMatrix(camera);
    
    modelViewMatrix.MultMatrix(camera);
    modelViewMatrix.Translate(0, 0, -20);
   
    M3DMatrix44f mObjectFrame;
    objectFrame.GetMatrix(mObjectFrame);
    modelViewMatrix.MultMatrix(objectFrame);
//    shaderManager.UseStockShader(GLT_SHADER_FLAT,transformPipeline.GetModelViewProjectionMatrix(),vColor);
//    sphereBatch.Draw();
    
    switch (nStep) {
        case 0:
            DrawWireFramedBatch(&sphereBatch);
            break;
        case 1:
            DrawWireFramedBatch(&torusBatch);

            break;
        case 2:
            DrawWireFramedBatch(&cylinderBatch);

            break;
        case 3:
            DrawWireFramedBatch(&coneBatch);

            break;
        case 4:
            DrawWireFramedBatch(&diskBatch);

            break;
        default:
            break;
    }
    
    
    modelViewMatrix.PopMatrix();
    glutSwapBuffers();
}
void ChangeSize(int w, int h)
{
    if (h == 0) {
        h = 1;
    }
    glViewport(0, 0, w, h);
    
    //透视投影
    viewFrustum.SetPerspective(35.0f, float(w)/float(h), 1.0f, 500.0f);
    
    projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
    
    //对模型视图矩阵加载一个单元矩阵
    modelViewMatrix.LoadIdentity();
//    transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
    
}
void SpecialKeys(int key, int x, int y)
{
    if (GLUT_KEY_UP == key) {
        objectFrame.RotateWorld(m3dDegToRad(-5.0f), 1.0f, 0.0f, 0.0f);
    }
    
    if (GLUT_KEY_DOWN == key) {
        objectFrame.RotateWorld(m3dDegToRad(5.0f), 1.0f, 0.0f, 0.0f);
    }
    
    if (GLUT_KEY_LEFT== key) {
        objectFrame.RotateWorld(m3dDegToRad(-5.0f), 0.0f, 1.0f, 0.0f);
    }
    
    if (GLUT_KEY_RIGHT == key) {
        objectFrame.RotateWorld(m3dDegToRad(5.0f), 0.0f, 1.0f, 0.0f);

    }
    glutPostRedisplay();

}

void SetupRC()
{
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
    shaderManager.InitializeStockShaders();
    //开启深度测试
    glEnable(GL_DEPTH_TEST);
    
    transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
    
//    cameraFrame.MoveForward(-20);
    //球
    /*
     gltMakeSphere(GLTriangleBatch& sphereBatch, GLfloat fRadius, GLint iSlices, GLint iStacks);
     参数1:三角形批次类对象
     参数2:球体的半径
     参数3:从球体底部到顶部的三角形带的数量.其实球体是一圈一圈的三角形带组成的.
     参数4:围绕球体一圈排列的三角形对数
     */
    gltMakeSphere(sphereBatch, 3.0f, 10, 20);
    
    //甜甜圈
    /*
     gltMakeTorus(GLTriangleBatch& torusBatch, GLfloat majorRadius, GLfloat minorRadius, GLint numMajor, GLint numMinor);
     参数1:三角形批次类
     参数2:甜甜圈的中心到外圈的半径
     参数3:甜甜圈的中心到内圈的半径
     参数4:沿着主半径的三角形的对数
     参数5:颜色内部较小半径的三角形对数.!!!尽量!!!:numMajor = 2 *numMinor
     */
    gltMakeTorus(torusBatch, 3.0f, 0.75f, 24, 12);
    
    //圆柱
    /*
     gltMakeCylinder(GLTriangleBatch& cylinderBatch, GLfloat baseRadius, GLfloat topRadius, GLfloat fLength, GLint numSlices, GLint numStacks);
     参数1:三角形批次对象
     参数2:底部半径
     参数3:头部半径
     参数4:圆柱的长度
     参数5:三角形对的数量
     参数6:底部堆叠到顶部圆环的三级形对数
     */
    gltMakeCylinder(cylinderBatch, 2.0f, 2.0f, 3.0f, 15.0f, 2);
    
    
    //圆锥
    //圆柱体:从0开始向Z的正方向延伸
    //圆锥体:一端半径为0,另一端半径可指定
    gltMakeCylinder(coneBatch, 2.0f, 0, 3, 13.0f, 2);
    //磁盘
    /*
     gltMakeDisk(GLTriangleBatch& diskBatch, GLfloat innerRadius, GLfloat outerRadius, GLint nSlices, GLint nStacks);
     //参数1:三角形批次类
     //参数2:内圆半径
     //参数3:外圆半径
     //参数4:圆盘围绕Z轴的三角形对数
     //参数5:y圆盘外圈到内圈的三角形对数
     */
    gltMakeDisk(diskBatch, 1.5, 3.0, 13, 3);
}

int main(int argc, char * argv[]) {
    gltSetWorkingDirectory(argv[0]);
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Sphere");
    glutReshapeFunc(ChangeSize);
    glutKeyboardFunc(KeyPressFunc);
    glutSpecialFunc(SpecialKeys);
    glutDisplayFunc(RenderScene);
    
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
        return 1;
    }
    
    
    SetupRC();
    
    glutMainLoop();
    return 0;
    
}
