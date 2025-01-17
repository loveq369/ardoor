//#############################################################################
//  File:      SLCylinder.h
//  Author:    Marcus Hudritsch
//  Date:      February 2013
//  Copyright (c): 2002-2013 Marcus Hudritsch
//             This software is provide under the GNU General Public License
//             Please visit: http://opensource.org/licenses/GPL-3.0
//#############################################################################

#ifndef SLCYLINDER_H
#define SLCYLINDER_H

#include "SLRevolver.h"

//-----------------------------------------------------------------------------
//! SLCylinder is creates sphere mesh based on its SLRevolver methods     
class SLCylinder: public SLRevolver 
{  public:                     
                              SLCylinder  (SLfloat  cylinderRadius,
                                           SLfloat  cylinderHeight,
                                           SLint    stacks = 1,
                                           SLint    slices = 16,
                                           SLbool   hasTop = true,
                                           SLbool   hasBottom = true,
                                           SLstring name = "Cylinder",
                                           SLMaterial* mat = 0);
                                           
                             ~SLCylinder    (){;}               
                             
               SLShape*       shapeCopy()
                              {  return new SLCylinder(_radius,
                                                       _height,
                                                       _stacks, 
                                                       _slices,
                                                       _hasTop,
                                                       _hasBottom,
                                                       _name);
                              }
                              
               // Getters
               SLfloat        radius()    {return _radius;}
               SLfloat        height()    {return _height;}
               SLint          stacks()    {return _stacks;}
               SLbool         hasTop()    {return _hasTop;}
               SLbool         hasBottom() {return _hasBottom;}
               
   private:    
               SLfloat       _radius;     //!< radius of cylinder
               SLfloat       _height;     //!< height of cylinder
               SLint         _stacks;     //!< No. of stacks of cylinder
               SLbool        _hasTop;     //!< Flag if cylinder has a top
               SLbool        _hasBottom;  //!< Flag if cylinder has a bottom
};
//-----------------------------------------------------------------------------
#endif //SLCYLINDER_H

