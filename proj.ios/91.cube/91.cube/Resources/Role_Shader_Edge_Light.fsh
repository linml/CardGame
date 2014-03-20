// Shader taken from: http://webglsamples.googlecode.com/hg/electricflower/electricflower.html

#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform sampler2D CC_Texture0;

uniform vec2 blurSize;
uniform vec4 substract;

void main() {
	
//	vec4 sum = vec4(0.0);
//	sum += texture2D(CC_Texture0, v_texCoord - 4.0 * blurSize) * 0.05;
//	sum += texture2D(CC_Texture0, v_texCoord - 3.0 * blurSize) * 0.09;
//	sum += texture2D(CC_Texture0, v_texCoord - 2.0 * blurSize) * 0.12;
//	sum += texture2D(CC_Texture0, v_texCoord - 1.0 * blurSize) * 0.15;
//	sum += texture2D(CC_Texture0, v_texCoord                 ) * 0.16;
//	sum += texture2D(CC_Texture0, v_texCoord + 1.0 * blurSize) * 0.15;
//	sum += texture2D(CC_Texture0, v_texCoord + 2.0 * blurSize) * 0.12;
//	sum += texture2D(CC_Texture0, v_texCoord + 3.0 * blurSize) * 0.09;
//	sum += texture2D(CC_Texture0, v_texCoord + 4.0 * blurSize) * 0.05;
//	gl_FragColor = (sum - substract) * v_fragmentColor;



	
	// Convert to greyscale using NTSC weightings  
	//float alpha = texture2D(CC_Texture0, v_texCoord).a;  
	//float grey = dot(texture2D(CC_Texture0, v_texCoord).rgb, vec3(0.299, 0.587, 0.114));  
	//gl_FragColor = vec4(grey, grey, grey, alpha);  






	
	vec4 outColor = texture2D ( CC_Texture0, v_texCoord);// * v_fragmentColor;
	vec4 sum = vec4(0.0);	
	

	float fScope = 0.08;
	float upScope = fScope/1.414;
	sum += texture2D( CC_Texture0,v_texCoord+vec2(fScope,0) ) * v_fragmentColor;
	sum += texture2D( CC_Texture0,v_texCoord+vec2(0,fScope) ) * v_fragmentColor;
	sum += texture2D( CC_Texture0,v_texCoord+vec2(0,-fScope) )* v_fragmentColor ;
	sum += texture2D( CC_Texture0,v_texCoord+vec2(-fScope,0) )* v_fragmentColor ;
	sum += texture2D ( CC_Texture0, v_texCoord+vec2(upScope,upScope) )* v_fragmentColor ;
	sum += texture2D ( CC_Texture0, v_texCoord+vec2(upScope,-upScope) )* v_fragmentColor;
	sum += texture2D ( CC_Texture0, v_texCoord+vec2(-upScope, upScope) )* v_fragmentColor ;
	sum += texture2D ( CC_Texture0, v_texCoord+vec2(-upScope, -upScope) )* v_fragmentColor ;

	
	vec4 o  = vec4(1.0,0.847,0.1137,1.0) -outColor;
	vec4 o1 = o * sum;
	
	
	o1.rgb = vec3(1.0,0.847,0.1137) * o1.a;

	
	o1.rgb = o1.rgb * o1.a;


	o1.a = outColor.a + o1.a;
	
	
	//o1.rgb = o1.rgb + outColor.rgb;

	o1   = o1 + outColor;

	gl_FragColor = o1;
}
