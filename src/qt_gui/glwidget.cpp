#ifdef HAVE_CONFIG_H
  #include "config.h"
#endif

#include <QtWidgets/QWidget>
#include <QOpenGLWidget>
#include <QImage>
#ifndef DARWIN
#include <GL/glu.h>
#else
#include <glu.h>
#endif
#include <map>
#include <set>
#include <cmath>
#include "objreader.h"
#include "glwidget.h"
#include <iostream> 

#include "pathconfig.h"

// Helper function to create OpenGL texture from QImage (replaces QGLWidget::bindTexture)
static GLuint createTextureFromImage(const QImage &image)
{
  GLuint textureId;
  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);
  
  // QGLWidget::convertToGLFormat() (used in Qt4/Qt5) flipped the image vertically
  // to match OpenGL's bottom-left origin. QImage uses a top-left origin, so we need
  // to mirror the image on upload to keep the UV mapping correct.
  QImage glImage = image.convertToFormat(QImage::Format_RGBA8888).mirrored(false, true);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glImage.width(), glImage.height(), 0,
               GL_RGBA, GL_UNSIGNED_BYTE, glImage.bits());
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  return textureId;
}

ReaderThread::ReaderThread(): QThread()
{
}

void ReaderThread::run()
{
     read_obj();
     //std::cout<<"Object read!\n";
     emit done();
}


void GLWidget::objectsRead()
{
  makeCurrent();
  makeObjects();
  update();  // QOpenGLWidget uses update() instead of updateGL()
  emit ready();
}

GLWidget::GLWidget(QWidget *parent)
     : QOpenGLWidget(parent), rt(new ReaderThread())
 {
     xRot = 0;
     yRot = 0;
     zRot = 0;
     xTrans = 0;
     yTrans = 0;
     zTrans = 0;
     trolltechPurple = QColor::fromCmykF(0.0, 0.0, 0.0, 0.0);
     
     connect(rt, SIGNAL(done()), this, SLOT(objectsRead()));
     rt->start();
 }

 GLWidget::~GLWidget()
 {
     //rt->terminate();
     if(rt->isRunning()){
       rt->wait();
     }
     makeCurrent();
     std::vector<GLuint>::iterator i;
     for(i = objects.begin(); i != objects.end(); ++i){
       glDeleteLists(*i, 1);
     }
     // Clean up textures - use set to avoid deleting same texture multiple times
     // Multiple display lists from same object share the same texture
     std::set<GLuint> deletedTextures;
     std::vector<GLuint>::iterator texIt;
     for(texIt = objectTextures.begin(); texIt != objectTextures.end(); ++texIt){
       if(*texIt != 0 && deletedTextures.find(*texIt) == deletedTextures.end()){
         glDeleteTextures(1, &(*texIt));
         deletedTextures.insert(*texIt);
       }
     }
     objectTextures.clear();
     delete rt;
 }

 QSize GLWidget::minimumSizeHint() const
 {
     return QSize(50, 50);
 }

 QSize GLWidget::sizeHint() const
 {
     return QSize(400, 400);
 }

 void GLWidget::setXRotation(float angle)
 {
     if (angle != xRot) {
         xRot = angle;
     }
 }

 void GLWidget::setYRotation(float angle)
 {
     if (angle != yRot) {
         yRot = angle;
     }
 }

 void GLWidget::setZRotation(float angle)
 {
     if (angle != zRot) {
         zRot = angle;
     }
 }

 void GLWidget::setXTrans(float val)
 {
   xTrans = val;
 }

 void GLWidget::setYTrans(float val)
 {
   yTrans = val;
 }

 void GLWidget::setZTrans(float val)
 {
   zTrans = val;
 }

 void GLWidget::initializeGL()
 {
     QColor bgColor = trolltechPurple.darker();
     glClearColor(bgColor.redF(), bgColor.greenF(), bgColor.blueF(), bgColor.alphaF());
     makeObjects();
     glShadeModel(GL_FLAT);
     glEnable(GL_DEPTH_TEST);
     glEnable(GL_CULL_FACE);
     glEnable(GL_TEXTURE_2D);
 }

void GLWidget::paintGL()
{
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     glLoadIdentity();
     
     glRotated(-xRot, 1.0, 0.0, 0.0);
     glRotated(-yRot, 0.0, 1.0, 0.0);
     glRotated(-zRot, 0.0, 0.0, 1.0);
     glTranslated(-xTrans, -yTrans, -zTrans);
     
     glPushMatrix();
     glTranslated(0.0, -0.7, -2.265);
     
     // Qt6/QOpenGLWidget: Bind textures during rendering, not during display list compilation
     // This ensures texture state is correct when display lists are executed
     std::vector<GLuint>::const_iterator i;
     std::vector<GLuint>::const_iterator texIt = objectTextures.begin();
     for(i = objects.begin(); i != objects.end(); ++i, ++texIt){
       // Bind texture before calling display list
       if(texIt != objectTextures.end() && *texIt != 0){
         glBindTexture(GL_TEXTURE_2D, *texIt);
       } else {
         // No texture for this display list
         glBindTexture(GL_TEXTURE_2D, 0);
       }
       glCallList(*i);
     }
     glPopMatrix();
}

 void GLWidget::resizeGL(int width, int height)
 {
     glViewport(0, 0, width, height);

     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     gluPerspective(55.0, (double)width/height, 0.1, 45.0);
     glMatrixMode(GL_MODELVIEW);
 }


bool textured;
object_t obj;


static void make_vortex(int index)
{
  vtx_t vtx = obj.vtx_table[index];
  glNormal3f(vtx.nx, vtx.ny, vtx.nz);
  if(textured)
    glTexCoord2f(vtx.s, vtx.t);
  glVertex3f(vtx.x, vtx.y, vtx.z);
}

static void make_triangle(int index1, int index2, int index3)
{
  make_vortex(index3);
  make_vortex(index2);
  make_vortex(index1);
}

bool GLWidget::makeObjects()
 {
   int triangles = 0;
   
   // Clear existing objects and textures
   std::vector<GLuint>::iterator i;
   for(i = objects.begin(); i != objects.end(); ++i){
     glDeleteLists(*i, 1);
   }
   // Use set to avoid deleting same texture multiple times
   // Multiple display lists from same object share the same texture
   std::set<GLuint> deletedTextures;
   std::vector<GLuint>::iterator texIt;
   for(texIt = objectTextures.begin(); texIt != objectTextures.end(); ++texIt){
     if(*texIt != 0 && deletedTextures.find(*texIt) == deletedTextures.end()){
       glDeleteTextures(1, &(*texIt));
       deletedTextures.insert(*texIt);
     }
   }
   objects.clear();
   objectTextures.clear();
   
   std::vector<object_t>::const_iterator obj_index;
   std::vector<tri_t>::const_iterator tris_index;
   
   for(obj_index = object_table.begin(); obj_index != object_table.end(); ++obj_index){
     obj = *obj_index;
     
     // Determine if this object has a texture
     if(obj.texture.isEmpty()){
       textured = false;
     }else{
       textured = true;
     }
     
     // Load texture for this object if it has one
     GLuint objectTexture = 0;
     if(textured){
       //std::cout<<"loading texture "<<obj.texture<<"\n";
       objectTexture = createTextureFromImage(QImage(QString(obj.texture)));
     }
     
     // Create display lists for each triangle group in this object
     GLuint list = glGenLists(obj.tris_table.size());
     for(tris_index = obj.tris_table.begin(); 
         tris_index != obj.tris_table.end(); ++tris_index){
       glNewList(list, GL_COMPILE);
       
       // Qt6/QOpenGLWidget: Don't bind texture during display list compilation
       // Texture will be bound during rendering in paintGL()
       
       if(tris_index->glass){
         glEnable (GL_BLEND); 
         glDepthMask (GL_FALSE);
         glBlendFunc (GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
       }
       glBegin(GL_TRIANGLES);
       
       int offset = tris_index->offset;
       int count = tris_index->count;
       for(int i = 0; i < count; i+=3){
         make_triangle(obj.vtx_indices[offset + i], obj.vtx_indices[offset + i + 1], 
                   obj.vtx_indices[offset + i + 2]);
         triangles++;
       }
       glEnd();
       if(tris_index->glass){
         glDepthMask (GL_TRUE);
         glDisable(GL_BLEND);
       }
       glEndList();
       
       // Store display list and associated texture
       objects.push_back(list);
       objectTextures.push_back(objectTexture);
       
       ++list;
     }
   }
   //std::cout<<triangles<<" triangles.\n";
   return true;
 }


