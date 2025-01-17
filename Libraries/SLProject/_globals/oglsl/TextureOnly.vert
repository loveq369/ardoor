//#############################################################################
//  File:      TextureOnly.vert
//  Purpose:   GLSL vertex program for texture mapping only
//  Author:    Marcus Hudritsch
//  Date:      February 2013
//             This software is provide under the GNU General Public License
//             Please visit: http://opensource.org/licenses/GPL-3.0
//#############################################################################

attribute   vec4     a_position;    // Vertex position attribute
attribute   vec3     a_texCoord;    // Vertex texture coord. attribute

uniform     mat4     u_mvpMatrix; // = projection * modelView

varying     vec2     v_texCoord;    // texture coordinate at vertex

void main()
{     
   // Set the texture coord. varying for interpolated tex. coords.
   v_texCoord = a_texCoord.xy;
   
   // Set the transformes vertex position   
   gl_Position = u_mvpMatrix * a_position;
}
