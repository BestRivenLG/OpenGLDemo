#include "GLTools.h"
#include "GLFrame.h"
#include "GLFrustum.h"
#include "GLMatrixStack.h"
#include "GLGeometryTransform.h"

#include <GLUT/GLUT.h>

GLShaderManager shaderManager;

GLFrame objectFrame;

//投影矩阵
GLFrustum viewFrustum;

GLMatrixStack modelViewMatrix;
GLMatrixStack projectionMatrix;
GLFrame cameraFrame;

//几何变换的管道
GLGeometryTransform transformPipeline;

GLBatch pointBatch;
GLBatch lineBatch;
GLBatch lineStripBatch;
GLBatch lineLoopBatch;
GLBatch triangleBatch;
GLBatch triangleStripBatch;
GLBatch triangleFanBatch;

GLfloat vGreen[] = { 0.0f, 1.0f, 0.0f, 1.0f };
GLfloat vBlack[] = { 0.0f, 0.0f, 0.0f, 1.0f };

// 跟踪效果步骤
int nStep = 0;


void ChangeSize(int w, int h){
    glViewport(0, 0, w, h);
    //创建投影矩阵，并将它载入投影矩阵堆栈中
    viewFrustum.SetPerspective(35.0f, float(w) / float(h), 1.0f, 500.0f);
    projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
    
    //调用顶部载入单元矩阵
    modelViewMatrix.LoadIdentity();
}

void DrawWireFramedBatch(GLBatch* pBatch){
    /*------------画绿色部分----------------*/
    /* GLShaderManager 中的Uniform 值——平面着色器
     参数1：平面着色器
     参数2：运行为几何图形变换指定一个 4 * 4变换矩阵
     --transformPipeline 变换管线（指定了2个矩阵堆栈）
     参数3：颜色值
     */
    shaderManager.UseStockShader(GLT_SHADER_FLAT,transformPipeline.GetModelViewProjectionMatrix(),vGreen);
    pBatch->Draw();
    
    /*-----------边框部分-------------------*/
    /*
     glEnable(GLenum mode); 用于启用各种功能。功能由参数决定
     参数列表：http://blog.csdn.net/augusdi/article/details/23747081
     注意：glEnable() 不能写在glBegin() 和 glEnd()中间
     GL_POLYGON_OFFSET_LINE  根据函数glPolygonOffset的设置，启用线的深度偏移
     GL_LINE_SMOOTH          执行后，过虑线点的锯齿
     GL_BLEND                启用颜色混合。例如实现半透明效果
     GL_DEPTH_TEST           启用深度测试 根据坐标的远近自动隐藏被遮住的图形（材料
     
     
     glDisable(GLenum mode); 用于关闭指定的功能 功能由参数决定
     
     */
    //画黑色边框
    glPolygonOffset(-1.0f, -1.0f);// 偏移深度，在同一位置要绘制填充和边线，会产生z冲突，所以要偏移
    glEnable(GL_POLYGON_OFFSET_LINE);
    
    // 画反锯齿，让黑边好看些
    glEnable(GL_LINE_SMOOTH);
    
    //开启混合
    glEnable(GL_BLEND);
    
    //指定混合因子
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    //绘制线框几何黑色版 三种模式，实心，边框，点，可以作用在正面，背面，或者两面
    //通过调用glPolygonMode将多边形正面或者背面设为线框模式，实现线框渲染
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    //设置线条宽度
    glLineWidth(2.5f);

    shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeline.GetModelViewProjectionMatrix(), vBlack);
    pBatch->Draw();

    // 复原原本的设置


    //关闭GL_POLYGON_OFFSET_LINE 模型
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_POLYGON_OFFSET_LINE);
    
    //设置线条宽度
    glLineWidth(1.0f);

    //关闭混合
    glDisable(GL_BLEND);
    
    //关闭线条光滑功能
    glDisable(GL_LINE_SMOOTH);
}

//渲染
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

//    glColor3d(0.5, 1,0, 0.32);
    
    /*
    shaderManager.UseStockShader(GLT_SHADER_IDENTITY,vBlack);

    switch (nStep) {
        case 0:
            glPointSize(4.0f);
            pointBatch.Draw();
            glPointSize(1.0f);
            break;
        case 1:
            lineBatch.Draw();
            break;
        case 2:
            lineStripBatch.Draw();
            break;
        case 3:
            lineLoopBatch.Draw();
            break;
        default:
            break;
    }
    */
    
    //压栈
    modelViewMatrix.PushMatrix();
    M3DMatrix44f cameraF;
    cameraFrame.GetCameraMatrix(cameraF);
    //矩阵乘以矩阵堆栈的顶部矩阵，相乘的结果随后简存储在堆栈的顶部
    modelViewMatrix.MultMatrix(cameraF);
    
    M3DMatrix44f mObjectFrame;
    //只要使用 GetMatrix 函数就可以获取矩阵堆栈顶部的值，这个函数可以进行2次重载。用来使用GLShaderManager 的使用。或者是获取顶部矩阵的顶点副本数据
    objectFrame.GetMatrix(mObjectFrame);
    modelViewMatrix.MultMatrix(mObjectFrame);
    shaderManager.UseStockShader(GLT_SHADER_FLAT,transformPipeline.GetModelViewProjectionMatrix(),vGreen);
    
    switch (nStep) {
        case 0:
            glPointSize(4.0f);
            pointBatch.Draw();
            glPointSize(1.0f);
            break;
        case 1:
            lineBatch.Draw();
            break;
        case 2:
            lineStripBatch.Draw();
            break;
        case 3:
            lineLoopBatch.Draw();
            break;
        case 4:
            DrawWireFramedBatch(&triangleBatch);
            break;
        case 5:
            DrawWireFramedBatch(&triangleFanBatch);
            break;
        case 6:
            DrawWireFramedBatch(&triangleStripBatch);
            break;
        default:
            break;
    }
    modelViewMatrix.PopMatrix();
    glutSwapBuffers();
}



//初始化函数
void setupRC(){
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    
    
    shaderManager.InitializeStockShaders();
    glEnable(GL_DEPTH_TEST);
    //设置变换管线以使用两个矩阵堆栈
    transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
    cameraFrame.MoveForward(-15.0f);
    
    GLfloat vCoast[24][3] = {
        {2.80, 1.20, 0.0 }, {2.0,  1.20, 0.0 },
        {2.0,  1.08, 0.0 },  {2.0,  1.08, 0.0 },
        {0.0,  0.80, 0.0 },  {-.32, 0.40, 0.0 },
        {-.48, 0.2, 0.0 },   {-.40, 0.0, 0.0 },
        {-.60, -.40, 0.0 },  {-.80, -.80, 0.0 },
        {-.80, -1.4, 0.0 },  {-.40, -1.60, 0.0 },
        {0.0, -1.20, 0.0 },  { .2, -.80, 0.0 },
        {.48, -.40, 0.0 },   {.52, -.20, 0.0 },
        {.48,  .20, 0.0 },   {.80,  .40, 0.0 },
        {1.20, .80, 0.0 },   {1.60, .60, 0.0 },
        {2.0, .60, 0.0 },    {2.2, .80, 0.0 },
        {2.40, 1.0, 0.0 },   {2.80, 1.0, 0.0 }};
    
    //用点的形式--表示佛罗里达州的形状
    pointBatch.Begin(GL_POINTS, 24);
    pointBatch.CopyVertexData3f(vCoast);
    pointBatch.End();
    
    //通过线的形式--表示佛罗里达州的形状
    lineBatch.Begin(GL_LINES, 24);
    lineBatch.CopyVertexData3f(vCoast);
    lineBatch.End();
    
    //通过线段的形式--表示佛罗里达州的形状
    lineStripBatch.Begin(GL_LINE_STRIP, 24);
    lineStripBatch.CopyVertexData3f(vCoast);
    lineStripBatch.End();
    
    //通过线环的形式--表示佛罗里达州的形状
    lineLoopBatch.Begin(GL_LINE_LOOP, 24);
    lineLoopBatch.CopyVertexData3f(vCoast);
    lineLoopBatch.End();
    
    //通过三角形创建金字塔
    GLfloat vPyramid[12][3] = {
        -2.0f, 0.0f, -2.0f,
        2.0f, 0.0f, -2.0f,
        0.0f, 4.0f, 0.0f,
        
        2.0f, 0.0f, -2.0f,
        2.0f, 0.0f, 2.0f,
        0.0f, 4.0f, 0.0f,
        
        2.0f, 0.0f, 2.0f,
        -2.0f, 0.0f, 2.0f,
        0.0f, 4.0f, 0.0f,
        
        -2.0f, 0.0f, 2.0f,
        -2.0f, 0.0f, -2.0f,
        0.0f, 4.0f, 0.0f
    };
    triangleBatch.Begin(GL_TRIANGLES, 12);
    triangleBatch.CopyVertexData3f(vPyramid);
    triangleBatch.End();

    // 三角形扇形--六边形
    GLfloat vPoints[100][3];    //超过我们需要的数组
    int nVerts = 0;
    //半径
    GLfloat r = 3.0f;
    
    //原点(x,y,z) = (0,0,0);
    vPoints[nVerts][0] = 0.0f;
    vPoints[nVerts][1] = 0.0f;
    vPoints[nVerts][2] = 0.0f;
    
    //M3D_2PI 就是2Pi 的意思，就一个圆的意思。 绘制圆形
    
    for(GLfloat angle = 0; angle < M3D_2PI; angle += M3D_2PI / 6.0f) {
        
        //数组下标自增（每自增1次就表示一个顶点）
        nVerts++;
        /*
         弧长=半径*角度,这里的角度是弧度制,不是平时的角度制
         既然知道了cos值,那么角度=arccos,求一个反三角函数就行了
         */
        //x点坐标 cos(angle) * 半径
        vPoints[nVerts][0] = float(cos(angle)) * r;
        //y点坐标 sin(angle) * 半径
        vPoints[nVerts][1] = float(sin(angle)) * r;
        //z点的坐标
        vPoints[nVerts][2] = -0.5f;
    }
    
    // 结束扇形 前面一共绘制7个顶点（包括圆心）
    printf("三角形扇形顶点数:%d\n",nVerts);
    //添加闭合的终点
    //课程添加演示：屏蔽177-180行代码，并把绘制节点改为7.则三角形扇形是无法闭合的。
    nVerts++;
    vPoints[nVerts][0] = r;
    vPoints[nVerts][1] = 0;
    vPoints[nVerts][2] = 0.0f;
    
    // 加载！
    //GL_TRIANGLE_FAN 以一个圆心为中心呈扇形排列，共用相邻顶点的一组三角形
    triangleFanBatch.Begin(GL_TRIANGLE_FAN, 8);
    triangleFanBatch.CopyVertexData3f(vPoints);
    triangleFanBatch.End();
    
    //三角形条带，一个小环或圆柱段
    //顶点下标
    int iCounter = 0;
    //半径
    GLfloat radius = 3.0f;
    //从0度~360度，以0.3弧度为步长
    for(GLfloat angle = 0.0f; angle <= (2.0f*M3D_PI); angle += 0.3f)
    {
        //或许圆形的顶点的X,Y
        GLfloat x = radius * sin(angle);
        GLfloat y = radius * cos(angle);
        
        //绘制2个三角形（他们的x,y顶点一样，只是z点不一样）
        vPoints[iCounter][0] = x;
        vPoints[iCounter][1] = y;
        vPoints[iCounter][2] = -0.5;
        iCounter++;
        
        vPoints[iCounter][0] = x;
        vPoints[iCounter][1] = y;
        vPoints[iCounter][2] = 0.5;
        iCounter++;
    }
    
    // 关闭循环
    printf("三角形带的顶点数：%d\n",iCounter);
    //结束循环，在循环位置生成2个三角形
    vPoints[iCounter][0] = vPoints[0][0];
    vPoints[iCounter][1] = vPoints[0][1];
    vPoints[iCounter][2] = -0.5;
    iCounter++;
    
    vPoints[iCounter][0] = vPoints[1][0];
    vPoints[iCounter][1] = vPoints[1][1];
    vPoints[iCounter][2] = 0.5;
    iCounter++;
    
    // GL_TRIANGLE_STRIP 共用一个条带（strip）上的顶点的一组三角形
    triangleStripBatch.Begin(GL_TRIANGLE_STRIP, iCounter);
    triangleStripBatch.CopyVertexData3f(vPoints);
    triangleStripBatch.End();
    

}

//上下左右键
void keyBoardFunc(int key, int x, int y){

    if (key == GLUT_KEY_UP) {
        objectFrame.RotateWorld(m3dDegToRad(-5.0f), 1.0f, 0.0f, 0.0f);
    }
    if (key == GLUT_KEY_DOWN) {
        objectFrame.RotateWorld(m3dDegToRad(5.0f), 1.0f, 0.0f, 0.0f);

    }
    if (key == GLUT_KEY_LEFT) {
        objectFrame.RotateWorld(m3dDegToRad(-5.0f), 0.0f, 1.0f, 0.0f);
    }
    if (key == GLUT_KEY_RIGHT) {
        objectFrame.RotateWorld(m3dDegToRad(5.0f), 0.0f, 1.0f, 0.0f);
    }
    glutPostRedisplay();
}

//根据空格次数。切换不同的“窗口名称”
//key:敲击的键位
//x,y: 光标的位置
void keyPressFunc(unsigned char key, int x, int y){
    if (key == 32) {
        nStep ++;
        if (nStep>6) {
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
    
    //特殊键位函数（上下左右）
    glutSpecialFunc(keyBoardFunc);
    
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
