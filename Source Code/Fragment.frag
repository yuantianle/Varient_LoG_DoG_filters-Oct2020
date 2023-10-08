#version 430

//struct Material
//{ 
//	vec3 ambient;               //FOR MODIFYING
//	vec3 diffuse;
//	vec3 specular;
//	sampler2D diffuseTex;       //COLOR DATA 
//	sampler2D specularTex;
//};

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;

out vec4 fs_color;

//Uniforms------------------------------------------
//uniform Material material;

uniform sampler2D texture0;            //Color                                 //SAMPLER: like a hand, each time picks a pixel at a time // THIS ONE IS DEFAULT "0"
//uniform sampler2D texture1;            //Depth

uniform vec3 LightPos0;
uniform vec3 CameraPos;


mat3 GAUSS_operator(float sigma);
vec4 dip_filter(mat3 _filter, sampler2D _image, vec2 _xy, vec2 texSize);

//Functions-----------------------------------------


//vec3 calculateAmbient(Material material)
//{
//	return material.ambient;
//}
//
//vec3 calculateDiffuse(Material material, vec3 vs_position, vec3 vs_normal, vec3 LightPos0)
//{
//	vec3  PositionLightDirVec = normalize(LightPos0 - vs_position);
//	float Diffuse = clamp(dot(PositionLightDirVec, vs_normal), 0, 1);   //Dot producttion; clamp is the absolute value
//	vec3  DiffuseFinal = material.diffuse * Diffuse;                        //White light * 
//	
//	return DiffuseFinal;
//}
//
//vec3 calculationSpecular(Material material, vec3 vs_position, vec3 vs_normal, vec3 LightPos0, vec3 CameraPos)
//{
//	vec3  LightToPosDirVec = normalize(vs_position - LightPos0);
//	vec3  ReflectDirVec = normalize(reflect(LightToPosDirVec, normalize(vs_normal)));
//	vec3  PosToViewDirVec = normalize(CameraPos - vs_position);
//	float SpecularConstant = pow(max(dot(PosToViewDirVec, ReflectDirVec), 0), 50); //50 is the strength of the light
//	vec3  SpecularFinal = material.specular * SpecularConstant;
//
//	return SpecularFinal;
//}

//------------------Test4 function Gauss Filter-------------------//
vec4 dip_filter_gauss(mat3 _filter, sampler2D _image, vec2 _xy, vec2 texSize)               
{                        
	float sigma = 1;                                                         //step1 gauss blur & grey
	mat3 _smooth_fil = GAUSS_operator(sigma)*2;                              //

	float r = 2.5;
	mat3 _filter_pos_delta_x=mat3(vec3(-r, 0.0, r), vec3(-r, 0.0 ,r) ,vec3(-r,0.0,r));          
    mat3 _filter_pos_delta_y=mat3(vec3(-r,-r,-r),vec3(0.0,0.0,0.0),vec3(r,r,r));   
	vec4 final_color = vec4(0.0, 0.0, 0.0, 0.0);                                      
	for(int i = 0; i<3; i++)                                                          
	{                                                                                 
		for(int j = 0; j<3; j++)                                                      
		{                                                                             
			vec2 _xy_new = vec2(_xy.x + _filter_pos_delta_x[i][j], _xy.y + _filter_pos_delta_y[i][j]); 
			vec2 _uv_new = vec2(_xy_new.x/texSize.x, _xy_new.y/texSize.y);   

			//final_color += texture2D(_image,_uv_new)* _filter[i][j];
			vec4 gauss_color = dip_filter(_smooth_fil, _image, _xy_new, texSize); //
			final_color += gauss_color * _filter[i][j];                                  //
		}																		
	}																			
	return final_color;															
}	

//--------------Test sobel & canny filter----------------//
vec4 sobel_filter(mat3 _filter1, mat3 _filter2, sampler2D _image, vec2 _xy, vec2 texSize)               
{                         
	float r = 1;
	mat3 _filter_pos_delta_x=mat3(vec3(-r, 0.0, r), vec3(-r, 0.0 ,r) ,vec3(-r,0.0,r));          
    mat3 _filter_pos_delta_y=mat3(vec3(-r,-r,-r),vec3(0.0,0.0,0.0),vec3(r,r,r));             
	vec4 final_color = vec4(0.0, 0.0, 0.0, 0.0);  
	vec4 final1_color = vec4(0.0, 0.0, 0.0, 0.0); 
	vec4 final2_color = vec4(0.0, 0.0, 0.0, 0.0); 
	for(int i = 0; i<3; i++)                                                          
	{                                                                                 
		for(int j = 0; j<3; j++)                                                      
		{                                                                             
			vec2 _xy_new = vec2(_xy.x + _filter_pos_delta_x[i][j], _xy.y + _filter_pos_delta_y[i][j]); 
			vec2 _uv_new = vec2(_xy_new.x/texSize.x, _xy_new.y/texSize.y); 
			vec4 color = texture2D(_image,_uv_new);
			//if (color.r == color.g && color.g == color.b && color.b == 1) discard;
			final1_color += color * _filter1[i][j]; 
			final2_color += color * _filter2[i][j];
			
		}																		
	}
	final_color += sqrt(final1_color * final1_color + final2_color * final2_color);																			
	return final_color;															
}	

float sobel_gaussblur_filter(mat3 _filter1, mat3 _filter2, sampler2D _image, vec2 _xy, vec2 texSize)               
{   
	float sigma = 1;                                                         //step1 gauss blur & grey
	mat3 _smooth_fil = GAUSS_operator(sigma)*2;

	float r = 2;
	mat3 _filter_pos_delta_x=mat3(vec3(-r, 0.0, r), vec3(-r, 0.0 ,r) ,vec3(-r,0.0,r));          
    mat3 _filter_pos_delta_y=mat3(vec3(-r,-r,-r),vec3(0.0,0.0,0.0),vec3(r,r,r));   

	vec4 gauss_color = vec4(0.0, 0.0, 0.0, 0.0); 
	float final_color;  
	float final1_color; 
	float final2_color; 
	for(int i = 0; i<3; i++)                                                          
	{                                                                                 
		for(int j = 0; j<3; j++)                                                      
		{                                                                             
			vec2 _xy_new = vec2(_xy.x + _filter_pos_delta_x[i][j], _xy.y + _filter_pos_delta_y[i][j]); 
			vec2 _uv_new = vec2(_xy_new.x/texSize.x, _xy_new.y/texSize.y); 

			vec4 gauss_color = dip_filter_gauss(_smooth_fil, _image, _xy_new, texSize); 
			vec4 W = vec4(0.3,0.45,0.1,0);
			float luminace = dot(gauss_color,W);

			//if (color.r == color.g && color.g == color.b && color.b == 1) discard;
			final1_color += luminace * _filter1[i][j]; 
			final2_color += luminace * _filter2[i][j];
			
		}																		
	}
	
	final_color += sqrt(final1_color * final1_color + final2_color * final2_color);		//gradientMagnitude;
	//vec2 normalizedDirection = normalize(final1_color,final2_color);
	return final_color;															
}	

//float canny_filter(mat3 _filter1, mat3 _filter2, sampler2D _image, vec2 _xy, vec2 texSize){}               

float log_filter(mat3 _filter, sampler2D _image, vec2 _xy, vec2 texSize)               
{                                    
	float sigma = 1;                                                         //step1 gauss blur & grey
	mat3 _smooth_fil = GAUSS_operator(sigma)*2;

	float r = 2;
	mat3 _filter_pos_delta_x=mat3(vec3(-r, 0.0, r), vec3(-r, 0.0 ,r) ,vec3(-r,0.0,r));          
    mat3 _filter_pos_delta_y=mat3(vec3(-r,-r,-r),vec3(0.0,0.0,0.0),vec3(r,r,r));   
 
    float color[3][3];
	float final_color;                                      
	for(int i = 0; i<3; i++)                                                          
	{                                                                                 
		for(int j = 0; j<3; j++)                                                      
		{                                                                             
			vec2 _xy_new = vec2(_xy.x + _filter_pos_delta_x[i][j], _xy.y + _filter_pos_delta_y[i][j]); 
			vec2 _uv_new = vec2(_xy_new.x/texSize.x, _xy_new.y/texSize.y); 

			vec4 gauss_color = dip_filter_gauss(_smooth_fil, _image, _xy_new, texSize); 
			vec4 W = vec4(0.3,0.45,0.1,0);
			float luminace = dot(gauss_color,W);

			color[i][j] += luminace * _filter[i][j];            
		}																		
	}		

	if (final_color > 1.5) final_color = 1; else final_color = 0;

	return final_color;															
}	

//---------------------------------------------------

vec4 dip_filter(mat3 _filter, sampler2D _image, vec2 _xy, vec2 texSize)               
{                                                									  
	float r = 2;
	mat3 _filter_pos_delta_x=mat3(vec3(-r, 0.0, r), vec3(-r, 0.0 ,r) ,vec3(-r,0.0,r));          
    mat3 _filter_pos_delta_y=mat3(vec3(-r,-r,-r),vec3(0.0,0.0,0.0),vec3(r,r,r));         
	vec4 final_color = vec4(0.0, 0.0, 0.0, 0.0);                                      
	for(int i = 0; i<3; i++)                                                          
	{                                                                                 
		for(int j = 0; j<3; j++)                                                      
		{                                                                             
			vec2 _xy_new = vec2(_xy.x + _filter_pos_delta_x[i][j], _xy.y + _filter_pos_delta_y[i][j]); 
			vec2 _uv_new = vec2(_xy_new.x/texSize.x, _xy_new.y/texSize.y); 
			vec4 color = texture2D(_image,_uv_new);
			if (color.r == color.g && color.g == color.b && color.b == 1) discard;
			final_color += color * _filter[i][j];            
		}																		
	}																			
	return final_color;															
}	

vec4 dip_filter2(mat3 _filter, mat3 _filter2, sampler2D _image, vec2 _xy, vec2 texSize)               
{                                                									  
	mat3 _filter_pos_delta_x=mat3(vec3(-1.0, 0.0, 1.0), vec3(-1.0, 0.0 ,1.0) ,vec3(-1.0,0.0,1.0));          
    mat3 _filter_pos_delta_y=mat3(vec3(-1.0,-1.0,-1.0),vec3(0.0,0.0,0.0),vec3(1.0,1.0,1.0));             
	vec4 final_color = vec4(0.0, 0.0, 0.0, 0.0);                                      
	for(int i = 0; i<3; i++)                                                          
	{                                                                                 
		for(int j = 0; j<3; j++)                                                      
		{                                                                             
			vec2 _xy_new = vec2(_xy.x + _filter_pos_delta_x[i][j], _xy.y + _filter_pos_delta_y[i][j]); 
			vec2 _uv_new = vec2(_xy_new.x/texSize.x, _xy_new.y/texSize.y);       
			final_color += texture2D(_image,_uv_new) * _filter[i][j] * _filter2[i][j];            
		}																		
	}																			
	return final_color;															
}	

//Gauss Operater
mat3 GAUSS_operator(float sigma)
{
	float N = 3;
    mat3 val;
    float R = (N-1)/2;    //***** Use N-1 because the array start from O rather than 1
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            float r = (i-R)*(i-R)+(j-R)*(j-R);
            float res = exp(-r/(2*sigma*sigma));
            res = (res /(2*3.14*sigma*sigma));
            val[i][j] = res;
        }
    }    
	return val;
}

//LOG and DOG Operater
mat3 LOG_operator(float sigma)
{
	float N = 3;
    mat3 val;
    float R = (N-1)/2;    //***** Use N-1 because the array start from O rather than 1
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            float r = (i-R)*(i-R)+(j-R)*(j-R);
            float res = exp(-r/(2*sigma*sigma));
            res = (res * ((i-R)*(i-R)+(j-R)*(j-R) - 2 * sigma* sigma))/(2*3.14*sigma*sigma*sigma*sigma*sigma*sigma);
            val[i][j] = res;
        }
    }    
	return val;
}

mat3 DOG_operator(float sigma, float k)
{
	float N = 3;
    mat3 val;
    float R = (N-1)/2;    //***** Use N-1 because the array start from O rather than 1
	sigma = k* sigma;
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            float r = (i-R)*(i-R)+(j-R)*(j-R);
            float res = exp(-r/(2*sigma*sigma));
            res = (1-k) * sigma * (res * ((i-R)*(i-R)+(j-R)*(j-R) - 2 * sigma* sigma))/(2*3.14*sigma*sigma*sigma*sigma*sigma*sigma);
            val[i][j] = res;
        }
    }    
	return val;
}

// if using thresholding:
//
//float epsilon = 0.1;
// (width, height) = shape(img)
// for i in range(width):
// 	for j in range(height):
// 		if differenced[i,j] > epsilon:
// 			differenced[i,j] = 255
// 		else:
// 			differenced[i,j] = 0

//----------------------------------------------------//

void main()
{
	//Outline drawing
	//vec3 result=texture(texture0, vs_texcoord).rgb;
	//
	//if(texture(texture0,vs_texcoord).r<0.8)
	//	result=texture(texture0,vs_texcoord).rgb;;
	//
	//for(int i=-3;i<3;i++)
	//{
	//	for(int j=-3;j<3;j++)
	//	{
	//		vec2 texSample=vec2(vs_texcoord.x+i/4000.0f,vs_texcoord.y+j/4000.0f);
	//		if(abs(texture(texture0,vs_texcoord).r-texture(texture0,texSample).r)>0.01)
	//			result=vec3(0.0,0.0,1.0);
	//	}
	//}


	//Ambient light
	//vec3  AmbientFinal = calculateAmbient(material);                            //The strength of the light
	//
	////Diffuse light                   
	//vec3  DiffuseFinal = calculateDiffuse(material, vs_position, vs_normal, LightPos0);
	//
	////Specular light
	//vec3  SpecularFinal = calculationSpecular(material, vs_position, vs_normal, LightPos0, CameraPos);


	//fs_color//vec4(vs_color, 1.f);//vec4(1,0,1,1);// 
	//	texture(material.diffuseTex, vs_texcoord) * vec4(result,1.0)
	//	* (vec4(AmbientFinal, 1.f) + vec4(DiffuseFinal, 1.f) + vec4(SpecularFinal, 1.f));
	
	
	//vec4 color =                                                                   //the same(using different uniform variable)
	//	  texture(texture0, vs_texcoord) //* vec4(vs_color, 1.f);
	//	  * (vec4(AmbientFinal, 1.f) + vec4(DiffuseFinal, 1.f) + vec4(SpecularFinal, 1.f));     //Note: the fragment(pixel) shader runs once for each pixel
    //if (color.r == color.g && color.g == color.b) discard;
	//fs_color = texture(texture0,vs_texcoord);//vec4(vs_color, 1.f);




	//-------Test1 Black & White--------//
	//vec4 W = vec4(0.0,0.59,0.11,0);
	//float luminace = dot(texture(texture0, vs_texcoord),W);
	//fs_color = vec4(luminace, luminace, luminace, 1.0);

	//-------Test2 Simple rectangular mosaic--------//
	//vec2 mosaicSize = vec2(8,8);
	//ivec2 texSize = textureSize(texture0,0);

	//vec2 intXY = vec2(vs_texcoord.x*texSize.x, vs_texcoord.y*texSize.y);   
	//vec2 XYMosaic = vec2(floor(intXY.x/mosaicSize.x)*mosaicSize.x,floor(intXY.y/mosaicSize.y)*mosaicSize.y) + 0.5*mosaicSize;
	//vec2 UVMosaic = vec2(XYMosaic.x/texSize.x, XYMosaic.y/texSize.y);
	//vec4 _finalColor = texture2D(texture0,UVMosaic);
	//fs_color = _finalColor
	//	* (vec4(AmbientFinal, 1.f)+ vec4(DiffuseFinal, 1.f)) * vec4(vs_color, 1.f);

	//-------Test3 Simple circle mosaic--------//
	//vec2 mosaicSize = vec2(8,8);
	//ivec2 texSize = textureSize(texture0,0);

	//vec2 intXY = vec2(vs_texcoord.x*texSize.x, vs_texcoord.y*texSize.y);   
	//vec2 XYMosaic = vec2(floor(intXY.x/mosaicSize.x)*mosaicSize.x,floor(intXY.y/mosaicSize.y)*mosaicSize.y) + 0.5*mosaicSize;
	//vec2 delXY = XYMosaic - intXY;  
	//float delL = length(delXY);     
	//vec2 UVMosaic = vec2(XYMosaic.x/texSize.x,XYMosaic.y/texSize.y);
	//vec4 _finalColor;               
	//if(delL< 0.5*mosaicSize.x)      
	//	_finalColor = texture2D(texture0,UVMosaic); 
	//else                            
	//	_finalColor = texture2D(texture0,vs_texcoord); 
	//fs_color = _finalColor
	//	* (vec4(AmbientFinal, 1.f)+ vec4(DiffuseFinal, 1.f)) * vec4(vs_color, 1.f);

	//----------Test4 Box blur filter----------//	
	//ivec2 texSize = textureSize(texture0,0);
	//vec2 intXY = vec2(vs_texcoord.x*texSize.x, vs_texcoord.y*texSize.y);   	
	//mat3 _smooth_fil = mat3(1.0,1.0,1.0,							
	//						1.0,1.0,1.0,							
	//						1.0,1.0,1.0)/10.0;							
	//vec4 tmp = dip_filter(_smooth_fil, texture0, intXY, texSize);
	//fs_color = tmp;                                                  		
								
	//----------Test4 Gauss blur filter----------//
	//ivec2 texSize = textureSize(texture0,0);
	//vec2 intXY = vec2(vs_texcoord.x*texSize.x, vs_texcoord.y*texSize.y);   	
	////mat3 _smooth_fil = mat3(1.0,2.0,1.0,							
	////						2.0,4.0,2.0,							
	////						1.0,2.0,1.0)/16.0;	
	//float sigma = 1.5;
	//mat3 _smooth_fil = GAUSS_operator(sigma)*2.25;
	//
	//vec4 tmp = dip_filter_gauss(_smooth_fil, texture0, intXY, texSize);
	//fs_color = tmp;       
	////fs_color = texture2D(texture0, vs_texcoord);


	//----------Test4 Laplace sharpen filter----------//
	//ivec2 texSize = textureSize(texture0,0);
	//vec2 intXY = vec2(vs_texcoord.x*texSize.x, vs_texcoord.y*texSize.y);   	
	//mat3 _smooth_fil = mat3(-1.0,-1.0,-1.0,							
	//						-1.0,9.0,-1.0,							
	//						-1.0,-1.0,-1.0);							
	//vec4 tmp = dip_filter(_smooth_fil, texture0, intXY, texSize);
	//fs_color = tmp;       

	//----------Test4 Emboss filter----------//
	//ivec2 texSize = textureSize(texture0,0);
	//vec2 intXY = vec2(vs_texcoord.x*texSize.x, vs_texcoord.y*texSize.y);   	
	//mat3 _smooth_fil = mat3(-0.5,-1.0,0.0,							
	//						-1.0,0.0,1.0,							
	//						0.0,1.0,0.5);						// minus the top-left angle	
	//vec4 tmp = dip_filter(_smooth_fil, texture0, intXY, texSize);
	//vec4 W = vec4(0.3,0.59,0.11,0);         
	//float luminace = dot(tmp,W);
	//if (luminace < 0.0) luminace = -1.0 * luminace;   //get the abs
	//luminace = 1.0 - luminace;                      //luminace is in  Black--[0,1]--White: abs bigger -> 1 => 1-1 = 0 => black
	//                                               //if we comment that line: abs bigger -> 1 => white
	//fs_color = vec4(luminace,luminace,luminace,1.0);    

	//----------Test4 Sobel filter----------//   ****
    //ivec2 texSize = textureSize(texture0,0);
    //vec2 intXY = vec2(vs_texcoord.x*texSize.x, vs_texcoord.y*texSize.y);   	
    //mat3 _smooth_fil1 = mat3(-1.0,-2.0,-1.0,							
    //						0.0,0.0,0.0,							
    //						1.0,2.0,1.0);						
    //mat3 _smooth_fil2 = mat3(-1.0,0.0,1.0,							
    //						-2.0,0.0,2.0,							
    //						-1.0,0.0,1.0);						
    //vec4 tmp = sobel_filter(_smooth_fil1, _smooth_fil2, texture0, intXY, texSize);
    //vec4 W = vec4(0.3,0.45,0.1,0);                   //for gray graph
    //float luminace = dot(tmp,W);
    ////if (luminace < 0.0) luminace = -1.0 * luminace;   //get the abs
    //luminace = 1.0 - luminace;                      //luminace is in  Black--[0,1]--White: abs bigger -> 1 => 1-1 = 0 => black
    //                                              //if we comment that line: abs bigger -> 1 => white
	//if (luminace > 0.67) luminace = 1; else luminace = 0;
	//fs_color = vec4(luminace,luminace,luminace,1.0); //* vec4(vs_color, 1.f);


	//----------Test5 Sobel+Gauss_blur(first half of canny) filter ----------//   ****
	//ivec2 texSize = textureSize(texture0,0);
    //vec2 intXY = vec2(vs_texcoord.x*texSize.x, vs_texcoord.y*texSize.y);   	
    //mat3 _smooth_fil1 = mat3(-1.0,-2.0,-1.0,							
    //						0.0,0.0,0.0,							
    //						1.0,2.0,1.0);						
    //mat3 _smooth_fil2 = mat3(-1.0,0.0,1.0,							
    //						-2.0,0.0,2.0,							
    //						-1.0,0.0,1.0);						
    //float tmp = sobel_gaussblur_filter(_smooth_fil1, _smooth_fil2, texture0, intXY, texSize);
	//if (tmp < 0.0) tmp = -1.0 * tmp;
	//tmp = 1.0 - tmp;                    
	//if (tmp > 0.26) tmp = 1; else tmp = 0;
	//fs_color = vec4(tmp,tmp,tmp,1.0);


	//----------Test5 Canny filter ----------//

	
	//----------Test6 LoG filter----------// ****
    //vec2 texSize = textureSize(texture0,0);
    //vec2 intXY = vec2(vs_texcoord.x*texSize.x, vs_texcoord.y*texSize.y);   	
	//
	//float sigma = 0.54;
	////mat3 val = LOG_operator(sigma);
	//mat3 val = mat3(-1,-1,-1,	
	//		    -1,8.0,-1,	
	//		    -1,-1,-1)*1.25;
	//
    //vec4 tmp1 = dip_filter_gauss(val, texture0, intXY, texSize);
	//vec4 W = vec4(0.3,0.45,0.3,0);
	//float luminace = dot(tmp1,W); 
	//
	//if (luminace < 0.0) luminace = -1.0 * luminace;
	//luminace = 1.0 - luminace;                
	//
	//float ipselon = 0.645;
	//if (luminace > ipselon) luminace = 1;         //Tao(ipselon)(u)
	//else luminace = 0;                    
	//                                              
	//fs_color = vec4(luminace,luminace,luminace,1.0);//vec4(tmp1,tmp1,tmp1,1.0); //vec4(luminace,luminace,luminace,1.0);


	//----------Test7 DoG filter (approximate)----------//
    //ivec2 texSize = textureSize(texture0,0);
    //vec2 intXY = vec2(vs_texcoord.x*texSize.x, vs_texcoord.y*texSize.y);   	
	//
	//float sigma = 1.5;
	//float k = 1.6;
	//
	//mat3 val1 = DOG_operator(sigma, k);
    //vec4 tmp1 = dip_filter(val1, texture0, intXY, texSize)*(4);
	//
	//vec4 W = vec4(0.1,0.59,0.11,0); 
	//float luminace = dot(tmp1,W);        
	//                 
	//float fi = 14;
	//float ipselon = 0.15;
	//if (luminace > ipselon) luminace = 1;        //Tao(ipselon,fi)(u)
	//else luminace = 1 + tanh(fi*(luminace-0.15));            
	//fs_color = vec4(luminace,luminace,luminace,1.0);

	//----------Test7 DoG filter (G(x)-Gk(x))----------//  ****
	ivec2 texSize = textureSize(texture0,0);
    vec2 intXY = vec2(vs_texcoord.x*texSize.x, vs_texcoord.y*texSize.y);   	
	
	float sigma = 1.5;
	float k = 1.6;
	
	mat3 val1 = GAUSS_operator(sigma);
    vec4 tmp1 = dip_filter(val1, texture0, intXY, texSize);
	
	mat3 val2 = GAUSS_operator(sigma* k);
    vec4 tmp2 = dip_filter(val2, texture0, intXY, texSize);
	
	float p = 0.3;
	float tao = 0.175;
	vec4 W = vec4(0.1,0.59,0.11,0); 
	float luminace1 = dot(tmp1,W);        
	float luminace2 = dot(tmp2,W);  
	
	float ipselon = 0.1;	float fi = 20;
	
	if (luminace2 > ipselon) luminace2 = 1;
	else luminace2 = 1 + tanh(fi*(luminace2-ipselon));  
	
	float luminace = luminace1 - tao * luminace2;
	
	luminace = 3.2*luminace + 1.2*luminace1;   //XDOG
	
	if (luminace > 0.48) luminace = 1;
	else luminace = 0;  
	
	fs_color = vec4(luminace,luminace,luminace,1.0);


	//-----------Test8 XDoG filter(approximate)-----------//
	//ivec2 texSize = textureSize(texture0,0);
    //vec2 intXY = vec2(vs_texcoord.x*texSize.x, vs_texcoord.y*texSize.y);   	
	//
	//float sigma = 1.5;
	//float k = 1.6;
	//
	//mat3 val1 = GAUSS_operator(sigma);
    //vec4 tmp1 = dip_filter(val1, texture0, intXY, texSize)*(4);
	//
	//mat3 val2 = DOG_operator(sigma, k);
    //vec4 tmp2 = dip_filter(val2, texture1, intXY, texSize)*(4);
	//
	//float p = 0.3;
	//vec4 W = vec4(0.1,0.59,0.11,0); 
	//float luminace1 = dot(tmp1,W);        
	//float luminace2 = dot(tmp2,W);  
	//	 
	//float luminace = luminace1 + p * luminace2;
	//
	//float fi = 50;
	//float ipselon = 0.15;
	//if (luminace > ipselon) luminace = 1;
	//else luminace = 1 + tanh(fi*(luminace-0.15));            
	//fs_color = vec4(luminace,luminace,luminace,1.0);

	//-----------Test8 XDoG filter-----------//
	//ivec2 texSize = textureSize(texture0,0);
    //vec2 intXY = vec2(vs_texcoord.x*texSize.x, vs_texcoord.y*texSize.y);   	
	//
	//float sigma = 1.5;
	//float k = 1.6;
	//
	//mat3 val1 = GAUSS_operator(sigma);
    //vec4 tmp1 = dip_filter(val1, texture0, intXY, texSize)*(4);
	//
	//mat3 val2 = GAUSS_operator(sigma* k);
    //vec4 tmp2 = dip_filter(val2, texture0, intXY, texSize)*(4);
	//
	//float p = 0.7;
	//vec4 W = vec4(0.1,0.59,0.11,0); 
	//float luminace1 = dot(tmp1,W);        
	//float luminace2 = dot(tmp2,W);  
	//	 
	//float luminace = (1+p) * luminace1 - p * luminace2;
	//
	//float fi = 55;
	//float ipselon = 2;
	////if (luminace > ipselon) luminace = 1;
	////else luminace =  1 + tanh(fi*(luminace-0.15));            
	//fs_color = vec4(luminace,luminace,luminace,1.0);


	//---------Questions----------//
	//1. Think about this: why it shows some trasparent effections on the other sides?
	//fs_color =  
		//(texture(texture0, vs_texcoord).r*0.3 + texture(texture0, vs_texcoord).g*0.59 + texture(texture0, vs_texcoord).b*0.11);
		//(vec4(AmbientFinal, 1.f) * 1.0f + vec4(DiffuseFinal, 1.f) * 0.0f + vec4(SpecularFinal, 1.f) * 0.f);

	
}
