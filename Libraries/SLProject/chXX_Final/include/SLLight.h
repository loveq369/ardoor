//#############################################################################
//  File:      SLLight.h
//  Author:    Marcus Hudritsch
//  Date:      February 2013
//  Copyright (c): 2002-2013 Marcus Hudritsch
//             This software is provide under the GNU General Public License
//             Please visit: http://opensource.org/licenses/GPL-3.0
//#############################################################################

#ifndef SLLIGHTGL_H
#define SLLIGHTGL_H

#include <stdafx.h>

class SLSceneView;
class SLRay;

//-----------------------------------------------------------------------------
//! Abstract Light class
/*!      
The abstract SLLight class encapsulates an invisible light source according to 
the OpenGL specification.
*/
class SLLight
{  public:
                        SLLight   (SLfloat ambiPower = 0.1f,
                                   SLfloat diffPower = 1.0f,
                                   SLfloat specPower = 1.0f,
                                   SLint id=-1);
                       ~SLLight   (){}
                       
    virtual void        setState    (SLGLState* state) = 0;
            
            // Setters
            void        id          (const SLint id)     {_id = id;}
            void        on          (const SLbool on)    {_on = on;}
            void        ambient     (SLCol4f ambi)       {_ambient = ambi;}
            void        diffuse     (SLCol4f diff)       {_diffuse = diff;}
            void        specular    (SLCol4f spec)       {_specular = spec;}
            void        spotCutoff  (const SLfloat cut)  {_spotCutoff = cut;
                                                          _spotCosCut = cos(SL_DEG2RAD*_spotCutoff);}
            void        spotExponent(const SLfloat exp)  {_spotExponent = exp;}
            void        kc          (const SLfloat kc);
            void        kl          (const SLfloat kl);
            void        kq          (const SLfloat kq);
            void        attenuation (const SLfloat kC,
                                     const SLfloat kL,
                                     const SLfloat kQ)   {kc(kC); kl(kL); kq(kQ);}
            // Getters
            SLint       id          () {return _id;}
            SLbool      on          () {return _on;}
            SLCol4f     ambient     () {return _ambient;}
            SLCol4f     diffuse     () {return _diffuse;}
            SLCol4f     specular    () {return _specular;}
            SLfloat     spotCutoff  () {return _spotCutoff;}
            SLfloat     spotCosCut  () {return _spotCosCut;}
            SLfloat     spotExponent() {return _spotExponent;}
            SLfloat     kc          () {return _kc;}
            SLfloat     kl          () {return _kl;}
            SLfloat     kq          () {return _kq;}
            SLbool      isAttenuated() {return _isAttenuated;}
            SLfloat     attenuation (SLfloat dist) 
                        {  return 1.0f/(_kc+_kl*dist+_kq*dist*dist);
                        }
            SLlong      photons     () {return _photons;}
            
            // some virtuals needed for ray tracing
   virtual  SLVec3f     positionWS  () = 0;
   virtual  SLVec3f     spotDirWS   () = 0;
   virtual  SLfloat     shadowTest  (SLRay* ray,       
                                     const SLVec3f& L, 
                                     const SLfloat lightDist) = 0;  
   virtual  void        photonEmission() = 0; // PM                                          
            
   protected:
            SLint       _id;           //!< OpenGL light number (0-7)
            SLbool      _on;           //!< Flag if light is on or off
            SLCol4f     _ambient;      //!< Ambient light intensity Ia
            SLCol4f     _diffuse;      //!< Diffuse light intensity Id
            SLCol4f     _specular;     //!< Specular light intensity Is
            SLfloat     _spotCutoff;   //!< Half the spot cone angle
            SLfloat     _spotCosCut;   //!< cosine of spotCutoff angle
            SLfloat     _spotExponent; //!< Spot attenuation from center to edge of cone
            SLfloat     _kc;           //!< Constant light attenuation
            SLfloat     _kl;           //!< Linear light attenuation
            SLfloat     _kq;           //!< Quadratic light attenuation
            SLbool      _isAttenuated; //!< fast attenuation flag for ray tracing
            
            // Photon Mapping
            SLint       _samples;      //!< number of samples for area lights            
            SLlong      _photons;      //!< number of photons to be emitted
};
//-----------------------------------------------------------------------------
//! STL vector of light pointers
typedef std::vector<SLLight*>  SLVLight;
//-----------------------------------------------------------------------------
#endif
