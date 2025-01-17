//#############################################################################
//  File:      SLGLBuffer.cpp
//  Purpose:   Template wrapper around OpenGL Vertex Buffer Objects 
//  Author:    Marcus Hudritsch
//  Date:      February 2013
//  Copyright (c): 2002-2013 Marcus Hudritsch
//             This software is provide under the GNU General Public License
//             Please visit: http://opensource.org/licenses/GPL-3.0
//#############################################################################

#include <stdafx.h>           // precompiled headers
#ifdef SL_MEMLEAKDETECT
#include <nvwa/debug_new.h>   // memory leak detector
#endif

#include <SLGLBuffer.h>
#include <SLScene.h>

//-----------------------------------------------------------------------------
SLuint SLGLBuffer::totalBufferSize  = 0;
SLuint SLGLBuffer::totalBufferCount = 0;
//-----------------------------------------------------------------------------
/*! Constructor initializing with default values
*/
SLGLBuffer::SLGLBuffer()
{  _id           = 0;
   _numElements  = 0;
   _elementSize  = 0;
   _dataTypeGL   = SL_FLOAT;
   _targetTypeGL = SL_ARRAY_BUFFER;
   _usageTypeGL  = SL_STATIC_DRAW;
   _attribIndex  = -1;
}
//-----------------------------------------------------------------------------
/*! Destructor calling dispose
*/
SLGLBuffer::~SLGLBuffer() 
{  dispose();
}
//-----------------------------------------------------------------------------
/*! Deletes the buffer object
*/
void SLGLBuffer::dispose()
{  
   if (_id) 
   {  
      glDeleteBuffers(1, &_id);
      
      totalBufferSize -= (_numElements*_elementSize*_typeSize);
      totalBufferCount--;
      //cout << "disposed: " << _id << ", used:" << totalBufferCount << endl;
      _id = 0;
   }
} 
//-----------------------------------------------------------------------------
/*! Vertex Buffer generation. The vertex buffe object on the graphiccard is
generate and identified by _id<0. The data is copied to the buffer with 
glBufferData and could be deleted afterwards in the clients memory. 
The buffer type can be FLOAT for vertex attributes (position, normals, tangents
etc.) or UNSIGNED_BYTE or SL_UNSIGNED_SHORT for vertex indexes.
*/
void SLGLBuffer::generate(void* dataPointer, 
                          SLint numElements, 
                          SLint elementSize, 
                          SLBufferType type,        
                          SLBufferTarget target,
                          SLBufferUsage usage)
{  
   // init members
   _elementSize  = elementSize;
   _numElements  = numElements;
   _dataTypeGL   = type;
   _targetTypeGL = target;
   _usageTypeGL  = usage;
   
   // determine size of the OpenGL type in bytes
   switch (_dataTypeGL)
   {  case GL_FLOAT:          _typeSize = sizeof(GLfloat);  break; 
      case GL_UNSIGNED_BYTE:  _typeSize = sizeof(GLubyte);  break;
      case GL_UNSIGNED_SHORT: _typeSize = sizeof(GLushort); break;
      default:                _typeSize = sizeof(SLfloat);  break;
   }
   
   // if buffer exist delete it first
   if (_id) dispose();

   // Generate a buffer id
   glGenBuffers(1, &_id);
   
   // binds (activates) the buffer that is used next
   glBindBuffer(_targetTypeGL, _id);
   
   // determine the buffersize in bytes
   SLint bufSize = _numElements*_elementSize*_typeSize;
   
   // pass the data to the buffer object on the GPU
   glBufferData(_targetTypeGL, bufSize, dataPointer, _usageTypeGL);
   
   // update stats
   totalBufferSize += bufSize;
   totalBufferCount++;
   //SL_LOG("VBO created: %d", _id);
   //cout << "created: " << _id << ", used:" << totalBufferCount << endl;
}
//-----------------------------------------------------------------------------      
/*! Updates a buffer object by copying new data or subdata to the buffer
*/
void SLGLBuffer::update(const void* dataPointer, 
                        SLint numElements, 
                        SLint offsetElements)
{  assert(_id);
   assert(dataPointer);
   assert(numElements);
   assert(numElements <= _numElements);
   assert(offsetElements < numElements);

   // copy subdata into existing buffer object
   glBindBuffer(_targetTypeGL, _id);
   glBufferSubData(_targetTypeGL,
                   offsetElements*_elementSize*_typeSize,
                   _numElements*_elementSize*_typeSize,
                   dataPointer);
}        
//-----------------------------------------------------------------------------      
/*! Binds the buffer and enables the GLSL attribute by index. If the attribute
index is -1, it means that the shader does not use the vertex attribute, so we
don't need to bind and enable it. 
*/
void SLGLBuffer::bindAndEnableAttrib(SLint attribIndex,
                                     SLint dataOffsetBytes,
                                     SLint stride)
{  
   assert(_id);
   
   // Only activate buffer if the shader uses the attribute
   if (attribIndex >= 0)
   {  
      // binds (activates) the buffer that is used next
      glBindBuffer(_targetTypeGL, _id);
      
      // enable the vertex attribute data array by index
      glEnableVertexAttribArray(attribIndex);
      
      // defines the vertex attribute data array by index
      glVertexAttribPointer(attribIndex, 
                            _elementSize,
                            _dataTypeGL, 
                            GL_FALSE, 
                            stride, 
                            (void*)dataOffsetBytes);
      
      _attribIndex = attribIndex;
   }
}
//-----------------------------------------------------------------------------      
/*! Binds the index buffer and draws the elements with a primitive type
*/
void SLGLBuffer::bindAndDrawElementsAs(SLPrimitive primitiveType,
                                       SLint numIndexes,
                                       SLint indexOffsetBytes)
{  assert(_id);
   assert(numIndexes <= _numElements*_elementSize);
   assert(_targetTypeGL==GL_ELEMENT_ARRAY_BUFFER);
   
   glBindBuffer(_targetTypeGL, _id);
   glDrawElements(primitiveType, 
                  numIndexes ? numIndexes : _numElements,
                  _dataTypeGL, 
                  (void*)indexOffsetBytes);
}
//-----------------------------------------------------------------------------      
/*! Draws the vertex array buffer with a primitive type
*/
void SLGLBuffer::drawArrayAs(SLPrimitive primitiveType,
                             SLint indexFirstVertex,
                             SLint numVertices)
{  assert(_id);
   assert(numVertices <= _numElements);
   assert(_targetTypeGL==GL_ARRAY_BUFFER);
   
   glDrawArrays(primitiveType, 
                indexFirstVertex, 
                numVertices?numVertices:_numElements);
}
        
//-----------------------------------------------------------------------------      
/*! Disable attribute arrays. Not doing this step after drawing can lead to
access violations in glDrawElements.
*/
void SLGLBuffer::disableAttribArray()
{  assert(_targetTypeGL==GL_ARRAY_BUFFER);
   
   if (_attribIndex >= 0)
      glDisableVertexAttribArray(_attribIndex);
}
//-----------------------------------------------------------------------------
/*! Draws a vertex array buffer as line primitive with constant color
*/
void SLGLBuffer::drawArrayAsConstantColorLines(SLCol3f color,
                                               SLfloat lineWidth,
                                               SLint   indexFirstVertex,
                                               SLint   numVertices)
{  assert(_id);
   assert(numVertices <= _numElements);
   assert(_targetTypeGL==GL_ARRAY_BUFFER);
   
   // Prepare shader
   SLMaterial::current = 0;
   SLGLShaderProg* sp = SLScene::current->shaderProgs(ColorUniform);
   SLGLState* state = SLGLState::getInstance();
   sp->useProgram();
   sp->uniformMatrix4fv("u_mvpMatrix", 1, (SLfloat*)state->mvpMatrix());
   
   // Set uniform color           
   SLint indexC = sp->getUniformLocation("u_color");
   glUniform4fv(indexC, 1, (SLfloat*)&color);
   
   #ifndef SL_GLES2
   if (lineWidth!=1.0f);
      glLineWidth(lineWidth);
   #endif
   
   // Bind buffer & draw array with line primitives 
   bindAndEnableAttrib(sp->getAttribLocation("a_position"));
   
   glDrawArrays(GL_LINES,
                indexFirstVertex, 
                numVertices ? numVertices : _numElements);
                
   disableAttribArray();
   
   #ifndef SL_GLES2
   if (lineWidth!=1.0f);
      glLineWidth(1.0f);
   #endif
}
//-----------------------------------------------------------------------------
/*! Draws a vertex array buffer as linestrip primitive with constant color
*/
void SLGLBuffer::drawArrayAsConstantColorLineStrip(SLCol3f color,
                                                   SLfloat lineWidth,
                                                   SLint   indexFirstVertex,
                                                   SLint   numVertices)
{  assert(_id);
   assert(numVertices <= _numElements);
   assert(_targetTypeGL==GL_ARRAY_BUFFER);
   
   // Prepare shader
   SLMaterial::current = 0;
   SLGLShaderProg* sp = SLScene::current->shaderProgs(ColorUniform);
   SLGLState* state = SLGLState::getInstance();
   sp->useProgram();
   sp->uniformMatrix4fv("u_mvpMatrix", 1, (SLfloat*)state->mvpMatrix());
   
   // Set uniform color           
   SLint indexC = sp->getUniformLocation("u_color");
   glUniform4fv(indexC, 1, (SLfloat*)&color);
   
   #ifndef SL_GLES2
   if (lineWidth!=1.0f);
      glLineWidth(lineWidth);
   #endif
   
   // Bind buffer & draw array with line primitives 
   bindAndEnableAttrib(sp->getAttribLocation("a_position"));
   
   glDrawArrays(GL_LINE_STRIP,
                indexFirstVertex, 
                numVertices ? numVertices : _numElements);
                
   disableAttribArray();
   
   #ifndef SL_GLES2
   if (lineWidth!=1.0f);
      glLineWidth(1.0f);
   #endif
}
//-----------------------------------------------------------------------------
/*! Draws a vertex array buffer as line primitive with constant color attribute
*/
void SLGLBuffer::drawArrayAsConstantColorPoints(SLCol4f color,
                                                SLfloat pointSize,
                                                SLint   indexFirstVertex,
                                                SLint   numVertices)
{  assert(_id);
   assert(numVertices <= _numElements);
   assert(_targetTypeGL==GL_ARRAY_BUFFER);
   
   // Prepare shader
   SLMaterial::current = 0;
   SLGLShaderProg* sp = SLScene::current->shaderProgs(ColorUniform);
   SLGLState* state = SLGLState::getInstance();
   sp->useProgram();
   sp->uniformMatrix4fv("u_mvpMatrix", 1,
                        (SLfloat*)state->mvpMatrix());
   
   // Set uniform color           
   SLint indexC = sp->getUniformLocation("u_color");
   glUniform4fv(indexC, 1, (SLfloat*)&color);
   
   #ifndef SL_GLES2
   if (pointSize!=1.0f);
      glPointSize(pointSize);
   #endif

   // Bind buffer & draw array with line primitives 
   bindAndEnableAttrib(sp->getAttribLocation("a_position"));
   
   glDrawArrays(GL_POINTS,
                indexFirstVertex, 
                numVertices ? numVertices : _numElements);
                
   disableAttribArray();
   
   #ifndef SL_GLES2
   if (pointSize!=1.0f);
      glPointSize(1.0f);
   #endif
}
//-----------------------------------------------------------------------------