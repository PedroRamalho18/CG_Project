//Projeto meta 2 - Pedro Tiago Gomes Ramalho nº 2019248594 
#ifndef MATERIALS_H
#define MATERIALS_H
#include "ofMain.h"

inline void loadMaterial(int material) {

	GLfloat muroDeCastelo1Amb[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat muroDeCastelo1Dif[] = { 0.4, 0.4, 0.4, 1.0 };
	GLfloat muroDeCastelo1Spec[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat muroDeCastelo1Coef = 32.0;

	GLfloat muroDeCastelo2Amb[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat muroDeCastelo2Dif[] = { 0.6, 0.6, 0.6, 1.0 };
	GLfloat muroDeCastelo2Spec[] = { 0.7, 0.7, 0.7, 1.0 };
	GLfloat muroDeCastelo2Coef = 32.0;

	GLfloat aguaAmb[] = { 0.0, 0.1, 0.1, 1.0 };
	GLfloat aguaDif[] = { 0.0, 0.5, 0.5, 1.0 };
	GLfloat aguaSpec[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat aguaCoef = 16.0;

	GLfloat chaoDePedraAmb[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat chaoDePedraDif[] = { 0.4, 0.4, 0.4, 1.0 };
	GLfloat chaoDePedraSpec[] = { 0.1, 0.1, 0.1, 1.0 };
	GLfloat chaoDePedraCoef = 8.0;

	GLfloat crocodiloAmb[] = { 0.2, 0.2, 0.1, 1.0 };
	GLfloat crocodiloDif[] = { 0.4, 0.4, 0.2, 1.0 };
	GLfloat crocodiloSpec[] = { 0.3, 0.3, 0.1, 1.0 };
	GLfloat crocodiloCoef = 8.0;

	
	GLfloat madeiraAmb[] = { 0.4, 0.2, 0.0, 1.0 };
	GLfloat madeiraDif[] = { 0.6, 0.3, 0.0, 1.0 };
	GLfloat madeiraSpec[] = { 0.3, 0.1, 0.0, 1.0 };
	GLfloat madeiraCoef = 8.0;

	switch (material) {
	case 0: // muro 1
		glMaterialfv(GL_FRONT, GL_AMBIENT, muroDeCastelo1Amb);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, muroDeCastelo1Dif);
		glMaterialfv(GL_FRONT, GL_SPECULAR, muroDeCastelo1Spec);
		glMaterialf(GL_FRONT, GL_SHININESS, muroDeCastelo1Coef);
		break;
	case 1: // muro2
		glMaterialfv(GL_FRONT, GL_AMBIENT, muroDeCastelo2Amb);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, muroDeCastelo2Dif);
		glMaterialfv(GL_FRONT, GL_SPECULAR, muroDeCastelo2Spec);
		glMaterialf(GL_FRONT, GL_SHININESS, muroDeCastelo2Coef);
		break;
	case 2: //chao de pedra
		glMaterialfv(GL_FRONT, GL_AMBIENT, chaoDePedraAmb);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, chaoDePedraDif);
		glMaterialfv(GL_FRONT, GL_SPECULAR, chaoDePedraSpec);
		glMaterialf(GL_FRONT, GL_SHININESS, chaoDePedraCoef);
		break;
	case 3: // agua
		glMaterialfv(GL_FRONT, GL_AMBIENT, aguaAmb);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, aguaDif);
		glMaterialfv(GL_FRONT, GL_SPECULAR, aguaSpec);
		glMaterialf(GL_FRONT, GL_SHININESS, aguaCoef);
		break;
	case 4: // crocodilo
		glMaterialfv(GL_FRONT, GL_AMBIENT, crocodiloAmb);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, crocodiloDif);
		glMaterialfv(GL_FRONT, GL_SPECULAR, crocodiloSpec);
		glMaterialf(GL_FRONT, GL_SHININESS, crocodiloCoef);
		break;
	case 5: // madeira
		glMaterialfv(GL_FRONT, GL_AMBIENT, madeiraAmb);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, madeiraDif);
		glMaterialfv(GL_FRONT, GL_SPECULAR, madeiraSpec);
		glMaterialf(GL_FRONT, GL_SHININESS, madeiraCoef);
		break;
	}

}

#endif
