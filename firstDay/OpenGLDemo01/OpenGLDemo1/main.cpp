//#include <iostream>
#include "GLShaderManager.h"
#include "GLTools.h"
#include <GLUT/GLUT.h>
//定义一个，着色管理器
GLShaderManager shaderManager;

GLBatch triangleBatch;

void draw()
{
    //1.清除一个或者一组特定的缓存区
    /*
     缓冲区是一块存在图像信息的储存空间，红色、绿色、蓝色和alpha分量通常一起分量通常一起作为颜色缓存区或像素缓存区引用。
     OpenGL 中不止一种缓冲区（颜色缓存区、深度缓存区和模板缓存区）
     清除缓存区对数值进行预置
     参数：指定将要清除的缓存的
     GL_COLOR_BUFFER_BIT :指示当前激活的用来进行颜色写入缓冲区
     GL_DEPTH_BUFFER_BIT :指示深度缓存区
     GL_STENCIL_BUFFER_BIT:指示模板缓冲区
     */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

//    glColor3f(1.0f, 0.0f, 0.0f);
    //2.设置一组浮点数来表示红色
    GLfloat vRed[] = {1.0,0.0,0.0,1.0f};
    
    //传递到存储着色器，即GLT_SHADER_IDENTITY着色器，这个着色器只是使用指定颜色以默认笛卡尔坐标第在屏幕上渲染几何图形
    shaderManager.UseStockShader(GLT_SHADER_IDENTITY,vRed);
    //提交着色器
    triangleBatch.Draw();
    
    //在开始的设置openGL 窗口的时候，我们指定要一个双缓冲区的渲染环境。这就意味着将在后台缓冲区进行渲染，渲染结束后交换给前台。这种方式可以防止观察者看到可能伴随着动画帧与动画帧之间的闪烁的渲染过程。缓冲区交换平台将以平台特定的方式进行。
    //将后台缓冲区进行渲染，然后结束后交换给前台
    glutSwapBuffers();
}


void setupRC()
{
    glClearColor(0.98f, 0.40f, 0.7f, 1);

    //没有着色器，在OpenGL 核心框架中是无法进行任何渲染的。初始化一个渲染管理器。
    //在前面的课程，我们会采用固管线渲染，后面会学着用OpenGL着色语言来写着色器
    shaderManager.InitializeStockShaders();
    //指定顶点
    //在OpenGL中，三角形是一种基本的3D图元绘图原素。
    GLfloat vVerts[] = {
        -0.5f,0.0f,0.0f,
        0.5f,0.0f,0.0f,
        0.0f,0.5f,0.0f
    };
    
    triangleBatch.Begin(GL_TRIANGLES, 3);
    triangleBatch.CopyVertexData3f(vVerts);
    triangleBatch.End();
}


////通过顶点来绘制
//void draw()
//{
//    ////-----------------画正方形-------------------------
//
//    //1.设置清屏颜色 红、绿、蓝、透明度
//    /*
//     在windows 颜色成分取值范围：0-255之间
//     在iOS、OS 颜色成分取值范围：0-1之间浮点值
//     */
//    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//
//    /*
//     清除缓存区对数值进行预置
//     参数：指定将要清楚的缓存区的遮罩的按位或运算。
//     GL_COLOR_BUFFER_BIT: 指示当前激活的用来进行颜色写入缓存区
//     GL_DEPTH_BUFFER_BIT:指示深度缓存区
//     GL_STENCIL_BUFFER_BIT:指示模板缓存区
//
//     每个缓存区的清楚值根据这个缓存区的清楚值设置不同。
//
//     错误：
//     如果设定不是以上三个参考值，返回GL_INVALID_VALUE
//     */
//    //glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
//    glClear(GL_COLOR_BUFFER_BIT);
//
//    glColor3f(1.0f, 0.0f, 0.0f);
//
//    //设置绘图是的坐标系统
////    glOrtho(0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f);
//
//    //开始渲染
////    glBegin(GL_POLYGON);
//
//    //设置多边形的4个顶点
////    glVertex3f(0.25f, 0.25f, 0.0f);
////    glVertex3f(0.75f, 0.25f, 0.0f);
////    glVertex3f(0.75f, 0.75f, 0.0f);
////    glVertex3f(0.25f, 0.75f, 0.0f);
//
//
//    //// ------------------------五角形-----------------------------
////    glBegin(GL_LINE_LOOP);
////
////    const GLfloat Pi = 3.1415926536f;
////    //边长
////    int a = 0.75;
////    GLfloat ax = cos(72*Pi/180)*a;
////    GLfloat ay = sin(72*Pi/180)*a;
//
//
////    glVertex3f(0.0f, 0.25f, 0.0f);
////    glVertex3f(-0.25f, -0.25f, 0.0f);
////    glVertex3f(ax, ay, 0.0f);
////    glVertex3f(-0.25f, 0.0f, 0.0f);
////    glVertex3f(0.25f, -0.25f, 0.0f);
//
//
//
//    //----------- 画出正弦函数的图形---------------
//    /*
//     由于OpenGL默认坐标值只能从-1到1，（可以修改，但方法留到以后讲）
//     所以我们设置一个因子factor，把所有的坐标值等比例缩小，
//     这样就可以画出更多个正弦周期
//     试修改factor的值，观察变化情况
//     */
////    const GLfloat factor = 0.1f;
////    GLfloat x;
////    glClear(GL_COLOR_BUFFER_BIT);
////    glBegin(GL_LINES);
////    glVertex2f(-1.0f, 0.0f);
////    glVertex2f(1.0f, 0.0f);        // 以上两个点可以画x轴
////    glVertex2f(0.0f, -1.0f);
////    glVertex2f(0.0f, 1.0f);        // 以上两个点可以画y轴
////    glEnd();
////    glBegin(GL_LINE_STRIP);
////    for(x=-1.0f/factor; x<1.0f/factor; x+=0.01f)
////    {
////        glVertex2f(x*factor, sin(x)*factor);
////    }
//
//    //----------- 画出三角形---------------
//
//    glBegin(GL_LINE_LOOP);
//
//    glVertex3f(0.0f, 0.5f, 0.0f);
//    glVertex3f(0.5f, 0.0f, 0.0f);
//    glVertex3f(-0.5f, 0.0f, 0.0f);
//
//    //结束渲染
//    glEnd();
//
//    //强制刷新缓存区，保证绘制命令得以执行
//    glFlush();
//
//}


int main(int argc, char * argv[]) {
    
    gltSetWorkingDirectory(argv[0]);

    //1.初始化一个GLUT库
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH|GLUT_STENCIL);

    
    glutInitWindowSize(800, 600);

    //2.创建一个窗口并制定窗口名
    glutCreateWindow("LGWindows");
    
    //3.注册一个绘图函数，操作系统在必要时刻就会对窗体进行重绘制操作
    //它设置了一个显示回调（diplay callback），即GLUT在每次更新窗口内容的时候回自动调用该例程

    glutDisplayFunc(draw);
    
    
    GLenum status = glewInit();
    if (GLEW_OK != status) {
        
        printf("GLEW Error:%s\n",glewGetErrorString(status));
        return 1;
        
    }

    setupRC();
    
    //这是一个无限执行的循环，它会负责一直处理窗口和操作系统的用户输入等操作。（注意：不会执行在glutMainLoop()之后的所有命令。）
    glutMainLoop();

    return 0;
}
