//Projeto meta 2 - Pedro Tiago Gomes Ramalho nº 2019248594 
#include "ofApp.h"
#include "cg_drawing_extras.h"
void ofApp::setup() {
	ofSetFrameRate(40);
	ofSetVerticalSync(false);
	ofBackground(0, 0, 0);
	glEnable(GL_DEPTH_TEST);

	setupIlumination();
	setupTextures();
	setupCity();
}

void ofApp::setupIlumination() {
	//Iluminação
	ambientOn = true;
	dirOn = false;
	pointOn = false;
	spotOn = false;
	autoMove = true;
	wireframe = false;
	localViewer = false;
	pointZtheta = 0;
	dirVecTheta = 0.;
	spotExponent = 0;
	spotCutoff = 1;
	spotTheta = 0;
}

void ofApp::setupTextures() {
	//Texturas
	ofDisableArbTex();//coords de textura 0-1
	glShadeModel(GL_SMOOTH); //implica o uso de interpolação de Gouraud.
	originalOrBump = true;
	bumpScale = 0.0008;
	albedo.load("albedo.jpg");
	normalMap.load("normalMap.jpg");
	waterTex.load("water.jpg");
	wood.load("wood.jpg");
	rocksFloor.load("rocksFloor.jpg");
	crocodile.load("crocodile.jpg");

	normalPixels = new unsigned char[3 * 256 * 256];
	normalPixels = normalMap.getPixels().getData();
	for (int i = 0; i < 10; i++) {
		freqs[i] = 0.25 + 0.25 * i * 5;
		amps[i] = 1. / (i * 0.25 + 1);
		cout << endl << amps[i];
	}
	phaseX = 0.;
	phaseY = 0.;
	px = 128;
	py = 128;
	sky.allocate(px, py, OF_IMAGE_COLOR);
	skyPixels = new unsigned char[px * py * 3];
}

void ofApp::setupCity() {

	//floor parameters
	resX = 17;
	resY = 17;
	floorWidth = gw() * 0.5;
	floorHeight = gw() * 0.5;
	squareWidth = floorWidth / GLfloat(resX);
	squareHeight = floorHeight / GLfloat(resY);
	squareDepth = gh() * 0.4;

	//crocodile parameters
	crocodileDepth = resX * 2;
	crocodileHeight = squareHeight;
	crocodileWidth = squareWidth;
	crocodilePos.x = -squareWidth * (resX / 2 - 1) + squareWidth * 0.5;
	crocodilePos.y = -squareHeight * (resX / 2 - 1);
	crocodilePos.z = resX * 2 * 0.5;
	moveRight = true;
	moveDown = false;
	moveLeft = false;
	moveUp = false;

	//Bridge parameters
	bridgeRotationAngle = 0;
	bridgeHeight = 3 * squareHeight;
	angle = true;

	//ViewTower parameters
	down = true;
}

void ofApp::update() {
	ofSetWindowTitle(ofToString(ofGetFrameRate()));

	if (autoMove) {
		
		//calculo da posicao da luz pontual
		pointPos[0] = gw() * 0.5 * sin(3 * pointZtheta * PI / 180.0);
		pointPos[1] = gh() * 0.5 * cos(pointZtheta * PI / 180.0);
		pointPos[2] = 200 + 199 * cos(8 * pointZtheta * PI / 180.0);
		pointZtheta += 0.25;

		// Cálculo da direção da luz direcional
		float amplitude = gh();
		float z = amplitude * cos(dirVecTheta * PI / 180.) * 0.5;
		float y = amplitude * sin(dirVecTheta * PI / 180.);
		float x = 0;

		dirVec3f = ofVec3f(x, y, z);
		dirVecTheta += 1;
	}
	else {
		//luz pontual
		//centro acima
		pointPos[0] = 0;
		pointPos[1] = 0;
		pointPos[2] = gh()*0.65;

		//luz direcional
		dirVec3f = ofVec3f(0, -gh() * 0.5, 0);

		// luz foco
		spotDir[0] = 0;
		spotDir[1] = 0;
		spotDir[2] = -1;
	}

	// Bridge Rotation
	if (angle) {
		bridgeRotationAngle += 1.0; 
		if (bridgeRotationAngle == 90) {
			angle = false;
		}
	}
	else {
		bridgeRotationAngle -= 1.0;
		if (bridgeRotationAngle == 0){
			angle = true;
		}
	}

	//View Tower movement
	if (down && (viewTowerPos.z > 0)) {
		viewTowerPos.z -= velViewTower;
	}
	else {
		down = false;
	}

	if (down == false && (viewTowerPos.z < squareDepth)) {
		viewTowerPos.z += velViewTower;
	}
	else {
		down = true;
	}


	//crocodile movement
	if (moveRight) {
		if (crocodilePos.x < squareWidth * (resX / 2 - 1)) {
			crocodilePos.x += velCrocodile;
		}
		else {
			moveRight = false;
			moveDown = true;
		}
	}
	else if (moveDown) {
		if (crocodilePos.y < squareHeight * (resX / 2 - 1)) {
			crocodilePos.y += velCrocodile;
		}
		else {
			moveDown = false;
			moveLeft = true;
		}
	}
	else if (moveLeft) {
		if (crocodilePos.x > -squareWidth * (resX / 2 - 1)) {
			crocodilePos.x -= velCrocodile;
		}
		else {
			moveLeft = false;
			moveUp = true;
		}
	}
	else if (moveUp) {
		if (crocodilePos.y > -squareHeight * (resX / 2 - 1)) {
			crocodilePos.y -= velCrocodile;
		}
		else {
			moveUp = false;
			moveRight = true;
		}
	}
}

void ofApp::draw() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	lensAngle = 60;
	alpha = 10;
	beta = 1000;

	if (ortogonal_view == 0) {
		perspective(lensAngle, alpha, beta); //vista perspetiva
	}
	else if (ortogonal_view == 1) {
		glOrtho(-gw() * 0.5, gw() * 0.5, -gh(), gh(), -10000, 10000); // vista ortogonal
	}


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();

	if (currentViewMode == 0) {
		lookat(0, -gh()*0.75, gw()*0.75, 0, 0, 0, 0, 1, 0); // vista costumizada com perspetiva
	}
	else if (currentViewMode == 1) {
		lookat(0, 0, gw(), 0, 0, 0, 0, 1, 0); // vista de topo
	}
	else if (currentViewMode == 2) {
		lookat(0, -gh(), 0, 0, 0, 0, 0, 0, 1); // vista frontal
	}

	//inicia iluminação
	glEnable(GL_LIGHTING);//habilita o uso de iluminação
	glEnable(GL_NORMALIZE);//utiliza versores para normais (normais normalizadas)
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, localViewer);
	
	//fonte de luz que só tem componente ambiente
	//não conta como uma fonte de luz LIGHT0 - LIGHT8
	if (ambientOn) {
		ambientLight[0] = 1.;//R
		ambientLight[1] = 1.;//G
		ambientLight[2] = 1.;//B
		ambientLight[3] = 1;//useless
	}
	else {
		ambientLight[0] = 0.;
		ambientLight[1] = 0.;
		ambientLight[2] = 0.;
		ambientLight[3] = 1;
	}
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	
	//luz direcional
	dirVec[0] = dirVec3f.x;//x
	dirVec[1] = dirVec3f.y;//y
	dirVec[2] = dirVec3f.z;//z
	dirVec[3] = 0;//vetor - direção!
	
	dirAmb[0] = 1.0;//R
	dirAmb[1] = 1.0;//G
	dirAmb[2] = 1.0;//B
	dirAmb[3] = 1.;//constante

	dirDif[0] = 1.;//R
	dirDif[1] = 1.;//G
	dirDif[2] = 1.;//B
	dirDif[3] = 1.;//constante

	dirSpec[0] = 1.;//R
	dirSpec[1] = 1.;//G
	dirSpec[2] = 1.;//B
	dirSpec[3] = 1.;//constante

	glLightfv(GL_LIGHT0, GL_POSITION, dirVec);
	glLightfv(GL_LIGHT0, GL_AMBIENT, dirAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dirDif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, dirSpec);
	if (dirOn) {
		glEnable(GL_LIGHT0);
	}
	else {
		glDisable(GL_LIGHT0);
	}

	//#################################################
	//luz pontual
	//pointPos[0] = 0;//x
	//pointPos[1] = gh()*0.5;//y
	//pointPos[2] = 20;//z
	pointPos[3] = 1;//ponto - posi��o!

	pointAmb[0] = 1.;//R
	pointAmb[1] = 1.;//G
	pointAmb[2] = 1.;//B
	pointAmb[3] = 1.;//constante

	pointDif[0] = 1.;//R
	pointDif[1] = 1.;//G
	pointDif[2] = 1.;//B
	pointDif[3] = 1.;//constante

	pointSpec[0] = 1.;//R
	pointSpec[1] = 1.;//G
	pointSpec[2] = 1.;//B
	pointSpec[3] = 1.;//constante

	glLightfv(GL_LIGHT1, GL_POSITION, pointPos);
	glLightfv(GL_LIGHT1, GL_AMBIENT, pointAmb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, pointDif);
	glLightfv(GL_LIGHT1, GL_SPECULAR, pointSpec);
	//atenuação
	//reparem que não é fv (float vector) é só f (float)
	pointAtC = 1;
	pointAtL = 0.0001;
	pointAtQ = 0.00001;
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, pointAtC);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, pointAtL);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, pointAtQ);
	if (pointOn) {
		glEnable(GL_LIGHT1);
	}
	else {
		glDisable(GL_LIGHT1);
	}
	
	//#################################################
	//luz foco 

	//posicao
	spotPos[0] = 0.;
	spotPos[1] = 0.;
	spotPos[2] = gh()*0.6;
	//spotPos[3] = 1.;

	//direcao
	spotDir[0] = 0.;
	spotDir[1] = 0.;
	spotDir[2] = -1;
	//spotDir[3] = 0.; //Não tem a 4 coordenada, é sempre vetor

	//ambiente
	spotAmb[0] = 0.;//R
	spotAmb[1] = 0.;//G
	spotAmb[2] = 0.;//B
	spotAmb[3] = 0.;//constante

	//difusa
	spotDif[0] = 1.;//R
	spotDif[1] = 1.;//G
	spotDif[2] = 1.;//B
	spotDif[3] = 1.;//constante

	//specular
	spotSpecular[0] = 1.;//R
	spotSpecular[1] = 1.;//G
	spotSpecular[2] = 1.;//B
	spotSpecular[3] = 1.;//constante

	//concentracao
	spotExponent = 1;//0 - 128

	//angulo
	spotCutoff = 90;//0 - 180

	glLightfv(GL_LIGHT2, GL_POSITION, spotPos);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spotDir);

	glLightfv(GL_LIGHT2, GL_AMBIENT, spotAmb);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, spotDif);
	glLightfv(GL_LIGHT2, GL_SPECULAR, spotSpecular);

	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, spotExponent);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, spotCutoff);

	//atenuação
	spotAtC = 1.;
	spotAtL = 0.;
	spotAtQ = 0.;
	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, spotAtC);
	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, spotAtL);
	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, spotAtQ);
	if (spotOn) {
		glEnable(GL_LIGHT2);
	}else {
		glDisable(GL_LIGHT2);
	}

	drawCity();

	//#################################################
	//desenha representação das luzes e vetores utilizados
	//sem iluminação
	glDisable(GL_LIGHTING);//desabilita iluminação
	//representação da posição da Light1
	if (pointOn) {
		glColor3f(1, 1, 1);
		glPushMatrix();
		glTranslatef(pointPos[0], pointPos[1], pointPos[2]);
		glScalef(30, 30, 30);
		cube_unit();
		glPopMatrix();
	}
	//representação da posição da Light0
	if (dirOn) {
		glColor3f(1, 1, 1);
		glPushMatrix();
		glTranslatef(dirVec3f.x, dirVec3f.y, dirVec3f.z);
		glScalef(30, 30, 30);
		cube_unit();
		glPopMatrix();
		/*
		glPushMatrix();
		glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(dirVec3f.x, dirVec3f.y, dirVec3f.z);
		glEnd();
		glPopMatrix();
		*/
	}
	//representação da posição da luz spot
	if (spotOn) {
		glColor3f(1, 1, 1);
		glPushMatrix();
		glTranslatef(spotPos[0], spotPos[1], spotPos[2]);
		glScalef(30, 30, 30);
		cube_unit();
		glPopMatrix();
	}
	glPopMatrix();
}

void ofApp::drawCity() {
	drawSky();

	glPushMatrix();
	glColor3f(0, 0, 0);
	loadMaterial(2);
	drawFloor(); //chão construido com malha de retangulos
	loadMaterial(0);
	drawMainTower(); //predio em malha
	loadMaterial(1);
	drawViewTower(); //predio construido com posicionamento costumizado de vertices -> contem movimento relativo
	loadMaterial(5);
	drawBridge();//predio construido com malha de retangulos -> contém movimento rotativo
	loadMaterial(0);
	drawTower(-squareHeight * (resX / 2 - 2), -squareHeight * (resX / 2 - 2), squareDepth * 0.5); //frente esquerda -> construido com malha de retangulos
	drawTower(-squareHeight * (resX / 2 - 2), squareHeight * (resX / 2 - 2), squareDepth * 0.5); // tras esquerda -> construido com malha de retangulos
	drawTower(squareHeight * (resX / 2 - 2), -squareHeight * (resX / 2 - 2), squareDepth * 0.5);//frente direita -> construido com malha de retangulos
	drawTower(squareHeight * (resX / 2 - 2), squareHeight * (resX / 2 - 2), squareDepth * 0.5);//tras direita -> predio construido com cubo
	loadMaterial(0);
	drawWall(-squareHeight * (resX / 2 - 2), 0, squareDepth * 0.25, 1); //muro esquerdo -> predio construido com cubo
	drawWall(squareHeight * (resX / 2 - 2), 0, squareDepth * 0.25, 1); //muro direito -> predio construido com cubo
	drawWall(0, squareHeight * (resX / 2 - 2), squareDepth * 0.25, 2); //muro tras -> predio construido com cubo
	drawWall(-squareHeight * (resX / 4), -squareHeight * (resX / 2 - 2), squareDepth * 0.25, 3); //muro frente esquerda -> predio construido com cubo
	drawWall(squareHeight * (resX / 4), -squareHeight * (resX / 2 - 2), squareDepth * 0.25, 3); //muro frente direita -> predio construido com cubo
	loadMaterial(3);
	drawRiver();// cosntuido com malha de retangulos
	loadMaterial(4);
	drawCrocodile();//predio constuido com malha de retangulos -> contem movimento relativo
	glPopMatrix();

	if (wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		ofNoFill();
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		ofFill();
	}
	rx += 0.1;
	ry += 0.2;
	rz += 0.15;
}

void ofApp::drawSky() {
	// Limpa o buffer de cor e profundidade antes de renderizar a cena.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	phaseX += 0.0005 * cos(dirVecTheta * 0.001);
	phaseY -= 0.0006 * cos(dirVecTheta * 0.0001);
	skyPixels = sky.getPixels().getData();
	ofColor A(0, 0, 250);
	ofColor B(200, 200, 200);
	for (int j = 0; j < py; j++) {
		for (int i = 0; i < px; i++) {
			int index = 3 * (j * px + i);
			float noise = getMultiNoise(0.008 * (i)+phaseX, 0.008 * (j)+phaseY, freqs, amps, 10);  // Aumentar o valor no último parâmetro para suavizar o ruído
			ofColor auxColor = A.getLerped(B, noise);
			skyPixels[index] = auxColor.r;
			skyPixels[index + 1] = auxColor.g;
			skyPixels[index + 2] = auxColor.b;
		}
	}
	sky.setFromPixels(skyPixels, px, py, OF_IMAGE_COLOR);
	sky.update();

	glEnable(GL_TEXTURE);
	sky.bind();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glColor3f(1, 1, 1);
	glPushMatrix();
	glRotatef(0, 1, 0, 0);
	glRotatef(0, 0, 1, 0);
	glRotatef(0, 0, 0, 1);
	glScalef(gw() * 50, gw() * 50., gw() * 50.);
	ofDrawSphere(1);
	glPopMatrix();

	sky.unbind();
	glDisable(GL_TEXTURE);
}

void ofApp::drawFloor() {
	glEnable(GL_TEXTURE);
	rocksFloor.bind();
	glPushMatrix(); 
	glScalef(floorWidth, floorHeight, 1.);
	malha_unit_text(resX, resY);
	glPopMatrix(); 
	rocksFloor.unbind();
	glDisable(GL_TEXTURE);
}

void ofApp::drawMainTower() {
	glEnable(GL_TEXTURE);
	albedo.bind();
	glPushMatrix();
	glTranslatef(0, 0, squareDepth * 0.5);
	glScalef(4 * squareWidth, 4 * squareHeight, squareDepth);
	cube_malha_unit_text(resX, resY);
	glPopMatrix(); 
	albedo.unbind();
	glDisable(GL_TEXTURE);
}

void ofApp::drawViewTower() {
	glEnable(GL_TEXTURE);
	albedo.bind();
	glPushMatrix();
	glTranslatef(0, 0, viewTowerPos.z - squareDepth);
	glBegin(GL_QUADS);

	// Face frontal
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-squareWidth, -squareWidth, squareDepth);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-squareWidth, squareWidth, squareDepth);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(squareWidth, squareWidth, squareDepth);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(squareWidth, -squareWidth, squareDepth);

	// Face traseira
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-squareWidth, -squareWidth, squareDepth * 1.25);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-squareWidth, squareWidth, squareDepth * 1.25);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(squareWidth, squareWidth, squareDepth * 1.25);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(squareWidth, -squareWidth, squareDepth * 1.25);

	// Lado esquerdo
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-squareWidth, -squareWidth, squareDepth);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-squareWidth, -squareWidth, squareDepth * 1.25);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-squareWidth, squareWidth, squareDepth * 1.25);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-squareWidth, squareWidth, squareDepth);

	// Lado direito
	glTexCoord2f(0.0, 0.0);
	glVertex3f(squareWidth, -squareWidth, squareDepth);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(squareWidth, -squareWidth, squareDepth * 1.25);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(squareWidth, squareWidth, squareDepth * 1.25);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(squareWidth, squareWidth, squareDepth);

	// Topo
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-squareWidth, squareWidth, squareDepth);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-squareWidth, squareWidth, squareDepth * 1.25);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(squareWidth, squareWidth, squareDepth * 1.25);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(squareWidth, squareWidth, squareDepth);

	// Base
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-squareWidth, -squareWidth, squareDepth);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-squareWidth, -squareWidth, squareDepth * 1.25);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(squareWidth, -squareWidth, squareDepth * 1.25);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(squareWidth, -squareWidth, squareDepth);

	glEnd();
	glPopMatrix();

	albedo.unbind();
	glDisable(GL_TEXTURE);
}

void ofApp::drawBridge() {
	glEnable(GL_TEXTURE);
	wood.bind();
	glPushMatrix(); 
	glTranslatef(0, -2.5 * squareHeight, squareHeight);
	glRotatef(bridgeRotationAngle, 1, 0, 0);
	glTranslatef(0, 0, squareDepth * 0.25);
	glScalef(3.5 * squareWidth, squareHeight, squareDepth*0.6);
	cube_malha_unit_text(resX, resY);
	glPopMatrix();
	wood.unbind();
	glDisable(GL_TEXTURE);
}

void ofApp::drawTower(float x, float y, float z) {
	glEnable(GL_TEXTURE);
	albedo.bind();
	glPushMatrix(); 
	glTranslatef(x, y, z);
	glScalef(squareWidth, squareHeight, squareDepth);
	cube_malha_unit_text(resX, resY);
	albedo.unbind();
	glDisable(GL_TEXTURE);
	glPopMatrix(); 
}

void ofApp::drawWall(float x, float y, float z,int side) {
	glEnable(GL_TEXTURE);
	albedo.bind();
	
	glPushMatrix(); 
	glTranslatef(x, y, z);
	if (side == 1) {
		glScalef(squareWidth, squareHeight * (resX - 4), squareDepth * 0.5); //lados
	}
	if (side == 2) {
		glScalef(squareWidth * (resX - 4), squareHeight , squareDepth * 0.5); //trás
	}
	if (side == 3) {
		glScalef(squareWidth * (resX*0.5 -4), squareHeight, squareDepth * 0.5); //frente esquerda
	}
	
	cube_malha_unit_text(resX, resY);
	glPopMatrix(); 
	albedo.unbind();
	glDisable(GL_TEXTURE);
	
}

void ofApp::drawRiver() {
	glEnable(GL_TEXTURE);
	waterTex.bind();
	glPushMatrix();//river push fundo
	glTranslatef(0, squareHeight*(resX/2-1), 5 * 0.5);
	glScalef(squareWidth * (resY-2), squareHeight, 5);
	cube_malha_unit_text(resX, resY);
	glPopMatrix();//river pop fundo
	
	glPushMatrix();//river push frente
	glTranslatef(0, -squareHeight * (resX / 2 - 1), 5 * 0.5);
	glScalef(squareWidth * (resY - 2), squareHeight, 5);
	cube_malha_unit_text(resX, resY);
	glPopMatrix();//river pop frente

	glPushMatrix(); // river push esquerda
	glTranslatef(-squareHeight * (resX / 2 - 1), 0, 5 * 0.5);
	glScalef(squareWidth, squareHeight * (resY - 2), 5);
	cube_malha_unit_text(resX, resY);
	glPopMatrix(); // river pop esquerda

	glPushMatrix(); // river push direita
	glTranslatef(squareHeight * (resX / 2 - 1), 0, 5 * 0.5);
	glScalef(squareWidth, squareHeight * (resY - 2), 5);
	cube_malha_unit_text(resX, resY);
	glPopMatrix(); // river pop direita

	waterTex.unbind();
	glDisable(GL_TEXTURE);
}

void ofApp::drawCrocodile() {
	glEnable(GL_TEXTURE);
	crocodile.bind();
	glPushMatrix(); 
	glTranslatef(crocodilePos.x, crocodilePos.y, crocodilePos.z);
	glScalef(crocodileWidth, crocodileHeight, crocodileDepth);
	cube_malha_unit_text(resX, resY);
	glPopMatrix();
	crocodile.unbind();
	glDisable(GL_TEXTURE);
}

void ofApp::keyPressed(int key) {
	switch (key) {
	case '1': //vista customizada com perspetiva
		currentViewMode = 0;
		break;
	case '2': //vista de topo
		currentViewMode = 1;
		break;
	case '3': //vista frontal
		currentViewMode = 2;
		break;
	case 'o': //escolher entre vista perspectiva ou ortogonal
		if (ortogonal_view == 0) {
			ortogonal_view = 1;
		}
		else {
			ortogonal_view = 0;
		}
		break;
	case 'w':
		wireframe = !wireframe;
		break;
	case 'a':
		ambientOn = !ambientOn; //luz ambiente
		break;
	case 'd':
		dirOn = !dirOn;  //luz direcional
		break;
	case 'p':
		pointOn = !pointOn; //luz pontual
		break;
	case 'm':
		glShadeModel(GL_FLAT);
		break;
	case 'n':
		glShadeModel(GL_SMOOTH);
		break;
	case 'q':
		autoMove = !autoMove;  //direção dinâmica
		break;
	case 's':
		spotOn = !spotOn; 
		break;
	case '9':
		localViewer = !localViewer;
		break;
	}
}

void ofApp::keyReleased(int key) {
}

void ofApp::mouseMoved(int x, int y) {

}

void ofApp::mouseDragged(int x, int y, int button) {

}

void ofApp::mousePressed(int x, int y, int button) {
	cout << endl << x << " " << y;
}

void ofApp::mouseReleased(int x, int y, int button) {

}

void ofApp::mouseEntered(int x, int y) {

}

void ofApp::mouseExited(int x, int y) {

}

void ofApp::windowResized(int w, int h) {
	setup();
}

void ofApp::gotMessage(ofMessage msg) {

}

void ofApp::dragEvent(ofDragInfo dragInfo) {

}