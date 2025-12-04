#pragma once

#include "ofMain.h"

class MeshNode {
	public:

		void calculateVertexBounds(){
			min.x = std::numeric_limits<float>::max();
			max.x = -min.x;

			min.y = min.x;
			max.y = max.x;

			min.z = min.x;
			max.z = max.x;

			const auto& verts = mesh.getVertices();
			for( const auto& v : verts ){
				if( v.x > max.x ){
					max.x = v.x;
				}
				if( v.x < min.x ){
					min.x = v.x;
				}

				if( v.y > max.y ){
					max.y = v.y;
				}
				if( v.y < min.y ){
					min.y = v.y;
				}

				if( v.z > max.z ){
					max.z = v.z;
				}
				if( v.z < min.z ){
					min.z = v.z;
				}
			}
		}

		ofNode node;
		ofMesh mesh;

		glm::vec3 min;
		glm::vec3 max;

		vector<ofMesh> srcMeshes;
		vector<ofMesh> offsetMeshes;
		vector<ofMesh> sideMeshes;
		vector<ofPolyline> polylines;
};

class ofApp : public ofBaseApp {
	public:
		void setup();
		void update();
		void draw();

		void drawBounds( glm::vec3 min, glm::vec3 max, float size );
		void stringToMeshNodes( string astring, float extrudeAmount );

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofEasyCam camera;
		ofTrueTypeFont font;
		vector<MeshNode> meshNodes;

		ofMaterial material;
		ofLight light;

		string message;

		glm::vec3 meshMin;
		glm::vec3 meshMax;
		glm::vec3 meshCentroid = {0,0,0}; // can be used in place of glm::vec3(0,0,0);
		float extrusion = 100;
};
