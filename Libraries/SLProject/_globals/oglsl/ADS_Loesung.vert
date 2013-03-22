//#############################################################################
//  File:      ADSTex.vert
//  Purpose:   GLSL vertex program for ambient, diffuse & specular per vertex 
//             point lighting with texture mapping.
//  Author:    Marcus Hudritsch
//  Date:      September 2011 (HS11)
//             THIS SOFTWARE IS PROVIDED FOR EDUCATIONAL PURPOSE ONLY AND
//             WITHOUT ANY WARRANTIES WHETHER EXPRESSED OR IMPLIED.
//#############################################################################

//-----------------------------------------------------------------------------
attribute   vec4     a_position;       // Vertex position attribute
attribute   vec3     a_normal;         // Vertex normal attribute

uniform     mat4     u_mvMatrix;       // modelView matrix
uniform     mat4     u_mvpMatrix;      // = projection * modelView
uniform     mat3     u_nMatrix;        // normal matrix=transpose(inverse(mv))

uniform     vec4     u_globalAmbi;     // global ambient intensity (Iaglobal)
uniform     vec3     u_lightPosVS;     // light position in view space
uniform     vec3     u_lightDirVS;     // light direction in view space
uniform     vec4     u_lightAmbi;      // light ambient light intensity (Ia)
uniform     vec4     u_lightDiff;      // light diffuse light intensity (Id)
uniform     vec4     u_lightSpec;      // light specular light intensity (Is)
uniform     float    u_lightSpotCut;   // lights spot cutoff angle
uniform     float    u_lightSpotCos;   // cosine of the lights spot cutoff angle
uniform     float    u_lightSpotExp;   // lights spot exponent
uniform     vec3     u_lightAtt;       // light attenuation coeff.
uniform     vec4     u_matAmbi;        // material ambient reflection (ka)
uniform     vec4     u_matDiff;        // material diffuse reflection (kd)
uniform     vec4     u_matSpec;        // material specular reflection (ks)
uniform     vec4     u_matEmis;        // material emissiveness (ke)
uniform     float    u_matShine;       // material shininess exponent

varying     vec4     v_color;          // The resulting color per vertex

//-----------------------------------------------------------------------------
void main()
{     
   // transform vertex pos into view space
   vec3 P_VS = vec3(u_mvMatrix * a_position);

   // transform normal into view space
   vec3 N = normalize(u_nMatrix * a_normal);

   // eye position is the inverse of the vertex pos. in VS
   vec3 E = normalize(-P_VS);

   // vector from P_VS to the light in VS
   vec3 L = u_lightPosVS - P_VS;

   // list distance
   float dist = length(L);

   // light attenuation = 1 / (c1 + c2*dist + c3*dist^2)    
   float fatt = 1.0 / (u_lightAtt.x +
                       u_lightAtt.y * dist +
                       u_lightAtt.z * dist * dist);

   // normalize light vector
   L = normalize(L);
   
   // Normalized halfvector between N and L
   vec3 H = normalize(L+E);

   // diffuse factor
   float diffFactor = max(dot(N,L), 0.0);

   // specular factor
   float specFactor = pow(max(dot(N,H), 0.0), u_matShine);

   // Calculate spot attenuation
   float Sspoteffect = 1.0;
   if (u_lightSpotCut < 180.0)
   {  float spotDot; // Cosine of angle between L and spotdir
      spotDot = dot(-L, u_lightDirVS);
      if (spotDot < u_lightSpotCos) Sspoteffect = 0.0;
      else Sspoteffect = max(pow(spotDot, u_lightSpotExp), 0.0);
   }

   // Calculate the full Blinn/Phong light equation 
   v_color =  u_matEmis +
              u_globalAmbi * u_matAmbi +
              fatt * Sspoteffect * (u_lightAmbi * u_matAmbi +
                                    u_lightDiff * u_matDiff * diffFactor +
                                    u_lightSpec * u_matSpec * specFactor);

   // Set the transformes vertex position           
   gl_Position = u_mvpMatrix * a_position;
}
//-----------------------------------------------------------------------------