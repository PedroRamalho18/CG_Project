//Projeto meta 2 - Pedro Tiago Gomes Ramalho nº 2019248594 
#pragma once

#include "ofMain.h"
#include "cg_extras.h"
#include "cg_drawing_extras.h"
#include "cg_cam_extras_solution.h"
#include "materiais.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		//camera variables
		int currentViewMode = 0, ortogonal_view = 0;
		GLfloat lensAngle;
		GLfloat alpha, beta;
		ofVec3f camPos;
		ofVec3f lookatPos;
		ofVec3f camUp;
		GLfloat orbitAngle;

		bool wireframe;

		//lights
		bool ambientOn;
		bool dirOn;
		bool pointOn;
		bool autoMove;
		bool localViewer;

		GLfloat ambientLight[4];

		GLfloat dirVec[4];
		GLfloat dirAmb[4];
		GLfloat dirDif[4];
		GLfloat dirSpec[4];
		ofVec3f dirVec3f;
		GLfloat dirVecTheta;

		GLfloat pointPos[4];
		GLfloat pointAmb[4];
		GLfloat pointDif[4];
		GLfloat pointSpec[4];
		GLfloat pointZtheta;
		GLfloat pointAtC, pointAtL, pointAtQ;

		GLfloat spotPos[4];
		GLfloat spotDir[3];
		GLfloat spotAmb[4];
		GLfloat spotDif[4];
		GLfloat spotSpecular[4];
		GLfloat spotExponent;
		GLfloat spotCutoff;
		GLfloat spotTheta;
		GLfloat spotAtC, spotAtL, spotAtQ;
		bool spotOn;

		//materiais
		int mat;

		//sphere
		bool drawSphere;

		GLfloat mview[4][4];

		GLfloat customMatAmb[4];
		GLfloat customMatDif[4];
		GLfloat customMatSpec[4];

		//Textures
		int sketch;

		//texturas
		ofImage bump;
		unsigned char* pixels;
		unsigned char* normalPixels = NULL;
		ofImage albedo;
		ofImage normalMap;
		ofImage wood;
		ofImage rocksFloor;
		ofImage crocodile;
		ofPixels p;
		bool originalOrBump;
		float bumpScale;

		//water
		float phaseX;
		float phaseY;
		float freqs[10];
		float amps[10];
		ofImage waterTex;
		vector<ofVec3f> vertexes;
		vector<ofVec3f> normals;
		vector<ofVec2f> texCoords;

		//skybox
		float rx, ry, rz;

		//procedural texture
		ofImage sky;
		unsigned char* skyPixels = NULL;
		int px, py, numPixels;

		//functions
		void setupIlumination();
		void setupTextures();
		void setupCity();
		void drawCity();
		void drawSky();
		void drawFloor();
		void drawTower(float x, float y, float z);
		void drawWall(float x, float y, float z,int side);
		void drawMainTower();
		void drawRiver();
		void drawCrocodile();
		void drawBridge();
		void drawViewTower();

	
		//floor
		GLint resX, resY;
		GLfloat floorWidth, floorHeight, floorHeightPos;
		
		//crocodile
		GLint crocodilePosX, crocodilePosY;
		ofVec3f crocodilePos;
		GLfloat crocodileWidth, crocodileDepth, crocodileHeight;
		bool moveCrocodile;
		bool moveRight;
		bool moveDown;
		bool moveLeft;
		bool moveUp;
		GLfloat velCrocodile = 6;

		//bridge
		bool angle;
		float bridgeRotationAngle,bridgeHeight;

		//viewtower
		GLfloat velViewTower = 1;
		bool down;
		ofVec3f viewTowerPos;

		//Towers
		GLfloat squareWidth, squareDepth, squareHeight;
};
