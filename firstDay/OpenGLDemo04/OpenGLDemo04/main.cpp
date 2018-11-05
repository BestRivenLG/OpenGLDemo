#include "GLTools.h"
#include "GLFrustum.h"
#include "StopWatch.h"

#include <GLUT/GLUT.h>

GLShaderManager shaderManager;

//视景体-投影矩阵通过它来设置
GLFrustum viewFrustum;

GLTriangleBatch triangleBatch;


int nStep = 0;

void display(){
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLfloat vBlack[] = {1.0f,1.0f,0.0f,1.0f};

//    shaderManager.UseStockShader(GLT_SHADER_IDENTITY,vBlack);
    
    
    //建立一个基于时间变化的动画
    static CStopWatch rotTimer;
    
    float yRot = rotTimer.GetElapsedSeconds() * 60.0f;
    
    M3DMatrix44f mModelViewProjection,mModelView,mTranslate,mRotate;
    
    m3dTranslationMatrix44(mTranslate, 0, 0, -2.5f);
    m3dRotationMatrix44(mRotate, m3dDegToRad(yRot), 0, 1, 0);
    m3dMatrixMultiply44(mModelView, mTranslate, mRotate);
    
    m3dMatrixMultiply44(mModelViewProjection, viewFrustum.GetProjectionMatrix(), mModelView);
   
    //平面着色器来渲染图像
    shaderManager.UseStockShader(GLT_SHADER_FLAT,mModelViewProjection,vBlack);

    
    triangleBatch.Draw();
    glutSwapBuffers();
    glutPostRedisplay();

}

void setupRC(){
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    shaderManager.InitializeStockShaders();
    
    gltMakeSphere(triangleBatch, 0.4f, 10, 20);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPointSize(5.0f);

}

void ChangeSize(int width, int height){
    if (height==0) {
        height=1;
    }
    glViewport(0, 0, width, height);
    //设置投影矩阵
    viewFrustum.SetPerspective(35.0f, float(width)/float(height), 1.0f, 1000.0f);
}

//根据空格次数。切换不同的“窗口名称”
//key:敲击的键位
//x,y: 光标的位置
void keyPressFunc(unsigned char key, int x, int y){
    if (key == 32) {
        nStep ++;
        if (nStep>1) {
            nStep = 0;
        }
    }
    glutPostRedisplay();
}


int main(int argc,char * argv[]){
    
    gltSetWorkingDirectory(argv[0]);
    glutInit(&argc, argv);
    
    /*
     颜色缓存区：
     就是帧缓冲区（图形设备的内存），需要渲染的场景的每一个像素都最终写入该缓冲区，然后由他渲染到屏幕上显示
     
     深度缓冲区：
     与帧缓冲区对应，用于记录上面每个像素的深度值，通过深度缓冲区，我们可以进行深度测试，从而确定像素的遮挡关系，保证渲染正确。（注意区分深度测试和背面剔除）
     
     双缓存区：
     双缓冲甚至是多缓冲，在许多情况下都很有用。一般需要使用双缓冲区的地方都是由于“生产者”和“消费者”供需不一致所造成的
     
     例1：计算机中的三级缓存结构：外存（硬盘）、内存、高速缓存（介于CPU和内存之间，可能由多级）。从左到右他们的存储容量不断减小，但速度不断提升，当然价格也是越来越贵。作为“生产者”的 CPU 处理速度很快，而内存存取速度相对CPU较慢，如果直接在内存中存取数据，他们的速度不一致会导致 CPU  能力下降。因此在他们之间又增加的高速缓存来作为缓冲区平衡二者速度上的差异。
     
     例2：在图形图像显示过程中，计算机从显示缓冲区取数据然后显示，很多图形的操作都很复杂需要大量的计算，很难访问一次显示缓冲区就能写入待显示的完整图形数据，通常需要多次访问显示缓冲区，每次访问时写入最新计算的图形数据。而这样造成的后果是一个需要复杂计算的图形，你看到的效果可能是一部分一部分地显示出来的，造成很大的闪烁不连贯。而使用双缓冲，可以使你先将计算的中间结果存放在另一个缓冲区中，但全部的计算结束，该缓冲区已经存储了完整的图形之后，再将该缓冲区的图形数据一次性复制到显示缓冲区。
     
     例2 中使用双缓冲是为了提高 CPU 的处理效率，而例3使用双缓冲是为了防止显示图形时的闪烁延迟等不良体验。
     
     
     模板缓存区：
     与深度缓冲区类似，通过设置模板缓冲每个像素的值，我们可以在渲染的时候只渲染后写像素，从而可以达到一些特殊的效果。
     模板缓冲区可以为屏幕上的每个像素点保存一个无符号的整数值，在渲染过程中，可以用这个值与一个预先设定的参考值相比较，根据比较的结果来决定是否更新相应的像素点的颜色值。
     模板测试发生在透明度测试之后，深度测试之前，如果模板测试通过，则相应的像素点更新，否则不更新。
     */
    //申请一个颜色缓存区、深度缓存区、双缓存区、模板缓存区
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    
    //设置window 的尺寸
    glutInitWindowSize(800, 600);
    
    //创建window的名称
    glutCreateWindow("LGWindows");
    
    //显示函数
    glutDisplayFunc(display);
    
    //点击空格时，调用的函数
    //注册一个键盘ASC2 码的键位事件的回调函数，当有键盘按下时，则执行回调函数
    glutKeyboardFunc(keyPressFunc);
    
    //注册回调函数（改变尺寸）
    glutReshapeFunc(ChangeSize);
    //判断一下是否能初始化glew库，确保项目能正常使用OpenGL 框架
    GLenum status = glewInit();
    if (GLEW_OK != status) {
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(status));
        return 1;
    }
    setupRC();
    glutMainLoop();
    return 0;
}
