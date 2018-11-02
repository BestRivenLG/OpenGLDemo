#include "GLTools.h"
#include "GLShaderManager.h"

#include <GLUT/GLUT.h>

GLShaderManager shaderManager;
GLBatch triangleBatch;

//blockSize 边长
GLfloat blockSize = 0.1f;

//正方形的4个点坐标
GLfloat vVerts[] = {
    -blockSize,-blockSize,0.0f,
    blockSize,-blockSize,0.0f,
    blockSize,blockSize,0.0f,
    -blockSize,blockSize,0.0f
};


void changeSize(int width, int height){
    
}

void specialKey(int key, int x, int y){
    GLfloat stepSize = 0.025f;
    GLfloat blockX = vVerts[0];
    GLfloat blockY = vVerts[10];

    
    if (key == GLUT_KEY_LEFT) {
        blockX -= stepSize;

    }
    
    if (key == GLUT_KEY_RIGHT) {
        blockX += stepSize;

    }
    
    if (key == GLUT_KEY_UP) {
        blockY += stepSize;

    }
    
    if (key == GLUT_KEY_DOWN) {
        blockY -= stepSize;

    }
    
    //触碰到边界（4个边界）的处理
    
    //当正方形移动超过最左边的时候
    if (blockX < -1.0f) {
        blockX = -1.0f;
    }
    
    //当正方形移动到最右边时
    //1.0 - blockSize * 2 = 总边长 - 正方形的边长 = 最左边点的位置
    if (blockX > (1.0 - blockSize * 2)) {
        blockX = 1.0f - blockSize * 2;
    }
    
    //当正方形移动到最下面时
    //-1.0 - blockSize * 2 = Y（负轴边界） - 正方形边长 = 最下面点的位置
    if (blockY < -1.0f + blockSize * 2 ) {
        
        blockY = -1.0f + blockSize * 2;
    }
    
    //当正方形移动到最上面时
    if (blockY > 1.0f) {
        
        blockY = 1.0f;
        
    }
    
    
    
    printf("blockX = %f\n",blockX);
    printf("blockY = %f\n",blockY);
    
    // Recalculate vertex positions
    vVerts[0] = blockX;
    vVerts[1] = blockY - blockSize*2;
    
    printf("(%f,%f)\n",vVerts[0],vVerts[1]);
    
    
    vVerts[3] = blockX + blockSize*2;
    vVerts[4] = blockY - blockSize*2;
    printf("(%f,%f)\n",vVerts[3],vVerts[4]);
    
    vVerts[6] = blockX + blockSize*2;
    vVerts[7] = blockY;
    printf("(%f,%f)\n",vVerts[6],vVerts[7]);
    
    vVerts[9] = blockX;
    vVerts[10] = blockY;
    printf("(%f,%f)\n",vVerts[9],vVerts[10]);
    
    triangleBatch.CopyVertexData3f(vVerts);
    
    glutPostRedisplay();
}

void setupRC(){
    glClearColor(0.98f, 0.40f, 0.7f, 1);
    shaderManager.InitializeStockShaders();
    triangleBatch.Begin(GL_TRIANGLE_FAN, 4);
    triangleBatch.CopyVertexData3f(vVerts);
    triangleBatch.End();
}


void draw(){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    GLfloat vColor[] = {1.0f,0.0f,0.0f,1.0f};
    shaderManager.UseStockShader(GLT_SHADER_IDENTITY,vColor);
    triangleBatch.Draw();
    glutSwapBuffers();
}


int main(int argc,char * argv[])
{
    gltSetWorkingDirectory(argv[0]);
    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutCreateWindow("LGWindow");
    //注册显示函数
    glutDisplayFunc(draw);
    //注册重塑函数
    glutReshapeFunc(changeSize);
    
    glutSpecialFunc(specialKey);
    GLenum status = glewInit();
    if (GLEW_OK != status) {
        printf("GLEW Error:%s\n",glewGetErrorString(status));
        return 1;
    }
    
    setupRC();
    glutMainLoop();
    return 0;
}
