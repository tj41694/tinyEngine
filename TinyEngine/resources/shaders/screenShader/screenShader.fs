#version 300 es
precision highp float;

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D blurTexture;
uniform float exposure;


void main()
{ 
/**************************直通********************************/
    //FragColor = texture(screenTexture, TexCoords) * exposure;
	
/**************************反相********************************/
	//FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0) * exposure;
	
/****************************核********************************/
	//const float offset = 1.0 / 300.0;  
	//vec2 offsets[9] = vec2[](
    //    vec2(-offset,  offset), // 左上
    //    vec2( 0.0f,    offset), // 正上
    //    vec2( offset,  offset), // 右上
    //    vec2(-offset,  0.0f),   // 左
    //    vec2( 0.0f,    0.0f),   // 中
    //    vec2( offset,  0.0f),   // 右
    //    vec2(-offset, -offset), // 左下
    //    vec2( 0.0f,   -offset), // 正下
    //    vec2( offset, -offset)  // 右下
    //);
    //
    ////float kernel[9] = float[](  //锐化
    ////    -1.0, -1.0, -1.0,
    ////    -1.0,  9.0, -1.0,
    ////    -1.0, -1.0, -1.0
    ////);
	//
	//float kernel[9] = float[](  //模糊
	//	1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0,
	//	2.0 / 16.0, 4.0 / 16.0, 2.0 / 16.0,
	//	1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0  
    //);
    //
    //vec3 sampleTex[9];
    //for(int i = 0; i < 9; i++)
    //{
    //    sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    //}
    //vec3 col = vec3(0.0);
    //for(int i = 0; i < 9; i++)
    //    col += sampleTex[i] * kernel[i];
    //
    //FragColor = vec4(col, 1.0) * exposure;
	
/****************************色调映射(HDR)****************************/
	//const float gamma = 2.2;
    //vec3 hdrColor = texture(screenTexture, TexCoords).rgb;
    //vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);  // 曝光色调映射
    //mapped = pow(mapped, vec3(1.0 / gamma));  // Gamma校正 
    //FragColor = vec4(mapped, 1.0);
	
/****************************光晕(bloom)****************************/
	const float gamma = 2.2;
    vec3 hdrColor = texture(screenTexture, TexCoords).rgb;      
    vec3 bloomColor = texture(blurTexture, TexCoords).rgb * 0.8f;
    hdrColor += bloomColor; // additive blending
    
    //vec3 result = vec3(1.0) - exp(-hdrcolor * exposure); // tone mapping
     
    //result = pow(result, vec3(1.0 / gamma));  // also gamma correct while we're at it      
    FragColor = vec4(hdrColor, 1.0f);
}