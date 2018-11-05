#include "GLTools.h"
#include <GLUT/GLUT.h>

GLShaderManager shaderManager;
GLBatch moveBatch;

GLBatch greenBatch;
GLBatch redBatch;
GLBatch blueBatch;
GLBatch blackBatch;


GLfloat blockSize = 0.2f;
GLfloat vVerts[] = {
    -blockSize, -blockSize, 0.0f,
    blockSize, -blockSize, 0.0f,
    blockSize,  blockSize, 0.0f,
    -blockSize,  blockSize, 0.0f
    
};

void display(){
    
    glClear(GL_COLOR_BUFFER_BIT);

    GLfloat vYellow[] = {1.0f,1.0,0.0,0.5f};

    GLfloat vRed[] = {1.0f,0.0,0.0,1.0f};
    GLfloat vGreen[] = {0.0f,1.0f,0.0f,1.0f};
    GLfloat vBlue[] = {0.0f,0.0f,1.0f,1.0f};
    GLfloat vBlack[] = {0.0f,0.0f,0.0f,1.0f};

    //将4个固定的矩阵绘制到屏幕上
    //使用单元着色器
    shaderManager.UseStockShader(GLT_SHADER_IDENTITY,vGreen);
    greenBatch.Draw();
    
    shaderManager.UseStockShader(GLT_SHADER_IDENTITY,vRed);
    redBatch.Draw();
    
    shaderManager.UseStockShader(GLT_SHADER_IDENTITY,vBlue);
    blueBatch.Draw();
    
    shaderManager.UseStockShader(GLT_SHADER_IDENTITY,vBlack);
    blackBatch.Draw();
    
    
    //组合(颜色混合的核心代码)
    glEnable(GL_BLEND);
    
    //2.设置颜色混合因子
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //3.使用着色器管理器
    shaderManager.UseStockShader(GLT_SHADER_IDENTITY,vYellow);
    
    //4.绘制
    moveBatch.Draw();
    
    //5.关闭混合功能
    glDisable(GL_BLEND);

    
    //强制执行缓存区
    glutSwapBuffers();
}

void setupRC(){
    
    //1.设置颜色到缓冲区
//    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    //2.初始化
    shaderManager.InitializeStockShaders();
    
    //3.绘制第一个可移动矩形块
    moveBatch.Begin(GL_TRIANGLE_FAN, 4);
    moveBatch.CopyVertexData3f(vVerts);
    moveBatch.End();
    
    //4.绘制4个固定的矩形块
    //第一个固定矩形块!指定顶点
    GLfloat vBlock[] = {
        0.25f,0.25f,0.0f,
        0.75f,0.25,0.0f,
        0.75f,0.75f,0.0f,
        0.25f,0.75f,0.0f
    };
    greenBatch.Begin(GL_TRIANGLE_FAN, 4);
    greenBatch.CopyVertexData3f(vBlock);
    greenBatch.End();
    
    //2.第二个矩形块
    GLfloat vBlock2[] = {
        -0.75f,0.25f,0.0f,
        -0.25f,0.25f,0.0f,
        -0.25f,0.75f,0.0f,
        -0.75f,0.75f,0.0f
    };
    redBatch.Begin(GL_TRIANGLE_FAN, 4);
    redBatch.CopyVertexData3f(vBlock2);
    redBatch.End();
    
    //3.第三个矩形块
    GLfloat vBlock3[] = { -0.75f, -0.75f, 0.0f,
        -0.25f, -0.75f, 0.0f,
        -0.25f, -0.25f, 0.0f,
        -0.75f, -0.25f, 0.0f};
    
    blueBatch.Begin(GL_TRIANGLE_FAN, 4);
    blueBatch.CopyVertexData3f(vBlock3);
    blueBatch.End();
    
    //第四个矩形块
    GLfloat vBlock4[] = { 0.25f, -0.75f, 0.0f,
        0.75f, -0.75f, 0.0f,
        0.75f, -0.25f, 0.0f,
        0.25f, -0.25f, 0.0f};
    
    blackBatch.Begin(GL_TRIANGLE_FAN, 4);
    blackBatch.CopyVertexData3f(vBlock4);
    blackBatch.End();
}

void moveAction(int key, int x, int y){
    //参考第一节课内容
    GLfloat stepSize = 0.025f;
    
    GLfloat blockX = vVerts[0];
    GLfloat blockY = vVerts[7];
    
    if(key == GLUT_KEY_UP)
        blockY += stepSize;
    
    if(key == GLUT_KEY_DOWN)
        blockY -= stepSize;
    
    if(key == GLUT_KEY_LEFT)
        blockX -= stepSize;
    
    if(key == GLUT_KEY_RIGHT)
        blockX += stepSize;
    
    if(blockX < -1.0f) blockX = -1.0f;
    if(blockX > (1.0f - blockSize * 2)) blockX = 1.0f - blockSize * 2;;
    if(blockY < -1.0f + blockSize * 2)  blockY = -1.0f + blockSize * 2;
    if(blockY > 1.0f) blockY = 1.0f;
    
    vVerts[0] = blockX;
    vVerts[1] = blockY - blockSize*2;
    
    vVerts[3] = blockX + blockSize*2;
    vVerts[4] = blockY - blockSize*2;
    
    vVerts[6] = blockX + blockSize*2;
    vVerts[7] = blockY;
    
    vVerts[9] = blockX;
    vVerts[10] = blockY;
    
    moveBatch.CopyVertexData3f(vVerts);
    
    glutPostRedisplay();
}

int main(int argc,char * argv[]){
    gltSetWorkingDirectory(argv[0]);
    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutCreateWindow("LGWindows");
    glutDisplayFunc(display);
    glutSpecialFunc(moveAction);
    GLenum status = glewInit();
    if (GLEW_OK!=status) {
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(status));
        return 1;
    }
    
    setupRC();
    glutMainLoop();
    return 0;
}


