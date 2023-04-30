#version 330

out vec4 FRAGCOLOR;

//-- coming from the vert shader ---- //
in vec4 oColor;
// ---------------------------------- //

void main(){
	FRAGCOLOR = oColor;
}
